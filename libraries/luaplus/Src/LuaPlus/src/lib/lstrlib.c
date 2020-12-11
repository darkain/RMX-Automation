/*
** $Id: lstrlib.c,v 1.98 2003/04/03 13:35:34 roberto Exp $
** Standard library for string operations and pattern-matching
** See Copyright Notice in lua.h
*/


#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lstrlib_c

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

NAMESPACE_LUA_BEGIN

/* macro to `unsign' a character */
#ifndef uchar
#define uchar(c)        ((unsigned char)(c))
#endif


typedef long sint32;	/* a signed version for size_t */


static int str_len (lua_State *L) {
  size_t l;
  luaL_checklstring(L, 1, &l);
  lua_pushnumber(L, (lua_Number)l);
  return 1;
}


static sint32 posrelat (sint32 pos, size_t len) {
  /* relative string position: negative means back from end */
  return (pos>=0) ? pos : (sint32)len+pos+1;
}


static int str_sub (lua_State *L) {
  size_t l;
  const char *s = luaL_checklstring(L, 1, &l);
  sint32 start = posrelat(luaL_checklong(L, 2), l);
  sint32 end = posrelat(luaL_optlong(L, 3, -1), l);
  if (start < 1) start = 1;
  if (end > (sint32)l) end = (sint32)l;
  if (start <= end)
    lua_pushlstring(L, s+start-1, end-start+1);
  else lua_pushliteral(L, "");
  return 1;
}


static int str_lower (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  luaL_buffinit(L, &b);
  for (i=0; i<l; i++)
    luaL_putchar(&b, tolower(uchar(s[i])));
  luaL_pushresult(&b);
  return 1;
}


static int str_upper (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  luaL_buffinit(L, &b);
  for (i=0; i<l; i++)
    luaL_putchar(&b, toupper(uchar(s[i])));
  luaL_pushresult(&b);
  return 1;
}

static int str_rep (lua_State *L) {
  size_t l;
  luaL_Buffer b;
  const char *s = luaL_checklstring(L, 1, &l);
  int n = luaL_checkint(L, 2);
  luaL_buffinit(L, &b);
  while (n-- > 0)
    luaL_addlstring(&b, s, l);
  luaL_pushresult(&b);
  return 1;
}


static int str_byte (lua_State *L) {
  size_t l;
  const char *s = luaL_checklstring(L, 1, &l);
  sint32 pos = posrelat(luaL_optlong(L, 2, 1), l);
  if (pos <= 0 || (size_t)(pos) > l)  /* index out of range? */
    return 0;  /* no answer */
  lua_pushnumber(L, uchar(s[pos-1]));
  return 1;
}


static int str_char (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  luaL_Buffer b;
  luaL_buffinit(L, &b);
  for (i=1; i<=n; i++) {
    int c = luaL_checkint(L, i);
    luaL_argcheck(L, uchar(c) == c, i, "invalid value");
    luaL_putchar(&b, uchar(c));
  }
  luaL_pushresult(&b);
  return 1;
}


static int writer (lua_State *L, const void* b, size_t size, void* B) {
  (void)L;
  luaL_addlstring((luaL_Buffer*) B, (const char *)b, size);
  return 1;
}


static int str_dump (lua_State *L) {
  luaL_Buffer b;
  luaL_checktype(L, 1, LUA_TFUNCTION);
  luaL_buffinit(L,&b);
  if (!lua_dump(L, writer, &b))
    luaL_error(L, "unable to dump given function");
  luaL_pushresult(&b);
  return 1;
}



/*
** {======================================================
** PATTERN MATCHING
** =======================================================
*/

#ifndef MAX_CAPTURES
#define MAX_CAPTURES 32  /* arbitrary limit */
#endif


#define CAP_UNFINISHED	(-1)
#define CAP_POSITION	(-2)

typedef struct MatchState {
  const char *src_init;  /* init of source string */
  const char *src_end;  /* end (`\0') of source string */
  lua_State *L;
  int level;  /* total number of captures (finished or unfinished) */
  struct {
    const char *init;
    sint32 len;
  } capture[MAX_CAPTURES];
} MatchState;


#define ESC		'%'
#define SPECIALS	"^$*+?.([%-"


static int check_capture (MatchState *ms, int l) {
  l -= '1';
  if (l < 0 || l >= ms->level || ms->capture[l].len == CAP_UNFINISHED)
    return luaL_error(ms->L, "invalid capture index");
  return l;
}


static int capture_to_close (MatchState *ms) {
  int level = ms->level;
  for (level--; level>=0; level--)
    if (ms->capture[level].len == CAP_UNFINISHED) return level;
  return luaL_error(ms->L, "invalid pattern capture");
}


static const char *luaI_classend (MatchState *ms, const char *p) {
  switch (*p++) {
    case ESC: {
      if (*p == '\0')
        luaL_error(ms->L, "malformed pattern (ends with `%')");
      return p+1;
    }
    case '[': {
      if (*p == '^') p++;
      do {  /* look for a `]' */
        if (*p == '\0')
          luaL_error(ms->L, "malformed pattern (missing `]')");
        if (*(p++) == ESC && *p != '\0')
          p++;  /* skip escapes (e.g. `%]') */
      } while (*p != ']');
      return p+1;
    }
    default: {
      return p;
    }
  }
}


static int match_class (int c, int cl) {
  int res;
  switch (tolower(cl)) {
    case 'a' : res = isalpha(c); break;
    case 'c' : res = iscntrl(c); break;
    case 'd' : res = isdigit(c); break;
    case 'l' : res = islower(c); break;
    case 'p' : res = ispunct(c); break;
    case 's' : res = isspace(c); break;
    case 'u' : res = isupper(c); break;
    case 'w' : res = isalnum(c); break;
    case 'x' : res = isxdigit(c); break;
    case 'z' : res = (c == 0); break;
    default: return (cl == c);
  }
  return (islower(cl) ? res : !res);
}


