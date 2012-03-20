/* myclient.cc: sample client program */

#include "../shared/connection.h"
//#include "stringCmd.h"
#include "../shared/protocol.h"
#include "client.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>    /* for exit() and atoi() */

using namespace std;
using namespace protocol;
using namespace client_server;

int Client::getCommand(string& cmd){
	return scmd.getCommand(cmd);
}

void Client::printHelp(){
	map<std::string,int>::reverse_iterator it;
	it = scmd.commands.rbegin();
	cout << CLEAR << "Please use one of the following commands:" << endl;
	while(it != scmd.commands.rend()){
		cout << "\t" << it->first << endl;
		++it;
	}
}

// TODO: Add COM_END and paramaters according to chap.6
void Client::handleCommand(int nbr){	
	switch(nbr){
		case Protocol::COM_LIST_NG:					
			send.comListNG(conn);
			get.ansListNG(conn);
			break;
		case Protocol::COM_CREATE_NG:	
			send.comCreateNG(conn);
			get.ansCreateNG(conn);
			break;
		case Protocol::COM_DELETE_NG:			
			send.comDeleteNG(conn);
			get.ansDeleteNG(conn);
			break;
		case Protocol::COM_LIST_ART:			
			send.comListArt(conn);
			get.ansListArt(conn);
			break;
		case Protocol::COM_CREATE_ART:			
			send.comCreateArt(conn);
			get.ansCreateArt(conn);
			break;
		case Protocol::COM_DELETE_ART:			
			send.comArt(conn,false);
			get.ansDeleteArt(conn);
			break;
		case Protocol::COM_GET_ART:			
			send.comArt(conn,true);
			get.ansGetArt(conn);
			break;

	}
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
			c.handleCommand(nbr);
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



