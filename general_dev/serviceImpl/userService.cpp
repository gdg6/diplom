#ifndef __USER_SERVICE__
#define __USER_SERVICE__

#include "../orm/userORM.cpp"
#include "bcrypt.h"
#include <string>
#include <string.h>
#include <unistd.h>

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
	
	// convert char array responce from descriptor. last symbol change to '\0'
	inline std::string convertToString(char str[], const size_t size)
	{
		if(strlen(str) < size) return std::string("");
		char tmpStr[size];
		memset(tmpStr, 0, size);
		memcpy(tmpStr, str, size - 1);
		std::string rez = tmpStr;
		return rez;
	}
	
public:
	
	UserService(sqlite3 * db) : userORM(db) 
	{
		
	}
	
	std::shared_ptr<User> AuthorizationUser(int fd)
	{
		const int LEN = 255;
		std::shared_ptr<User> user;
		std::string login = "";
		std::string password = "";
		std::string hash;
		char tmpLogin[LEN];
		char tmpPassword[LEN];
		
		memset(tmpLogin, 0, LEN);
		memset(tmpPassword, 0, LEN);
		
		char enterLogin[] =  "Enter login: ";
		size_t lenLogin = strlen(enterLogin);
		
		char enterPassword[] = "Enter	password: ";
		size_t lenPassword = strlen(enterPassword);
		
		size_t tmpLen(0);
		
		if(write(fd, enterLogin, lenLogin) <= 0 || fd < 0) {
			user->setStatus(User::BAD_SOCKET);
			return user;
		} 

		if((tmpLen = read(fd, tmpLogin, LEN)) < 0 )
		{
			user->setStatus(User::BAD_SOCKET);
			return user;
		}
		
		login = convertToString(tmpLogin, tmpLen);
				
		user = userORM.getByLogin(login);
		
		if(user->getStatus() != ORM::SUCCESS_INIT)
		{
			std::cout << user->getStatus()  << std::endl;
			return user;
		}	
		
		if(write(fd, enterPassword, lenPassword) <= 0 || fd < 0) {
			user->setStatus(User::BAD_SOCKET);
			return user;
		} 
				
		if((tmpLen = read(fd, tmpPassword, LEN)) < 0 )
		{
			user->setStatus(User::BAD_SOCKET);
			return user;
		}
				
		password = convertToString(tmpPassword, tmpLen);
		hash = user->getPassword();
		
		if(checkPassword(password, hash) )
		{
			user->setActive(User::AUTHORIZED_USER);
		}  
		
		return user;
	}

};

#endif














