#pragma once
#ifndef DB_CONNECTION_H_
#define DB_CONNECTION_H_

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>

class db_response
{
private:
    static SQLHENV hEnv;
    static SQLHDBC hDbc;
    static SQLHSTMT hStmt;

public:
    static void ConnectionFunction(); // Connect and test query
    static void CloseConnection();    // Clean up handles
    static bool isConnected();
    static SQLHDBC GethDbc() 
    {
        return hDbc;
    }
};

#endif // DB_CONNECTION_H_