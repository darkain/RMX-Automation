/*
** $Id: lua.h,v 1.175b 2003/03/18 12:31:39 roberto Exp $
** Lua - An Extensible Extension Language
** Tecgraf: Computer Graphics Technology Group, PUC-Rio, Brazil
** http://www.lua.org	mailto:info@lua.org
** See Copyright Notice at the end of this file
*/


#ifndef lua_h
#define lua_h

#ifndef LUA_REFCOUNT
#define LUA_REFCOUNT 0
#endif // LUA_REFCOUNT

#include "../LuaLink.h"

#if _MSC_VER >= 1400
#pragma warning(disable: 4996)	// deprecated
#endif /* _MSC_VER */

#include <stdarg.h>
#include <stddef.h>

NAMESPACE_LUA_BEGIN

#define LUA_VERSION	"Lua 5.0.2"
#define LUA_COPYRIGHT	"Copyright (C) 1994-2004 Tecgraf, PUC-Rio"
#define LUA_AUTHORS 	"R. Ierusalimschy, L. H. de Figueiredo & W. Celes"



/* option for multiple returns in `lua_pcall' and `lua_call' */
#define LUA_MULTRET	(-1)


/*
** pseudo-indices
*/
#define LUA_REGISTRYINDEX	(-10000)
#define LUA_GLOBALSINDEX	(-10001)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))


/* error codes for `lua_load' and `lua_pcall' */
#define LUA_ERRRUN	1
#define LUA_ERRFILE	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5


typedef struct lua_State lua_State;

typedef int (*lua_CFunction) (lua_State *L);


/*
** functions that read/write blocks when loading/dumping Lua chunks
*/
/* type of lex characters in Lua */
typedef unsigned short lua_WChar;

typedef const char * (*lua_Chunkreader) (lua_State *L, void *ud, size_t *sz);

typedef int (*lua_Chunkwriter) (lua_State *L, const void* p,
                                size_t sz, void* ud);


/*
** prototype for memory-allocation functions
*/
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize, const char* allocName, unsigned int flags);


/*
** basic types
*/
#define LUA_TNONE	(-1)

#define LUA_TNIL	0
#define LUA_TBOOLEAN	1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER	3
#define LUA_TSTRING	4
#define LUA_TTABLE	5
#define LUA_TFUNCTION	6
#define LUA_TUSERDATA	7
#define LUA_TTHREAD 8
#define LUA_TWSTRING	9
#define LUA_NTYPES 11


/* minimum Lua stack available to a C function */
#define LUA_MINSTACK	20


/*
** generic extra include file
*/
#ifdef LUA_USER_H
#include LUA_USER_H
#endif


/* type of numbers in Lua */
#ifndef LUA_NUMBER
typedef double lua_Number;
#else
typedef LUA_NUMBER lua_Number;
#endif


/* type for integer functions */
#ifndef LUA_INTEGER
typedef long lua_Integer;
#else
typedef LUA_INTEGER lua_Integer;
#endif


/* mark for all API functions */
#ifndef LUA_API
#define LUA_API		extern
#endif


/*
** state manipulation
*/
LUA_API lua_State *lua_open (void);
LUA_API void       lua_close (lua_State *L);
LUA_API lua_State *lua_newthread (lua_State *L);

LUA_API lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf);


/*
** basic stack manipulation
*/
LUA_API int   lua_gettop (lua_State *L);
LUA_API void  lua_settop (lua_State *L, int idx);
LUA_API void  lua_pushvalue (lua_State *L, int idx);
LUA_API void  lua_remove (lua_State *L, int idx);
LUA_API void  lua_insert (lua_State *L, int idx);
LUA_API void  lua_replace (lua_State *L, int idx);
LUA_API int   lua_checkstack (lua_State *L, int sz);

LUA_API void  lua_xmove (lua_State *from, lua_State *to, int n);


/*
** access functions (stack -> C)
*/

