#ifndef __WALK_REQUEST_ORM__
#define __WALK_REQUEST_ORM__

class WalkRequestORM : public ORM
{
private: 

	int rc;
	sqlite3 * db;

public: 

	WalkRequestORM(sqlite3 * db) : rc(0)
	{
		this -> db = db;
	}


	~WalkRequestORM()
	{

	}

};


#endif