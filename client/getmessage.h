#ifndef GETMESSAGE_H
#define GETMESSAGE_H

#include "../shared/connection.h"
#include "../shared/protocol.h"
#include "../shared/invalidstringexception.h"
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
