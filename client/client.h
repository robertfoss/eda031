#ifndef CLIENT_H
#define CLIENT_H

#include "../clientserver/connection.h"
#include "stringCmd.h"

class Client{
	private:
		StringCmd scmd;
		client_server::Connection conn;
		void com_create_ng();
	public:
		Client(client_server::Connection&);
		int getCommand(std::string&);
		void writeNumber(int);
		void sendCommand(int);
		void printHelp();
		std::string readString();
};


#endif
