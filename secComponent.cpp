/*
(c) Matthew Slocum 2015

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
  /*  
  rc4(n, r, k):
    l <- length k
    -- Initialize the array.
    S <- "zero-based array of 256 bytes"
    for i in 0..255
        S[i] <- i
    -- Do key scheduling.
    j <- 0
    repeat r times
        for i in 0..255
            j <- (j + S[i] + k[i mod l]) mod 256
            S[i] <-> S[j]
    -- Finally, produce the stream.
    keystream <- "zero-based array of" n "bytes"
    j <- 0
    for i in 0..n-1
        i' <- (i + 1) mod 256
        j <- (j + S[i']) mod 256
        S[i'] <-> S[j]
        keystream[i] <- S[(S[i'] + S[j]) mod 256]
  return keystream
  */  

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
  //cout << "keystream: " << keystream << endl;
  return keystream;
}


//"Ciphersaber-2 encrypt message" m "with key" k "and" r "rounds of key scheduling" "outputting to" ciphertext
//return length of ciphertext
int secComponent::encrypt(string m, string k, int r, char * ciphertext) {
  /*  
  encrypt(m, r, k):
     n <- length m
     iv <- "appropriately-chosen 10-byte IV"
     k' <- prepend k to iv
     keystream <- rc4(n, r, k')
     ciphertext <- "zero-based array of" n + 10 "bytes"
     for i in 0..9
         ciphertext[i] <- iv[i]
     for i in 0..n
         ciphertext[i + 10] <- m[i] xor keystream[i]
   return ciphertext
  */
  
    //get length of message
    int n = m.length();
    string iv = "1234567890"; //CHANGEME

    //create a holder(kp) for the key + iv
    char kp[10+k.length()];

    //load the key into kp  
    for(int i=0; i<k.length(); i++) {
        kp[i] = k[i];
    }

    //load the iv into kp
    for(int i=k.length(); i<k.length()+10; i++) {
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
  /*
  -- 
  decrypt(m, r, k):
     n <- length m
     iv <- m[0..9]
     "delete the first 10 characters of" m
     k' <- prepend k to iv
     keystream <- rc4(n - 10, r, k')
     plaintext <- "zero-based array of" n - 10 "bytes"
     for i in 0..n-10
         plaintext[i] <- m[i] xor keystream[i]
     return plaintext
  */
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
  
          //debug code 
          /*
          cout << "msg_no_iv: ";
          for(int k=0;k<m_len-10;k++) {
              cout << msg_no_iv[k];
          }
          cout << endl;
          */
  
  //prepend k to iv (store in kp)
  char kp[k.length()+10];
  //put the key in
  for(int i=0; i<k.length(); i++) {
      kp[i] = k[i];
  }
  //put the iv in
  for(int i=k.length(); i<k.length()+10; i++) {
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