LUA_API int             lua_isnumber (lua_State *L, int idx);
LUA_API int             lua_isstring (lua_State *L, int idx);
LUA_API int             lua_iscfunction (lua_State *L, int idx);
LUA_API int             lua_isuserdata (lua_State *L, int idx);
LUA_API int             lua_type (lua_State *L, int idx);
LUA_API const char     *lua_typename (lua_State *L, int tp);

LUA_API int            lua_equal (lua_State *L, int idx1, int idx2);
LUA_API int            lua_rawequal (lua_State *L, int idx1, int idx2);
LUA_API int            lua_lessthan (lua_State *L, int idx1, int idx2);

LUA_API lua_Number      lua_tonumber (lua_State *L, int idx);
LUA_API int             lua_toboolean (lua_State *L, int idx);
LUA_API const char     *lua_tostring (lua_State *L, int idx);
LUA_API size_t          lua_strlen (lua_State *L, int idx);
LUA_API lua_CFunction   lua_tocfunction (lua_State *L, int idx);
LUA_API void	       *lua_touserdata (lua_State *L, int idx);
LUA_API lua_State      *lua_tothread (lua_State *L, int idx);
LUA_API const void     *lua_topointer (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  lua_pushnil (lua_State *L);
LUA_API void  lua_pushnumber (lua_State *L, lua_Number n);
LUA_API void  lua_pushlstring (lua_State *L, const char *s, size_t l);
LUA_API void  lua_pushstring (lua_State *L, const char *s);
LUA_API const char *lua_pushvfstring (lua_State *L, const char *fmt,
                                                    va_list argp);
LUA_API const char *lua_pushfstring (lua_State *L, const char *fmt, ...);
LUA_API void  lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);
LUA_API void  lua_pushboolean (lua_State *L, int b);
LUA_API void  lua_pushlightuserdata (lua_State *L, void *p);


/*
** get functions (Lua -> stack)
*/
LUA_API void  lua_gettable (lua_State *L, int idx);
LUA_API void  lua_rawget (lua_State *L, int idx);
LUA_API void  lua_rawgeti (lua_State *L, int idx, int n);
LUA_API void  lua_newtable (lua_State *L);
LUA_API void *lua_newuserdata (lua_State *L, size_t sz);
LUA_API int   lua_getmetatable (lua_State *L, int objindex);
LUA_API void  lua_getfenv (lua_State *L, int idx);


/*
** set functions (stack -> Lua)
*/
LUA_API void  lua_settable (lua_State *L, int idx);
LUA_API void  lua_rawset (lua_State *L, int idx);
LUA_API void  lua_rawseti (lua_State *L, int idx, int n);
LUA_API int   lua_setmetatable (lua_State *L, int objindex);
LUA_API int   lua_setfenv (lua_State *L, int idx);


/*
** `load' and `call' functions (load and run Lua code)
*/
LUA_API void  lua_call (lua_State *L, int nargs, int nresults);
LUA_API int   lua_pcall (lua_State *L, int nargs, int nresults, int errfunc);
LUA_API int lua_cpcall (lua_State *L, lua_CFunction func, void *ud);
LUA_API int   lua_load (lua_State *L, lua_Chunkreader reader, void *dt,
                        const char *chunkname);
LUA_API int   lua_wload (lua_State *L, lua_Chunkreader reader, void *dt,
                        const char *chunkname);

LUA_API int lua_dump (lua_State *L, lua_Chunkwriter writer, void *data);


/*
** coroutine functions
*/
LUA_API int  lua_yield (lua_State *L, int nresults);
LUA_API int  lua_resume (lua_State *L, int narg);

/*
** garbage-collection functions
*/
LUA_API int   lua_getgcthreshold (lua_State *L);
LUA_API int   lua_getgccount (lua_State *L);
LUA_API void  lua_setgcthreshold (lua_State *L, int newthreshold);

/*
** miscellaneous functions
*/

LUA_API const char *lua_version (void);

LUA_API int   lua_error (lua_State *L);

LUA_API int   lua_next (lua_State *L, int idx);

