#ifndef __USER_ORM__
#define __USER_ORM__

#include "ORM.cpp"
#include "../model/user.cpp"

#include <iostream>

class UserORM : public ORM
{
private:
	//~ std::string sql_select_by_login = "SELECT id, login, password_digest, role FROM users WHERE login = 'Denis';";
	std::string sql_select_by_login = "SELECT id, login, password_digest, role FROM users WHERE id = ?";
#ifdef __SQLITE3__
	sqlite3 * db;
	int rc;
#else
    //pg
#endif	
	

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
#ifdef __SQLITE3__
	UserORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}
#else
    //pg
#endif

	std::shared_ptr<User> getByLogin(std::string login)
	{
		
		std::shared_ptr<User> user(new User);
#ifdef __SQLITE3__		
		sqlite3_stmt *stmt;

		int rc = sqlite3_prepare_v2(db, sql_select_by_login.c_str(), -1, &stmt, NULL);
		
		if (rc != SQLITE_OK)
		{
			std::cout << sqlite3_errmsg(db);
			return user;
		}
//~ 
		rc = sqlite3_bind_text(stmt, 1, login.c_str(), login.length(), SQLITE_STATIC);    // Using parameters ("?") is not
		//~ rc = sqlite3_bind_int(stmt, 1, 1);    // Using parameters ("?") is not
		if (rc != SQLITE_OK) 
		{                 	
							   // really necessary, but recommended
			std::cout << sqlite3_errmsg(db);   // (especially for strings) to avoid
			sqlite3_finalize(stmt);            // formatting problems and SQL
			return user;       				   // injection attacks.
		}
 
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_ROW && rc != SQLITE_DONE) 
		{
			std::cout << sqlite3_errmsg(db);
			sqlite3_finalize(stmt);
			return user;
		}
		
		if (rc == SQLITE_DONE) 
		{
			std::cout << sqlite3_errmsg(db);
			sqlite3_finalize(stmt);
			return user;
		}
		
		if(rc == SQLITE_ROW) 
		{
			user -> setId(atoi((const char *)sqlite3_column_text(stmt, 0)));
			user -> setLogin(std::string ((const char *)sqlite3_column_text(stmt, 1)));
			user -> setPassword(std::string ((const char *)sqlite3_column_text(stmt, 2)));
			user -> setRole(atoi((const char *)sqlite3_column_text(stmt, 3)));
		}
		sqlite3_finalize(stmt);
#else

#endif
		return user;
	}

};

#endif

#include <iostream>
int main()
{
	sqlite3 * db;
	sqlite3_open("../snmp_db", & db);
	UserORM userORM(db);
	std::shared_ptr<User>  user = userORM.getByLogin(std::string("Denis"));
	std::cout << user->getLogin() << std::endl;
	std::cout << user->getPassword() << std::endl;
	sqlite3_close(db);
	return 0;
}
