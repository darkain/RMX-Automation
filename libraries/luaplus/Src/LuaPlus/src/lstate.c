/*
** $Id: lstate.c,v 1.123 2003/04/03 13:35:34 roberto Exp $
** Global State
** See Copyright Notice in lua.h
*/


#include <stdlib.h>

#define lstate_c

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "llex.h"
#include "lmem.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"

NAMESPACE_LUA_BEGIN

/*
** macro to allow the inclusion of user information in Lua state
*/
#ifndef LUA_USERSTATE
#define EXTRASPACE	0
#else
union UEXTRASPACE {L_Umaxalign a; LUA_USERSTATE b;};
#define EXTRASPACE (sizeof(union UEXTRASPACE))
#endif



/*
** you can change this function through the official API:
** call `lua_setpanicf'
*/
static int default_panic (lua_State *L) {
  UNUSED(L);
  return 0;
}


static void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize, const char* allocName, unsigned int flags) {
  (void)osize;
  (void)ud;
  (void)allocName;
  (void)flags;
  if (nsize == 0) {
    free(ptr);
    return NULL;
  }
  else
    return realloc(ptr, nsize);
}


static lua_Alloc luaHelper_defaultAlloc = l_alloc;
static void* luaHelper_ud = NULL;


void lua_getdefaultallocfunction(lua_Alloc* allocFunc, void** ud)
{
	*allocFunc = luaHelper_defaultAlloc;
	*ud = luaHelper_ud;
}


void lua_setdefaultallocfunction(lua_Alloc allocFunc, void* ud)
{
	luaHelper_defaultAlloc = allocFunc ? allocFunc : l_alloc;
	luaHelper_ud = ud;
}


static int lua_minimumnumstrings = NUM_RESERVED + NUM_TAGS;

void lua_setminimumstringtablesize(int numstrings)
{
	if (numstrings == 0)
		lua_minimumnumstrings = NUM_RESERVED + NUM_TAGS;
	else
		lua_minimumnumstrings = numstrings;
}

static int lua_minimumglobaltableentries = 10;

void lua_setminimumglobaltablesize(int numentries)
{
	if (numentries == 0)
		lua_minimumglobaltableentries = 10;
	else
		lua_minimumglobaltableentries = numentries;
}

static int lua_minimumauxspace = 0;

void lua_setminimumauxspace(int size)
{
	lua_minimumauxspace = size;
}

static void defaultFatalErrorFunc(void)
{
	exit(EXIT_FAILURE);
}


#ifdef LUA_MTSUPPORT
LUA_API void lua_setlockfunctions(lua_State *L, void (*lockFunc)(void *),
                                  void (*unlockFunc)(void *), void *lockData)
{
  G(L)->lockFunc = lockFunc;
  G(L)->unlockFunc = unlockFunc;
  G(L)->lockData = lockData;
}
#endif /* LUA_MTSUPPORT */


LUA_API void lua_setfatalerrorfunction(lua_State *L, void (*fatalErrorFunc)(void))
{
	G(L)->fatalErrorFunc = fatalErrorFunc;
}


LUA_API void* lua_getglobaluserdata(lua_State *L)
{
  return G(L)->globalUserData;
}


LUA_API void lua_setglobaluserdata(lua_State *L, void* globalUserData)
{
  G(L)->globalUserData = globalUserData;
}


LUA_API void lua_setusergcfunction(lua_State *L, void (*userGCFunction)(void*))
{
  G(L)->userGCFunction = userGCFunction;
}


LUA_API void* lua_getstateuserdata(lua_State *L)
{
  return L->stateUserData;
}


LUA_API void lua_setstateuserdata(lua_State *L, void* stateUserData)
{
  L->stateUserData = stateUserData;
}


static lua_State *mallocstate (lua_State *L) {
  lu_byte *block = (lu_byte *)luaM_malloc(L, sizeof(lua_State) + EXTRASPACE);
  if (block == NULL) return NULL;
  else {
    block += EXTRASPACE;
    return cast(lua_State *, block);
  }
}


static void freestate (lua_State *L, lua_State *L1) {
  luaM_free(L, cast(lu_byte *, L1) - EXTRASPACE,
               sizeof(lua_State) + EXTRASPACE);
}


