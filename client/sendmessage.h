#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include "../shared/connection.h"
#include "../shared/protocol.h"
#include "../shared/connectionclosedexception.h"

#include <string>

class SendMessage{
	public:
		void comCreateNG(client_server::Connection& c);
		void comListNG(client_server::Connection& c);
		void comDeleteNG(client_server::Connection& c);
		void comListArt(client_server::Connection& c);
		void comCreateArt(client_server::Connection& c);
		void comArt(client_server::Connection& c, const bool);
	private:
		void writeNumber(int value, client_server::Connection& c) throw(client_server::ConnectionClosedException);
		void sendStringParameter(const std::string& s, client_server::Connection& c);
};


#endif
