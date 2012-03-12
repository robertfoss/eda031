/* myclient.cc: sample client program */

#include "../clientserver/connection.h"
#include "../clientserver/connectionclosedexception.h"
#include "stringCmd.h"
#include "../clientserver/protocol.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>    /* for exit() and atoi() */

using namespace std;
using namespace protocol;
using client_server::Connection;
using client_server::ConnectionClosedException;

StringCmd scmd;

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
 * Read a string from the server.
 */

string readString(Connection& conn) {
    string s;
    char ch;
    while ((ch = conn.read()) != '$')  // '$' is end of string
        s += ch;
    return s;
}

int getCommand(string& cmd){
	return scmd.getCommand(cmd);
}

void printHelp(){
	map<std::string,int>::reverse_iterator it;
	it = scmd.commands.rbegin();
	cout << "Please use one of the following commands:" << endl;
	while(it != scmd.commands.rend()){
		cout << it->first << endl;
		++it;
	}
}

// TODO: Add COM_END and paramaters according to chap.6
void sendCommand(int nbr, Connection& conn){
	cout << "sending.. " << nbr << endl;
	switch(nbr){
		case Protocol::COM_LIST_NG:			
			conn.write(Protocol::COM_LIST_NG);
			break;
		case Protocol::COM_CREATE_NG:			
			conn.write(Protocol::COM_CREATE_NG;
			break;
		case Protocol::COM_DELETE_NG:			
			conn.write(Protocol::COM_DELETE_NG);
			break;
		case Protocol::COM_LIST_ART:			
			conn.write(Protocol::COM_LIST_ART);
			break;
		case Protocol::COM_CREATE_ART:			
			conn.write(Protocol::COM_CREATE_ART);
			break;
		case Protocol::COM_DELETE_ART:			
			conn.write(Protocol::COM_DELETE_ART);
			break;
		case Protocol::COM_GET_ART:			
			conn.write(Protocol::COM_GET_ART);
			break;

	}
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: client host-name port-number" << endl;
        exit(1);
    }
    

    Connection conn(argv[1], atoi(argv[2]));
    if (! conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
    
    cout << "Type a command or help to list all commands: ";
    int nbr;
	string cmd;
	
    while (getline(cin,cmd)) {
		
		// translate every char to lowercase.
		transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	
		nbr = getCommand(cmd);
		
		if(nbr > 0){
			

			sendCommand(nbr, conn);

//			getAnswer() // ANS_ACK -> ok! ANS_NAK -> Error
			// Do stuff


		}else if(nbr == Protocol::COM_HELP){
			printHelp();
		}else if(nbr == Protocol::COM_EXIT){
			cout << "Good bye!" << endl;
			exit(0);
		}
/*
        try {
            writeNumber(nbr, conn);
            string answer = readString(conn);
            cout << nbr << " is " << answer << endl;
            cout << "Type another number: ";
        } 
        catch (ConnectionClosedException&) {
            cerr << "Server closed down!" << endl;
            exit(1);
        }
*/
    }
}



