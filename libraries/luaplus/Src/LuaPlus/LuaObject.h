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
#ifndef LUAOBJECT_H
#define LUAOBJECT_H

#include "LuaPlusInternal.h"

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

/**
	Representation of a Lua object.
**/
class LUAPLUS_CLASS LuaObject
{
public:
	LuaObject();
	LuaObject(LuaState* state) throw();
	LuaObject(LuaState* state, int stackIndex) throw();
	LuaObject(LuaState* state, const lua_TObject* obj);
	LuaObject(const LuaObject& src) throw();
	LuaObject(const LuaStackObject& src) throw();
	LuaObject& operator=(const LuaObject& src) throw();
	LuaObject& operator=(const LuaStackObject& src) throw();

/*	template <typename T>
	LuaObject& operator=(const T& value)
	{
		assert(m_state);
		LCD::Assign(*this, value);
		return *this;
	}*/

	~LuaObject();

	void Reset();

	/**
		Retrieves the LuaState object associated with this LuaObject.
	**/
	LuaState* GetState() const;
	lua_State* GetCState() const;
	lua_TObject* GetTObject() const;

	bool operator==(const LuaObject& right) const;
	bool operator<(const LuaObject& right) const;

	const char* TypeName() const;
	int Type() const;

	bool IsNil() const;
	bool IsTable() const;
	bool IsUserData() const;
	bool IsCFunction() const;
	bool IsInteger() const;
	bool IsNumber() const;
	bool IsString() const;
	bool IsWString() const;
	bool IsConvertibleToInteger() const;
	bool IsConvertibleToNumber() const;
	bool IsConvertibleToString() const;
	bool IsConvertibleToWString() const;
	bool IsFunction() const;
	bool IsNone() const;
	bool IsLightUserData() const;
	bool IsBoolean() const;

	int ToInteger();
	lua_Number ToNumber();
	const char* ToString();
	const lua_WChar* ToWString();
	size_t ToStrLen();

	int GetInteger() const;
	float GetFloat() const;
	double GetDouble() const;
	lua_Number GetNumber() const;
	const char* GetString() const;
	const lua_WChar* GetWString() const;
	int StrLen();
	lua_CFunction GetCFunction() const;
	void* GetUserData();
	const void* GetLuaPointer();
	void* GetLightUserData() const;
	bool GetBoolean() const;

	LuaStackObject PushStack() const;

	LuaObject GetMetaTable();
	void SetMetaTable(const LuaObject& valueObj);

	int GetN();
	void SetN(int n);

	void Insert(LuaObject& obj);
	void Insert(int index, LuaObject& obj);
	void Remove(int index = -1);
	void Sort();

	int GetCount();
	int GetTableCount();

	LuaObject Clone();

	LuaObject CreateTable(const char* key, int narray = 0, int lnhash = 0);
	LuaObject CreateTable(int key, int narray = 0, int lnhash = 0);
	LuaObject CreateTable(LuaObject& key, int narray = 0, int lnhash = 0);

	template <typename KeyT, typename ValueT> LuaObject& Set(const KeyT& key, const ValueT& value);
	template <typename KeyT> LuaObject& SetNil(const KeyT& key);

	LuaObject& SetNil(const char* key);
	LuaObject& SetNil(int key);
	LuaObject& SetNil(LuaObject& key);
	LuaObject& SetBoolean(const char* key, bool value);
	LuaObject& SetBoolean(int key, bool value);
	LuaObject& SetBoolean(LuaObject& key, bool value);
	LuaObject& SetInteger(const char* key, int value);
	LuaObject& SetInteger(int key, int value);
	LuaObject& SetInteger(LuaObject& key, int value);
	LuaObject& SetNumber(const char* key, lua_Number value);
	LuaObject& SetNumber(int key, lua_Number value);
	LuaObject& SetNumber(LuaObject& key, lua_Number value);
	LuaObject& SetString(const char* key, const char* value, int len = -1);
	LuaObject& SetString(int key, const char* value, int len = -1);
	LuaObject& SetString(LuaObject& key, const char* value, int len = -1);
	LuaObject& SetWString(const char* key, const lua_WChar* value, int len = -1);
	LuaObject& SetWString(int key, const lua_WChar* value, int len = -1);
	LuaObject& SetWString(LuaObject& key, const lua_WChar* value, int len = -1);
	LuaObject& SetUserData(const char* key, void* value);
	LuaObject& SetUserData(int key, void* value);
	LuaObject& SetUserData(LuaObject& key, void* value);
	LuaObject& SetLightUserData(const char* key, void* value);
	LuaObject& SetLightUserData(int key, void* value);
	LuaObject& SetLightUserData(LuaObject& key, void* value);
	LuaObject& SetObject(const char* key, LuaObject& value);
	LuaObject& SetObject(int key, LuaObject& value);
	LuaObject& SetObject(LuaObject& key, LuaObject& value);

