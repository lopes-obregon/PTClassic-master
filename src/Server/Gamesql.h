#ifndef __GAMESQL_H__
#define __GAMESQL_H__

#include "..\\nettype.h"
#include "SqlStructuresh.h"

#ifdef _W_SERVER

#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <odbcss.h>

#define REM_LEN 1024

class SQLDATA_BASE
{
private:
	char connectString[256];
	char databaseName[64];

public:
	SQLHENV env_hdl;
	SQLHDBC conn_hdl;
	SQLHSTMT stmt_hdl;

	char stmt_buf[512];
	char errmsg[512];

public:
	SQLDATA_BASE();
	~SQLDATA_BASE();

	bool Initialize(char* _databasename);
	bool Start_ODBC();
	bool End_ODBC();

protected:
	void ShowErrorInfo(SQLRETURN rc, SQLSMALLINT hType, SQLHANDLE h);
};

class ACCOUNTDB : public SQLDATA_BASE
{
private:
	char serverName[64];

public:
	ACCOUNTDB();
	~ACCOUNTDB();

	int Initialize(char* _servername, char* _databaseName);
	int	LogOn(char *szID, char *szPassword);

	int SQLLogon(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *s_guid, char *lpszPCRNo, char *lpszGubunCode);

	int EventSQLLogon(char *lpszPID, char *lpszPWD, char *lpszIP, char *lpszGPCode, char *lpszDisuseDay, char *lpszUsePeriod, char *lpszS_guid, char *lpszPCRNo, char *lpszGubunCode);
	int SQLLogout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, int lpszUsePeriod, char *s_guid, int PCRNo);
	int EventSQLLogout(char *lpszPID, char *lpszCID, char *lpszIP, char *lpszGPCode, time_t lpszDisuseDay, long lpszUsePeriod, char *s_guid, long PCRNo, char *lpszNewGPCode, char *News_guid);

	int UserUseTime(char *lpszPID, char *lpszCID, char *lpszIP, char *ServerName, char *lpszGPCode, int lpszUseTime, char* pFlag);
	int PCUseTime(char *lpszPID, char *lpszCID, char *ServerName, char *lpszGPCode, int lpszUseTime, char *lpszIP, char* pFlag);
	int SetGameUser(char *lpszPID, char *lpszCID, char *ServerName, char* pFlag);
	int GetGPCode(char *GPCode, char *PG);
	int PCUse(char *lpszPID, char *lpszGPCode, char *ServerName, char *lpszIP, int lpszRNo, char *s_guid);
	int PCSearchReserv(char *lpszuserid, int lpszRNo, char *lpszGPCode, char *IP, char *NewRNo, char *NewGPCode, char *NewDisuseDay, char *NewUsePeriod, char *s_guid);
	int ChangeUseridGPCodeGame(int PCRNo, char *lpszUserID, char *lpszNewGPCOde, char *lpszNewDisUseDay, char *lpszNewUsePeriod, char *lpszNewGUID);

	int PCRNoReservProduct(int lpszRNo, char *lpszGPCode);

	int CharLog(char *lpszPID, int lpszCharType, char *lpszLID, char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, int lpszExp, int lpszGameMoney, int nFlag);
	int GameLog(char *lpszPID, int lpszCharType, char *lpszCID, char *lpszGPCode, char *lpszIP, char *m_guid, int m_nServerPacket, int m_nClientPacket, int nFlag);
	int CharRecordLog(char *lpszPID, int lpszCharType, char *lpszLID, char *lpszGPCode, char *lpszIP, char *lpszServerName, int lpszGameLevel, __int64 lpszExp, int lpszGameMoney, int nFlag);

	int SetAllGameUser();
	int ServerDown();
	int ServerExit();

	int LogITEMRecord(char *lpszPID, LogITEM *LogItem);
	int LogITEMTradeRecord(char *lpszPID, LogITEM_TRADE *LogItem);
	int LogITEMPostRecord(char *lpszPID, LogGiftITEM *GiftItem);

	int IPRecord(char *lpszServerName, JUNGIP *JungIP);
	int IPRecordTime(char *lpszServerName, JUNGIP *JungIP);

	int WithSODRecordRanks(char *lpszUserID, char *lpszCharName, int CharType, int point, int KillCount, int GLevel, int TotalPoint, int TotalUser, int SuccessUser, int PCRNo, char *lpszGPCode, char *lpszIP, long servertime);
	int UserStar(char *lpszPID, int lpszStar);
	int UserStarRecord(char *lpszPID, int lpszStar, char *GameLogonID, int CharType, int GLevel, int Gubun);
	int Contribution(char *lpszUserID, char *lpszCharName, int lpszMoney);
	int ClanMoney(char *lpszPID, char *GameLogonID, int lpszClanMoney, int Gubun);
	int EventJoin(char *lpszUserID, char *lpszCharName);

	__int64 SiegeMoney(char *lpszPID, char *GameLogonID, __int64 lpszSiegeMoney, int Gubun);
	int SiegeMoneyTax(char *lpszClanMarkNum, __int64 lpszMx, __int64 lpszAg, __int64 lpszSh, __int64 lpszPo1, __int64 lpszPo2, __int64 lpszPo3, __int64 lpszFo, __int64 lpszWp, __int64 lpszSk, __int64 lpszTt, __int64 lpszTax);
	int PollRecord(char *lpszPID, time_t PollTime, struct POLL *PollItem, int Gubun);

	//////////////////////////////// ÇØ¿ÜÆÇ URS ////////////////////////
	int URSLoginIP(const char *ID, const char *PW, const char *IP);
	int URSLogoutIP(const char *ID, const char *IP);
	int URSLogoutIPBlock(const char *ID, const char *IP, const int min);
	int Expined(char *szID);

	//////////////////////// ExpGameTime.DLL //////////////////////
	int GetExpGameTime(char *lpszuserid, char *lpszChName, long GamePlayTime);
};

class CLANDB : public SQLDATA_BASE
{
private:
	char serverName[64];
	char databaseName[64];
public:
	CLANDB();
	~CLANDB();

	int Initialize(char* _servername, char* _databaseName);

	int GetTicket();
	int GetClanSubIDX(char* lpszuserid, char* lpszChName);
	int ClanUserTicket(char *lpszuserid, char *lpszChName, char *lpszIP, int TicketNum, int gubun);
	int ClanUserTicKets(char *lpszuserid, char *lpszChName, char *lpszIP, int TicketNum, int gubun, int CharType, int CharLevel);
	int ClanPointInsert(char *lpszuserid, char *lpszChName, char * lpszIP, int TicketNum, int Cpoint);
	int ClanUserMarkNum(char *lpszuserid, char *lpszChName);
};

extern ACCOUNTDB sqlAccountDb;
extern CLANDB  sqlClanDb;

#endif

////////////////////////////////////////////////////////////////////////
struct ODBC_CONFIG {
	char	Dsn[32];			//Á¢¼Ó ¼­ºñ½º¸í
	char	Name[32];			//Á¢¼ÓÀÚ ID
	char	Password[32];		//Á¢¼ÓÀÚ ¾ÏÈ£

	char	Table[32];			//Å×ÀÌºí ÀÌ¸§
	char	Table_Id[32];		//Å×ÀÌºí °èÁ¤
	int		Table_Password;		//Å×ÀÌºí ¾ÏÈ£ ¿­
	int		Table_Play;			//Å×ÀÌºí Çã°¡ ¿­

	int		CloseAccount;		//Å×½ºÆ® À¯Àú¸¸ Çã¿ë

};

extern ODBC_CONFIG	Odbc_Config;

#endif