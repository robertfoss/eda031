#ifndef STRINGCMD_H
#define STRINGCMD_H

#include <map>
#include <string>
#include "../clientserver/protocol.h"


class StringCmd {
	
	public:
		std::map<std::string,int> commands;
		std::map<std::string,int>::iterator iterator;
		StringCmd(){
			commands["exit"] = protocol::Protocol::COM_EXIT;
			commands["help"] = protocol::Protocol::COM_HELP;
			commands["list newsgroups"] = protocol::Protocol::COM_LIST_NG;
			commands["list newsgroups"] = protocol::Protocol::COM_LIST_NG;	
			commands["create newsgroup"] = protocol::Protocol::COM_CREATE_NG;
			commands["delete newsgroup"] = protocol::Protocol::COM_DELETE_NG;	
			commands["list articles"] = protocol::Protocol::COM_LIST_ART;	
			commands["create article"] = protocol::Protocol::COM_CREATE_ART;	
			commands["delete article"] = protocol::Protocol::COM_DELETE_ART;	
			commands["get article"] = protocol::Protocol::COM_GET_ART;	
		}


		int getCommand(std::string& cmd){
			if(commands.count(cmd) == 1){	
				return commands[cmd];
			}else{
				return protocol::Protocol::COM_HELP;
			}			
		}

			
};


#endif
