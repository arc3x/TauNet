#include <iostream>
#include <stdlib.h>
#include <boost/asio.hpp>
#include <pthread.h>
#include <ncurses.h>
#include "msgComponent.h"
#include "regComponent.h"


using namespace std;
using boost::asio::ip::tcp;




msgComponent::msgComponent() {
	
}


//Code derived from:
// daytime_server.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
void* listen(void *threadid) {
    try
      {
        //extern msgComponent* msg;
        extern regComponent* reg;
        boost::asio::io_service io_service;

        tcp::endpoint endpoint(tcp::v4(), 13);
        tcp::acceptor acceptor(io_service, endpoint);

        for (;;)
        {
          tcp::iostream stream;
          boost::system::error_code ec;
          acceptor.accept(*stream.rdbuf(), ec);
          if (!ec)
          {           
            string line;
            getline(stream, line);
            cout.clear();
            cout << endl << endl << line << endl << endl << "TauNet [TO: " << reg->name_list[reg->dest] << "]> "; cout.flush();             
          }
        }
        
      }
      catch (std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        //std::cerr << "Press ENTER to continue...";
        //char t;
        //std::cin >> t;
      }
    pthread_exit(NULL);
}



void msgComponent::run() {
    //pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_t thread;
    int rc;
    rc = pthread_create(&thread, NULL, listen, NULL);
    if (rc){
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    
 
	command="";
	//cin.clear();cin.ignore();
	//fork for listener
	draw_header();
	draw_dest();
	while(command!=":q") {
        
		get_input();
		
		if(command==":dlist") {
			reg->printDest();
		}
		
		else if (command.find(":dset")==0) {
			//cout << command.substr(6) << endl;
			int d = atoi((command.substr(6)).c_str());
			d--;
			if(d>=0 && d<reg->dest_count) {
				reg->setDest(d);
				draw_dest();
			} else {
				cout << "Destination out of range." << endl;
			}
		}
        
        //its a message, send it!
        //CODE derived from daytime_client.cpp (Boost::asio)
        //Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
        //Distributed under the Boost Software License, Version 1.0 at http://www.boost.org/LICENSE_1_0.txt)
        else {
            try {            
                tcp::iostream stream(reg->dest_list[reg->dest], "13");
                if (!stream)
                {
                  std::cout << "Unable to connect: " << stream.error().message() << std::endl;
                  //return;
                }
                stream << command;
                //std::string line;
                //std::getline(stream, line);
                //std::cout << line << std::endl;
            }
            catch (std::exception& e) {
                std::cout << "Exception: " << e.what() << std::endl;
            }
        }
		
	}
}

void msgComponent::bind_regComponent(regComponent * in) {
	reg = in;
}

void msgComponent::draw_header() {
	system("clear");
	cout << "|------------TauNet Messenger------------|" << endl;
	cout << "|  Commands" << endl;
	cout << "|   :q        - quit" << endl;
	cout << "|   :dlist    - list destinations" << endl;
	cout << "|   :dset #   - set destination" << endl; 
	cout << "|------------TauNet Messenger------------|" << endl;
}

void msgComponent::draw_dest() {
	cout << "***     DESTINATION: [" << reg->name_list[reg->dest] << "]" << reg->dest_list[reg->dest] << "     ***" << endl;	
}

void msgComponent::get_input() {
	cout << endl;
	cin.clear();
	cout << "TauNet [TO: " << reg->name_list[reg->dest] << "]> ";
	getline(cin,command);
}

void msgComponent::draw_get_input() {
	//cout << endl;
	//cin.clear();
	cout << "TauNet [TO: " << reg->name_list[reg->dest] << "]> ";
	//getline(cin,command);
}
