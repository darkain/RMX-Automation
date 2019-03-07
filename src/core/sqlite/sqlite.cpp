/******************************  RMX SDK  ******************************\
*  Copyright (c) 2010 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/rmxString.h"
#include "../../../sdk/win32/timerDelete.h"
#include "sqlite.h"
#ifdef _RMX_SQLITE_


vSqlite::vSqlite() {
	db = NULL;
	int ret = 0;

    char path[MAX_PATH] = "";
	VPRINTF(path, sizeof(path), "%s\\storage.rmx", RMXOS::getPath());

	ret = sqlite3_open(path, &db);
	if (ret) {
		sqlite3_close(db);
		db = NULL;
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING DATABASE CONNECTION");
		return;
	}


	//Enable Foreign Key Constraints
	setPragma("foreign_keys", "ON");


	//Create Settings Table
	if (sql("CREATE TABLE IF NOT EXISTS [settings] (`key` TEXT PRIMARY KEY, `value` TEXT, `updated` NUMERIC)")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING `settings` TABLE");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}

	if (sql("CREATE TRIGGER IF NOT EXISTS `settings_updated` AFTER UPDATE OF `key`, `value` ON `settings` BEGIN UPDATE `settings` SET `updated`=strftime('%s','now') WHERE `key`=new.key; END;")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING `settings` TRIGGER");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}


	//Create Object Type Table
	if (sql("CREATE TABLE IF NOT EXISTS `object_type` (`object_type_id` INTEGER PRIMARY KEY, `object_type_name` TEXT UNIQUE)")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING `object_type` TABLE");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}


	//Create Primary Object Table
	if (sql("CREATE TABLE IF NOT EXISTS `objects` (`object_id` INTEGER PRIMARY KEY, `object_type_id` INTEGER, `root_id` INTEGER, `parent_id` INTEGER, `name` TEXT, FOREIGN KEY(`object_type_id`) REFERENCES `object_type`(`object_type_id`) ON UPDATE CASCADE ON DELETE CASCADE, FOREIGN KEY(`root_id`) REFERENCES `objects`(`object_id`) ON UPDATE CASCADE ON DELETE CASCADE, FOREIGN KEY(`parent_id`) REFERENCES `objects`(`object_id`) ON UPDATE CASCADE ON DELETE CASCADE)")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING `object` TABLE");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}


	//Create Param Name Table
	if (sql("CREATE TABLE IF NOT EXISTS `param_names` (`param_name_id` INTEGER PRIMARY KEY, `param_name` TEXT UNIQUE)")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING `param_names` TABLE");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}


	//Create Param Table
	if (sql("CREATE TABLE IF NOT EXISTS `params` (`param_id` INTEGER PRIMARY KEY, `param_name_id` INTEGER, `object_id` INTEGER, `param_value`, FOREIGN KEY(`param_name_id`) REFERENCES `param_name`(`param_name_id`) ON UPDATE CASCADE ON DELETE CASCADE, FOREIGN KEY(`object_id`) REFERENCES `objects`(`object_id`) ON UPDATE CASCADE ON DELETE CASCADE)")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING `params` TABLE");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}


	//Create Mime-Type Table
	if (sql("CREATE TABLE IF NOT EXISTS [mimetypes] ([file_extension] TEXT UNIQUE, [mime_type] TEXT)")) {
		CONSOLE_MAIN->printError("SQLite", "ERROR CREATING [mimetypes] TABLE");
		CONSOLE_MAIN->printError("SQLite", getLastError());
	}
}


vSqlite::~vSqlite() {
	if (!db) return;
	sqlite3_close(db);
	db = NULL;
}


int vSqlite::sql(const char *query) {
	if (!db) {
		sql_error = "SQLite database not initialized";
		return SQLITE_ERROR;
	}

	if (!query  ||  !*query) {
		sql_error = "Empty SQL query string";
		return SQLITE_ERROR;
	}

	char *err = NULL;

	int ret = sqlite3_exec(db, query, NULL, NULL, &err);

	if (err) {
		sql_error = err;
		sqlite3_free(err);
	}

	return ret;
}


sqlite3_stmt *vSqlite::sqlPrepare(const char *query, const char **params, int count, sqlite3_destructor_type type) {
	if (!db  ||  !query) return NULL;

	sqlite3_stmt *statement = NULL;
	int err = sqlite3_prepare_v2(db, query, VSTRLEN(query), &statement, NULL);

	if (!statement) {
		//TODO: see if we need to "free" this memory here from errmsg
		sql_error = sqlite3_errmsg(db);
		return NULL;
	}

	if (params) {
		for (int i=0; i<count; i++) {
			sqlite3_bind_text(statement, i+1, params[i], -1, type);
		}
	}

	return statement;
}




int vSqlite::sqlPrepared(const char *query, const char **params, int count, sqlite3_destructor_type type) {
	sqlite3_stmt *statement = sqlPrepare(query, params, count, type);
	if (!statement) return SQLITE_ERROR;

	sqlite3_step(statement);
	return sqlite3_finalize(statement); 
}



int vSqlite::sqlGetCellInt(const char *column, const char *table, const char *clause) {
	if (! column  ||  ! table  ||  ! clause) return 0; //Check to see if any string is NULL
	if (!*column  ||  !*table  ||  !*clause) return 0; //Check to see if any string is zero-length


	//TODO: verify that input data is SQL safe!!!
	rmxString string;
	string += "SELECT [";
	string += column;
	string += "] FROM [";
	string += table;
	string += "] WHERE ";
	string += clause;
	string += " LIMIT 1;";

	sqlite3_stmt *statement = sqlPrepare(string);
	if (!statement) {
		CONSOLE_MAIN->printError("SQLite", getLastError());
		return 0;
	}

	sqlite3_step(statement);
	int ret = sqlite3_column_int(statement, 0);
	sqlite3_finalize(statement); 
	return ret;
}


const char *vSqlite::sqlGetCellStr(const char *column, const char *table, const char *clause) {
	return NULL;
}


int vSqlite::setSetting(const char *string, char separator) {
	if (string == NULL  ||  separator == NULL) return 0;

	rmxString str(string);
	int pos = str.find(separator);
	if (pos < 1) return 0;

	char sep[2] = { separator, 0 };
	str.replace(pos, 1, sep);
	const char *data = str;
	return setSetting(data, data+pos+1);
}


int vSqlite::setSetting(const char *key, const char *value) {
	const char *insert[] = { key, value };
	return sqlPrepared("REPLACE INTO [settings] VALUES (?, ?, strftime('%s','now'))", insert, 2);
}


int vSqlite::setSetting(const char *key, int value) {
	char str[32] = "";
	VPRINTF(str, sizeof(str), "%d", value);
	return setSetting(key, str);
}


int vSqlite::setSetting(const char *key, double value) {
	char str[32] = "";
	VPRINTF(str, sizeof(str), "%f", value);
	return setSetting(key, str);
}



const char *vSqlite::getSettingString(const char *key) {
	sqlite3_stmt *statement = sqlPrepare("SELECT [value] FROM [settings] WHERE [key]=? LIMIT 1", &key, 1);
	if (!statement) return 0;

	sqlite3_step(statement);

	const char *ret = (const char*) sqlite3_column_text(statement, 0);
	timerString *str = new timerString(ret);

	sqlite3_finalize(statement); 

	str->deleteThis();
	return *str;
}


int vSqlite::getSettingInteger(const char *key) {
	sqlite3_stmt *statement = sqlPrepare("SELECT [value] FROM [settings] WHERE [key]=? LIMIT 1", &key, 1);
	if (!statement) return 0;

	sqlite3_step(statement);

	int ret = sqlite3_column_int(statement, 0);

	sqlite3_finalize(statement); 

	return ret;
}


double vSqlite::getSettingNumber(const char *key) {
	sqlite3_stmt *statement = sqlPrepare("SELECT [value] FROM [settings] WHERE [key]=? LIMIT 1", &key, 1);
	if (!statement) return 0;

	sqlite3_step(statement);

	double ret = sqlite3_column_double(statement, 0);

	sqlite3_finalize(statement); 

	return ret;
}




void vSqlite::setMimeType(const char *extension, const char *mimetype) {
	const char *insert[] = { extension, mimetype };
	sqlPrepared("REPLACE INTO [mimetypes] VALUES (?, ?)", insert, 2);
}


const char *vSqlite::getMimeType(const char *extension) {
	const char *def = "application/octet-stream";

	sqlite3_stmt *statement = sqlPrepare("SELECT [mime_type] FROM [mimetypes] WHERE [file_extension]=? LIMIT 1", &extension, 1);
	if (!statement) return def;

	sqlite3_step(statement);

	const char *ret = (const char*) sqlite3_column_text(statement, 0);

	if (!ret  ||  !*ret) {
		sqlite3_finalize(statement); 
		return def; 
	}

	timerString *str = new timerString(ret);

	sqlite3_finalize(statement); 

	str->deleteThis();
	return *str;
}


int vSqlite::setPragma(const char *name, const char *value) {
	char str[256] = "";
	if (!name  ||  !*name  ||  !value  ||  !*value) return SQLITE_ERROR;
	VPRINTF(str, sizeof(str), "PRAGMA %s = %s", name, value);
	return sql(str);
}


const char *vSqlite::getPragma(const char *name) {
	char str[256] = "";
	if (!name  ||  !*name) return NULL;

	VPRINTF(str, sizeof(str), "PRAGMA %s", name);
	sqlite3_stmt *statement = sqlPrepare(str, &name, 1);
	if (!statement) return 0;

	sqlite3_step(statement);

	const char *ret = (const char*) sqlite3_column_text(statement, 0);
	timerString *tstr = new timerString(ret);

	sqlite3_finalize(statement); 

	tstr->deleteThis();
	return *tstr;
}



int vSqlite::getObjectTypeId(const char *name, bool add) {
	if (!name  ||  !*name) return 0;

	if (add) {
		sqlPrepared("INSERT OR IGNORE INTO [object_type] ([object_type_name]) VALUES (?)", &name, 1);
	}

	char clause[256] = "";  //TODO: SANATIZE SQL !!!!
	VPRINTF(clause, sizeof(clause), "[object_type_name]='%s'", name);

	return sqlGetCellInt("object_type_id", "object_type", clause);
}


int vSqlite::getParamNameId(const char *name, bool add) {
	if (!name  ||  !*name) return 0;

	if (add) {
		sqlPrepared("INSERT OR IGNORE INTO [param_names] ([param_name]) VALUES (?)", &name, 1);
	}

	char clause[256] = "";  //TODO: SANATIZE SQL !!!!
	VPRINTF(clause, sizeof(clause), "[param_name]='%s'", name);

	return sqlGetCellInt("param_name_id", "param_name", clause);
}



#endif