static int matchbracketclass (int c, const char *p, const char *ec) {
  int sig = 1;
  if (*(p+1) == '^') {
    sig = 0;
    p++;  /* skip the `^' */
  }
  while (++p < ec) {
    if (*p == ESC) {
      p++;
      if (match_class(c, *p))
        return sig;
    }
    else if ((*(p+1) == '-') && (p+2 < ec)) {
      p+=2;
      if (uchar(*(p-2)) <= c && c <= uchar(*p))
        return sig;
    }
    else if (uchar(*p) == c) return sig;
  }
  return !sig;
}


static int luaI_singlematch (int c, const char *p, const char *ep) {
  switch (*p) {
    case '.': return 1;  /* matches any char */
    case ESC: return match_class(c, *(p+1));
    case '[': return matchbracketclass(c, p, ep-1);
    default:  return (uchar(*p) == c);
  }
}


static const char *match (MatchState *ms, const char *s, const char *p);


static const char *matchbalance (MatchState *ms, const char *s,
                                   const char *p) {
  if (*p == 0 || *(p+1) == 0)
    luaL_error(ms->L, "unbalanced pattern");
  if (*s != *p) return NULL;
  else {
    int b = *p;
    int e = *(p+1);
    int cont = 1;
    while (++s < ms->src_end) {
      if (*s == e) {
        if (--cont == 0) return s+1;
      }
      else if (*s == b) cont++;
    }
  }
  return NULL;  /* string ends out of balance */
}


static const char *max_expand (MatchState *ms, const char *s,
                                 const char *p, const char *ep) {
  sint32 i = 0;  /* counts maximum expand for item */
  while ((s+i)<ms->src_end && luaI_singlematch(uchar(*(s+i)), p, ep))
    i++;
  /* keeps trying to match with the maximum repetitions */
  while (i>=0) {
    const char *res = match(ms, (s+i), ep+1);
    if (res) return res;
    i--;  /* else didn't match; reduce 1 repetition to try again */
  }
  return NULL;
}


static const char *min_expand (MatchState *ms, const char *s,
                                 const char *p, const char *ep) {
  for (;;) {
    const char *res = match(ms, s, ep+1);
    if (res != NULL)
      return res;
    else if (s<ms->src_end && luaI_singlematch(uchar(*s), p, ep))
      s++;  /* try with one more repetition */
    else return NULL;
  }
}


static const char *start_capture (MatchState *ms, const char *s,
                                    const char *p, int what) {
  const char *res;
  int level = ms->level;
  if (level >= MAX_CAPTURES) luaL_error(ms->L, "too many captures");
  ms->capture[level].init = s;
  ms->capture[level].len = what;
  ms->level = level+1;
  if ((res=match(ms, s, p)) == NULL)  /* match failed? */
    ms->level--;  /* undo capture */
  return res;
}


static const char *end_capture (MatchState *ms, const char *s,
                                  const char *p) {
  int l = capture_to_close(ms);
  const char *res;
  ms->capture[l].len = s - ms->capture[l].init;  /* close capture */
  if ((res = match(ms, s, p)) == NULL)  /* match failed? */
    ms->capture[l].len = CAP_UNFINISHED;  /* undo capture */
  return res;
}


static const char *match_capture (MatchState *ms, const char *s, int l) {
  size_t len;
  l = check_capture(ms, l);
  len = ms->capture[l].len;
  if ((size_t)(ms->src_end-s) >= len &&
      memcmp(ms->capture[l].init, s, len) == 0)
    return s+len;
  else return NULL;
}


static const char *match (MatchState *ms, const char *s, const char *p) {
  init: /* using goto's to optimize tail recursion */
  switch (*p) {
    case '(': {  /* start capture */
      if (*(p+1) == ')')  /* position capture? */
        return start_capture(ms, s, p+2, CAP_POSITION);
      else
        return start_capture(ms, s, p+1, CAP_UNFINISHED);
    }
    case ')': {  /* end capture */
      return end_capture(ms, s, p+1);
    }
    case ESC: {
      switch (*(p+1)) {
        case 'b': {  /* balanced string? */
          s = matchbalance(ms, s, p+2);
          if (s == NULL) return NULL;
          p+=4; goto init;  /* else return match(ms, s, p+4); */
        }
        case 'f': {  /* frontier? */
          const char *ep; char previous;
          p += 2;
          if (*p != '[')
            luaL_error(ms->L, "missing `[' after `%%f' in pattern");
          ep = luaI_classend(ms, p);  /* points to what is next */
          previous = (s == ms->src_init) ? '\0' : *(s-1);
          if (matchbracketclass(uchar(previous), p, ep-1) ||
             !matchbracketclass(uchar(*s), p, ep-1)) return NULL;
          p=ep; goto init;  /* else return match(ms, s, ep); */
        }
        default: {
          if (isdigit(uchar(*(p+1)))) {  /* capture results (%0-%9)? */
            s = match_capture(ms, s, *(p+1));
            if (s == NULL) return NULL;
            p+=2; goto init;  /* else return match(ms, s, p+2) */
          }
          goto dflt;  /* case default */
        }
      }
    }
    case '\0': {  /* end of pattern */
      return s;  /* match succeeded */
    }
    case '$': {
      if (*(p+1) == '\0')  /* is the `$' the last char in pattern? */
        return (s == ms->src_end) ? s : NULL;  /* check end of string */
      else goto dflt;
    }
    default: dflt: {  /* it is a pattern item */
      const char *ep = luaI_classend(ms, p);  /* points to what is next */
      int m = s<ms->src_end && luaI_singlematch(uchar(*s), p, ep);
      switch (*ep) {
        case '?': {  /* optional */
          const char *res;
          if (m && ((res=match(ms, s+1, ep+1)) != NULL))
            return res;
          p=ep+1; goto init;  /* else return match(ms, s, ep+1); */
        }
        case '*': {  /* 0 or more repetitions */
          return max_expand(ms, s, p, ep);
        }
        case '+': {  /* 1 or more repetitions */
          return (m ? max_expand(ms, s+1, p, ep) : NULL);
        }
        case '-': {  /* 0 or more repetitions (minimum) */
          return min_expand(ms, s, p, ep);
        }
        default: {
          if (!m) return NULL;
          s++; p=ep; goto init;  /* else return match(ms, s+1, ep); */
        }
      }
    }
  }
}