static void stack_init (lua_State *L1, lua_State *L) {
  luaM_setname(L, "Lua_Stack");
  L1->stack = luaM_newvector(L, BASIC_STACK_SIZE + EXTRA_STACK, TObject);
  L1->stacksize = BASIC_STACK_SIZE + EXTRA_STACK;
  L1->top = L1->stack;
  L1->stack_last = L1->stack+(L1->stacksize - EXTRA_STACK)-1;
  {
    StkId i;
    for (i = L1->stack; i != L1->stack_last; ++i)
      newvalue(i);
  }
  luaM_setname(L, "Lua_CallInfo");
  L1->base_ci = luaM_newvector(L, BASIC_CI_SIZE, CallInfo);
  L1->ci = L1->base_ci;
  L1->ci->state = CI_C;  /*  not a Lua function */
  setnilvalue2n(L1->top++);  /* `function' entry for this `ci' */
  L1->base = L1->ci->base = L1->top;
  L1->ci->top = L1->top + LUA_MINSTACK;
  L1->size_ci = BASIC_CI_SIZE;
  L1->end_ci = L1->base_ci + L1->size_ci;
}


static void freestack (lua_State *L, lua_State *L1) {
  luaM_freearray(L, L1->base_ci, L1->size_ci, CallInfo);
  luaM_freearray(L, L1->stack, L1->stacksize, TObject);
}


/*
** open parts that may cause memory-allocation errors
*/
static void f_luaopen (lua_State *L, void *ud) {
  int i;
  global_State globalState;
  lua_State luaState;
  global_State *g;
#ifdef _DEBUG
  luaState.allocName = "Lua_global_State";
#endif /* _DEBUG */
  luaState.l_G = &globalState;
  globalState.realloc = luaHelper_defaultAlloc;
  globalState.ud = luaHelper_ud;
  globalState.nblocks = sizeof(lua_State) + sizeof(global_State);	// Bogus.
  /* create a new global state */
  g = luaM_new(&luaState, global_State);
  UNUSED(ud);
  if (g == NULL) luaD_throw(L, LUA_ERRMEM);
  L->l_G = g;
  g->mainthread = L;
  g->GCthreshold = 0;  /* mark it as unfinished state */
  g->strt.size = 0;
  g->strt.nuse = 0;
  g->strt.hash = NULL;
  setnilvalue2n(defaultmeta(L));
  setnilvalue2n(registry(L));
  luaZ_initbuffer(L, &g->buff);
  g->panic = default_panic;
  g->rootgc = NULL;
  g->rootudata = NULL;
  g->tmudata = NULL;
  setnilvalue2n(gkey(g->dummynode));
  setnilvalue2n(gval(g->dummynode));
  g->dummynode->next = NULL;
  g->nblocks = sizeof(lua_State) + sizeof(global_State);

  g->realloc = luaHelper_defaultAlloc;
  g->ud = luaHelper_ud;
  g->fatalErrorFunc = defaultFatalErrorFunc;
#ifdef LUA_MTSUPPORT
  g->lockData = NULL;
  g->lockFunc = NULL;
  g->unlockFunc = NULL;
#endif /* LUA_MTSUPPORT */
  g->userGCFunction = NULL;
  g->globalUserData = NULL;

  stack_init(L, L);  /* init stack */

  for (i = 0; i < LUA_NTYPES; i++)
  {
    defaultmetatypes(L, i)->tt = LUA_TNIL;
	defaultmetatypes(L, i)->value.gc = NULL;
  }

  /* create default meta table with a dummy table, and then close the loop */
  defaultmeta(L)->tt = LUA_TNIL;
  sethvalue(defaultmeta(L), luaH_new(L, 0, 0));
  lua_addreftable(hvalue(defaultmeta(L)));
  hvalue(defaultmeta(L))->metatable = hvalue(defaultmeta(L));
  lua_addreftable(hvalue(defaultmeta(L))->metatable);

  /* build meta tables */
  for (i = LUA_TBOOLEAN; i < LUA_NTYPES; i++)
  {
    luaM_setname(L, "Lua_defaultMetaTypes");
    sethvalue(defaultmetatypes(L, i), luaH_new(L, 0, 0));
    hvalue(defaultmetatypes(L, i))->metatable = hvalue(defaultmeta(L));
  }

  luaM_setname(L, "Lua_Globals");
  newvalue(gt(L));
  sethvalue(gt(L), luaH_new(L, 0, 4));  /* table of globals */
  lua_addreftable(hvalue(gt(L)));
  luaM_setname(L, "Lua_Registry");
  sethvalue(registry(L), luaH_new(L, 4, 4));  /* registry */
  lua_addref(registry(L));
  g->minimumstrings = lua_minimumnumstrings;
  luaS_resize(L, MINSTRTABSIZE);  /* initial size of string table */
  luaT_init(L);
  luaX_init(L);
  luaS_fix(luaS_newliteral(L, MEMERRMSG));
  g->GCthreshold = 4*G(L)->nblocks;

  luaZ_openspace(L, &g->buff, lua_minimumauxspace);
}


