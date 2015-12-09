/*
(c) Matthew Slocum 2015

secComponent.h

secComponent is responsible for the encryption/decryption of messages.
*/

#include <string>

using namespace std;

class secComponent {               
    public:
        secComponent();
        
        //encrypts input using CS2
        //arguemnts: plaintext, key, round of key scheduling, ciphertext(output)
        //return: length of ciphertext
        int encrypt(string m, string k, int r, char * ciphertext);
        
        //decrypts a CS2 encryption
        //arguemnts: ciphertext, ciphertext length, key, round of key scheduling, plaintext(output)
        //return: length of plaintext
        int decrypt(char* m, int m_len, string k, int r, char* plaintext);
        
        //decrypts a CS2 encryption
        //arguemnts: ciphertext, ciphertext length, key, round of key scheduling, plaintext(output)
        //return: length of plaintext        
        //this function is specifically for testing on encrypted files that are larger than the recommended lenght of a message
        int decrypt_long(char* m, int m_len, string k, int r, char* plaintext);
        
        //generates an rc4 keystream
        //arguments: desired length, rounds of key scheduling, key, keylength, keystream(output)
        void rc4(int n, int r, char* k, int l, char* keystream);     
};

