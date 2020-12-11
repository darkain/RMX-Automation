/******************************  RMX SDK  ******************************\
*  Copyright (c) 2010 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __SQLITE_H__
#define __SQLITE_H__


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/rmxString.h"
#include "../plugin.h"
#ifdef _RMX_SQLITE_


#include "../../../libraries/sqlite/sqlite3.h"



class vSqlite {
public:
	vSqlite();
	virtual ~vSqlite();

public:
	int sql(const char *query);

	sqlite3_stmt *sqlPrepare(const char *query, const char **params=NULL, int count=0, sqlite3_destructor_type type=SQLITE_TRANSIENT);

	int sqlPrepared(const char *query, const char **params, int count, sqlite3_destructor_type type=SQLITE_TRANSIENT);

	int         sqlGetCellInt(const char *column, const char *table, const char *clause);
	const char *sqlGetCellStr(const char *column, const char *table, const char *clause);

	inline int beginTransaction()    { return sql("BEGIN TRANSACTION"   ); }
	inline int commitTransaction()   { return sql("COMMIT TRANSACTION"  ); }
	inline int rollbackTransaction() { return sql("ROLLBACK TRANSACTION"); }

	inline __int64 getInsertId()    const { return sqlite3_last_insert_rowid(db); }
	inline int     getRowsChanged() const { return sqlite3_changes(db); }


	int setSetting(const char *string, char separator='=');
	int setSetting(const char *key, const char *value);
	int setSetting(const char *key, int         value);
	int setSetting(const char *key, double      value);

	const char *getSettingString( const char *key);
	int         getSettingInteger(const char *key);
	double      getSettingNumber( const char *key);

	void setMimeType(const char *extension, const char *mimetype);
	const char *getMimeType(const char *extension);


	int setPragma(const char *name, const char *value);
	const char *getPragma(const char *name);

	inline const char *getLastError() const { return sql_error; }


	int getObjectTypeId(const char *name, bool add=true);

	int getParamNameId(const char *name, bool add=true);

private:
	sqlite3 *db;

	rmxString sql_error;
};



#define sqlite3_column_char(pStmt, i) ((const char*)sqlite3_column_text(pStmt, i))


#endif _RMX_SQLITE_

#endif __SQLITE_H__
