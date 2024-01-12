#pragma once

/*

계정 정보를 관리한다.

Account 클래스는 데이터 베이스의 캐릭터 정보 PlayerDB를 지닌다.

PlayerDB는 게임 안에서 가지고 있던 아이템의 DB 정보를 소지하고 있는다.

#데이터 베이스에서 캐릭터 불러오기
Account 정보에서 찾아낸 값으로 PlayeerDB를 가져오고,

PlayerDB를 외래키로 하는 ItemDB 정보들을 PlayerDB의 아이템 목록에 넣어준다.

*/


class Account
{
public:
	Account(string accountID, int32 DB_Key);
	~Account();

	string					GetAccountID(){return _accountID;}
	int32					GetAccountDB_Key(){return _accountDB_Key;}
	
private:
	
	string _accountID;
	int32 _accountDB_Key;
};


class PlayerDB
{
public:
	PlayerDB();
	~PlayerDB();



	/*
		DB에 저장된 캐릭터의  정보
		HP.  MP . EXP . 현재 위치 등등
	*/



private:
	int _accountDB_Key;
	shared_ptr<Account> _account;
	unordered_map<int, class ItemDB> _playerItems;
};


class ItemDB
{
public:
	ItemDB();
	~ItemDB();


private:
	int _playerDB_Key;
	int _itemDB_Key;
};