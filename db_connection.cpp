#include "db_connection.h"

// Static member definitions
SQLHENV db_response::hEnv = NULL;
SQLHDBC db_response::hDbc = NULL;
SQLHSTMT db_response::hStmt = NULL;
void db_response::ConnectionFunction()
{
    SQLRETURN ret;

    // 1. Allocate environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::wcerr << L" Error allocating environment handle." << std::endl;
        return;
    }

    // 2. Set ODBC version
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

    // 3. Allocate connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::wcerr << L" Error allocating connection handle." << std::endl;
        return;
    }

    // 4. Connect using DSN-less connection string (Unicode)
    SQLWCHAR connStr[] = L"Driver={ODBC Driver 18 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=cpp_airlinereservation_db;Trusted_Connection=yes;Encrypt=no;";
    ret = SQLDriverConnectW(hDbc, nullptr, connStr, SQL_NTS, nullptr, 0, nullptr, SQL_DRIVER_COMPLETE);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        std::wcout << L" Connected to SQL Server successfully.\n";
    }
    else
    {
        SQLWCHAR sqlState[6], message[1024];
        SQLGetDiagRecW(SQL_HANDLE_DBC, hDbc, 1, sqlState, nullptr, message, 1024, nullptr);
        std::wcerr << L" Connection failed: " << message << L" (SQLState: " << sqlState << L")" << std::endl;
        return;
    }

    // 5. Allocate statement handle
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::wcerr << L" Error allocating statement handle." << std::endl;
        return;
    }

    // 6. Execute SQL query using Unicode
    const SQLWCHAR *query = L"SELECT flight_id, destination FROM flights";
    ret = SQLExecDirectW(hStmt, (SQLWCHAR *)query, SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        std::wcout << L" Query executed successfully.\n";

        SQLINTEGER flightId;
        SQLWCHAR destination[100];

        SQLBindCol(hStmt, 1, SQL_C_SLONG, &flightId, 0, nullptr);
        SQLBindCol(hStmt, 2, SQL_C_WCHAR, destination, sizeof(destination), nullptr);

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            std::wcout << L"Flight ID: " << flightId << L", Destination: " << destination << std::endl;
        }
    }
    else
    {
        SQLWCHAR sqlState[6], message[1024];
        SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, sqlState, nullptr, message, 1024, nullptr);
        std::wcerr << L" Query failed: " << message << L" (SQLState: " << sqlState << L")" << std::endl;
    }
}
void db_response::CloseConnection()
{
    if (hStmt)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        hStmt = NULL;
    }
    if (hDbc)
    {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        hDbc = NULL;
    }
    if (hEnv)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        hEnv = NULL;
    }
    std::wcout << L"🔌 Disconnected and cleaned up ODBC handles.\n";
}
bool db_response::isConnected()
{
    return hDbc != NULL;
}
