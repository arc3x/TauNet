/*
(c) Matthew Slocum 2015

secComponent.cpp

secComponent is responsible for the encryption/decryption of messages.


This file contains code from the public domain found at https://web.archive.org/web/20050110121238/http://www.xs4all.nl/~cg/ciphersaber/comp/c++.txt
// ciphersabre-1 by graydon hoare
// placed in public domain, jun 2000

This file contain sudo code from https://github.com/PSU-CS-300-Fall2015/ciphersaber2
this sudo code is licenced under the MIT Licence
*/
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include "secComponent.h"

using namespace std;

secComponent::secComponent() {

}

//"Produce an RC4 keystream of length" n "with" r "rounds of key scheduling given key" k "of length" l
//returns keystream
string secComponent::rc4(int n, int r, char* k, int l) {
  //init array
  unsigned char S [256] = { };
  for(int i=0; i<256; i++) {
      S[i]=i;
  }  
  //do key scheduling
  int j=0;
  for(int ri=0; ri<r; ri++) {
      for(int i=0; i<256; i++) {
          j = (j + S[i] + k[i%l]) % 256;
          swap(S[i], S[j]);
      }
  }
  //produce the keystream
  string keystream;
  j=0;
  int ip;
  for(int i=0; i<n; i++) {
      ip = (i+1) % 256;
      j = (j+S[ip]) % 256;
      swap(S[ip], S[j]);
      keystream.append(to_string(S[(S[ip]+S[j]) % 256]));      
  }
  return keystream;
}

//"Ciphersaber-2 encrypt message" m "with key" k "and" r "rounds of key scheduling" "outputting to" ciphertext
//return length of ciphertext
int secComponent::encrypt(string m, string k, int r, char * ciphertext) {
    //get length of message
    int n = m.length();
    string iv = "1234567890"; //CHANGEME
    //create a holder(kp) for the key + iv
    char kp[10+k.length()];
    //load the key into kp  
    for(unsigned int i=0; i<k.length(); i++) {
        kp[i] = k[i];
    }
    //load the iv into kp
    for(unsigned int i=k.length(); i<k.length()+10; i++) {
        kp[i] = iv[i-k.length()];
    }
    //get a keystream
    string keystream = rc4(n, r, kp, k.length()+10);
    //load the iv into the payload
    for(int i=0; i<10; i++) {
        ciphertext[i] = iv[i];
    }
    //encrypt the message with the keystream and save to output
    for(int i=0; i<n; i++) {      
        ciphertext[i+10] = m[i] ^ keystream[i];  //replaced xor with ^            
    }
    //return size of ciphertext
    return n+10; 
}

//"Ciphersaber-2 decrypt ciphertext" m "with key" k "and" r "rounds of key scheduling" "ouputting to" plaintext
//returns length of plaintext
int secComponent::decrypt(char* m, int m_len, string k, int r, char* plaintext) {
  //get length of m (unneccisary)  
  int n = m_len;
  //load iv
  char iv [10];
  for(int i=0; i<10; i++) {
      iv[i] = m[i];
  } 
  //get the message without the iv  
  char msg_no_iv [m_len-10];
  for(int k=0; k<m_len-10; k++) {
      msg_no_iv[k]=m[k+10];
  }
  //prepend k to iv (store in kp)
  char kp[k.length()+10];
  //put the key in
  for(unsigned int i=0; i<k.length(); i++) {
      kp[i] = k[i];
  }
  //put the iv in
  for(unsigned int i=k.length(); i<k.length()+10; i++) {
      kp[i] = iv[i-k.length()];
  }
  //get a keystream
  string keystream = rc4(n-10, r, kp, k.length()+10); 
  //decrypt the message with the keystream and save to output
  for(int i=0; i<n-10; i++) { 
    plaintext[i] = msg_no_iv[i] ^ keystream[i];
  }
  //return the length of the plaintext  
  return n-10;
}





