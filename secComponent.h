/*
(c) Matthew Slocum 2015

secComponent.h

secComponent is responsible for the encryption/decryption of messages.
*/

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