static void preinit_state (lua_State *L) {
  L->stack = NULL;
  L->stacksize = 0;
  L->errorJmp = NULL;
  L->hook = NULL;
  L->hookmask = L->hookinit = 0;
  L->basehookcount = 0;
  L->allowhook = 1;
  resethookcount(L);
  L->openupval = NULL;
  L->size_ci = 0;
  L->nCcalls = 0;
  L->base_ci = L->ci = NULL;
  L->errfunc = 0;
  L->stateUserData = NULL;
  setnilvalue2n(gt(L));
}


static void close_state (lua_State *L) {
  global_State globalState;
  lua_State luaState;

  luaF_close(L, L->stack);  /* close all upvalues for this thread */

  luaState.l_G = &globalState;
  globalState.realloc = G(L)->realloc;
  globalState.ud = G(L)->ud;

  if (G(L)) {  /* close global state */
    luaC_sweep(L, 1);  /* collect all elements */
    lua_assert(G(L)->rootgc == NULL);
    lua_assert(G(L)->rootudata == NULL);
    luaS_freeall(L);
    luaZ_freebuffer(L, &G(L)->buff);
  }
  freestack(L, L);
  if (G(L)) {
//jj This check doesn't work anymore, because LuaState variables are allocated through the
//jj memory interface, too.
//jj    lua_assert(G(L)->nblocks == sizeof(lua_State) + sizeof(global_State));
    luaM_freelem(&luaState, G(L));
  }
  freestate(&luaState, L);
}


lua_State *luaE_newthread (lua_State *L) {
  lua_State *L1 = mallocstate(L);
  luaC_link(L, valtogco(L1), LUA_TTHREAD);
  preinit_state(L1);
  L1->l_G = L->l_G;
  stack_init(L1, L);  /* init stack */
  setobj2n(gt(L1), gt(L));  /* share table of globals */
  return L1;
}


void luaE_freethread (lua_State *L, lua_State *L1) {
  luaF_close(L1, L1->stack);  /* close all upvalues for this thread */
  lua_assert(L1->openupval == NULL);
  freestack(L, L1);
  freestate(L, L1);
}


LUA_API lua_State *lua_open (void) {
  lua_State *L;
  global_State globalState;
  lua_State luaState;
#ifdef _DEBUG
  luaState.allocName = "Lua_lua_State";
#endif /* _DEBUG */
  luaState.l_G = &globalState;
  globalState.realloc = luaHelper_defaultAlloc;
  globalState.ud = luaHelper_ud;
  globalState.nblocks = sizeof(lua_State) + sizeof(global_State);	// Bogus.
  L = mallocstate(&luaState);
  if (L) {  /* allocation OK? */
    L->tt = LUA_TTHREAD;
    L->marked = 0;
    L->next = L->gclist = NULL;
    preinit_state(L);
    L->l_G = NULL;
    if (luaD_rawrunprotected(L, f_luaopen, NULL) != 0) {
      /* memory allocation error: free partial state */
      close_state(L);
      L = NULL;
    }
  }
  lua_userstateopen(L);
  return L;
}


static void callallgcTM (lua_State *L, void *ud) {
  UNUSED(ud);
  luaC_callGCTM(L);  /* call GC metamethods for all udata */
}


LUA_API void lua_close (lua_State *L) {
  lua_lock(L);
  L = G(L)->mainthread;  /* only the main thread can be closed */
  luaF_close(L, L->stack);  /* close all upvalues for this thread */
  luaC_separateudata(L);  /* separate udata that have GC metamethods */
  L->errfunc = 0;  /* no error function during GC metamethods */
  do {  /* repeat until no more errors */
    L->ci = L->base_ci;
    L->base = L->top = L->ci->base;
    L->nCcalls = 0;
  } while (luaD_rawrunprotected(L, callallgcTM, NULL) != 0);
  lua_assert(G(L)->tmudata == NULL);
  close_state(L);
}

NAMESPACE_LUA_END
