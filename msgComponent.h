/*
(c) Matthew Slocum

msgComponent.h

msgComponent is resbonaible for sending and revieving messages
    upon creation it spins off a thread to listen for incoming messages.
    the parent process takes input from the user and executes commands or sends messages.
*/

#include <string>

using namespace std;

class regComponent;
class secComponent;

class msgComponent {
	public:
		regComponent * reg;
		secComponent * sec;
		string command;
		msgComponent();
		void bind_regComponent(regComponent *);
		void bind_secComponent(secComponent *);
		void run();
		void get_input();
		void draw_get_input();
		void draw_header();
		void draw_dest();
		
};

