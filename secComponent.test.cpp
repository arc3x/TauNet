#include "secComponent.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

int test_encrypt_decrypt_inverting(secComponent * sec, string message, string key, int r, char * ciphertext, char * plaintext) {
    
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

void zero(char * ar, int len) {
    for (int i=0; i<len; i++) {
        ar[i] = 0;
    }
}

int main(void) {
    secComponent * sec = new secComponent();
    int r = 20;
    char ciphertext [1000];
    char plaintext[1000];
    string key = "password";
    string message = "";
   
    
    //
    //'Unit' test for the encryption / decryption protocols
    //
    
    //test 1
    //encrypt+decrypt on a single letter message
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = "X";        
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl; 
    } else {
        cout << "secComponent.test Test 1 failed" << endl;
    }
    
    //test 2
    //encrypt+decrypt on a single word message 
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = "hello";
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl; 
    } else {
        cout << "secComponent.test Test 2 failed" << endl;
    }
    
    //test 3
    //encrypt+decrypt on a multi word message 
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = "hello world";
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl; 
    } else {
        cout << "secComponent.test Test 3 failed" << endl;
    }
        
    
    //test 4
    //encrypt+decrypt on a multi word message with special characters
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = "hello world!";
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl;  
    } else {
        cout << "secComponent.test Test 4 failed" << endl;
    }
    
    //test 5
    //encrypt+decrypt on a multi line message 
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = "hello world\r\nfish\r\n\r\nsalmon\r\n";
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl;  
    } else {
        cout << "secComponent.test Test 5 failed" << endl;
    }
    
    //test 6
    //encrypt+decrypt on a multi line message with special characters
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = "!@#$%%^&*\0x94hello world!\r\nfish\r\n\r\ns@lmon;\r\n";
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl;  
    } else {
        cout << "secComponent.test Test 6 failed" << endl;
    }
    
    //test 6
    //encrypt+decrypt on a multi line message with special characters
    zero(ciphertext,1000);
    zero(plaintext,1000);
    message = string("version: 0.2\r\n") 
                        + "from: " + "source_user" + "\r\n"
                        + "to: " + "dest_user" + "\r\n"
                        + "\r\n"
                        + "hey look a message!" + "\r\n";
                        
    if(test_encrypt_decrypt_inverting(sec, message, key, r, ciphertext, plaintext)) {
       cout << "Pass" << endl;  
    } else {
        cout << "secComponent.test Test 7 failed" << endl;
    }
    
    
    //
    //Decrytion tests using known encrypted values
    //
    cout << "Decryption tests using known good values: " << endl;
    
    //test 7
    //decrypt cstest1.cs1 
    //Key "asdfg".
    //Plaintext will be "This is a test of CipherSaber." with no newline. 
    zero(ciphertext,1000);
    zero(plaintext,1000);
    key="asdfg";
    ifstream cipherFile;
    cipherFile.open("tst/cstest1.cs1");
    
    char c;
    int i=0;
    char ciphertext2 [1000];
    if (cipherFile.is_open()) {
        while (!cipherFile.eof()) {
            //cipherFile >> ciphertext;
            ciphertext[i] = cipherFile.get();
            ciphertext2[i++] = cipherFile.get();
        }
        cout<<ciphertext << endl << endl;
        cout<<ciphertext2;
    }
    cipherFile.close();
    int plaintext_len = sec->decrypt(ciphertext, message.length()+10, key, r, plaintext);
    
    
    
    return 0;
}

