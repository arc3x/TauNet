/*
(c) Matthew Slocum 2015

msgComponent.cpp 

msgComponent is resbonaible for sending and revieving messages
    upon creation it spins off a thread to listen for incoming messages.
    the parent process takes input from the user and executes commands or sends messages.

*/

#include <iostream>
#include <stdlib.h>
#include <boost/asio.hpp>
#include <pthread.h>
#include <string>
       //#include <sys/types.h>
       //#include <sys/socket.h>
       //#include <netdb.h>
#include "msgComponent.h"
#include "regComponent.h"
#include "secComponent.h"


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

//Function for listener thread
//This fucntion listens for incoming connections, recieves a message, decrypts it and displays it.
void* listen(void *threadid) {
    try
      {
        extern regComponent* reg;
        extern secComponent* sec;
        boost::asio::io_service io_service;

        tcp::endpoint endpoint(tcp::v4(), 6283);
        tcp::acceptor acceptor(io_service, endpoint);

        for (;;)
        {
          tcp::iostream stream;
          boost::system::error_code ec;
          acceptor.accept(*stream.rdbuf(), ec);
          if (!ec)
          {           
            string message = "";
            string line;           
            char c;
            char msg [1024];
            int msg_count=0;
            
            //get message
            while(stream.get(c)) {           
                msg[msg_count++]=c;
            }

                    //debug code
                    /*
                    cout << "Recieveing (" << msg_count << "):" << endl;
                    for(int i=0; i<msg_count; i++) {
                        cout << msg[i];                        
                    }
                    cout << endl;
                    */
            
            //make holder for plaintext
            char plaintext[msg_count];   
            
            //decrypt message and store in plaintext
            sec->decrypt(msg, msg_count, reg->key, 20, plaintext);
            
            //display plaintext message
            cout << endl << endl;
            for(int i=0; i<msg_count-10; i++) {
                cout << plaintext[i];
            }
            cout << endl;
            
            
            //redraw user input prompt
            cout.clear();cout.flush();    
            cout << endl << endl << "TauNet [TO: " << reg->name_list[reg->dest] << "]> ";        cout.clear();      
          }
        }
        
      }
      catch (std::exception& e)
      {
        std::cerr << e.what() << std::endl;        
      }
    pthread_exit(NULL);
}



void msgComponent::run() {
    //pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //for later
    
    //spin off thread for listener
    pthread_t thread;
    int rc;
    rc = pthread_create(&thread, NULL, listen, NULL);
    if (rc){
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    
 
	command="";
	//draw some ui
    draw_header();
	draw_dest();
    
    //c&c loop
	while(command!=":q") {        
        //get input
		get_input();
		
        //parse special commands
        //list destinations
		if(command==":dlist") {
			reg->printDest();
		}		
        //set destination
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
        
        //else, its a message, send it!
        //CODE derived from daytime_client.cpp (Boost::asio)
        //Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
        //Distributed under the Boost Software License, Version 1.0 at http://www.boost.org/LICENSE_1_0.txt)
        else {
            try {           
                boost::asio::io_service io_service;
                boost::asio::ip::tcp::resolver resolver(io_service);
                boost::asio::ip::tcp::resolver::query query(reg->dest_list[reg->dest], "6283");
                //XXXbelow is some crap REVISE IT!
                for(boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
                            i != boost::asio::ip::tcp::resolver::iterator();
                            ++i)
                {
                    boost::asio::ip::tcp::endpoint end = *i;
                    //std::cout << end.address() << ' ';
                }
                
                //attempt to open connection
                tcp::iostream stream(reg->dest_list[reg->dest], "6283");
                if (!stream)
                {
                  std::cout << "Unable to connect: " << stream.error().message() << std::endl;
                  //return;
                }
                
                //build message
                string message;
                message = string("version: 0.2\r\n") 
                        + "from: " + reg->username + "\r\n"
                        + "to: " + reg->name_list[reg->dest] + "\r\n"
                        + "\r\n"
                        + command + "\r\n";
                
                //init holder for ciphertext
                char ciphertext [message.length()+10];
                
                //encrypt the message
                sec->encrypt(message,reg->key,20, ciphertext);   
                
                //send the message
                for(int i=0; i<message.length()+10; i++) {
                    stream << ciphertext[i];
                }

                //close the connection
                stream.close();
            }
            catch (std::exception& e) {
                std::cout << "Exception: " << e.what() << std::endl;
            }
        }
		
	}
}


//let the msgComponent know about the regComponent
void msgComponent::bind_regComponent(regComponent * in) {
	reg = in;
}


//let the msgComponent know about the secComponent
void msgComponent::bind_secComponent(secComponent * in) {
	sec = in;
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
	cout << "TauNet [TO: " << reg->name_list[reg->dest] << "]> ";
}
