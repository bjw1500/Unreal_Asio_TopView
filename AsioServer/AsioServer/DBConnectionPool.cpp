#include "pch.h"
#include "DBConnectionPool.h"

/*-------------------
	DBConnectionPool
--------------------*/

DBConnectionPool::DBConnectionPool()
{
	InitializeCriticalSection(&_lock);
}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect(int32 connectionCount, const WCHAR* connectionString)
{
	EnterCriticalSection(&_lock);

	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment) != SQL_SUCCESS)
		return false;

	if (::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	for (int32 i = 0; i < connectionCount; i++)
	{
		DBConnection* connection = new DBConnection();
		if (connection->Connect(_environment, connectionString) == false)
			return false;

		_connections.push_back(connection);
	}

	LeaveCriticalSection(&_lock);

	return true;
}

void DBConnectionPool::Clear()
{
	EnterCriticalSection(&_lock);

	if (_environment != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
		_environment = SQL_NULL_HANDLE;
	}

	for (DBConnection* connection : _connections)
		delete connection;

	_connections.clear();

	LeaveCriticalSection(&_lock);
}

DBConnection* DBConnectionPool::Pop()
{
	EnterCriticalSection(&_lock);

	if (_connections.empty())
		return nullptr;

	DBConnection* connection = _connections.back();
	_connections.pop_back();

	LeaveCriticalSection(&_lock);
	return connection;
}

void DBConnectionPool::Push(DBConnection* connection)
{
	EnterCriticalSection(&_lock);
	_connections.push_back(connection);
	LeaveCriticalSection(&_lock);
}
