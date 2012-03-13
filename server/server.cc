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

unsigned int readSize(Connection* conn)
throw(ConnectionClosedException) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int readConst(Connection* conn)
throw(ConnectionClosedException) {
    unsigned char lsb = conn->read();
	//cout << "lsb: " << lsb << endl; // dleete me
    return (0x00 << 24) | (0x00 << 16) | (0x00 << 8) | lsb;
}


/** Read a string from the server. */
string readString(Connection* conn) throw (InvalidStringException) {
    string s;
    char ch;
	if((ch = conn->read()) == Protocol::PAR_STRING){
		unsigned int n = readSize(conn);
		for(unsigned int i = 0; i < n; ++i){
			s += conn->read();
		}
		return s;
	}
    throw InvalidStringException();
}


/** Send the integer 'value' to the server. */
void writeNumber(Connection& conn, int value) throw(ConnectionClosedException) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)  & 0xFF);
    conn.write(value & 0xFF);
	cout << "writeNumber: " << (((value >> 24) & 0xFF) | ((value >> 16) & 0xFF) | ((value >> 8)  & 0xFF) | (value & 0xFF)) << endl;
}


/** Send one byte constant. */
void writeConst(Connection& conn, int value) throw(ConnectionClosedException) {
    conn.write((value & 0xFF));
	cout << "writeConst: " << (value & 0xFF) << endl;
}


/** Write string an corresponding string length N before. */
void writeString(Connection& conn, const string& s) throw(ConnectionClosedException) {
	writeNumber(conn, s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        conn.write(s[i]);
    }
    conn.write(Protocol::ANS_END);
}


void wtf(Connection* conn, int nbr) throw (ConnectionClosedException){
	cout << "Retarded message received.." << endl;
	stringstream ss(stringstream::in | stringstream::out);
	ss << "Message invalid, \'" << nbr << "\'.";
	writeString(*conn, ss.str());
	throw ConnectionClosedException();
}


void com_list_ng(Connection* conn){
	cout << "Listing newsgroups.. " << endl;
	readConst(conn); // Read COM_END

	writeConst(*conn, Protocol::ANS_LIST_NG);
	writeConst(*conn, Protocol::PAR_NUM);
	writeNumber(*conn, 0); // Nbr of newsgroups we're sending.
	writeConst(*conn, Protocol::ANS_END);
}
void com_create_ng(Connection* conn){
	string ng = readString(conn);
	cout << "Creating newsgroup: " << ng << endl;
	
	readConst(conn); // Read COM_END

	writeConst(*conn, Protocol::ANS_CREATE_NG);
	writeConst(*conn, Protocol::ANS_ACK); // [ACK | (NACK & ERR_NG_ALREADY_EXISTS)] If group was created
	writeConst(*conn, Protocol::ANS_END);
}
void com_del_ng(Connection* conn){
	unsigned int ngrp = readSize(conn);
	cout << "Deleting newsgroup: " << ngrp << endl;

	readConst(conn); // Read COM_END

	writeConst(*conn, Protocol::ANS_DELETE_NG);
	writeConst(*conn, Protocol::ANS_NAK); // [ANS_ACK | ANS_NAK & ERR_NG_DOES_NOT_EXIST]
	writeConst(*conn, Protocol::ERR_NG_DOES_NOT_EXIST);
	writeConst(*conn, Protocol::ANS_END);
}
void com_list_art(Connection* conn){
	unsigned int ngrp = readSize(conn);
	cout << "Listing articles.. " << ngrp << endl;

	readConst(conn); // Read COM_END

	writeConst(*conn, Protocol::ANS_LIST_NG);
	writeConst(*conn, Protocol::ANS_NAK); // [ANS_ACK  num_p(PAR_NUM!!"#!"#) [num_p(id) string_p(title)]* | ANS_NAK & ERR_NG_DOES_NOT_EXIST]
	writeConst(*conn, Protocol::ERR_NG_DOES_NOT_EXIST);
	writeConst(*conn, Protocol::ANS_END);
}
void com_create_art(Connection* conn){
	unsigned int ngrp = readSize(conn);
	string title = readString(conn);
	string auth = readString(conn);
	string txt = readString(conn);
	cout << "Creating article: " << ngrp << "\t" << title << " --" << auth << "\n" << txt << endl;

	readConst(conn); // Read COM_END
	
	writeConst(*conn, Protocol::ANS_CREATE_ART);
	writeConst(*conn, Protocol::ANS_ACK); // [ANS_ACK | ANS_NAK & ERR_NG_DOES_NOT_EXIST]
	writeConst(*conn, Protocol::ANS_END);
}
void com_del_art(Connection* conn){
	unsigned int ngrp = readSize(conn);
	unsigned int nart = readSize(conn);
	cout << "Deleting article: " << nart << " from group " << ngrp << endl;

	readConst(conn); // Read COM_END

	writeConst(*conn, Protocol::ANS_CREATE_ART);
	writeConst(*conn, Protocol::ANS_ACK); // [ANS_ACK | ANS_NAK [ ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST ] ]
	writeConst(*conn, Protocol::ANS_END);
	
}
void com_get_art(Connection* conn){
	unsigned int ngrp = readSize(conn);
	unsigned int nart = readSize(conn);
	cout << "Getting article: " << nart << " from group " << ngrp << endl;

	readConst(conn); // Read COM_END

	writeConst(*conn, Protocol::ANS_GET_ART);
	writeConst(*conn, Protocol::ANS_NAK); // [ANS_ACK strin_p string_p string_p | ANS_NAK [ ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST ]]
	writeConst(*conn, Protocol::ERR_NG_DOES_NOT_EXIST);
	writeConst(*conn, Protocol::ANS_END);
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
                int nbr = readConst(conn);
				cout << "Command received: " << nbr << endl;
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
