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
using client_server::Connection;
using client_server::ConnectionClosedException;


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

int getCommand(string& cmd, StringCmd& scmd){
	return scmd.getCommand(cmd);
}

void printHelp(StringCmd& scmd){
	map<std::string,int>::reverse_iterator it;
	it = scmd.commands.rbegin();
	cout << "Please use one of the following commands:" << endl;
	while(it != scmd.commands.rend()){
		cout << it->first << endl;
		++it;
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
	StringCmd scmd;
    while (getline(cin,cmd)) {
		
		// translate every char to lowercase.
		transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	
		nbr = getCommand(cmd, scmd);
		
		if(nbr > 0){

			// Do stuff


		}else if(nbr == 0){
			printHelp(scmd);
		}else if(nbr == -1){
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



