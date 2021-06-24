#ifndef __SQLSTRUCTURES_H__
#define __SQLSTRUCTURES_H__

#define	JUNGIP_MAX	1024

struct JUNGIP {
	int	  IPCount;	// 처리할 IP 갯수
	DWORD IP[JUNGIP_MAX];	// IP 정보
};

struct	_LogITEM {
	DWORD	dwCode;
	DWORD	dwINo[2];
};

//아이템 구조체
struct LogITEM {
	int 		size;			// 사용한 구조체의 크기
	char 		UserID[32];  		// 사용자 ID
	char 		CharName[24]; 		// 사용자 캐랙터 이름
	long 		IP; 			// 사용자 IP            <===== 추가
	int 		Flag;			// 아이템 정보
	int		ItemCount;		// 처리할 아이템 갯수
	_LogITEM	Item[32];		// 아이템 정보
};

//거래 아이템 처리 구조체
struct LogITEM_TRADE {
	int 		size;			// 사용한 구조체의 크기
	char 		UserID[32];  		// 사용자 ID
	char 		CharName[24]; 		// 사용자 캐랙터 이름
	long		IP;			// 사용자 IP         <========= 추가 
	int 		Flag;			// 아이템 정보

	char 		TUserID[32];		// 거래 아이디
	char 		TCharName[24];		// 거래 캐랙터 이름
	long 		TIP;			// 거래 IP           <========= 추가 
	int		Money;			// 거래한 금액
	int		ItemCount;		// 거래한 아이템 갯수
	_LogITEM 	Item[32];		// 아이템 정보
};

//아이템 구조체
struct LogGiftITEM {
	int 		size;		// 사용한 구조체의 크기
	char 		UserID[32];  	// 사용자 ID
	char 		CharName[24]; 	// 사용자 캐랙터 이름
	long 		IP; 		// 사용자 IP
	int 		Flag;		// 아이템 정보    (아이템교부13)
	long		GNo;		// 인증번호
	char 		TEL[16]; 	// 사용자 전화번호
	_LogITEM 	Gitem;		// 아이템 정보
};

#endif