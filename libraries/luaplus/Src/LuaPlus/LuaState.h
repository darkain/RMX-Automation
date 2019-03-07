///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2005 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER
#ifndef LUASTATE_H
#define LUASTATE_H

#include "LuaPlusInternal.h"

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

/**
	A lua_State wrapper.
**/
class LUAPLUS_CLASS LuaState
{
public:
	enum ErrorCodes
	{
		/* error codes for `lua_load' and `lua_pcall' */
		ERRRUN = 1,
		ERRFILE = 2,
		ERRSYNTAX = 3,
		ERRMEM = 4,
		ERRERR = 5,
	};

	enum Types
	{
		TNONE			= (-1),
		TNIL			= 0,
		TBOOLEAN		= 1,
		TLIGHTUSERDATA	= 2,
		TNUMBER			= 3,
		TSTRING			= 4,
		TTABLE			= 5,
		TFUNCTION		= 6,
		TUSERDATA		= 7,
		TTHREAD			= 8,
		TWSTRING		= 9,
		NTYPES			= 11,
	};

	enum DumpObjectTypes
	{
		DUMP_ALPHABETICAL		= 0x00000001,
		DUMP_WRITEALL			= 0x00000002,
		DUMP_WRITETABLEPOINTERS = 0x00000004,
	};


	///////////////////////////////////////////////////////////////////////////
	static LuaState* Create(bool initStandardLibrary = true, bool multithreaded = false);
	static LuaState* CreateThread(LuaState* parentState);
	static LuaState* CastState(lua_State* L);
	static void Destroy(LuaState* state);

	lua_CFunction AtPanic(lua_CFunction panicf);

	// LuaObject
	LuaObject GetGlobals() throw();
	LuaObject GetGlobal(const char *name);
	LuaObject GetRegistry();

	int Equal(const LuaObject& o1, const LuaObject& o2);
	int LessThan(const LuaObject& o1, const LuaObject& o2);

	// Stack functions.
	LuaStackObject Stack(int index);
	LuaStackObject StackTop();
	int GetTop();
	void SetTop(int index);
	void PushValue(int index);
	void PushValue(LuaStackObject& object);
	void Remove(int index);
	void Insert(int index);
	void Replace(int index);
	int CheckStack(int size);
	void XMove(LuaState* to, int n);
	int Equal(int index1, int index2);
	int RawEqual(int index1, int index2);
	int LessThan(int index1, int index2);

	// push functions (C -> stack)
	LuaStackObject PushNil();
	LuaStackObject PushInteger(int n);
	LuaStackObject PushNumber(lua_Number n);
	LuaStackObject PushLString(const char *s, size_t len);
	LuaStackObject PushLWString(const lua_WChar* s, size_t len);
	LuaStackObject PushString(const char *s);
	LuaStackObject PushWString(const lua_WChar* s);
	const char *PushVFString(const char *fmt, va_list argp);
	LuaStackObject PushCClosure(lua_CFunction fn, int n);

	LuaStackObject PushCClosure(int (*f)(LuaState*), int n);

	template <class Callee>
	void PushCClosure(const Callee& callee, int (Callee::*f)(LuaState*), int n)
	{
        PushCClosureHelper(&LPCD::LuaStateMemberDispatcherHelper<Callee>::LuaStateMemberDispatcher, &callee, sizeof(Callee), &f, sizeof(f), n);
	}

	LuaStackObject PushCFunction(lua_CFunction f);
	LuaStackObject PushBoolean(bool value);
	LuaStackObject PushLightUserData(void* p);

	// get functions (Lua -> stack)
	void GetTable(int index);
	void RawGet(int index);
	void RawGetI(int index, int n);
	LuaStackObject NewTable(int size = 0);
	void* NewUserData(size_t size);
	LuaStackObject GetMetaTable(int objindex);
	LuaStackObject GetDefaultMetaTable(int type);
	LuaStackObject GetGlobals_Stack();					// Backward compatible.
	LuaStackObject GetGlobal_Stack(const char *name);
	LuaStackObject GetRegistry_Stack();
	LuaStackObject GetRef(int ref);


	// set functions(stack -> Lua)
	void SetTable(int index);
	void RawSet(int index);
	void RawSetI(int index, int n);
	void SetMetaTable(int index);
	void SetDefaultMetaTable(int type);
	void SetGlobals(int index);
	void SetFEnv( int index );
	void SetGlobal(const char *name);
	int Ref(int lock);
	void Unref(int ref);

	void Call(int nargs, int nresults);
	int PCall(int nargs, int nresults, int errf);
	int CPCall(lua_CFunction func, void* ud);

	int Dump(lua_Chunkwriter writer, void* data);

    // `load' and `do' functions (load and run Lua code)
	int LoadFile(const char* filename);
	int DoFile(const char *filename);
	int DoFile(const char *filename, LuaObject& fenvObj);
	int LoadString(const char* str);
	int DoString(const char *str);
	int DoString(const char *str, LuaObject& fenvObj);
	int LoadWString(const lua_WChar* str);
	int DoWString(const lua_WChar* str, const char* name);
	int LoadBuffer(const char* buff, size_t size, const char* name);
	int DoBuffer(const char *buff, size_t size, const char *name);
	int DoBuffer(const char *buff, size_t size, const char *name, LuaObject& fenvObj);
	int LoadWBuffer(const lua_WChar* buff, size_t size, const char* name);
	int DoWBuffer(const lua_WChar* buff, size_t size, const char *name);

	// Coroutine functions
	int CoYield(int nresults);
	int CoResume(int narg);

	// Garbage-collection functions
	int GetGCThreshold();
	int GetGCCount();
	void SetGCThreshold(int newthreshold);

	// Miscellaneous functions
	int Error();

