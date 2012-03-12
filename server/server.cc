/* myserver.cc: sample server program */
#include "../clientserver/server.h"
#include "../clientserver/protocol.h"
#include "../clientserver/connection.h"
#include "../clientserver/connectionclosedexception.h"
#include "../clientserver/invalidstringexception.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
using namespace std;
using client_server::Server;
using client_server::Connection;
using client_server::ConnectionClosedException;
using protocol::Protocol;
using client_server::InvalidStringException;
typedef unsigned int uint;

int readNumber(Connection* conn)
throw(ConnectionClosedException) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Read a string from the server.
 */
string readString(Connection* conn) throw (InvalidStringException) {
    string s;
    char ch;
	if((ch = conn->read()) == Protocol::PAR_STRING){
		int n = readNumber(conn);
		if( n < 0){
			throw InvalidStringException();
		}
		for(int i = 0; i < n; ++i){
			s += conn->read();
		}
		return s;
	}
    throw InvalidStringException();
}

void writeString(const string& s, Connection* conn)
throw(ConnectionClosedException) {
    for (size_t i = 0; i < s.size(); ++i) {
        conn->write(s[i]);
    }
    conn->write(Protocol::ANS_END);
}

void wtf(Connection* conn, int nbr) throw (ConnectionClosedException){
	cout << "Retarded message received.." << endl;
	stringstream ss(stringstream::in | stringstream::out);
	ss << "Message invalid, \'" << nbr << "\'.";
	writeString(ss.str(), conn);
	throw ConnectionClosedException();
}

void com_list_ng(Connection* conn){
	conn = conn;
	cout << "Listing newsgroups.. " << endl;
}
void com_create_ng(Connection* conn){
	string ng = readString(conn);
	cout << "Creating newsgroup: " << ng << endl;
}
void com_del_ng(Connection* conn){
	conn = conn;
	//, uint ngrp
	//cout << "Deleting newsgroup: " << ngrp << endl;
}
void com_list_art(Connection* conn){
	conn = conn;
	//, uint ngrp
	//cout << "Listing articles.. " << ngrp << endl;
}
void com_create_art(Connection* conn){
	conn = conn;
	//, uint ngrp, string title, string auth, string txt
	//cout << "Creating article: " << ngrp << "\t" << title << " --" << auth << "\n" << txt << endl;
}
void com_del_art(Connection* conn){
	conn = conn;
	//, uint ngrp, uint nart
	//cout << "Deleting article: " << nart << " from group " << ngrp << endl;
}
void com_get_art(Connection* conn){
	conn = conn;
	//, uint ngrp, uint nart
	//cout << "Getting article: " << nart << " from group " << ngrp << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: server port-number" << endl;
        exit(1);
    }
    Server server(atoi(argv[1]));
    if (! server.isReady()) {
        cerr << "Server initialization error" << endl;
        exit(1);
    }
    while (true) {
		cout << "Waiting for activity.." << endl;
        Connection* conn = server.waitForActivity();
        if (conn != 0) {
            try {
                int nbr = readNumber(conn);
				cout << "Number received: " << nbr << endl;
                switch (nbr){
					case Protocol::COM_LIST_NG:		com_list_ng(conn); 		break;
					case Protocol::COM_CREATE_NG:	com_create_ng(conn);	break;
					case Protocol::COM_DELETE_NG:	com_del_ng(conn);		break;
					case Protocol::COM_LIST_ART:	com_list_art(conn);		break;
					case Protocol::COM_CREATE_ART:	com_create_art(conn);	break;
					case Protocol::COM_DELETE_ART:	com_del_art(conn);		break;
					case Protocol::COM_GET_ART:		com_get_art(conn);		break;
					default:						wtf(conn, nbr);			break;
				}
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                delete conn;
                cout << "Client closed connection" << endl;
            }
        } else {
            server.registerConnection(new Connection);
            cout << "New client connects" << endl;
        }
    }
}