static const char *lmemfind (const char *s1, size_t l1,
                               const char *s2, size_t l2) {
  if (l2 == 0) return s1;  /* empty strings are everywhere */
  else if (l2 > l1) return NULL;  /* avoids a negative `l1' */
  else {
    const char *init;  /* to search for a `*s2' inside `s1' */
    l2--;  /* 1st char will be checked by `memchr' */
    l1 = l1-l2;  /* `s2' cannot be found after that */
    while (l1 > 0 && (init = (const char *)memchr(s1, *s2, l1)) != NULL) {
      init++;   /* 1st char is already checked */
      if (memcmp(init, s2+1, l2) == 0)
        return init-1;
      else {  /* correct `l1' and `s1' to try again */
        l1 -= init-s1;
        s1 = init;
      }
    }
    return NULL;  /* not found */
  }
}


static void push_onecapture (MatchState *ms, int i) {
  int l = ms->capture[i].len;
  if (l == CAP_UNFINISHED) luaL_error(ms->L, "unfinished capture");
  if (l == CAP_POSITION)
    lua_pushnumber(ms->L, (lua_Number)(ms->capture[i].init - ms->src_init + 1));
  else
    lua_pushlstring(ms->L, ms->capture[i].init, l);
}


static int push_captures (MatchState *ms, const char *s, const char *e) {
  int i;
  luaL_checkstack(ms->L, ms->level, "too many captures");
  if (ms->level == 0 && s) {  /* no explicit captures? */
    lua_pushlstring(ms->L, s, e-s);  /* return whole match */
    return 1;
  }
  else {  /* return all captures */
    for (i=0; i<ms->level; i++)
      push_onecapture(ms, i);
    return ms->level;  /* number of strings pushed */
  }
}


static int str_find (lua_State *L) {
  size_t l1, l2;
  const char *s = luaL_checklstring(L, 1, &l1);
  const char *p = luaL_checklstring(L, 2, &l2);
  sint32 init = posrelat(luaL_optlong(L, 3, 1), l1) - 1;
  if (init < 0) init = 0;
  else if ((size_t)(init) > l1) init = (sint32)l1;
  if (lua_toboolean(L, 4) ||  /* explicit request? */
      strpbrk(p, SPECIALS) == NULL) {  /* or no special characters? */
    /* do a plain search */
    const char *s2 = lmemfind(s+init, l1-init, p, l2);
    if (s2) {
      lua_pushnumber(L, (lua_Number)(s2-s+1));
      lua_pushnumber(L, (lua_Number)(s2-s+l2));
      return 2;
    }
  }
  else {
    MatchState ms;
    int anchor = (*p == '^') ? (p++, 1) : 0;
    const char *s1=s+init;
    ms.L = L;
    ms.src_init = s;
    ms.src_end = s+l1;
    do {
      const char *res;
      ms.level = 0;
      if ((res=match(&ms, s1, p)) != NULL) {
        lua_pushnumber(L, (lua_Number)(s1-s+1));  /* start */
        lua_pushnumber(L, (lua_Number)(res-s));   /* end */
        return push_captures(&ms, NULL, 0) + 2;
      }
    } while (s1++<ms.src_end && !anchor);
  }
  lua_pushnil(L);  /* not found */
  return 1;
}


static int gfind_aux (lua_State *L) {
  MatchState ms;
  const char *s = lua_tostring(L, lua_upvalueindex(1));
  size_t ls = lua_strlen(L, lua_upvalueindex(1));
  const char *p = lua_tostring(L, lua_upvalueindex(2));
  const char *src;
  ms.L = L;
  ms.src_init = s;
  ms.src_end = s+ls;
  for (src = s + (size_t)lua_tonumber(L, lua_upvalueindex(3));
       src <= ms.src_end;
       src++) {
    const char *e;
    ms.level = 0;
    if ((e = match(&ms, src, p)) != NULL) {
      int newstart = e-s;
      if (e == src) newstart++;  /* empty match? go at least one position */
      lua_pushnumber(L, (lua_Number)newstart);
      lua_replace(L, lua_upvalueindex(3));
      return push_captures(&ms, src, e);
    }
  }
  return 0;  /* not found */
}


static int gfind (lua_State *L) {
  luaL_checkstring(L, 1);
  luaL_checkstring(L, 2);
  lua_settop(L, 2);
  lua_pushnumber(L, 0);
  lua_pushcclosure(L, gfind_aux, 3);
  return 1;
}


static void add_s (MatchState *ms, luaL_Buffer *b,
                   const char *s, const char *e) {
  lua_State *L = ms->L;
  if (lua_isstring(L, 3)) {
    const char *news = lua_tostring(L, 3);
    size_t l = lua_strlen(L, 3);
    size_t i;
    for (i=0; i<l; i++) {
      if (news[i] != ESC)
        luaL_putchar(b, news[i]);
      else {
        i++;  /* skip ESC */
        if (!isdigit(uchar(news[i])))
          luaL_putchar(b, news[i]);
        else {
          int level = check_capture(ms, news[i]);
          push_onecapture(ms, level);
          luaL_addvalue(b);  /* add capture to accumulated result */
        }
      }
    }
  }
  else {  /* is a function */
    int n;
    lua_pushvalue(L, 3);
    n = push_captures(ms, s, e);
    lua_call(L, n, 1);
    if (lua_isstring(L, -1))
      luaL_addvalue(b);  /* add return to accumulated result */
    else
      lua_pop(L, 1);  /* function result is not a string: pop it */
  }
}


