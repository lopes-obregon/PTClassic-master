#define SQLSOR_CPP

#include <windows.h>
#include <windowsx.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>

#include "common.h"
#include "GameSql.h"

#ifdef _W_SERVER

//±âÅ¸ ÀÓ½Ã ±â·Ï ÆÄÀÏ·Î ³²±è
int Record_TempLogFile(char *szMessage);

ACCOUNTDB sqlAccountDb;
CLANDB  sqlClanDb;

SQLDATA_BASE::SQLDATA_BASE()
{

}

SQLDATA_BASE::~SQLDATA_BASE()
{

}

bool SQLDATA_BASE::Initialize(char* _databaseName)
{
	strcpy(databaseName, _databaseName);
	return true;
}

void SQLDATA_BASE::ShowErrorInfo(SQLRETURN rc, SQLSMALLINT hType, SQLHANDLE h)
{
	SQLRETURN retcode;
	SQLSMALLINT iRecord = 1;
	SQLCHAR szSqlState[REM_LEN];
	SQLINTEGER pfNativeError;
	SQLCHAR szErrorMsg[REM_LEN];
	SQLSMALLINT cbErrorMsgMax = REM_LEN - 1;
	SQLSMALLINT pcbErrorMsg;


	Utils_Log(LOG_ERROR, "Handle type: %s\n", (hType == SQL_HANDLE_STMT) ? "Statement" :
		(hType == SQL_HANDLE_ENV) ? "Environment" :
		(hType == SQL_HANDLE_DBC) ? "DBC" : "???");


	retcode = SQLGetDiagRec(hType, h, // SQL_HANDLE_STMT, m_hstmt,
		iRecord, szSqlState, &pfNativeError,
		szErrorMsg, cbErrorMsgMax, &pcbErrorMsg);


	Utils_Log(LOG_ERROR, " *** %s *** sqlstate '%s'       errormsg '%s'\n",
		(rc == SQL_ERROR) ? "SQL_ERROR" :
		(rc == SQL_SUCCESS_WITH_INFO) ? "SQL_SUCCESS_WITH_INFO" : "",
		szSqlState, szErrorMsg);

	/*
	if (m_nLastError == 0)
	{
	m_nLastError = rc;
	m_strLastErrorSqlState = (char*)szSqlState;
	m_strLastErrorMessage = (char*)szErrorMsg;
	}
	*/
}

bool SQLDATA_BASE::Start_ODBC()
{
	char *svr_name;
	char *user_name;
	char *user_pswd;

	if (Odbc_Config.Dsn[0])
	{
		svr_name = Odbc_Config.Dsn;
		user_name = Odbc_Config.Name;
		user_pswd = Odbc_Config.Password;
	}
	else
	{
		svr_name = "localhost\\SQLEXPRESS";
		user_name = "c8master";
		user_pswd = "c8master";
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_hdl))
	{
		Utils_Log(LOG_ERROR, "SQLDATA: Unable to allocate handle for environment.");
		return false;
	}

	if (SQL_SUCCESS != SQLSetEnvAttr(env_hdl, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
	{
		Utils_Log(LOG_ERROR, "SQLDATA: Unable to set ODBC version.");
		return false;
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, env_hdl, &conn_hdl))
	{
		Utils_Log(LOG_ERROR, "SQLDATA: Unable to allocate handle for connection.");
		return false;
	}

	wsprintf(connectString, "DRIVER={SQL Server};SERVER=%s;DATABASE=%s;UID=%s;PWD=%s;", svr_name, databaseName, user_name, user_pswd);
	//Utils_Log(LOG_DEBUG, "User Sql Config Server: %s  User: %s Pass: %s", svr_name, user_name, user_pswd);

	SQLRETURN retcode = SQLDriverConnect(conn_hdl, NULL, (SQLCHAR*)TEXT(connectString), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
	if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
	{
		ShowErrorInfo(retcode, SQL_HANDLE_DBC, conn_hdl);
		return false;
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, conn_hdl, &stmt_hdl))
	{
		Utils_Log(LOG_ERROR, "SQLAllocHandle failed. Error 4");
		return false;
	}

	return true;
}

bool SQLDATA_BASE::End_ODBC()
{
	SQLRETURN r;

	if (stmt_hdl != NULL)
	{
		r = SQLFreeHandle(SQL_HANDLE_STMT, stmt_hdl);
		if (r == SQL_ERROR)
			return false;
	}

	if (conn_hdl != NULL)
	{
		r = SQLDisconnect(conn_hdl);
		if (r == SQL_ERROR)
			return false;
		r = SQLFreeHandle(SQL_HANDLE_DBC, conn_hdl);
		if (r == SQL_ERROR)
			return false;
	}

	if (env_hdl != NULL)
	{
		r = SQLFreeHandle(SQL_HANDLE_ENV, env_hdl);
		if (r == SQL_ERROR)
			return false;
	}
	return true;
}

