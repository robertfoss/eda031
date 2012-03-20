#include "../shared/connection.h"
#include "../shared/protocol.h"
#include "../shared/connectionclosedexception.h"
#include "sendmessage.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <cstdlib>    /* for exit() and atoi() */

using namespace std;
using namespace protocol;
using namespace client_server;

// TODO: Extract from here and server to separate class.
void SendMessage::writeNumber(int value, client_server::Connection& conn) throw(ConnectionClosedException) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)  & 0xFF);
    conn.write(value & 0xFF);
}

void SendMessage::sendStringParameter(const string& s, client_server::Connection& conn){
	conn.write(Protocol::PAR_STRING);
	writeNumber(s.size(),conn);
	for (size_t i = 0; i < s.size(); ++i) {
	        conn.write(s[i]);
	}
}


void SendMessage::comListNG(client_server::Connection& conn){
	conn.write(Protocol::COM_LIST_NG);
	conn.write(Protocol::COM_END);
}

void SendMessage::comCreateNG(client_server::Connection& conn){
	cout << "Enter title: ";
	string title;
	getline(cin,title);

	conn.write(Protocol::COM_CREATE_NG);
	sendStringParameter(title,conn);
	conn.write(Protocol::COM_END);
}

void SendMessage::comDeleteNG(client_server::Connection& conn){
	cout << "Enter newsgroup ID number: ";
	string idString;
	int id;
	getline(cin,idString);
	id = atoi(idString.c_str());

	conn.write(Protocol::COM_DELETE_NG);
	conn.write(Protocol::PAR_NUM);
	writeNumber(id,conn);
	conn.write(Protocol::COM_END);
}

void SendMessage::comListArt(client_server::Connection& conn){
	cout << "Enter newsgroup ID number: ";
	string idString;
	int id;
	getline(cin,idString);
	id = atoi(idString.c_str());

	conn.write(Protocol::COM_LIST_ART);
	conn.write(Protocol::PAR_NUM);
	writeNumber(id,conn);
	conn.write(Protocol::COM_END);
}

void SendMessage::comCreateArt(client_server::Connection& conn){
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
	writeNumber(id,conn);
	sendStringParameter(title,conn);
	sendStringParameter(author,conn);
	sendStringParameter(content,conn);
	conn.write(Protocol::COM_END);
}


void SendMessage::comArt(client_server::Connection& conn, const bool isGet){
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
	writeNumber(ngId,conn);
	conn.write(Protocol::PAR_NUM);
	writeNumber(artId,conn);
	conn.write(Protocol::COM_END);

}

