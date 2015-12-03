#include "secComponent.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

using namespace std;

int test(secComponent * sec, string message, string key, int r, char * ciphertext, char * plaintext) {
    
    int ciphertext_len = sec->encrypt(message, key, r, ciphertext);
    int plaintext_len = sec->decrypt(ciphertext, message.length()+10, key, r, plaintext);
    
    //cout << endl << endl;
    //cout << "message: " << message << endl;
    //cout << "ciphertext: ";
    for(int i=0; i<ciphertext_len; i++) {
        //cout << ciphertext[i];
    }
    //cout << endl;
    //cout << "plaintext: ";
    for(int i=0; i<plaintext_len; i++) {
        //cout << plaintext[i];
    }
    //cout << endl;
    
    for(int i=0; i<message.length(); i++) {
        if(message[i]!=plaintext[i])
            return 0;
    }
    return 1;
}

int main(void) {
    secComponent * sec = new secComponent();
    int r = 20;
    char ciphertext [1000];
    char plaintext[1000];
    string key = "password";
    string message = "";
    
    //test 1
    //encrypt+decrypt on a single letter message
    message = "X";        
    if(test(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl; 
    } else {
        cout << "secComponent.test Test 1 failed" << endl;
    }
    
    //test 2
    //encrypt+decrypt on a single word message 
    message = "hello";
    if(test(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl; 
    } else {
        cout << "secComponent.test Test 2 failed" << endl;
    }
    
    //test 3
    //encrypt+decrypt on a multi word message 
    message = "hello world";
    if(test(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl; 
    } else {
        cout << "secComponent.test Test 3 failed" << endl;
    }
        
    
    //test 4
    //encrypt+decrypt on a multi word message with special characters
    message = "hello world!";
    if(test(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl;  
    } else {
        cout << "secComponent.test Test 4 failed" << endl;
    }
    
    return 1;
}

