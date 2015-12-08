/*
(c) Matthew Slocum 2015

secComponent.test.cpp this file is responsible for unit test and component tests for the secComponent
*/

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
    
    for(unsigned int i=0; i<message.length(); i++) {
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
    cout << endl << "Unit Tests:" << endl;
    
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
    
    //test 7
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
    //Decrytion tests using known good encrypted values
    //
    cout << endl << "Decryption tests using known good values: " << endl;
    
    //test 8
    //decrypt tst/cstest1.cs1 
    //Key "asdfg".
    //Plaintext will be "This is a test of CipherSaber." with no newline. 
    zero(ciphertext,1000);
    zero(plaintext,1000);
    key="asdfg";
    string check = "This is a test of CipherSaber.";
    ifstream cipherFile;
    cipherFile.open("tst/cstest1.cs1");  
    int i=0;
    if (cipherFile.is_open()) {
        while (!cipherFile.eof()) {
            ciphertext[i] = cipherFile.get();
            i++;
        }       
    }
    cipherFile.close();            
    sec->decrypt(ciphertext, i-1, key, 1, plaintext);
    string out;
    for (int k=0; k<i-11; k++) {
        out+=plaintext[k];
    }        
    if(out==check) {
        cout << "Pass" << endl;
    } else {
        cout << "secComponent.test Test 8 failed" << endl;
    }
    
    //test 9
    //decrypt tst/cstest.cs2 
    //Key "asdfg".
    //Plaintext will be "This is a test of CipherSaber-2." with no newline. 
    zero(ciphertext,1000);
    zero(plaintext,1000);
    key="asdfg";
    check = "This is a test of CipherSaber-2.";
    cipherFile.open("tst/cstest.cs2");  
    i=0;
    if (cipherFile.is_open()) {
        while (!cipherFile.eof()) {
            ciphertext[i] = cipherFile.get();
            i++;
        }       
    }
    cipherFile.close();            
    sec->decrypt(ciphertext, i-1, key, 10, plaintext);
    out="";
    for (int k=0; k<i-11; k++) {
        out+=plaintext[k];
    }        
    if(out==check) {
        cout << "Pass" << endl;
    } else {
        cout << "secComponent.test Test 9 failed" << endl;
    }
    
    
    //test 10
    //decrypt cipher knight certificate
    //key ThomasJefferson
    key = "ThomasJefferson";
    char crypto_cert[20474];
    char plain_cert[20474];
    fstream cert;
    //read in the crypto file
    cipherFile.open("tst/cknight.cs1");  
    i=0;
    if (cipherFile.is_open()) {
        while (!cipherFile.eof()) {
            crypto_cert[i] = cipherFile.get();
            i++;
        }       
    }
    cipherFile.close();
    //decrypt it
    sec->decrypt_long(crypto_cert, i-1, key, 1, plain_cert);
    //write it out
    cert.open("tst/cknight.gif", fstream::out);
    if(cert.is_open()) {
        for(int k=0; k<i-1; k++) {
                cert << plain_cert[k];
        }        
    }       
    //Read in known good value
    ifstream trueCert;
    trueCert.open("tst/cknight.gif.ref");
    int flag = 0;
    i = 0;
    unsigned char c;
    if (trueCert.is_open()) {
        while (!trueCert.eof()) {
            c = trueCert.get();
            if(plain_cert[i] != c) {
                flag = 1;
            }            
            i++;
        }       
    }
    trueCert.close();
    //if the dont match fail the test. if they do then pass pass it
    if(flag==1) {
        cout << "Pass" << endl;
    } else {
        cout << "secComponent.test Test 10 failed" << endl;
    }
    
    return 0;
}

