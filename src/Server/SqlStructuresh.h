#ifndef __SQLSTRUCTURES_H__
#define __SQLSTRUCTURES_H__

#define	JUNGIP_MAX	1024

struct JUNGIP {
	int	  IPCount;	// ó���� IP ����
	DWORD IP[JUNGIP_MAX];	// IP ����
};

struct	_LogITEM {
	DWORD	dwCode;
	DWORD	dwINo[2];
};

//������ ����ü
struct LogITEM {
	int 		size;			// ����� ����ü�� ũ��
	char 		UserID[32];  		// ����� ID
	char 		CharName[24]; 		// ����� ĳ���� �̸�
	long 		IP; 			// ����� IP            <===== �߰�
	int 		Flag;			// ������ ����
	int		ItemCount;		// ó���� ������ ����
	_LogITEM	Item[32];		// ������ ����
};

//�ŷ� ������ ó�� ����ü
struct LogITEM_TRADE {
	int 		size;			// ����� ����ü�� ũ��
	char 		UserID[32];  		// ����� ID
	char 		CharName[24]; 		// ����� ĳ���� �̸�
	long		IP;			// ����� IP         <========= �߰� 
	int 		Flag;			// ������ ����

	char 		TUserID[32];		// �ŷ� ���̵�
	char 		TCharName[24];		// �ŷ� ĳ���� �̸�
	long 		TIP;			// �ŷ� IP           <========= �߰� 
	int		Money;			// �ŷ��� �ݾ�
	int		ItemCount;		// �ŷ��� ������ ����
	_LogITEM 	Item[32];		// ������ ����
};

//������ ����ü
struct LogGiftITEM {
	int 		size;		// ����� ����ü�� ũ��
	char 		UserID[32];  	// ����� ID
	char 		CharName[24]; 	// ����� ĳ���� �̸�
	long 		IP; 		// ����� IP
	int 		Flag;		// ������ ����    (�����۱���13)
	long		GNo;		// ������ȣ
	char 		TEL[16]; 	// ����� ��ȭ��ȣ
	_LogITEM 	Gitem;		// ������ ����
};

#endif