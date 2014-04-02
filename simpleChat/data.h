
#define MYPORT "8888"
#define MCONN  10

enum MesgType {
	REGISTER=0,
	LOGIN,
	CHAT,
	FILES,
	LOGOUT
};

enum LoginMsgType {
	PASS=0,
	FAIL_INCORRECT,
	FAIL_USR_UNAVAIL
};

typedef struct Message {
	char data[256];
	int msgType;
	int result;
}Message;


