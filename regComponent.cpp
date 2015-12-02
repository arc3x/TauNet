#include <iostream>
#include <string.h>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include "regComponent.h"
#include "msgComponent.h"

using namespace std;

regComponent::regComponent() {	
	loadKey();
	loadIPTable();
}

void regComponent::bind_msgComponent(msgComponent * in) {
	msg = in;
}

void regComponent::loadKey() {
	ifstream dest_file;
	//******************************//
	//****** READ CRYPTO KEY
	//******************************//
	//get lines in file
	dest_file.open("key.sec");	
	if(dest_file.is_open()) {
		while(!dest_file.eof()) {
			string s;
			getline(dest_file,s);
			if(s!="") {
				key=s;
			}
		}
	}
	dest_file.close();
}

void regComponent::loadIPTable() {
	//******************************//
	//****** READ IP/USER LIST
	//******************************//
	//read line in file
	ifstream dest_file;
	
	//load last dest
	dest_file.open("curdest.sec");	
	if(dest_file.is_open()) {
		while(!dest_file.eof()) {
			string s;
			getline(dest_file,s);
			if(s!="") {
				dest=atoi(s.c_str());
			}
		}
	}
	dest_file.close();
	
	dest_count = 0;
	dest_file.open("dest.sec");
	int lines = 0;
	if(dest_file.is_open()) {
		while(!dest_file.eof()) {
			string s;
			getline(dest_file,s);
			if(s!="") {
				lines++;
			}
		}
	}
	dest_file.close();
	
	//init arrays
	dest_list = new string[lines];
	name_list = new string[lines];
	
	//read file
	dest_file.open("dest.sec");
	if(dest_file.is_open()) {
		while(!dest_file.eof()) {
			string user;
			string ip;
			string s;
			getline(dest_file,s);
			if(s!="") {
				user=s.substr(0,s.find(","));
				ip=s.substr(s.find(",")+1);
				
				dest_list[dest_count]=ip;
				name_list[dest_count++]=user;		
			}
		}
	}
	dest_file.close();
}

void regComponent::addIPToTable(string ip) {
	ofstream dest_file;
	dest_file.open("dest.sec", ios::app);
	dest_file << endl << ip;
	dest_file.close();

	delete [] dest_list;
	delete [] name_list;
	
	loadIPTable();
}



void regComponent::saveKey(string k) {
	system("rm key.sec");
	ofstream key_file;
	key_file.open("key.sec");
	key_file << k;
	key_file.close();
}

void regComponent::setKey(string k) {
	key=k;
	saveKey(k);
}

void regComponent::printState() {
	cout << "Dest: " << name_list[dest];
}

void regComponent::printDest() {
	int i=0;
	for (i=0; i<dest_count; i++) {
		cout << "|  " << i+1 << ": [" << name_list[i] << "] " << dest_list[i] << endl; 
	}
}

void regComponent::setDest(int d) {
	dest = d;
	system("rm curdest.sec");
	ofstream key_file;
	key_file.open("curdest.sec");
	key_file << d;
	key_file.close();
}

int regComponent::getDest() {
	return dest;
}