LUA_API void  lua_concat (lua_State *L, int n);

LUA_API lua_Alloc lua_getallocf (lua_State *L, void **ud);



/*
** ===============================================================
** some useful macros
** ===============================================================
*/

#define lua_boxpointer(L,u) \
	(lua_newuserdatabox(L, (u)))

#define lua_unboxpointer(L,i)	(lua_touserdata(L, i))

#define lua_pop(L,n)		lua_settop(L, -(n)-1)

#define lua_register(L,n,f) \
	(lua_pushstring(L, n), \
	 lua_pushcfunction(L, f), \
	 lua_settable(L, LUA_GLOBALSINDEX))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, f, 0)

#define lua_isfunction(L,n)	(lua_type(L,n) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L,n) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L,n) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L,n) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L,n) == LUA_TBOOLEAN)
#define lua_isnone(L,n)		(lua_type(L,n) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L,n) <= 0)

#define lua_pushliteral(L, s)	\
	lua_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)



/*
** compatibility macros and functions
*/


LUA_API int lua_pushupvalues (lua_State *L);

#define lua_getregistry(L)	lua_pushvalue(L, LUA_REGISTRYINDEX)
#define lua_setglobal(L,s)	\
   (lua_pushstring(L, s), lua_insert(L, -2), lua_settable(L, LUA_GLOBALSINDEX))

#define lua_getglobal(L,s)	\
		(lua_pushstring(L, s), lua_gettable(L, LUA_GLOBALSINDEX))


/* compatibility with ref system */

/* pre-defined references */
#define LUA_NOREF	(-2)
#define LUA_REFNIL	(-1)

#define lua_ref(L,lock)	((lock) ? luaL_ref(L, LUA_REGISTRYINDEX) : \
      (lua_pushstring(L, "unlocked references are obsolete"), lua_error(L), 0))

#define lua_unref(L,ref)	luaL_unref(L, LUA_REGISTRYINDEX, (ref))

#define lua_getref(L,ref)	lua_rawgeti(L, LUA_REGISTRYINDEX, ref)



/*
** {======================================================================
** useful definitions for Lua kernel and libraries
** =======================================================================
*/

/* formats for Lua numbers */
#ifndef LUA_NUMBER_SCAN
#define LUA_NUMBER_SCAN		"%lf"
#endif

#ifndef LUA_NUMBER_FMT
#define LUA_NUMBER_FMT		"%.14g"
#endif

/* }====================================================================== */


/*
** {======================================================================
** Debug API
** =======================================================================
*/


/*
** Event codes
*/
#define LUA_HOOKCALL	0
#define LUA_HOOKRET	1
#define LUA_HOOKLINE	2
#define LUA_HOOKCOUNT	3
#define LUA_HOOKTAILRET 4


/*
** Event masks
*/
#define LUA_MASKCALL	(1 << LUA_HOOKCALL)
#define LUA_MASKRET	(1 << LUA_HOOKRET)
#define LUA_MASKLINE	(1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT	(1 << LUA_HOOKCOUNT)

typedef struct lua_Debug lua_Debug;  /* activation record */

typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);


