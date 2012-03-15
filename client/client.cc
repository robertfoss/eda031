/* myclient.cc: sample client program */

#include "../clientserver/connection.h"
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
using namespace client_server;

/*
 * Read a string from the server.
 */
string Client::readString() throw (InvalidStringException) {
    string s;
    char ch;
	if((ch = conn.read()) == Protocol::PAR_STRING){
		int n = readNumber();
		for(int i = 0; i < n; ++i){
			s += conn.read();
		}
		return s;
	}
    throw InvalidStringException();
}


int Client::readNumber() throw(ConnectionClosedException) {
    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int Client::getCommand(string& cmd){
	return scmd.getCommand(cmd);
}

void Client::printHelp(){
	map<std::string,int>::reverse_iterator it;
	it = scmd.commands.rbegin();
	cout << "\nPlease use one of the following commands:" << endl;
	while(it != scmd.commands.rend()){
		cout << "\t" << it->first << endl;
		++it;
	}
}

// TODO: Add COM_END and paramaters according to chap.6
void Client::handleCommand(int nbr){	
	switch(nbr){
		case Protocol::COM_LIST_NG:					
			comListNG();
			ansListNG();
			break;
		case Protocol::COM_CREATE_NG:	
			comCreateNG();
			ansCreateNG();
			break;
		case Protocol::COM_DELETE_NG:			
			comDeleteNG();
			ansDeleteNG();
			break;
		case Protocol::COM_LIST_ART:			
			comListArt();
			ansListArt();
			break;
		case Protocol::COM_CREATE_ART:			
			comCreateArt();
			break;
		case Protocol::COM_DELETE_ART:			
			comArt(false);
			break;
		case Protocol::COM_GET_ART:			
			comArt(true);
			break;

	}
}

// TODO: Extract from here and server to separate class.
void Client::writeNumber(int value) throw(ConnectionClosedException) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)  & 0xFF);
    conn.write(value & 0xFF);
	cout << "writeNumber: " << (((value >> 24) & 0xFF) | ((value >> 16) & 0xFF) | ((value >> 8)  & 0xFF) | (value & 0xFF)) << endl;
}

void Client::sendStringParameter(const string& s){
	conn.write(Protocol::PAR_STRING);
	writeNumber(s.size());
	for (size_t i = 0; i < s.size(); ++i) {
	        conn.write(s[i]);
	}
}

void Client::comListNG(){
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
}

void Client::ansListNG(){
	int n;
	if(conn.read() == Protocol::ANS_LIST_NG){
		if(conn.read() == Protocol::PAR_NUM){
			n = readNumber();
			if(n == 0){
				cout << "No newsgroups found." << endl;
			}
			for(int i = 0; i < n; ++i){
				int a;
				if(conn.read() == Protocol::PAR_NUM){
					a = readNumber();
					string s = readString();
					cout << a << "\t" << s << endl;
				}
			}
			conn.read(); // read ANS_END
		}
	}else{
		cout << "Server Error, please try again." << endl;
	}
}


void Client::comCreateNG(){
	cout << "Enter title: ";
	string title;
	getline(cin,title);

	conn.write(Protocol::COM_CREATE_NG);
	sendStringParameter(title);
	conn.write(Protocol::COM_END);
}


void Client::ansCreateNG(){
	char ch = conn.read();
	if(ch == Protocol::ANS_CREATE_NG){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			cout << "Newsgroup created." << endl;
			conn.read(); // read ANS_END
		}else if(ch == Protocol::ANS_NAK){
			conn.read(); // read ERR_NG_ALREADY_EXISTS
			cout << "Newsgroup already exists." << endl;	
			conn.read(); // read ANS_END	
		}else{
			cout << "Server Error, please try again." << endl;
		}
	}else{
		cout << "Server Error, ANS please try again." << endl;
	}
}

void Client::comDeleteNG(){
	cout << "Enter newsgroup ID number: ";
	string idString;
	int id;
	getline(cin,idString);
	id = atoi(idString.c_str());

	conn.write(Protocol::COM_DELETE_NG);
	conn.write(Protocol::PAR_NUM);
	writeNumber(id);
	conn.write(Protocol::COM_END);
}

void Client::ansDeleteNG(){
	char ch = conn.read();
	if(ch == Protocol::ANS_DELETE_NG){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			cout << "Newsgroup deleted." << endl;
			conn.read(); // read ANS_END
		}else if(ch == Protocol::ANS_NAK){
			conn.read(); // read ERR_NG_DOES_NOT_EXIST
			cout << "Newsgroup does not exist." << endl;	
			conn.read(); // read ANS_END	
		}else{
			cout << "Server Error, please try again." << endl;
		}
	}else{
		cout << "Server Error, please try again." << endl;
	}
}

void Client::comListArt(){
	cout << "Enter newsgroup ID number: ";
	string idString;
	int id;
	getline(cin,idString);
	id = atoi(idString.c_str());

	conn.write(Protocol::COM_LIST_ART);
	conn.write(Protocol::PAR_NUM);
	writeNumber(id);
	conn.write(Protocol::COM_END);
}

void Client::ansListArt(){
	char ch = conn.read();
	int n;
	if(ch == Protocol::ANS_LIST_ART){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			if(conn.read() == Protocol::PAR_NUM){
				n = readNumber();
				for(int i = 0; i < n; ++i){
					ch = conn.read(); // read PAR_NUM
					cout << readNumber() << "\t" << readString() << endl;					
				}
				conn.read(); // read ANS_END
			}
		}else{
			conn.read(); // read ERR_NG_DOES_NOT_EXIST
			cout << "Newsgroup does not exist." << endl;	
			conn.read(); // read ANS_END	
		}
	}else{
		cout << "Server Error, please try again." << endl; 
	}	
	
}

void Client::comCreateArt(){
	cout << "Enter newsgroup ID for the article: ";
	string idString;
	int id;
	getline(cin,idString);
	id = atoi(idString.c_str());
	
	cout << "\nEnter title of the article: ";
	string title;
	getline(cin,title);

	cout << "\nEnter the name of the author: ";
	string author;
	getline(cin,author);

	cout << "\nFinally, write the content of the article, end with '$$$'." << endl;
	string content;
	string line;
	while(getline(cin,line)){
		size_t pos = line.find("$$$");
		if(pos != string::npos){
			line = line.substr(0,pos);
			content += line;
			break;		
		}else{		
			content += line + "\n";
		}
	}

	conn.write(Protocol::COM_CREATE_ART);
	conn.write(Protocol::PAR_NUM);
	writeNumber(id);
	sendStringParameter(title);
	sendStringParameter(author);
	sendStringParameter(content);
	conn.write(Protocol::COM_END);
}

void Client::comArt(const bool isGet){
	cout << "Enter newsgroup ID: ";
	string ngIdString;
	int ngId;
	getline(cin,ngIdString);
	ngId = atoi(ngIdString.c_str());

	cout << "Enter article ID: ";
	string artIdString;
	int artId;
	getline(cin,artIdString);
	artId = atoi(artIdString.c_str());

	if(isGet){
		conn.write(Protocol::COM_GET_ART);
	}else{
		conn.write(Protocol::COM_DELETE_ART);
	}

	conn.write(Protocol::PAR_NUM);
	writeNumber(ngId);
	conn.write(Protocol::PAR_NUM);
	writeNumber(artId);
	conn.write(Protocol::COM_END);

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



