//msgComponent.h

#include <string>

using namespace std;

class regComponent;

class msgComponent {
	public:
		regComponent * reg;
		string command;
		msgComponent();
		void bind_regComponent(regComponent *);
		void run();
		void get_input();
		void draw_get_input();
		void draw_header();
		void draw_dest();
		
};

