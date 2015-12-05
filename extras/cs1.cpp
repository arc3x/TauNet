
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h>

using namespace std;

string rc4(int n, int r, unsigned char* k, int kl) {
    cout << "rc4: request for keystream of length " << n << " with " << r << " rounds of key scheduling and key " << k << " of length " << kl << endl;
  /*  
  -- "Produce an RC4 keystream of length" n "with"
  -- r "rounds of key scheduling given key" k
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
  //get key length
  //int l = k.length();

  //init array

  unsigned char S [256] = { };
  for(int i=0; i<256; i++) {
      S[i]=i;
  }
  
  //do key scheduling
  int j=0;
  //repeat r times
  for(int ri=0; ri<r; ri++) {
      for(int i=0; i<256; i++) {
          j = (j + S[i] + k[i%kl]) % 256;
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
      //int t = S[(S[ip]+S[j]) % 256];
      keystream.append(to_string(S[(S[ip]+S[j]) % 256]));      
  }
  cout << "rc4: returing keystream: " << keystream << endl;
  return keystream;
}

int encrypt(unsigned char * m, int ml, unsigned char * k, int kl, int r, unsigned char * ciphertext) {
  /*  
  -- "Ciphersaber-2 encrypt message" m "with key" k "and"
  -- r "rounds of key scheduling"
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

  string keystream = rc4(ml, r, k, kl);

  for(int i=0; i<ml; i++) {
      ciphertext[i] = m[i] ^ keystream[i];      
  }
  //cout << endl;
  
  //cout << "Sizeof ciphertext: " << sizeof(&ciphertext) << endl;
  //cout << "Sizeof message: " << (m.length()) << endl;
  
  //cout << "ENC ciphertext: " << ciphertext;
  //string ret(ciphertext);
  //cout << "ENC returning: " << ret;
  return ml;
}

int decrypt(unsigned char* m, int ml, unsigned char* k, int kl, int r, unsigned char* plaintext) {
    cout << "decrypting message of length " << ml << " using key '" << k << "' of length " << kl << " with " << r << " round of key scheduling" << endl;
  /*
  -- "Ciphersaber-2 decrypt ciphertext" m "with key" k "and"
  -- r "rounds of key scheduling"
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
  
  string keystream = rc4(ml, r, k, kl);
  
  //char plaintext [n-10];
  //cout << "n " << n << endl;
  for(int i=0; i<ml; i++) { //XXX n-9?
    plaintext[i] = m[i] ^ keystream[i];
    cout << m[i] << plaintext[i] << endl;
  }
  
  //string ret(plaintext);
  cout << "decrypt: returning a message of length " << ml << endl;
  return ml;
}

typedef unsigned char byte;
static byte i=0, j=0, c=0;

void zero(byte * ar, int len) {
    for (int i=0; i<len; i++) {
        ar[i] = 0;
    }
}



void doit(string &key, byte * in, int in_len, byte * out) {
    //cout << in[0] << endl;
  if (key.size() < 0 || key.size() > 256) {
    cerr << "key + 10 byte IV must be > 0 and < 256 bytes long" << endl; exit(1);} 
  byte state[256];
  for (int n = 0; n < 256; ++n) state[n] = n;  i = 0;  
  for (int n = 0; n < 256; ++n) {j = (j + state[i] + key[i % key.size()]) % 256; 
  swap(state[i], state[j]); ++i;
  }
  i = 0; j = 0;
  //while (cin.get(c)) {
  for (int a=0; a<in_len; a++) {
    i = (i + 1) % 256;
    j = (j + state[i]) % 256; swap(state[i],state[j]);    
    //cout.put(c ^ state[(state[i] + state[j]) % 256]);    
    out[a]=in[a] ^ state[(state[i] + state[j]) % 256];
    
  }
}

void encrypt1(string key, byte * plaintext, int plaintext_len, byte * ciphertext) {
  ifstream rng("/dev/urandom");
  if (!rng) { cerr << "canot open /dev/urandom" << endl; exit(1); }
  for (int n = 0; n < 10; ++n) {    
        key += n; //cout.put(c);
    }     
  doit(key, plaintext, plaintext_len, ciphertext);
}

void decrypt1(string key, byte * ciphertext, int ciphertext_len, byte * plaintext) {
  for (int n = 0; n < 10; ++n) {     
    key += ciphertext[i];
  }     
  
  byte cipher_noiv [ciphertext_len-10];
  for(int i=0; i<ciphertext_len-10; i++) {
      cipher_noiv[i] = ciphertext[i+10];
  }
  
  doit(key, cipher_noiv, ciphertext_len-10, plaintext);
}

int main(void) {
    string key = "asdfg";
    ifstream cipherFile;
    ifstream plainFile;
    cipherFile.open("tst/cstest1.cs1");    
    //cipherFile.open("tst/fake.5char.txt");    
    byte plaintext [1000];
    byte ciphertext [1000];       
    
    int i=0;    
    /*
    if (cipherFile.is_open()) { 
        while (!cipherFile.eof()) {
            //cipherFile >> ciphertext;
            ciphertext[i++] = cipherFile.get();
            
        } 
        
        //cout << "Ciphertext:" << endl;
        //for (int k=0; k<i-1; k++) {
            //cout << ciphertext[k];
        //}
        //cout << "|" << endl << endl << endl;
    }
    cipherFile.close();
    */
    
    
    int plaintext_len=0;
    plainFile.open("tst/cstest1.txt");
    if (plainFile.is_open()) { 
        while (!plainFile.eof()) {
            //cipherFile >> ciphertext;
            plaintext[i++] = plainFile.get();
            
        } 
        
        cout << "Plaintext:" << endl;
        
        for (plaintext_len=0; plaintext_len<i-1; plaintext_len++) {
            cout << plaintext[plaintext_len];
        }
        cout << "|" << endl << endl << endl;
    }
    cipherFile.close();
    //decrypt1(key, ciphertext, i-1, plaintext);
    //cout << plaintext << endl;
    encrypt1(key, plaintext, plaintext_len, ciphertext);
    for (int k=0; k<i-1; k++) {
        cout << ciphertext[k];
    }
    cout << "|" << endl << endl << endl;
    zero(plaintext,1000);
    decrypt1(key, ciphertext, plaintext_len+10, plaintext);
      for (int k=0; k<i-1; k++) {
        cout << plaintext[k];
    }
/*
    unsigned char plaintext [1000];
    unsigned char ciphertext [1000];   
    unsigned char message [1000];
    int ml;
    unsigned char key [100];
    int kl;
    //char c;    
    ifstream cipherFile;
    
    
    key[0]='a';
    key[1]='s';
    key[2]='d';
    key[3]='f';
    key[4]='g';
    
    kl = 5;
    cipherFile.open("tst/cstest1.cs1");        
    //cipherFile.open("tst/fake.5char.txt");        
    int i=0;    
    if (cipherFile.is_open()) {        
        while (!cipherFile.eof()) {
            //cipherFile >> ciphertext;
            ciphertext[i++] = cipherFile.get();
            
        } 
        
        cout << "Ciphertext:" << endl;
        for (int k=0; k<i-1; k++) {
            cout << ciphertext[k];
        }
        cout << "|" << endl;
    }
    cipherFile.close();
    
    int plaintext_len = decrypt(ciphertext, i-1, key, kl, 1, plaintext);
    
    for(int k=0; k<plaintext_len; k++) {
        cout << plaintext[k];
    }
 */   
}


