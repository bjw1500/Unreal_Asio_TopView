#include "pch.h"
#include "Account.h"

Account::Account(string accountID, int32 DB_Key)
	:_accountID(accountID),
	_accountDB_Key(DB_Key)
{
}

Account::~Account()
{
}

PlayerDB::PlayerDB()
{
}

PlayerDB::~PlayerDB()
{
}

ItemDB::ItemDB()
{
}

ItemDB::~ItemDB()
{
}
