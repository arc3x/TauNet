/*
DEPENDENCIES:
ncurses? sudo apt-get install libncurses5-dev
boost


*/


#include <iostream>
#include <stdio.h>
#include <string.h>
#include "regComponent.h"
#include "uiComponent.h"
#include "msgComponent.h"
#include "secComponent.h"

using namespace std;

msgComponent * msg = new msgComponent();
regComponent * reg = new regComponent();
secComponent * reg = new secComponent();
uiComponent * ui= new uiComponent();

int main(int argc, char *argv[]) {
	msg->bind_regComponent(reg);
    msg->bind_secComponent(sec);
	reg->bind_msgComponent(msg);	
    ui->bind_regComponent(reg);

	if(argc > 1 && string(argv[1])=="-msg") {		
		msg->run();
	} else {
		ui->run();
	}
  
		
	
	
}
