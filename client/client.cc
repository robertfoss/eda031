/* myclient.cc: sample client program */

#include "../clientserver/connection.h"
#include "../clientserver/connectionclosedexception.h"
//#include "stringCmd.h"
#include "../clientserver/protocol.h"
#include "client.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>    /* for exit() and atoi() */

using namespace std;
using namespace protocol;
using client_server::ConnectionClosedException;

/*
 * Send the integer 'value' to the server.
 */
void Client::writeNumber(int value) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)  & 0xFF);
    conn.write(value & 0xFF);
}

/*
 * Read a string from the server.
 */
string Client::readString() {
    string s;
    char ch;
    while ((ch = conn.read()) != '$')  // '$' is end of string
        s += ch;
    return s;
}

int Client::getCommand(string& cmd){
	return scmd.getCommand(cmd);
}

void Client::printHelp(){
	map<std::string,int>::reverse_iterator it;
	it = scmd.commands.rbegin();
	cout << "\n\n\n\n";	
	cout << "Please use one of the following commands:" << endl;
	while(it != scmd.commands.rend()){
		cout << "\t" << it->first << endl;
		++it;
	}
}

// TODO: Add COM_END and paramaters according to chap.6
void Client::sendCommand(int nbr){	
	switch(nbr){
		case Protocol::COM_LIST_NG:					
			conn.write(Protocol::COM_LIST_NG);
			conn.write(Protocol::COM_END);
			break;
		case Protocol::COM_CREATE_NG:	
			com_create_ng();
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

//string get_string_paramater

void Client::com_create_ng(){
	cout << "Enter title: ";
	string title;
	getline(cin,title);
	

	conn.write(Protocol::COM_CREATE_NG);
}

Client::Client(client_server::Connection& c) {
	conn = c;//		conn(host,port);
	if (! conn.isConnected()) {
        cerr << "Connection attempt failed" << endl;
        exit(1);
    }
}



int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: client host-name port-number" << endl;
        exit(1);
    }
	client_server::Connection conn(argv[1], atoi(argv[2]));
    Client c(conn);

   
    
    cout << "Type a command or help to list all commands: ";
    int nbr;
	string cmd;
	
    while (getline(cin,cmd)) {
		
		// translate every char to lowercase.
		transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	
		nbr = c.getCommand(cmd);
		
		if(nbr > 0){
			c.sendCommand(nbr);

//			getAnswer() // ANS_ACK -> ok! ANS_NAK -> Error
			// Do stuff


		}else if(nbr == Protocol::COM_HELP){
			c.printHelp();
		}else if(nbr == Protocol::COM_EXIT){
			cout << "Goodbye!" << endl;
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