static int str_gsub (lua_State *L) {
  size_t srcl;
  const char *src = luaL_checklstring(L, 1, &srcl);
  const char *p = luaL_checkstring(L, 2);
  int max_s = luaL_optint(L, 4, srcl+1);
  int anchor = (*p == '^') ? (p++, 1) : 0;
  int n = 0;
  MatchState ms;
  luaL_Buffer b;
  luaL_argcheck(L,
    lua_gettop(L) >= 3 && (lua_isstring(L, 3) || lua_isfunction(L, 3)),
    3, "string or function expected");
  luaL_buffinit(L, &b);
  ms.L = L;
  ms.src_init = src;
  ms.src_end = src+srcl;
  while (n < max_s) {
    const char *e;
    ms.level = 0;
    e = match(&ms, src, p);
    if (e) {
      n++;
      add_s(&ms, &b, src, e);
    }
    if (e && e>src) /* non empty match? */
      src = e;  /* skip it */
    else if (src < ms.src_end)
      luaL_putchar(&b, *src++);
    else break;
    if (anchor) break;
  }
  luaL_addlstring(&b, src, ms.src_end-src);
  luaL_pushresult(&b);
  lua_pushnumber(L, (lua_Number)n);  /* number of substitutions */
  return 2;
}

/* }====================================================== */


/* maximum size of each formatted item (> len(format('%99.99f', -1e308))) */
#define MAX_ITEM	512
/* maximum size of each format specification (such as '%-099.99d') */
#define MAX_FORMAT	20


static void luaI_addquoted (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  const char *s = luaL_checklstring(L, arg, &l);
  luaL_putchar(b, '"');
  while (l--) {
    switch (*s) {
      case '"': case '\\': case '\n': {
        luaL_putchar(b, '\\');
        luaL_putchar(b, *s);
        break;
      }
      case '\0': {
        luaL_addlstring(b, "\\000", 4);
        break;
      }
      default: {
        luaL_putchar(b, *s);
        break;
      }
    }
    s++;
  }
  luaL_putchar(b, '"');
}


static void luaI_addquotedbinary (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  if (lua_type(L, arg) == LUA_TWSTRING)
  {
	  const lua_WChar *s = luaL_checklwstring(L, arg, &l);
	  luaL_putchar(b, 'L');
	  luaL_putchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putchar(b, '\\');
			luaL_putchar(b, *s);
			break;
			case '\a':		luaL_putchar(b, '\\');  luaL_putchar(b, 'a');		break;
			case '\b':		luaL_putchar(b, '\\');  luaL_putchar(b, 'b');		break;
			case '\f':		luaL_putchar(b, '\\');  luaL_putchar(b, 'f');		break;
			case '\n':		luaL_putchar(b, '\\');  luaL_putchar(b, 'n');		break;
			case '\r':		luaL_putchar(b, '\\');  luaL_putchar(b, 'r');		break;
			case '\t':		luaL_putchar(b, '\\');  luaL_putchar(b, 't');		break;
			case '\v':		luaL_putchar(b, '\\');  luaL_putchar(b, 'v');		break;
			default:
				if (*s < 256  &&  isprint((unsigned char)*s))
				{
					luaL_putchar(b, *s);
				}
				else
				{
					char str[10];
					sprintf(str, "\\x%04x", (unsigned long)*s);
					luaL_addstring(b, str);
				}
		}
		s++;
	  }
	  luaL_putchar(b, '"');
  }
  else
  {
	  const char *s = luaL_checklstring(L, arg, &l);
	  luaL_putchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putchar(b, '\\');
			luaL_putchar(b, *s);
			break;
			case '\a':		luaL_putchar(b, '\\');  luaL_putchar(b, 'a');		break;
			case '\b':		luaL_putchar(b, '\\');  luaL_putchar(b, 'b');		break;
			case '\f':		luaL_putchar(b, '\\');  luaL_putchar(b, 'f');		break;
			case '\n':		luaL_putchar(b, '\\');  luaL_putchar(b, 'n');		break;
			case '\r':		luaL_putchar(b, '\\');  luaL_putchar(b, 'r');		break;
			case '\t':		luaL_putchar(b, '\\');  luaL_putchar(b, 't');		break;
			case '\v':		luaL_putchar(b, '\\');  luaL_putchar(b, 'v');		break;
			default:
				if (isprint((unsigned char)*s))
				{
					luaL_putchar(b, *s);
				}
				else
				{
					char str[10];
					sprintf(str, "\\x%02x", (unsigned long)(unsigned char)*s);
					luaL_addstring(b, str);
				}
		}
		s++;
	  }
	  luaL_putchar(b, '"');

/*
	  const char *s = luaL_checklstring(L, arg, &l);
	  luaL_putchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putchar(b, '\\');
			luaL_putchar(b, *s);
			break;
		  case '\0': luaL_addlstring(b, "\\000", 4); break;
		  default:
			  if (isprint(*s)  &&  *s != 13  &&  *s != 10)
				  luaL_putchar(b, *s);
			  else
			  {
				  char buf[5];
				  sprintf(buf, "\\%03d", *s);
				  luaL_addlstring(b, buf, 4);
			  }
		}
		s++;
	  }
	  luaL_putchar(b, '"');
*/
  }
}


/*
static void luaI_addquotedbinary (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  if (lua_type(L, arg) == LUA_TWSTRING)
  {
	  const lua_WChar *s = luaL_checklwstring(L, arg, &l);
	  luaL_putchar(b, 'L');
	  luaL_putchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putchar(b, '\\');
			luaL_putchar(b, *s);
			break;
			case '\a':		luaL_putchar(b, '\\');  luaL_putchar(b, 'a');		break;
			case '\b':		luaL_putchar(b, '\\');  luaL_putchar(b, 'b');		break;
			case '\f':		luaL_putchar(b, '\\');  luaL_putchar(b, 'f');		break;
			case '\n':		luaL_putchar(b, '\\');  luaL_putchar(b, 'n');		break;
			case '\r':		luaL_putchar(b, '\\');  luaL_putchar(b, 'r');		break;
			case '\t':		luaL_putchar(b, '\\');  luaL_putchar(b, 't');		break;
			case '\v':		luaL_putchar(b, '\\');  luaL_putchar(b, 'v');		break;
			default:
				if (*s < 256  &&  isprint((unsigned char)*s))
				{
					luaL_putchar(b, *s);
				}
				else
				{
					char str[10];
					sprintf(str, "\\x%04x", (unsigned long)*s);
					luaL_addstring(b, str);
				}
		}
		s++;
	  }
	  luaL_putchar(b, '"');
  }
  else
  {
	  const char *s = luaL_checklstring(L, arg, &l);
	  luaL_putchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putchar(b, '\\');
			luaL_putchar(b, *s);
			break;
		  case '\0': luaL_addlstring(b, "\\000", 4); break;
		  default:
			  if (isprint(*s)  &&  *s != 13  &&  *s != 10)
				  luaL_putchar(b, *s);
			  else
			  {
				  char buf[5];
				  sprintf(buf, "\\%03d", *s);
				  luaL_addlstring(b, buf, 4);
			  }
		}
		s++;
	  }
	  luaL_putchar(b, '"');
  }
}
*/