//////////////////////// AccountDB //////////////////////
ACCOUNTDB::ACCOUNTDB()
{
}

ACCOUNTDB::~ACCOUNTDB()
{
}

int ACCOUNTDB::Initialize(char* _servername, char* _databaseName)
{
	strcpy(serverName, _servername);
	SQLDATA_BASE::Initialize(_databaseName);
	return TRUE;
}

int	ACCOUNTDB::LogOn(char *szID, char *szPassword)
{
	if (SQLDATA_BASE::Start_ODBC() == false) {
		return 0;
	}

	char id[128] = { 0, };
	char pass[128] = { 0, };
	char sell, block;

	///////////////// ID Find a table according to the first letter ////////////////////// 
	BYTE ch = (BYTE)szID[0];

	char TableName[32];
	TableName[0] = 0;

	if (ch >= 'a' && ch <= 'z'){
		// TableName[0] = ch;
		// TableName[1] = 0;
		// Changed parts
		TableName[0] = ch - 32;
		TableName[1] = 0;
	}
	else{
		if (ch >= 'A' && ch <= 'Z')
		{
			// TableName[0] = ch+0x20;
			// TableName[1] = 0;
			// Changed parts
			TableName[0] = ch;
			TableName[1] = 0;
		}
		else{
			if (ch >= '0' && ch <= '9'){
				lstrcpy(TableName, "[0GameUser]");
			}
			else{
				lstrcpy(TableName, "[9GameUser]");
			}
		}
	}

	if (!TableName[0]) {
		SQLDATA_BASE::End_ODBC();
		return -1;
	}

	// º¯°æµÈ ºÎºÐ
	wsprintf(stmt_buf, "SELECT * FROM %sGameUser WHERE userid='%s'", TableName, szID);

	int r = SQLExecDirect(stmt_hdl, (unsigned char*)stmt_buf, SQL_NTS);
	if (r == SQL_ERROR) {
		SQLDATA_BASE::End_ODBC();
		return 0;											//No connection
	}

	// Changed parts
	r = SQLBindCol(stmt_hdl, 2, SQL_C_CHAR, &pass, 9, NULL);
	r = SQLBindCol(stmt_hdl, 11, SQL_C_UTINYINT, &sell, 1, NULL);
	r = SQLBindCol(stmt_hdl, 12, SQL_C_UTINYINT, &block, 1, NULL);

	if (SQLFetch(stmt_hdl) == SQL_NO_DATA_FOUND) {			//No account
		SQLDATA_BASE::End_ODBC();
		return -1;
	}

	SQLFreeStmt(stmt_hdl, SQL_CLOSE);

	if (lstrcmpi(szPassword, pass) != 0) {
		SQLDATA_BASE::End_ODBC();
		return -2;											//Password check
	}
	if (Odbc_Config.CloseAccount && sell != 1) {
		SQLDATA_BASE::End_ODBC();
		return -3;											//No permission (not a beta tester)
	}
	if (block) {
		SQLDATA_BASE::End_ODBC();
		return -3;											//Account block
	}

	SQLDATA_BASE::End_ODBC();
	return TRUE;		//Certification Success
}

int ACCOUNTDB::SQLLogon(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *s_guid, char *lpszPCRNo, char *lpszGubunCode)
{
	return 0;
}

int ACCOUNTDB::EventSQLLogon(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *lpszS_guid, char *lpszPCRNo, char *lpszGubunCode)
{
	return 0;
}

int ACCOUNTDB::SQLLogout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, int lpszUsePeriod, char *s_guid, int PCRNo)
{
	return 0;
}

int ACCOUNTDB::EventSQLLogout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, long lpszUsePeriod, char *s_guid, long PCRNo, char *lpszNewGPCode, char *News_guid)
{
	return 0;
}

int ACCOUNTDB::UserUseTime(char *lpszPID, char *lpszCID, char *lpszIP, char *ServerName, char *lpszGPCode, int lpszUseTime, char* pFlag)
{
	return 0;
}

int ACCOUNTDB::PCUseTime(char *lpszPID, char *lpszCID, char *ServerName, char *lpszGPCode, int lpszUseTime, char *lpszIP, char* pFlag)
{
	return 0;
}

int ACCOUNTDB::SetGameUser(char *lpszPID, char *lpszCID, char *ServerName, char* pFlag)
{
	return 0;
}

int ACCOUNTDB::GetGPCode(char *GPCode, char *PG)
{
	return 0;
}

