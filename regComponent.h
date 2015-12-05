/*
(c) Matthew Slocum

regComponent.h

regComponent is responsible for passing data around the rest of the program.
    it tracks users, ips, the current destination, and the crypto key.

*/



#include <string>

using namespace std;

class msgComponent;

class regComponent {
	//private:
	public:
		string message_out;
		string message_in;
		string key;
        string username;
		string IV;
		string * dest_list; //ips
		string * name_list; //usernames
		int dest;			//current destination
		int dest_count;		//total destinations
		msgComponent * msg;
	//public:
		regComponent();
		
		void bind_msgComponent(msgComponent *);

		void loadIPTable();
		void addIPToTable(string ip);

		void printState();
		void printDest();

		void setDest(int d);
		int getDest();
		
		void setKey(string k);
		void saveKey(string k);
		void loadKey();
		void loadUsername();
        

		void setMsgIn(string m);
		string getMsgIn();

		void setMsgOut(string m);
		string getMsgOut();

};