static const char *scanformat (lua_State *L, const char *strfrmt,
                                 char *form, int *hasprecision) {
  const char *p = strfrmt;
  while (strchr("-+ #0", *p)) p++;  /* skip flags */
  if (isdigit(uchar(*p))) p++;  /* skip width */
  if (isdigit(uchar(*p))) p++;  /* (2 digits at most) */
  if (*p == '.') {
    p++;
    *hasprecision = 1;
    if (isdigit(uchar(*p))) p++;  /* skip precision */
    if (isdigit(uchar(*p))) p++;  /* (2 digits at most) */
  }
  if (isdigit(uchar(*p)))
    luaL_error(L, "invalid format (width or precision too long)");
  if (p-strfrmt+2 > MAX_FORMAT)  /* +2 to include `%' and the specifier */
    luaL_error(L, "invalid format (too long)");
  form[0] = '%';
  strncpy(form+1, strfrmt, p-strfrmt+1);
  form[p-strfrmt+2] = 0;
  return p;
}


int str_format_helper (luaL_Buffer* b, lua_State *L, int arg) {
  size_t sfl;
  const char *strfrmt = luaL_checklstring(L, arg, &sfl);
  const char *strfrmt_end = strfrmt+sfl;
  luaL_buffinit(L, b);
  while (strfrmt < strfrmt_end) {
    if (*strfrmt != '%')
      luaL_putchar(b, *strfrmt++);
    else if (*++strfrmt == '%')
      luaL_putchar(b, *strfrmt++);  /* %% */
    else { /* format item */
      char form[MAX_FORMAT];  /* to store the format (`%...') */
      char buff[MAX_ITEM];  /* to store the formatted item */
      int hasprecision = 0;
      if (isdigit(uchar(*strfrmt)) && *(strfrmt+1) == '$')
        return luaL_error(L, "obsolete option (d$) to `format'");
      arg++;
      strfrmt = scanformat(L, strfrmt, form, &hasprecision);
      switch (*strfrmt++) {
        case 'c':  case 'd':  case 'i': {
          sprintf(buff, form, luaL_checkint(L, arg));
          break;
        }
        case 'B': {
          if (!lua_isboolean(L, arg)  &&  !lua_isnil(L, arg))
            luaL_typerror(L, arg, lua_typename(L, LUA_TBOOLEAN));
		  strcpy(buff, lua_toboolean(L, arg) ? "true" : "false");
          break;
        }
        case 'o':  case 'u':  case 'x':  case 'X': {
          sprintf(buff, form, (unsigned int)(luaL_checknumber(L, arg)));
          break;
        }
        case 'e':  case 'E': case 'f':
        case 'g': case 'G': {
          sprintf(buff, form, luaL_checknumber(L, arg));
          break;
        }
        case 'q': {
          luaI_addquoted(L, b, arg);
          continue;  /* skip the `addsize' at the end */
        }
        case 'Q': {
          luaI_addquotedbinary(L, b, arg);
          continue;  /* skip the `addsize' at the end */
        }
        case 's': {
          size_t l;
          const char *s = luaL_checklstring(L, arg, &l);
          if (!hasprecision && l >= 100) {
            /* no precision and string is too long to be formatted;
               keep original string */
            lua_pushvalue(L, arg);
            luaL_addvalue(b);
            continue;  /* skip the `addsize' at the end */
          }
          else {
            sprintf(buff, form, s);
            break;
          }
        }
        case 'b':
		{
		  buff[1] = buff[2] = buff[3] = buff[4] = buff[5] = buff[6] = buff[7] = buff[8] = 0;
		  switch (*strfrmt++)
		  {
		  case 'b': {
            unsigned int num = (unsigned int)luaL_checkint(L, arg);
		    buff[0] = (unsigned char)num;
            luaL_addlstring(b, buff, 1);
			break;
          }
		  case 'd': {
            unsigned int num = (unsigned int)luaL_checkint(L, arg);
		    *(unsigned int*)(&buff) = num;
            luaL_addlstring(b, buff, 4);
			break;
          }
		  case 'w': {
            unsigned int num = (unsigned int)luaL_checkint(L, arg);
			*(unsigned short*)(&buff) = (unsigned short)num;
            luaL_addlstring(b, buff, 2);
			break;
          }
		  case 'f': {
            float numF = (float)luaL_checknumber(L, arg);
			*(float*)(&buff) = numF;
            luaL_addlstring(b, buff, 4);
			break;
          }
		  case 'F': {
            double numD = (double)luaL_checknumber(L, arg);
			*(double*)(&buff) = numD;
            luaL_addlstring(b, buff, 8);
			break;
          }

		  default:
			  break;
		  }
		  buff[0] = 0;

		  break;
		}
        default: {  /* also treat cases `pnLlh' */
          return luaL_error(L, "invalid option to `format'");
        }
      }
      luaL_addlstring(b, buff, strlen(buff));
    }
  }

  return 1;
}


