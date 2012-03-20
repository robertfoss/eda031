#include "../clientserver/connection.h"
//#include "stringCmd.h"
#include "../clientserver/protocol.h"
#include "getmessage.h"

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
string GetMessage::readString(client_server::Connection& conn) throw (InvalidStringException) {
    string s;
    char ch;
	if((ch = conn.read()) == Protocol::PAR_STRING){
		int n = readNumber(conn);
		for(int i = 0; i < n; ++i){
			s += conn.read();
		}
		return s;
	}
    throw InvalidStringException();
}


int GetMessage::readNumber(client_server::Connection& conn) throw(ConnectionClosedException) {
    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void GetMessage::ansListNG(client_server::Connection& conn){
	int n;
	if(conn.read() == Protocol::ANS_LIST_NG){
		if(conn.read() == Protocol::PAR_NUM){
			n = readNumber(conn);
			if(n == 0){
				cout << "No newsgroups found." << endl;
			}
			for(int i = 0; i < n; ++i){
				int a;
				if(conn.read() == Protocol::PAR_NUM){
					a = readNumber(conn);
					string s = readString(conn);
					cout << a << "\t" << s << endl;
				}
			}
			conn.read(); // read ANS_END
		}
	}else{
		cout << "Server Error, please try again." << endl;
	}
}

void GetMessage::ansCreateNG(client_server::Connection& conn){
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

void GetMessage::ansDeleteNG(client_server::Connection& conn){
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

void GetMessage::ansListArt(client_server::Connection& conn){
	char ch = conn.read();
	int n;
	if(ch == Protocol::ANS_LIST_ART){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			if(conn.read() == Protocol::PAR_NUM){
				n = readNumber(conn);
				for(int i = 0; i < n; ++i){
					ch = conn.read(); // read PAR_NUM
				cout << readNumber(conn);				
					cout << "\t" << readString(conn) << endl;
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

void GetMessage::ansCreateArt(client_server::Connection& conn){
	char ch = conn.read();
	if(ch == Protocol::ANS_CREATE_ART){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			cout << "Article created." << endl;
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

void GetMessage::ansDeleteArt(client_server::Connection& conn){
	char ch = conn.read();
	if(ch == Protocol::ANS_DELETE_ART){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			cout << "Article deleted." << endl;
			conn.read(); // read ANS_END
		}else if(ch == Protocol::ANS_NAK){
			ch = conn.read(); // read Error
			if(ch == Protocol::ERR_NG_DOES_NOT_EXIST){
				cout << "Newsgroup does not exist." << endl;
			}else{
				cout << "Article does not exist." << endl;
			}
			cout << "Newsgroup does not exist." << endl;	
			conn.read(); // read ANS_END	
		}else{
			cout << "Server Error, please try again." << endl;
		}
	}else{
		cout << "Server Error, please try again." << endl;
	}
}


void GetMessage::ansGetArt(client_server::Connection& conn){
	char ch = conn.read();
	if(ch == Protocol::ANS_GET_ART){
		ch = conn.read(); // read ACK/NACK
		if(ch == Protocol::ANS_ACK){
			string title = readString(conn);
			string author = readString(conn);
			string content = readString(conn);
			cout << endl;
			cout << title << " by " << author << endl;
			cout << endl;
			cout << content << endl;
			conn.read(); // read ANS_END
		}else if(ch == Protocol::ANS_NAK){
			ch = conn.read(); // read Error
			if(ch == Protocol::ERR_NG_DOES_NOT_EXIST){
				cout << "Newsgroup does not exist." << endl;
			}else{
				cout << "Article does not exist." << endl;
			}	
			conn.read(); // read ANS_END	
		}else{
			cout << "Server Error, please try again." << endl;
		}
	}else{
		cout << "Server Error, please try again." << endl;
	}
}