	LuaObject& RawSetNil(const char* key);
	LuaObject& RawSetNil(int key);
	LuaObject& RawSetNil(LuaObject& key);
	LuaObject& RawSetBoolean(const char* key, bool value);
	LuaObject& RawSetBoolean(int key, bool value);
	LuaObject& RawSetBoolean(LuaObject& key, bool value);
	LuaObject& RawSetInteger(const char* key, int value);
	LuaObject& RawSetInteger(int key, int value);
	LuaObject& RawSetInteger(LuaObject& key, int value);
	LuaObject& RawSetNumber(const char* key, lua_Number value);
	LuaObject& RawSetNumber(int key, lua_Number value);
	LuaObject& RawSetNumber(LuaObject& key, lua_Number value);
	LuaObject& RawSetString(const char* key, const char* value, int len = -1);
	LuaObject& RawSetString(int key, const char* value, int len = -1);
	LuaObject& RawSetString(LuaObject& key, const char* value, int len = -1);
	LuaObject& RawSetWString(const char* key, const lua_WChar* value, int len = -1);
	LuaObject& RawSetWString(int key, const lua_WChar* value, int len = -1);
	LuaObject& RawSetWString(LuaObject& key, const lua_WChar* value, int len = -1);
	LuaObject& RawSetUserData(const char* key, void* value);
	LuaObject& RawSetUserData(int key, void* value);
	LuaObject& RawSetUserData(LuaObject& key, void* value);
	LuaObject& RawSetLightUserData(const char* key, void* value);
	LuaObject& RawSetLightUserData(int key, void* value);
	LuaObject& RawSetLightUserData(LuaObject& key, void* value);
	LuaObject& RawSetObject(const char* key, LuaObject& value);
	LuaObject& RawSetObject(int key, LuaObject& value);
	LuaObject& RawSetObject(LuaObject& key, LuaObject& value);

	void AssignNil(LuaState* state);
	void AssignBoolean(LuaState* state, bool value);
	void AssignInteger(LuaState* state, int value);
	void AssignNumber(LuaState* state, lua_Number value);
	void AssignString(LuaState* state, const char* value, int len = -1);
	void AssignWString(LuaState* state, const lua_WChar* value, int len = -1);
	void AssignUserData(LuaState* state, void* value);
	void AssignLightUserData(LuaState* state, void* value);
	void AssignObject(LuaObject& value);		// Should this function be removed??
	void AssignNewTable(LuaState* state, int narray = 0, int numhash = 0);
	void AssignTObject(LuaState* state, lua_TObject* value);

	LuaObject GetByName(const char* name);
	LuaObject GetByIndex(int index);
	LuaObject GetByObject(const LuaStackObject& obj);
	LuaObject GetByObject(const LuaObject& obj);
	LuaObject RawGetByName(const char* name);
	LuaObject RawGetByIndex(int index);
	LuaObject RawGetByObject(const LuaStackObject& obj);
	LuaObject RawGetByObject(const LuaObject& obj);

	// Raw
	LuaObject operator[](const char* name);
	LuaObject operator[](int index);
	LuaObject operator[](const LuaStackObject& obj);
	LuaObject operator[](const LuaObject& obj);

	LuaObject Lookup(const char* key);

	void Register(const char* funcName, lua_CFunction func, int nupvalues = 0);

	void Register(const char* funcName, int (*func)(LuaState*), int nupvalues = 0);
	void Register(const char* funcName, int (*func)(LuaState*, LuaStackObject*), int nupvalues = 0);

	template <class Callee>
	void Register(const char* funcName, const Callee& callee, int (Callee::*func)(LuaState*), int nupvalues = 0)
	{
		const void* pCallee = &callee;
		RegisterHelper(funcName, LPCD::LuaStateMemberDispatcherHelper<Callee>::LuaStateMemberDispatcher, nupvalues, &pCallee, sizeof(Callee*), &func, sizeof(func));
	}

	template <class Callee>
	void Register(const char* funcName, const Callee& callee, int (Callee::*func)(LuaState*, LuaStackObject*), int nupvalues = 0)
	{
		const void* pCallee = &callee;
		RegisterHelper(funcName, LPCD::LuaStateOldMemberDispatcherHelper<Callee>::LuaStateOldMemberDispatcher, nupvalues, &pCallee, sizeof(Callee*), &func, sizeof(func));
	}

	template <class Callee>
	void RegisterObjectFunctor(const char* funcName, int (Callee::*func)(LuaState*), int nupvalues = 0)
	{
		RegisterHelper(funcName, LPCD::Object_MemberDispatcher_to_LuaStateHelper<Callee>::Object_MemberDispatcher_to_LuaState, nupvalues, NULL, 0, &func, sizeof(func));
	}

