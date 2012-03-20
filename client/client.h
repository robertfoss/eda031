#ifndef CLIENT_H
#define CLIENT_H

#include "../shared/connection.h"
#include "../shared/connectionclosedexception.h"
#include "../shared/invalidstringexception.h"
#include "stringCmd.h"
#include "sendmessage.h"
#include "getmessage.h"

#define CLEAR ("\033[2J")

class Client{
	private:		
		StringCmd scmd;
		SendMessage send;
		GetMessage get;
		client_server::Connection conn;		
	public:
		Client(client_server::Connection&);
		int getCommand(std::string&);
		void handleCommand(int);
		void printHelp();
};


#endif