static int str_lualex(lua_State *L) {
  size_t l = 0;
  const char *str = luaL_checklstring(L, 1, &l);
  int isWide = luaL_checkint(L, 2) != 0;
  size_t i;
  luaL_Buffer b;
  luaL_buffinit(L, &b);

  for (i = 0; i < l; ++i)
  {
	int needWideZero = 1;

    switch (str[i])
	{
      case '\\':
		++i;
        switch (str[i]) {
          case 'a': luaL_putchar(&b, '\a'); break;
          case 'b': luaL_putchar(&b, '\b'); break;
          case 'f': luaL_putchar(&b, '\f'); break;
          case 'n': luaL_putchar(&b, '\n'); break;
          case 'r': luaL_putchar(&b, '\r'); break;
          case 't': luaL_putchar(&b, '\t'); break;
          case 'v': luaL_putchar(&b, '\v'); break;
          case 'x': {
			  int ch;
			  ++i;
			  ch = tolower(str[i]);
              if (!isdigit(ch) && !(ch >= 'a' && ch <= 'f') )
			  {
				  --i;
				  luaL_putchar(&b, 'x');
			  }
			  else {  /* \xxx */
				  size_t start = i;
				  int c = 0;
				  size_t numDigits = isWide ? 4 : 2;
				  do {
					  ch = tolower(str[i]);
					  if (isdigit(ch))
					    c = 16*c + (ch-'0');
					  else if (ch >= 'a' && ch <= 'f')
						c = 16*c + (ch-'a') + 10;
					  ++i;
					  ch = tolower(str[i]);
				  } while (i - start < numDigits && (isdigit(ch) || (ch >= 'a' && ch <= 'f')));
				  luaL_putchar(&b, (c & 0xff));
				  luaL_putchar(&b, ((c >> 8) & 0xff));
				  needWideZero = 0;
			  }
			  break;
          }
          default: {
            if (!isdigit(str[i]))
				luaL_putchar(&b, str[i]);
            else {  /* \xxx */
              int c = 0;
              int count = 0;
              do {
                c = 10*c + (str[i]-'0');
				++i;
              } while (++count<3 && isdigit(str[i]));
              luaL_putchar(&b, c);
            }
          }
        }
        break;
      default:
		  luaL_putchar(&b, str[i]);
    }
	if (isWide  &&  needWideZero)
	  luaL_putchar(&b, str[i]);
  }

  luaL_pushresult(&b);
  return 1;
}


static int str_format (lua_State *L) {
  luaL_Buffer b;
  int ret = str_format_helper(&b, L, 1);
  if (ret == 0)
	  return 0;
  luaL_pushresult(&b);
  return 1;
}


static int str_replace(lua_State *L) {
  size_t l1, l2, l3;
  const char *src = luaL_checklstring(L, 1, &l1);
  const char *p = luaL_checklstring(L, 2, &l2);
  const char *p2 = luaL_checklstring(L, 3, &l3);
  const char *s2;
  int n = 0;
  int init = 0;

  luaL_Buffer b;
  luaL_buffinit(L, &b);

   while (1) {
     s2 = lmemfind(src+init, l1-init, p, l2);
     if (s2) {
	     luaL_addlstring(&b, src+init, s2-(src+init));
	     luaL_addlstring(&b, p2, l3);
	     init = init + (s2-(src+init)) + l2;
	     n++;
     } else {
	    luaL_addlstring(&b, src+init, l1-init);
	    break;
     }
   }


   luaL_pushresult(&b);
   lua_pushnumber(L, (lua_Number)n);  /* number of substitutions */
   return 2;
}




static const luaL_reg strlib[] = {
  {"len", str_len},
  {"sub", str_sub},
  {"lower", str_lower},
  {"upper", str_upper},
  {"char", str_char},
  {"rep", str_rep},
  {"byte", str_byte},
  {"format", str_format},
  {"dump", str_dump},
  {"find", str_find},
  {"gfind", gfind},
  {"gsub", str_gsub},
  {"lualex", str_lualex},
  {"replace", str_replace},
  {NULL, NULL}
};


#include "malloc.h"

typedef struct Mbuffer {
  char *buffer;
  size_t buffsize;
} Mbuffer;


#define LUA_MINBUFFER	32

char *openspace (lua_State *L, Mbuffer *buff, size_t n);

#define luaZ_initbuffer(L, buff) ((buff)->buffer = NULL, (buff)->buffsize = 0)

#define luaZ_sizebuffer(buff)	((buff)->buffsize)
#define luaZ_buffer(buff)	((buff)->buffer)

#define luaZ_resizebuffer(L, buff, size) \
	((buff)->buffer = (char*)realloc((buff)->buffer, size), \
	(buff)->buffsize = size)

#define luaZ_freebuffer(L, buff)	luaZ_resizebuffer(L, buff, 0)

/* ------------------------------------------------------------------------ */
#if defined(_MSC_VER)
#pragma warning(disable: 4100)
#pragma warning(disable: 4244)
#endif

char *openspace (lua_State *L, Mbuffer *buff, size_t n) {
  if (n > buff->buffsize) {
    if (n < LUA_MINBUFFER) n = LUA_MINBUFFER;
    buff->buffer = (char*)realloc(buff->buffer, n);
    buff->buffsize = n;
  }
  return buff->buffer;
}

/*
* lpack.c
* a Lua library for packing and unpacking binary data
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 27 Apr 2004 00:08:42
* This code is hereby placed in the public domain.
* with contributions from Ignacio Casta±o <castanyo@yahoo.es> and
* Roberto Ierusalimschy <roberto@inf.puc-rio.br>.
*/

#define	OP_ZSTRING	'z'		/* zero-terminated string */
#define	OP_BSTRING	'p'		/* string preceded by length byte */
#define	OP_WSTRING	'P'		/* string preceded by length word */
#define	OP_SSTRING	'a'		/* string preceded by length size_t */
#define	OP_STRING	'A'		/* string */
#define	OP_FLOAT	'f'		/* float */
#define	OP_DOUBLE	'd'		/* double */
#define	OP_NUMBER	'n'		/* Lua number */
#define	OP_CHAR		'c'		/* char */
#define	OP_BYTE		'b'		/* byte = unsigned char */
#define	OP_SHORT	'h'		/* short */
#define	OP_USHORT	'H'		/* unsigned short */
#define	OP_INT		'i'		/* int */
#define	OP_UINT		'I'		/* unsigned int */
#define	OP_LONG		'l'		/* long */
#define	OP_ULONG	'L'		/* unsigned long */
#define	OP_LITTLEENDIAN	'<'		/* little endian */
#define	OP_BIGENDIAN	'>'		/* big endian */
#define	OP_NATIVE	'='		/* native endian */