LUA_API int lua_getstack (lua_State *L, int level, lua_Debug *ar);
LUA_API int lua_getinfo (lua_State *L, const char *what, lua_Debug *ar);
LUA_API const char *lua_getlocal (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *lua_setlocal (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *lua_getupvalue (lua_State *L, int funcindex, int n);
LUA_API const char *lua_setupvalue (lua_State *L, int funcindex, int n);

LUA_API int lua_sethook (lua_State *L, lua_Hook func, int mask, int count);
LUA_API lua_Hook lua_gethook (lua_State *L);
LUA_API int lua_gethookmask (lua_State *L);
LUA_API int lua_gethookcount (lua_State *L);


#define LUA_IDSIZE	60

struct lua_Debug {
  int event;
  const char *name;	/* (n) */
  const char *namewhat;	/* (n) `global', `local', `field', `method' */
  const char *what;	/* (S) `Lua', `C', `main', `tail' */
  const char *source;	/* (S) */
  int currentline;	/* (l) */
  int nups;		/* (u) number of upvalues */
  int linedefined;	/* (S) */
  char short_src[LUA_IDSIZE]; /* (S) */
  /* private part */
  int i_ci;  /* active function */
};

/* }====================================================================== */


/******************************************************************************
* Copyright (C) 1994-2004 Tecgraf, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

NAMESPACE_LUA_END

NAMESPACE_LUA_BEGIN

#ifdef LUA_MTSUPPORT
	#define lua_lock(L)		if (L->l_G  &&  L->l_G->lockFunc) (*L->l_G->lockFunc)(L->l_G->lockData)
	#define lua_unlock(L)	if (L->l_G  &&  L->l_G->unlockFunc) (*L->l_G->unlockFunc)(L->l_G->lockData)
	LUA_API void lua_setlockfunctions(lua_State *L, void (*lockFunc)(void *), void (*unlockFunc)(void *), void *lockData);
#else /* !LUA_MTSUPPORT */
	#define lua_lock(L)
	#define lua_unlock(L)
#endif /* LUA_MTSUPPORT */

#define LUA_ALLOC_TEMP 1
LUA_API void lua_getdefaultallocfunction(lua_Alloc* allocFunc, void** ud);
LUA_API void lua_setdefaultallocfunction(lua_Alloc reallocFunc, void* ud);

LUA_API void lua_setusergcfunction(lua_State *L, void (*userGCFunction)(void*));

LUA_API void* lua_getglobaluserdata(lua_State *L);
LUA_API void lua_setglobaluserdata(lua_State *L, void* globalUserData);

LUA_API void* lua_getstateuserdata(lua_State *L);
LUA_API void lua_setstateuserdata(lua_State *L, void* stateUserData);

LUA_API void lua_setfatalerrorfunction(lua_State *L, void (*fatalErrorFunc)(void));
LUA_API void lua_setminimumstringtablesize(int numstrings);
LUA_API void lua_setminimumglobaltablesize(int numentries);
LUA_API void lua_setminimumauxspace(int size);

LUA_API int             lua_iswstring (lua_State *L, int index);
LUA_API const lua_WChar  *lua_towstring (lua_State *L, int index);
LUA_API void  lua_pushlwstring (lua_State *L, const lua_WChar *s, size_t len);
LUA_API void  lua_pushwstring (lua_State *L, const lua_WChar *s);

#define lua_pushwliteral(L, s)	\
	lua_pushlwstring(L, (const lua_WChar*)(L"" s), (sizeof(s)/sizeof(lua_WChar))-1)

LUA_API void  lua_getdefaultmetatable (lua_State *L, int objindex);
LUA_API void  lua_setdefaultmetatable(lua_State *L, int type);

LUA_API void lua_newuserdatabox (lua_State *L, void *ptr);

LUA_API int lua_getn (lua_State *L, int index);

/* formats for Lua numbers */
#ifndef LUA_NUMBER_WSCAN
#define LUA_NUMBER_WSCAN	L"%lf"
#endif

#ifndef LUA_NUMBER_WFMT
#define LUA_NUMBER_WFMT_LOCAL const lua_WChar LUA_NUMBER_WFMT[] = { '%', '.', '1', '6', 'g', 0 }
#endif

#define LUA_WSTRLIBNAME	"wstring"
LUALIB_API int luaopen_wstring (lua_State *L);

LUALIB_API int luaL_loadwbuffer (lua_State *L, const lua_WChar *buff, size_t size, const char *name);
LUALIB_API int lua_dowbuffer (lua_State *L, const lua_WChar *buff, size_t size, const char *name);
LUALIB_API int lua_dowstring (lua_State *L, const lua_WChar *str, const char *name);

NAMESPACE_LUA_END

#ifdef _WIN32_WCE
#include "../luace.h"
#endif /* _WIN32_WCE */

#endif
