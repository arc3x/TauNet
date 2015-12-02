/*
This file contains code from the public domain found at https://web.archive.org/web/20050110121238/http://www.xs4all.nl/~cg/ciphersaber/comp/c++.txt
// ciphersabre-1 by graydon hoare
// placed in public domain, jun 2000
*/

typedef unsigned char byte;
static byte i=0, j=0, c=0;

void doit(string &key) {
  if (key.size() < 0 || key.size() > 256) {
    cerr << "key + 10 byte IV must be > 0 and < 256 bytes long" << endl; exit(1);} 
  byte state[256];
  for (int n = 0; n < 256; ++n) state[n] = n;  i = 0;  
  for (int n = 0; n < 256; ++n) {j = (j + state[i] + key[i % key.size()]) % 256; 
  swap(state[i], state[j]); ++i;
  }
  i = 0; j = 0;
  while (cin.get(c)) {
    i = (i + 1) % 256;
    j = (j + state[i]) % 256; swap(state[i],state[j]);    
    cout.put(c ^ state[(state[i] + state[j]) % 256]);    
  }
}

void encrypt(string key) {
  ifstream rng("/dev/urandom");
  if (!rng) { cerr << "canot open /dev/urandom" << endl; exit(1); }
  for (int n = 0; n < 10; ++n) { if (rng.get(c)) {key += c; cout.put(c);}     
  else {cerr << "cannot read byte " << n << " from /dev/urandom" << endl; exit(1);}}
  doit(key);
}

void decrypt(string key) {
  for (int n = 0; n < 10; ++n) {if (cin.get(c)) {key += c;}     
  else {cerr << "not enough bytes on input to form IV. try again" << endl; exit(1);}}
  doit(key);
}