#define	OP_BOOLEAN	'B'		/* boolean = unsigned char */
#define	OP_WIDESTRING	'w'
#define OP_PAD '@'

#include <ctype.h>
#include <string.h>

//#include "lualib.h"
//#include "lauxlib.h"

static void badcode(lua_State *L, int c)
{
 char s[]="bad code `?'";
 s[sizeof(s)-3]=c;
 luaL_argerror(L,1,s);
}

static int doendian(int c)
{
 int x=1;
 int e=*(char*)&x;
 if (c==OP_LITTLEENDIAN) return !e;
 if (c==OP_BIGENDIAN) return e;
 if (c==OP_NATIVE) return 0;
 return 0;
}

static void doswap(int swap, void *p, size_t n)
{
 if (swap)
 {
  char *a=(char*)p;
  int i,j;
  for (i=0, j=n-1, n=n/2; n--; i++, j--)
  {
   char t=a[i]; a[i]=a[j]; a[j]=t;
  }
 }
}

static void dowideswap(int swap, void *p, size_t n)
{
	if (swap)
	{
		char *a=(char*)p;
		size_t i;
		for (i = 0; i < n; ++i)
		{
			char temp = a[0];
			a[0] = a[1];
			a[1] = temp;
			a += 2;
		}
	}
}

#define UNPACKNUMBER(OP,T)		\
   case OP:				\
   {					\
    T a;				\
    int m=sizeof(a);			\
    if (i+m>(int)len) goto done;		\
    memcpy(&a,s+i,m);			\
    i+=m;				\
    doswap(swap,&a,m);			\
    lua_pushnumber(L,(lua_Number)a);	\
    ++n;				\
    break;				\
   }

#define UNPACKSTRING(OP,T)						\
	case OP:									\
	{											\
		T l;									\
		int m=sizeof(l);						\
		if (i+m>(int)len) goto done;			\
		memcpy(&l,s+i,m);						\
		doswap(swap,&l,m);						\
		if (isWide)								\
		{										\
			if (i+m+l*2>(int)len) goto done;	\
		}										\
		else									\
		{										\
			if (i+m+l>(int)len) goto done;		\
		}										\
		i+=m;									\
		if (isWide)								\
		{										\
			lua_WChar* out=(lua_WChar *)openspace(L,&buff,l*2);	\
			memcpy(out, s+i, l * 2);			\
			dowideswap(swap, (void*)out, l);	\
			lua_pushlwstring(L,out,l);			\
			i += l * 2;							\
		}										\
		else									\
		{										\
			lua_pushlstring(L,s+i,l);			\
			i+=l;								\
		}										\
		++n;									\
		break;									\
	}

static size_t lua_WChar_len(const lua_WChar* str)
{
	const lua_WChar* ptr = str;
	while (*ptr != 0)
	{
		ptr++;
	}

	return (size_t)(ptr - str);
}

static int l_unpack(lua_State *L) 		/** unpack(s,f,[init]) */
{
	Mbuffer buff;
	const char *s=luaL_checkstring(L,1);
	const char *f=luaL_checkstring(L,2);
	int i=(int)luaL_optnumber(L,3,1)-1;
	size_t len=lua_strlen(L,1);
	int n=0;
	int swap=0;
	luaZ_initbuffer(L, &buff);
	lua_pushnil(L);
	while (*f)
	{
		int c=*f++;
		int N=0;
		int isWide = 0;
		if (c == OP_WIDESTRING)
		{
			isWide = 1;
			c=*f++;
			if (!c)
				break;
		}
		while (isdigit(*f)) N=10*N+(*f++)-'0';
		if (N==0) N=1;
		while (N--)
		{
			switch (c)
			{
				case OP_LITTLEENDIAN:
				case OP_BIGENDIAN:
				case OP_NATIVE:
				{
					swap=doendian(c);
					N=0;
					break;
				}
				case OP_STRING:
				{
					++N;
					if (isWide)
					{
						lua_WChar* out;
						if (i+N*2>(int)len) goto done;
						out=(lua_WChar *)openspace(L,&buff,N*2);
						memcpy(out, s+i, N * 2);
						dowideswap(swap, (void*)out, N);
						lua_pushlwstring(L,out,N);
						i+=N*2;
					}
					else
					{
						if (i+N>(int)len) goto done;
						lua_pushlstring(L,s+i,N);
						i+=N;
					}
					++n;
					N=0;
					break;
				}
				case OP_ZSTRING:
				{
					size_t l;
					if (i>=(int)len) goto done;
					if (isWide)
					{
						lua_WChar* out;
						l=lua_WChar_len((const lua_WChar*)(s+i));
						out=(lua_WChar *)openspace(L,&buff,l*2);
						memcpy(out, s+i, l * 2);
						dowideswap(swap, (void*)out, l);
						lua_pushlwstring(L,out,l);
						i+=(l+1) * 2;
					}
					else
					{
						l=strlen(s+i);
						lua_pushlstring(L,s+i,l);
						i+=l+1;
					}
					++n;
					break;
				}
				UNPACKSTRING(OP_BSTRING, unsigned char)
				UNPACKSTRING(OP_WSTRING, unsigned short)
				UNPACKSTRING(OP_SSTRING, size_t)
				UNPACKNUMBER(OP_NUMBER, lua_Number)
				UNPACKNUMBER(OP_DOUBLE, double)
				UNPACKNUMBER(OP_FLOAT, float)
				UNPACKNUMBER(OP_CHAR, char)
				UNPACKNUMBER(OP_BYTE, unsigned char)
				UNPACKNUMBER(OP_SHORT, short)
				UNPACKNUMBER(OP_USHORT, unsigned short)
				UNPACKNUMBER(OP_INT, int)
				UNPACKNUMBER(OP_UINT, unsigned int)
				UNPACKNUMBER(OP_LONG, long)
				UNPACKNUMBER(OP_ULONG, unsigned long)
				case OP_BOOLEAN:
				{
					unsigned char a;
					int m=sizeof(a);
					if (i+m>(int)len) goto done;
					memcpy(&a,s+i,m);
					i+=m;
					doswap(swap,&a,m);
					lua_pushboolean(L,a != 0);
					++n;				\
					break;				\
				}


			case ' ': case ',':
				break;
			default:
				badcode(L,c);
				break;
			}
		}
	}
done:
	lua_pushnumber(L,i+1);
	lua_replace(L,-n-2);
	luaZ_freebuffer(L, &buff);
	return n+1;
}

