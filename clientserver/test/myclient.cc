/* myclient.cc: sample client program */

#include "connection.h"
#include "connectionclosedexception.h"
#include "../protocol.h"

#include <iostream>
#include <string>
#include <cstdlib>    /* for exit() and atoi() */
#include <sstream>

using namespace std;
using client_server::Connection;
using client_server::ConnectionClosedException;
using protocol::Protocol;


/*
 * Send the integer 'value' to the server.
 */
void writeNumber(int value, Connection& conn) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)  & 0xFF);
    conn.write(value & 0xFF);
}

/*
 * Send one byte constant.
 */
void writeConst(int value, Connection& conn) {
    conn.write(value & 0xFF);
}

/*
 * Send one byte constant.
 */
void writeString(const string& str, Connection& conn) {
	for(size_t i = 0; i < str.size(); ++i)
    	conn.write(str[i]);
}


/*
 * Read a string from the server.
 */
string readString(Connection& conn) {
    string s;
    char ch;
    while ((ch = conn.read()) != Protocol::ANS_END) s += ch;
    return (s += ch);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
        exit(1);
    }
    
    Connection conn(argv[1], atoi(argv[2]));
    if (! conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    
    int nbr;
    while (true) {
    	cout << "Type a command [2 40 3 hej]: ";
        try {
			//istringstream iss(cin);
			cin >> nbr;
			cout << "\"" << nbr << "\"" << endl;
            writeNumber(nbr, conn);
			cin >> nbr;
			cout << "\"" << nbr << "\"" << endl;
			writeConst(nbr, conn);
			cin >> nbr;
			cout << "\"" << nbr << "\"" << endl;
            writeNumber(nbr, conn);

			string str;
			cin >> str;
			cout << str << endl;
			

			stringstream ss(stringstream::in | stringstream::out);
			ss << readString(conn);
            cout << ss.str() << endl;
            cout << "Type another number: ";
        } 
        catch (ConnectionClosedException&) {
            cerr << "Server closed down!" << endl;
            exit(1);
        }
    }
}

