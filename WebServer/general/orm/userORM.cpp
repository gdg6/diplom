#ifndef __USER_ORM__
#define __USER_ORM__

#include "ORM.cpp"
#include "../model/user.cpp"

#include <iostream>

class UserORM : public ORM
{
private:

    std::string sql_create_table = "CREATE TABLE \'users\' (\'id\' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \'login\' varchar(255), \'password_digest\' varchar(255), \'role\' integer, \'created_at\' datetime, \'updated_a\' datetime);";
	std::string sql_select_by_login = "SELECT id, login, password_digest, role FROM users WHERE login = ?";

	sqlite3 * db;
	int rc;

	// atrophied API for user
	std::shared_ptr<std::vector<std::shared_ptr<User>>> getAll()
	{
			return NULL;
	}
	
	std::shared_ptr<User> getByPK(int id) 
	{
		return NULL;
	}
	
public: 

	UserORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}

	std::shared_ptr<User> getByLogin(std::string & login)
	{
		std::shared_ptr<User> user(new User);
	
		sqlite3_stmt *stmt;

		int rc = sqlite3_prepare_v2(db, sql_select_by_login.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			user->setStatus(ORM::BAD_INIT);
			return user;
		}

		rc = sqlite3_bind_text(stmt, 1, login.c_str(), login.length(), SQLITE_STATIC);    // Using parameters ("?") is not
		if (rc != SQLITE_OK) 
		{                 	
							   // really necessary, but recommended
			user->setStatus(ORM::BAD_INIT);
			//~ std::cout << sqlite3_errmsg(db);   // (especially for strings) to avoid
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return user;       				   // injection attacks.
		}
 
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) 
		{
			sqlite3_finalize(stmt);
			user->setStatus(ORM::BAD_INIT);
			return user;
		}
		
		if (rc == SQLITE_DONE) 
		{
			user->setStatus(ORM::NOT_FOUND);
			sqlite3_finalize(stmt);
			return user;
		}
		
		if(rc == SQLITE_ROW) 
		{
			user -> setStatus(ORM::SUCCESS_INIT);
			user -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			user -> setLogin(std::string ((const char *)sqlite3_column_text(stmt, 1)));
			user -> setPassword(std::string ((const char *)sqlite3_column_text(stmt, 2)));
			user -> setRole(atoi((const char *)sqlite3_column_text(stmt, 3)));
		}
		sqlite3_finalize(stmt);
		
		return user;
	}

};

#endif

