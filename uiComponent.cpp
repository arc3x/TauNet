#include <iostream>
#include <string>
#include <stdlib.h>
#include "uiComponent.h"
#include "regComponent.h"
#include "msgComponent.h"

using namespace std;

uiComponent::uiComponent() {
	cur_menu = MAIN;
	select = "0"; 
}

void uiComponent::bind_regComponent(regComponent * in) {
	reg = in;
}

void uiComponent::run() {
	while(select!="q") {
		//draw menu
		switch(cur_menu) {
			case(MAIN):
				draw_main();
				break;
			case(SET_DEST):
				draw_set_dest();
				break;
			case(SETTINGS):
				draw_settings();
				break;
			case(SETTINGS_VIEW_KEY):
				draw_view_key();
				break;
			case(SETTINGS_SET_KEY):
				draw_set_key();
				break;
			case(SETTINGS_ADD_IP):
				draw_add_ip();
				break;
		}
		
		//get input
		getInput();

		//use input
		switch(cur_menu) {
			case(MAIN):
				if(select=="1") {
					cur_menu = SET_DEST;
				} else if (select == "2") {
					cin.ignore();
					reg->msg->run();
				} else if (select == "s") {
					cur_menu = SETTINGS;
				} 
				break;
			case(SET_DEST):				
				if (select == "s") {
					cur_menu = SETTINGS;
				} else if (select == "m") {
					cur_menu = MAIN;
				} else {
					int i;
					i = atoi(select.c_str());
					if(i<=reg->dest_count && i>=1) {
						reg->setDest(i-1);			
						cur_menu = MAIN;
					}
				}
				 
				
				break;
			//settings->view_key menu options	
			case(SETTINGS_VIEW_KEY):				
				if (select == "s") {
					cur_menu = SETTINGS;
				} else if (select == "m") {
					cur_menu = MAIN;
				} 
				break;
			case(SETTINGS_SET_KEY):				
				reg->setKey(select);
				cur_menu = SETTINGS_VIEW_KEY;
				break;
			case(SETTINGS_ADD_IP):
				reg->addIPToTable(select);
				cur_menu = MAIN;
				break;	
			case(SETTINGS):
				if (select == "1") {
					cur_menu = SETTINGS_VIEW_KEY;
				} else if (select == "2") { 
					cur_menu = SETTINGS_SET_KEY;
					prompt_helper = "Enter New Crypto Key";
				} else if (select == "3") {
					cur_menu = SETTINGS_ADD_IP;
					prompt_helper = "Add Destination {name,IP}";
				} else if (select == "m") {
					cur_menu = MAIN;
				}
				break;	
		}
	}
}

void uiComponent::draw_menu_header(string menu_name) {
	string destination;
	if(reg->dest==-1) {
		destination = "Unset";
	} else {
		destination = "[" + reg->name_list[reg->dest] + "] " + reg->dest_list[reg->dest];
	}
	system("clear");
	cout << "|------------TauNet v0.01------------|" << endl;
	cout << "| Destination: " << destination << endl;
	cout << "| Menu: " << menu_name << endl;
	cout << "|------------------------------------|" << endl;
}



void uiComponent:: draw_menu_footer() {
	cout << "|" << endl;
	cout << "|  q. Quit" << endl;
	cout << "|------------------------------------|" << endl << endl;
}

void uiComponent::draw_main() {
	draw_menu_header("Main");
	cout << "|  1. Set Destination" << endl;
	cout << "|  2. Start Messenger" << endl;
	cout << "|" << endl;
	cout << "|  s. Settings Menu" << endl;
	cout << "|  d. Debug Menu" << endl;
	draw_menu_footer();
}

void uiComponent::draw_settings() {
	draw_menu_header("Settings");
	cout << "|  1. View crypto key" << endl;
	cout << "|  2. Change crypto key" << endl;
	cout << "|" << endl;
	cout << "|  3. Add IP" << endl;
	cout << "|" << endl;
	cout << "|  m. Main Menu" << endl;
	draw_menu_footer();
}

void uiComponent::draw_debug() {
	draw_menu_header("Debug");
	cout << "|  1. View IP list" << endl;
	cout << "|  2. View Crypto Key" << endl;
	cout << "|" << endl;
	cout << "|  m. Main Menu" << endl;
	draw_menu_footer();
}

void uiComponent::draw_set_dest() {
	draw_menu_header("Set Destination");
	reg->printDest();
	cout << "|" << endl;
	cout << "|  s. Settings Menu" << endl;
	cout << "|  m. Main Menu" << endl;
	draw_menu_footer();
}



void uiComponent::draw_view_key() {
	draw_menu_header("View Crypto Key");
	cout << "|  Key: " << reg->key << endl;
	cout << "|" << endl;
	cout << "|  s. Settings Menu" << endl;
	cout << "|  m. Main Menu" << endl;
	draw_menu_footer();
}

void uiComponent::draw_set_key() {
	draw_menu_header("View Crypto Key");
	cout << "|  Current Key: " << reg->key << endl;
	cout << "|------------------------------------|" << endl << endl;
}


void uiComponent::draw_add_ip() {
	draw_menu_header("Add User,IP");
}

void uiComponent::getInput() {
	cin.clear();
	if(prompt_helper!="") {
		cout << "TauNet [" << prompt_helper << "]> ";
		prompt_helper="";
	} else {
		cout << "TauNet> ";
	}
 	cin >> select;
}