int ACCOUNTDB::PCUse(char *lpszPID, char *lpszGPCode, char *ServerName, char *lpszIP, int lpszRNo, char *s_guid)
{
	return 0;
}

int ACCOUNTDB::PCSearchReserv(char *lpszuserid, int lpszRNo, char *lpszGPCode, char *IP, char *NewRNo, char *NewGPCode, char *NewDisuseDay, char *NewUsePeriod, char *s_guid)
{
	return 0;
}

int ACCOUNTDB::ChangeUseridGPCodeGame(int PCRNo, char *lpszUserID, char *lpszNewGPCOde, char *lpszNewDisUseDay, char *lpszNewUsePeriod, char *lpszNewGUID)
{
	return 0;
}

int ACCOUNTDB::PCRNoReservProduct(int lpszRNo, char *lpszGPCode)
{
	return 0;
}

int ACCOUNTDB::CharLog(char *lpszPID, int lpszCharType, char *lpszLID, char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, int lpszExp, int lpszGameMoney, int nFlag)
{
	return 0;
}

int ACCOUNTDB::GameLog(char *lpszPID, int lpszCharType, char *lpszCID, char *lpszGPCode, char *lpszIP, char *m_guid, int m_nServerPacket, int m_nClientPacket, int nFlag)
{
	return 0;
}

int ACCOUNTDB::CharRecordLog(char *lpszPID, int lpszCharType, char *lpszLID, char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, __int64 lpszExp, int lpszGameMoney, int nFlag)
{
	return 0;
}

int ACCOUNTDB::SetAllGameUser()
{
	return 0;
}

int ACCOUNTDB::ServerDown()
{
	return 0;
}

int ACCOUNTDB::ServerExit()
{
	return 0;
}

int ACCOUNTDB::LogITEMRecord(char *lpszPID, LogITEM *LogItem)
{
	return 0;
}

int ACCOUNTDB::LogITEMTradeRecord(char *lpszPID, LogITEM_TRADE *LogItem)
{
	return 0;
}

int ACCOUNTDB::LogITEMPostRecord(char *lpszPID, LogGiftITEM *GiftItem)
{
	return 0;
}

int ACCOUNTDB::IPRecord(char *lpszServerName, JUNGIP *JungIP)
{
	return 0;
}

int ACCOUNTDB::IPRecordTime(char *lpszServerName, JUNGIP *JungIP)
{
	return 0;
}

int ACCOUNTDB::WithSODRecordRanks(char *lpszUserID, char *lpszCharName, int CharType, int point, int KillCount, int GLevel, int TotalPoint, int TotalUser, int SuccessUser, int PCRNo, char *lpszGPCode, char *lpszIP, long servertime)
{
	return 0;
}

int ACCOUNTDB::UserStar(char *lpszPID, int lpszStar)
{
	return 0;
}

int ACCOUNTDB::UserStarRecord(char *lpszPID, int lpszStar, char *GameLogonID, int CharType, int GLevel, int Gubun)
{
	return 0;
}

int ACCOUNTDB::Contribution(char *lpszUserID, char *lpszCharName, int lpszMoney)
{
	return 0;
}

int ACCOUNTDB::ClanMoney(char *lpszPID, char *GameLogonID, int lpszClanMoney, int Gubun)
{
	return 0;

}

int ACCOUNTDB::EventJoin(char *lpszUserID, char *lpszCharName)
{
	return 0;
}

__int64 ACCOUNTDB::SiegeMoney(char *lpszPID, char *GameLogonID, __int64 lpszSiegeMoney, int Gubun)
{
	return 0;
}

int ACCOUNTDB::SiegeMoneyTax(char *lpszClanMarkNum, __int64 lpszMx, __int64 lpszAg, __int64 lpszSh, __int64 lpszPo1, __int64 lpszPo2, __int64 lpszPo3, __int64 lpszFo, __int64 lpszWp, __int64 lpszSk, __int64 lpszTt, __int64 lpszTax)
{
	return 0;
}

int ACCOUNTDB::PollRecord(char *lpszPID, time_t PollTime, struct POLL *PollItem, int Gubun)
{
	return 0;
}

//////////////////////////////// ÇØ¿ÜÆÇ URS ////////////////////////
int ACCOUNTDB::URSLoginIP(const char *ID, const char *PW, const char *IP)
{
	return 0;
}

int ACCOUNTDB::URSLogoutIP(const char *ID, const char *IP)
{
	return 0;
}

int ACCOUNTDB::URSLogoutIPBlock(const char *ID, const char *IP, const int min)
{
	return 0;
}

int ACCOUNTDB::Expined(char *szID)
{
	return 0;
}

