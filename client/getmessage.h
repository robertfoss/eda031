#ifndef GETMESSAGE_H
#define GETMESSAGE_H

#include "../clientserver/connection.h"
#include "../clientserver/protocol.h"
#include "../clientserver/invalidstringexception.h"
#include <string>

class GetMessage{
	public:
		void ansListNG(client_server::Connection& c);
		void ansCreateNG(client_server::Connection& c);
		void ansDeleteNG(client_server::Connection& c);
		void ansListArt(client_server::Connection& c);
		void ansCreateArt(client_server::Connection& c);
		void ansDeleteArt(client_server::Connection& c);
		void ansGetArt(client_server::Connection& c);
	private:
		std::string readString(client_server::Connection& c) throw (client_server::InvalidStringException);
		int readNumber(client_server::Connection& c) throw(client_server::ConnectionClosedException);
};



#endif