	template <typename Func>
	inline void RegisterDirect(const char* funcName, Func func, unsigned int nupvalues = 0)
	{
		RegisterHelper(funcName, LPCD::DirectCallFunctionDispatchHelper<Func>::DirectCallFunctionDispatcher, nupvalues, NULL, 0, &func, sizeof(func));
	}

	template <typename Callee, typename Func>
	inline void RegisterDirect(const char* funcName, const Callee& callee, Func func, unsigned int nupvalues = 0)
	{
		const void* pCallee = &callee;
		RegisterHelper(funcName, LPCD::DirectCallMemberDispatcherHelper<Callee, Func>::DirectCallMemberDispatcher, nupvalues, &pCallee, sizeof(Callee*), &func, sizeof(func));
	}

	template <typename Callee, typename Func>
	inline void RegisterObjectDirect(const char* funcName, const Callee* callee, Func func, unsigned int nupvalues = 0)
	{
		RegisterHelper(funcName, LPCD::DirectCallObjectMemberDispatcherHelper<Callee, Func, 2>::DirectCallMemberDispatcher, nupvalues, NULL, 0, &func, sizeof(func));
	}

	void Unregister(const char* funcName);

protected:
	void SetNilHelper(lua_TObject& obj);
	void RegisterHelper(const char* funcName, lua_CFunction function, int nupvalues, const void* callee, unsigned int sizeofCallee, void* func, unsigned int sizeofFunc);

	LuaObject& SetTableHelper(const char* key, lua_TObject* valueObj);
	LuaObject& SetTableHelper(int key, lua_TObject* valueObj);
	LuaObject& SetTableHelper(const lua_TObject* keyObj, const lua_TObject* valueObj);
	LuaObject& SetTableHelper(const LuaObject& key, lua_TObject* valueObj);
	LuaObject& RawSetTableHelper(const char* key, lua_TObject* valueObj);
	LuaObject& RawSetTableHelper(int key, lua_TObject* valueObj);
	LuaObject& RawSetTableHelper(const lua_TObject* keyObj, const lua_TObject* valueObj);
	LuaObject& RawSetTableHelper(const LuaObject& key, lua_TObject* valueObj);

private:
	// Private functions and data
	void AddToUsedList(LuaState* state);
	void AddToUsedList(LuaState* state, const lua_TObject& obj);
	void RemoveFromUsedList();

	LuaObject* m_next;		   // only valid when in free list
	LuaObject* m_prev;		   // only valid when in used list
	LuaState* m_state;
#if defined(__MINGW32_VERSION)
	enum { TOBJECT_SIZE = 16 };
#else
	enum { TOBJECT_SIZE = 12 };
#endif
#ifdef BUILDING_LUAPLUS
	lua_TObject m_object;
#else // BUILDING_LUAPLUS
	unsigned char m_object[TOBJECT_SIZE];
#endif // BUILDING_LUAPLUS
};


namespace detail
{
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, const LuaArgNil&);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, bool value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, char value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, unsigned char value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, short value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, unsigned short value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, int value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, unsigned int value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, float value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, double value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, const char* value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, const lua_WChar* value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, const LuaObject& value);
	LUAPLUS_API void AssignNewTObject(lua_State* L, lua_TObject* obj, void* value);

	LUAPLUS_API void SetNilValue(lua_State* L, lua_TObject* obj);
}

template <typename T>
inline LuaObject& LuaObject::SetNil(const T& key)
{
	luaplus_assert(m_state  &&  IsTable());
	unsigned char keyObj[TOBJECT_SIZE];
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)keyObj, LuaArgNil());
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)keyObj, key);
	unsigned char valueObj[TOBJECT_SIZE];
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)valueObj, LuaArgNil());
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)valueObj, LuaArgNil());
	SetTableHelper((lua_TObject*)keyObj, (lua_TObject*)valueObj);
	detail::SetNilValue(m_state->m_state, (lua_TObject*)keyObj);
	return *this;
}


template <typename KeyT, typename ValueT>
LuaObject& LuaObject::Set(const KeyT& key, const ValueT& value)
{
	luaplus_assert(m_state  &&  IsTable());
	unsigned char keyObj[TOBJECT_SIZE];
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)keyObj, LuaArgNil());
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)keyObj, key);
	unsigned char valueObj[TOBJECT_SIZE];
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)valueObj, LuaArgNil());
	detail::AssignNewTObject(m_state->m_state, (lua_TObject*)valueObj, value);
	SetTableHelper((lua_TObject*)keyObj, (lua_TObject*)valueObj);
	detail::SetNilValue(m_state->m_state, (lua_TObject*)valueObj);
	detail::SetNilValue(m_state->m_state, (lua_TObject*)keyObj);
	return *this;
}

} // namespace LuaPlus

#endif // LUAOBJECT_H
