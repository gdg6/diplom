#ifndef __USER_SERVICE__
#define __USER_SERVICE__

#include "../orm/userORM.cpp"
#include "bcrypt.h"
#include <string>
#include <string.h>

// g++ -std=gnu++11 userService.cpp bcrypt.a -lsqlite3
class UserService 
{
	UserORM userORM;
	
	bool checkPassword(std::string& password, std::string& passwordHash){
		char cpassword[password.length()];
		char hashInDatabase[BCRYPT_HASHSIZE];
		char outTestHash[BCRYPT_HASHSIZE];

		for (size_t i = 0; i < password.length(); i++){
			cpassword[i] = password[i];
		}
		for (size_t i = 0; i < BCRYPT_HASHSIZE; i++){
			hashInDatabase[i] = passwordHash[i];
		}

		if (bcrypt_hashpw(cpassword, hashInDatabase, outTestHash) == 0){
			if (strcmp(hashInDatabase, outTestHash) == 0) {
				// password matches
				return true;
			}
			// password does not match
		}
		return false;
	}
public:
	
	UserService(sqlite3 * db) : userORM(db) 
	{
		
	}
	
	std::shared_ptr<User> AuthorizationUser()
	{
		std::shared_ptr<User> user;
		std::string login;
		std::string password;
		std::string hash;
		
		std::cout << "Enter login: "; // write in sock for future
		std::cin >> login;
		user = userORM.getByLogin(login);
		if(user->getStatus())
		std::cout << "Enter	password: ";
		std::cin >> password;
		hash = user->getPassword();
		if(checkPassword(password, hash))
		{
			user->setActive(user->AUTHORIZED_USER);
		}
		
		return user;
	}
	
};

#endif