#define PACKNUMBER(OP,T)			\
   case OP:					\
   {						\
    T a=(T)luaL_checknumber(L,i++);		\
    doswap(swap,&a,sizeof(a));			\
    luaL_addlstring(&b,(const char*)&a,sizeof(a));	\
    break;					\
   }

#define PACKSTRING(OP,T)									\
	case OP:												\
	{														\
		size_t l;											\
		const char *a = NULL;								\
		const lua_WChar *wa = NULL;							\
		T ll;    											\
		if (isWide)											\
			wa=luaL_checklwstring(L,i++,&l);				\
		else												\
			a=luaL_checklstring(L,i++,&l);					\
		ll=(T)l;											\
		doswap(swap,&ll,sizeof(ll));						\
		luaL_addlstring(&b,(const char*)&ll,sizeof(ll));	\
		if (isWide)											\
		{													\
			size_t curPos = b.p - b.buffer;					\
			luaL_addlwstring(&b,wa,l);						\
			dowideswap(swap,b.buffer+curPos,l);				\
		}													\
		else												\
		{													\
			luaL_addlstring(&b, a, l);						\
		}													\
		break;												\
	}

static int l_pack(lua_State *L) 		/** pack(f,...) */
{
	int i=2;
	const char *f=luaL_checkstring(L,1);
	int swap=0;
	int padCount = 0;
	char padChar = 0;
	size_t padStartPos = 0;
	luaL_Buffer b;
	luaL_buffinit(L,&b);
	while (*f)
	{
		int c=*f++;
		int N=0;
		int isWide = 0;
		if (c == OP_WIDESTRING)
		{
			isWide = 1;
			c=*f++;
			if (!c)
				break;
		}
		else if (c == OP_PAD)
		{
			if (padCount == 0)
			{
				while (isdigit(*f))
					padCount = 10 * padCount + (*f++) - '0';
				if (*f == ':')
				{
					f++;
					while (isdigit(*f))
						padChar = 10 * (unsigned char)padChar + (*f++) - '0';
				}
				padStartPos = b.p - b.buffer;
			}
			else
			{
				size_t curPos = b.p - b.buffer;
				padCount -= curPos - padStartPos;
				while (padCount-- > 0)
					luaL_addlstring(&b, &padChar, 1);
			}
			continue;
		}
		while (isdigit(*f)) N=10*N+(*f++)-'0';
		if (N==0) N=1;
		while (N--)
		{
			switch (c)
			{
				case OP_LITTLEENDIAN:
				case OP_BIGENDIAN:
				case OP_NATIVE:
				{
					swap=doendian(c);
					N=0;
					break;
				}
				case OP_STRING:
				case OP_ZSTRING:
				{
					size_t l;
					if (isWide)
					{
						const lua_WChar *a=luaL_checklwstring(L,i++,&l);
						size_t curPos = b.p - b.buffer;
						luaL_addlwstring(&b,a,l+(c==OP_ZSTRING));
						dowideswap(swap,b.buffer+curPos,l+(c==OP_ZSTRING));
					}
					else
					{
						const char *a=luaL_checklstring(L,i++,&l);
						luaL_addlstring(&b,a,l+(c==OP_ZSTRING));
					}
					break;
				}
				PACKSTRING(OP_BSTRING, unsigned char)
				PACKSTRING(OP_WSTRING, unsigned short)
				PACKSTRING(OP_SSTRING, size_t)
				PACKNUMBER(OP_NUMBER, lua_Number)
				PACKNUMBER(OP_DOUBLE, double)
				PACKNUMBER(OP_FLOAT, float)
				PACKNUMBER(OP_CHAR, char)
				PACKNUMBER(OP_BYTE, unsigned char)
				PACKNUMBER(OP_SHORT, short)
				PACKNUMBER(OP_USHORT, unsigned short)
				PACKNUMBER(OP_INT, int)
				PACKNUMBER(OP_UINT, unsigned int)
				PACKNUMBER(OP_LONG, long)
				PACKNUMBER(OP_ULONG, unsigned long)
				case OP_BOOLEAN:
				{
					unsigned char a;
					if (!lua_isboolean(L, i))
						luaL_error(L, "expected a boolean");
					a = (unsigned char)lua_toboolean(L, i++);
					luaL_addlstring(&b,(const char*)&a,sizeof(a));
					break;
				}
				case ' ': case ',':
					break;
				default:
					badcode(L,c);
					break;
			}
		}
	}
	luaL_pushresult(&b);
	return 1;
}

static const luaL_reg R[] =
{
	{"pack",	l_pack},
	{"unpack",	l_unpack},
	{NULL,	NULL}
};

int luaopen_pack(lua_State *L)
{
#ifdef USE_GLOBALS
 lua_register(L,"bpack",l_pack);
 lua_register(L,"bunpack",l_unpack);
#else
 luaL_openlib(L, "string", R, 0);
#endif
 return 0;
}


/*
** Open string library
*/
LUALIB_API int luaopen_string (lua_State *L) {
  luaL_openlib(L, LUA_STRLIBNAME, strlib, 0);
  lua_getglobal(L, LUA_STRLIBNAME);
  lua_setdefaultmetatable(L, LUA_TSTRING);
  luaopen_pack(L);
  return 1;
}

NAMESPACE_LUA_END
