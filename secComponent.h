//secComponent.h

#include <string>

using namespace std;



class secComponent {
	private:
       
        
	public:
		secComponent();
        int encrypt(string, string, int, char*);
        int decrypt(char*, int, string, int, char *);
        string rc4(int, int, char*, int);
		
};

