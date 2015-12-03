#include "secComponent.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(void) {
    secComponent * sec = new secComponent();
    
    string message = "X";
    string key = "password";
    int r = 20;
    char ciphertext [1000];
    char plaintext[1000];
    
    
    sec->encrypt(message, key, r, ciphertext);
    sec->decrypt(ciphertext, message.length(), key, r, plaintext);
    
    cout << endl << endl;
    cout << "message: " << message << endl;
    cout << "ciphertext: " << ciphertext << endl;
    cout << "plaintext: " << plaintext << endl;
    
    return 1;
}