//////////////////////// ExpGameTime //////////////////////
int ACCOUNTDB::GetExpGameTime(char *lpszuserid, char *lpszChName, long GamePlayTime)
{
	return 0;
}

//////////////////////// ClanDB //////////////////////
CLANDB::CLANDB()
{
}

CLANDB::~CLANDB()
{
}

int CLANDB::Initialize(char* _servername, char* _databaseName)
{
	strcpy(serverName, _servername);
	SQLDATA_BASE::Initialize(_databaseName);
	return TRUE;
}

int CLANDB::GetTicket()
{
	int result = rand() % 1000;
	if (result == 0) {
		result = rand() % 1000;
		if (!result) {
			result = 1;
		}
	}
	return result;
}

int CLANDB::GetClanSubIDX(char* lpszuserid, char* lpszChName)
{
	SQLRETURN r;
	wsprintf(stmt_buf, "SELECT  IDX FROM UL WHERE userid='%s' AND ChName='%s' ", lpszuserid, lpszChName);
	r = SQLExecDirect(stmt_hdl, (SQLCHAR*)stmt_buf, SQL_NTS);
	SQLFreeStmt(stmt_hdl, SQL_CLOSE);

	int idx = 0;

	// Changed parts
	r = SQLBindCol(stmt_hdl, 1, SQL_INTEGER, &idx, 0, NULL);

	if (SQLFetch(stmt_hdl) == SQL_NO_DATA_FOUND) {			//No account
		SQLDATA_BASE::End_ODBC();
		return 0;
	}

	return idx;
}

int CLANDB::ClanUserTicket(char *lpszuserid, char *lpszChName, char *lpszIP, int TicketNum, int gubun)
{
	if (SQLDATA_BASE::Start_ODBC() == false) {
		return 0;
	}

	SQLRETURN r;
	if (gubun > 0)
	{
		wsprintf(stmt_buf, "DELETE FROM CT WHERE userid='%s'", lpszuserid);
		r = SQLExecDirect(stmt_hdl, (SQLCHAR*)stmt_buf, SQL_NTS);

	}
	else
	{
		wsprintf(stmt_buf, "INSERT INTO CT (userid, ChName, IP, ServerName, SNo) values('%s', '%s', '%s', '%s', '%d')", lpszuserid, lpszChName, lpszIP, serverName, TicketNum);
		r = SQLExecDirect(stmt_hdl, (SQLCHAR*)stmt_buf, SQL_NTS);

	}

	SQLFreeStmt(stmt_hdl, SQL_CLOSE);

	End_ODBC();
	return 0;
}

int CLANDB::ClanUserTicKets(char *lpszuserid, char *lpszChName, char *lpszIP, int TicketNum, int gubun, int CharType, int CharLevel)
{
	if (SQLDATA_BASE::Start_ODBC() == false) {
		return 0;
	}

	SQLRETURN r;
	if (gubun > 0)
	{
		wsprintf(stmt_buf, "DELETE FROM CT WHERE userid='%s'", lpszuserid);
		r = SQLExecDirect(stmt_hdl, (SQLCHAR*)stmt_buf, SQL_NTS);
		SQLFreeStmt(stmt_hdl, SQL_CLOSE);
		if (r == SQL_ERROR) {

		}
	}
	else
	{
		wsprintf(stmt_buf, "INSERT INTO CT (userid, ChName, IP, ServerName, SNo) values ('%s', '%s', '%s', '%s', '%d')", lpszuserid, lpszChName, lpszIP, serverName, TicketNum);
		r = SQLExecDirect(stmt_hdl, (SQLCHAR*)stmt_buf, SQL_NTS);
		SQLFreeStmt(stmt_hdl, SQL_CLOSE);
		if (r == SQL_ERROR) {

		}
	}

	int idx = GetClanSubIDX(lpszuserid, lpszChName);
	if (idx > 0) {
		wsprintf(stmt_buf, "UPDATE UL SET ChType=%d,ChLv=%d WHERE MIDX=%d", CharType, CharLevel, idx);
		r = SQLExecDirect(stmt_hdl, (SQLCHAR*)stmt_buf, SQL_NTS);
		SQLFreeStmt(stmt_hdl, SQL_CLOSE);
		if (r == SQL_ERROR) {

		}
	}


	SQLDATA_BASE::End_ODBC();
	return 0;
}

int CLANDB::ClanPointInsert(char *lpszuserid, char *lpszChName, char * lpszIP, int TicketNum, int Cpoint)
{
	return 0;
}

int CLANDB::ClanUserMarkNum(char *lpszuserid, char *lpszChName)
{
	return 0;
}


#endif
