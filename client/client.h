#ifndef CLIENT_H
#define CLIENT_H

#include "../clientserver/connection.h"
#include "../clientserver/connectionclosedexception.h"
#include "../clientserver/invalidstringexception.h"
#include "stringCmd.h"

class Client{
	private:
		StringCmd scmd;
		client_server::Connection conn;
		void writeNumber(int) throw(client_server::ConnectionClosedException);
		void sendStringParameter(const std::string& s);
		void comCreateNG();
		void comListNG();
		void comDeleteNG();
		void comListArt();
		void comCreateArt();
		void comArt(const bool);
		std::string readString() throw(client_server::InvalidStringException);
		int readNumber() throw(client_server::ConnectionClosedException);
		void ansListNG();
		void ansCreateNG();
		void ansDeleteNG();
		void ansListArt();
	public:
		Client(client_server::Connection&);
		int getCommand(std::string&);
		void handleCommand(int);
		void printHelp();
};


#endif