	int Next(int index);
	int GetN(int index);

	void Concat(int n);

	LuaObject NewUserDataBox(void* u);
	LuaStackObject NewUserDataBox_Stack(void* u);

	// Helper functions
	void Pop();
	void Pop(int amount);

	void CollectGarbage();

	bool DumpObject(const char* filename, const char* name, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);
	bool DumpObject(const char* filename, LuaObject& key, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);

	bool DumpObject(LuaStateOutFile& file, const char* name, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);
	bool DumpObject(LuaStateOutFile& file, LuaObject& key, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);

	bool DumpGlobals(const char* filename, unsigned int flags = DUMP_ALPHABETICAL, unsigned int maxIndentLevel = 0xFFFFFFFF);
	bool DumpGlobals(LuaStateOutFile& file, unsigned int flags = DUMP_ALPHABETICAL, unsigned int maxIndentLevel = 0xFFFFFFFF);

	operator lua_State*()						{  return m_state;  }
	lua_State* GetCState()						{  return m_state;  }

	// Arg functions.
	int TypeError(int narg, const char* tname);
	int ArgError(int narg, const char* extramsg);
	const char* CheckLString(int numArg, size_t* len);
	const char* OptLString(int numArg, const char *def, size_t* len);
	lua_Number CheckNumber(int numArg);
	lua_Number OptNumber(int nArg, lua_Number def);
	void ArgCheck(bool condition, int numarg, const char* extramsg);
	const char* CheckString(int numArg);
	const char* OptString(int numArg, const char* def);
	int CheckInt(int numArg);
	long CheckLong(int numArg);
	int OptInt(int numArg, int def);
	long OptLong(int numArg, int def);
	void CheckStack(int sz, const char* msg);
	void CheckType(int narg, int t);
	void CheckAny(int narg);
	void* CheckUData(int ud, const char* tname);

	// Debug functions.
	int GetStack(int level, lua_Debug* ar);
	int GetInfo(const char* what, lua_Debug* ar);
	const char* GetLocal(const lua_Debug* ar, int n);
	const char* SetLocal(const lua_Debug* ar, int n);

	int SetHook(lua_Hook func, int mask, int count);
	lua_Hook GetHook();
	unsigned long GetHookMask();

	// Extra
	LuaStackObject BoxPointer(void* u);
	void* UnBoxPointer(int stackIndex);

	int UpValueIndex(int i);

	LuaObject GetLocalByName( int level, const char* name );

protected:
	friend class LuaObject;

	LuaState(bool initStandardLibrary = true, bool multithreaded = false);
	LuaState(LuaState* script, bool initStandardLibrary = true);
	LuaState(lua_State* L);
	~LuaState();
	LuaState& operator=(LuaState& src);		// Not implemented.

	void Init(bool initStandardLibrary);

	bool CallFormatting(LuaObject& tableObj, LuaStateOutFile& file, int indentLevel,
			bool writeAll, bool alphabetical, bool writeTablePointers,
			unsigned int maxIndentLevel);

	void SetupStateEx();
	static void LuaPlusGCFunction(void* s);
    LuaStackObject PushCClosureHelper(lua_CFunction cf, void* callee, unsigned int sizeof_Callee, void* f, unsigned int sizeof_f, int n);

	struct MiniLuaObject
	{
		LuaObject* m_next;		   // only valid when in free list
		LuaObject* m_prev;		   // only valid when in used list
	};

	// We want the storage space, but we can't have the constructor run.
	MiniLuaObject* GetHeadObject()		{  return &m_headObject;  }
	MiniLuaObject* GetTailObject()		{  return &m_tailObject;  }

	lua_State* m_state;
	MiniLuaObject m_headObject;
	MiniLuaObject m_tailObject;
	LuaObject* m_threadObj;
	bool m_ownState;
};


class LuaStateAuto
{
public:
    operator LuaState*()							{  return m_state;  }
    operator const LuaState*() const				{  return m_state;  }
    operator LuaState*() const						{  return m_state;  }
    LuaState& operator*()							{  return *m_state; }
    const LuaState& operator*() const				{  return *m_state; }
    LuaState* operator->()							{  return m_state;  }
    const LuaState* operator->() const				{  return m_state;  }
	LuaState* Get() const							{  return m_state;  }

	LuaStateAuto() : m_state(NULL) {}
    LuaStateAuto(LuaState* newState) : m_state(newState) {}
	LuaStateAuto& operator=(LuaState* newState)
	{
		Assign(newState);
		return *this;
	}

	~LuaStateAuto()
	{
		Assign(NULL);
	}

    LuaState* m_state;

protected:
    LuaStateAuto(const LuaStateAuto&);					// Not implemented.
    LuaStateAuto& operator=(const LuaStateAuto&);		// Not implemented.

	void Assign(LuaState* state)
	{
		if (m_state)
			LuaState::Destroy(m_state);
		m_state = state;
	}
};


class LuaStateOwner : public LuaStateAuto
{
public:
    LuaStateOwner(bool initStandardLibrary = true)
	{
		m_state = LuaState::Create(initStandardLibrary);
	}

    LuaStateOwner(LuaState* newState) : LuaStateAuto(newState) {}
	LuaStateOwner& operator=(LuaState* newState)
	{
		Assign(newState);
		return *this;
	}

	~LuaStateOwner()
	{
		Assign(NULL);
	}

private:
    LuaStateOwner(const LuaStateOwner&);				// Not implemented.
    LuaStateOwner& operator=(const LuaStateOwner&);		// Not implemented.
};


} // namespace LuaPlus



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef LUAPLUS_ENABLE_INLINES
#include "LuaState.inl"
#endif // LUAPLUS_ENABLE_INLINES


#endif // LUASTATE_H
