/*-------------------------------------------------------------------*/
/* CS2.C - The CipherSaber-2 Encryption Program.                     */
/*                                                                   */
/* This program was written by Alan Pittman <apittman@home.com> and  */
/* is based on a CS1 shell by Hiroshi Yuki <hyuki@st.rim.or.jp>.     */
/* Last update: April 27, 2000.                                      */
/*                                                                   */
/* The CipherSaber concept is by Arnold G. Reinhold                  */
/* <arnold@iecc.com>. http://ciphersaber.gurus.com/                  */
/*                                                                   */
/* CipherSaber, CS1, CS2, and CipherKnight are trademarks of Arnold  */
/* G. Reinhold. A free, nonexclusive license is hereby granted to    */
/* use the marks CipherSaber, CS1 and CS2 on any product that is     */
/* interoperable with CipherSaber as demonstrated by the ability to  */
/* decrypt the test samples supplied and to produce files readable   */
/* by other implementations of CipherSaber. The marks may also be    */
/* used in supporting material that promotes CipherSaber.            */
/*                                                                   */
/* RC4 encryption algorithm was created by Ronald L. Rivest of RSA.  */
/* RC4 was published in the 2nd edition of Bruce Schneier's Applied  */
/* Cryptography. RC4 is a trademark of the RSA Division of Security  */
/* Dynamics.                                                         */
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*  !!! THE FOLLOWING NOTES ARE FROM THE CIPHER-SABER WEB PAGE !!!   */
/*-------------------------------------------------------------------*/
/* "What is CipherSaber?"                                            */
/*                                                                   */
/*     CipherSaber is an ultra-simple encryption method. Anyone with */
/*     a little programming skill can write a CipherSaber program in */
/*     as few as 16 lines of Basic. You can find out how at:         */
/*     http://ciphersaber.gurus.com                                  */
/*                                                                   */
/*     CipherSaber is a serious encryption tool that is very useful. */
/*     CipherSaber uses a respected algorithm called RC4 to provide  */
/*     very high strength.                                           */
/*                                                                   */
/* What is CipherSaber-2?                                            */
/*                                                                   */
/*     CipherSaber-2 is a modification to CipherSaber-1 that         */
/*     addresses concerns raised about possible statistical          */
/*     weaknesses in RC4. In CipherSaber-2 the entire state array    */
/*     mixing loop is repeated N times, where N is a number that the */
/*     sender and receiver agree upon. When N=1, CipherSaber-2 is    */
/*     the same as CipherSaber-1.                                    */
/*-------------------------------------------------------------------*/
/* RC4 is described in the second edition of Bruce Schneier's book   */
/* Applied Cryptography. You can find it on the Internet by          */
/* searching on "RC4 source" using your favorite search engine.      */
/* You might also search on "arcfour" which is the name of an        */
/* Internet draft standard that uses the same algorithm. Finally,    */
/* here is the RC4 algorithm in plain English:                       */
/*                                                                   */
/* RC4 uses two arrays of eight bit bytes. The "state" array is 256  */
/* bytes long and holds a permutation of the numbers 0 through 255.  */
/* The "key" array can be of any length up to 256 bytes. RC4 also    */
/* uses two index variables i and j that start off as zero. All      */
/* variables are eight bits long and all addition is performed       */
/* modulo 256.                                                       */
/*                                                                   */
/* RC4 has two phases: key setup and ciphering.                      */
/*                                                                   */
/* The setup phase is only done once per message and starts by       */
/* initializing the entire state array so that the first state       */
/* element is zero, the second is one, the third is two, and so on.  */
/*                                                                   */
/* The state array is then subjected to 256 mixing operations using  */
/* a loop that steps i through the values from zero to 255. Each     */
/* mixing operation consists of two steps:                           */
/*                                                                   */
/* Add to the variable j the contents of the ith element of the      */
/* state array and the nth element of the key, where n is equal to i */
/* modulo the length of the key.  Swap the ith and jth elements of   */
/* the state array.                                                  */
/*                                                                   */
/* After the entire mixing loop is completed, i and j are set to     */
/* zero.                                                             */
/*                                                                   */
/* During the ciphering operation, the following steps are performed */
/* for each byte of the message:                                     */
/*                                                                   */
/* The variable i is incremented by one The contents of the ith      */
/* element of S is then added to j The ith and jth elements of S are */
/* swapped and their contents are added together to form a new value */
/* n. The nth element of S is then combined with the message byte,   */
/* using a bit by bit exclusive-or operation, to form the output     */
/* byte.                                                             */
/*                                                                   */
/* The same ciphering steps are performed for encryption and for     */
/* decryption.                                                       */
/*                                                                   */
/* Note that in CipherSaber the RC4 key array consists of the user's */
/* CipherSaber key followed by the 10 byte initialization vector     */
/* (IV).                                                             */
/*                                                                   */
/* When you are encrypting a file, you generate a new IV randomly    */
/* and write out the 10 bytes before you write out the encrypted     */
/* file bytes. When you are decrypting the file, you read the IV     */
/* from the first 10 bytes of the encrypted file.                    */
/*-------------------------------------------------------------------*/

#include <stdio.h>
//#include <conio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>

/* prototypes */

void main(int argc, char *argv[]);
int RC4_selftest(void);
void cs_encryptfile(char *infile, char *outfile);
void cs_decryptfile(char *infile, char *outfile);
int get_password(unsigned char *prompt, unsigned char *buf, int len);
void get_random(unsigned char *buf, int len);
void RC4_exit(void);
void burn_it(unsigned char *buf, int len);
void RC4_init(unsigned char *key, size_t len);
void RC4_cipher(unsigned char *buffer, size_t len);

/* state, xi, xj and CS2_count are for RC4_init and RC4_cipher only */

unsigned char state[256];
int xi, xj, CS2_count;

/* Max length of password, excluding 10-byte Initialization Vector */

#define PASS 246

/* A big buffer for disk I/O */

#define BIGBUFFER (32*1024)
static unsigned char bigbuffer[BIGBUFFER];

unsigned char *banner[] = {
  "/****************************************************************/",
  "/*     CipherSaber-2 Encryption Program, version 2000-04-27     */",
  "/****************************************************************/",
  NULL,
};

unsigned char *usage[] = {
  "This program was written by Alan Pittman <apittman@home.com> and  ",
  "is based on a CS1 shell by Hiroshi Yuki <hyuki@st.rim.or.jp>.     ",
  "Last update: April 27, 2000.                                      ",
  "",
  "The CipherSaber concept is by Arnold G. Reinhold <arnold@iecc.com>",
  "http://ciphersaber.gurus.com/ CipherSaber, CS1, CS2, and          ",
  "CipherKnight are trademarks of Arnold G. Reinhold.                ",
  "",
  "RC4 encryption algorithm was created by Ronald L. Rivest of RSA.  ",
  "RC4 was published in the 2nd edition of Bruce Schneier's Applied  ",
  "Cryptography.  RC4 is a trademark of the RSA Division of Security ",
  "Dynamics.                                                         ",
  "",
  "WARNING: RC4 is not patented, but it's status is unclear.         ",
  "Do not use in a commercial product without permission from RSA.   ",
  "",
  "Usage: cs2 -e infile outfile     for encryption                   ",
  "Usage: cs2 -d infile outfile     for decryption                   ",
  "",
  "Please run in a DOS Window                                        ",
  NULL,
};

/*-------------------------------------------------------------------*/
/* RC4 test vectors.                                                 */
/* These are to verify that the code is correctly implemented.       */
/* To test, use RC4_selftest() function.                             */
/*-------------------------------------------------------------------*/

/* The maximum length of each data size (key, plaintext, and ciphertext) */

#define RC4_TESTDATA 32

typedef struct {
    int statemixes;
    int keysize; unsigned char key[RC4_TESTDATA];
    int plainsize; unsigned char plain[RC4_TESTDATA];
    int ciphersize; unsigned char cipher[RC4_TESTDATA];
} RC4_TEST;

/* The number of test vectors */

#define RC4_TESTSIZE 6

RC4_TEST RC4_test[RC4_TESTSIZE] = {
    {  /* The first 4 test vectors are from Hiroshi Yuki */
         1,
         8, { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
         8, { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
         8, { 0x75, 0xB7, 0x87, 0x80, 0x99, 0xE0, 0xC5, 0x96 },
    },
    {
         1,
         8, { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
         8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
         8, { 0x74, 0x94, 0xC2, 0xE7, 0x10, 0x4B, 0x08, 0x79 },
    },
    {
         1,
         8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
         8, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
         8, { 0xDE, 0x18, 0x89, 0x41, 0xA3, 0x37, 0x5D, 0x3A },
    },
    {
         1,
         4, { 0xEF, 0x01, 0x23, 0x45 },
        10, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        10, { 0xD6, 0xA1, 0x41, 0xA7, 0xEC, 0x3C, 0x38, 0xDF, 0xBD, 0x61 },
    },
    {  /* Test CS1.                                                  */
       /* This is the cstest1.cs1 file from the CipherSaber Web Page */
       /* Plaintext = "This is a test of CipherSaber."               */
       /* Key = "asdfg", N = 1                                       */
        1,
        15, { 0x61, 0x73, 0x64, 0x66, 0x67,
              0x6F, 0x6D, 0x0B, 0xAB, 0xF3, 0xAA, 0x67, 0x19,
              0x03, 0x15 },
        30, { 0x30, 0xED, 0xB6, 0x77, 0xCA, 0x74, 0xE0, 0x08,
              0x9D, 0xD0, 0xE7, 0xB8, 0x85, 0x43, 0x56, 0xBB,
              0x14, 0x48, 0xE3, 0x7C, 0xDB, 0xEF, 0xE7, 0xF3,
              0xA8, 0x4F, 0x4F, 0x5F, 0xB3, 0xFD },
        30, { 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20,
              0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x6F,
              0x66, 0x20, 0x43, 0x69, 0x70, 0x68, 0x65, 0x72,
              0x53, 0x61, 0x62, 0x65, 0x72, 0x2E },
    },
    {  /* Test CS2.                                                   */
       /* This is the cs2test1.cs2 file from the CipherSaber Web Page */
       /* Plaintext = "This is a test of CipherSaber-2."              */
       /* Key = "asdfg", N = 10                                       */
        10,
        15, { 0x61, 0x73, 0x64, 0x66, 0x67,
              0xBA, 0x9A, 0xB4, 0xCF, 0xFB, 0x77, 0x00, 0xE6,
              0x18, 0xE3 },
        32, { 0x82, 0xE8, 0xFC, 0xC5, 0xAB, 0x98, 0x13, 0xB1,
              0xAB, 0xC4, 0x36, 0xBA, 0x7D, 0x5C, 0xDE, 0xA1,
              0xA3, 0x1F, 0xB7, 0x2F, 0xB5, 0x76, 0x3C, 0x44,
              0xCF, 0xC2, 0xAC, 0x77, 0xAF, 0xEE, 0x19, 0xAD },
        32, { 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20,
              0x61, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x6F,
              0x66, 0x20, 0x43, 0x69, 0x70, 0x68, 0x65, 0x72,
              0x53, 0x61, 0x62, 0x65, 0x72, 0x2D, 0x32, 0x2E },
    }
};

/*-------------------------------------------------------------------*/
/* This is the main function.                                        */
/*-------------------------------------------------------------------*/

void main(int argc, char *argv[])
{
    char *infile = 0;
    char *outfile = 0;
    int i;
    char str[246];

    /* print the program title */

    for (i = 0; banner[i] != 0; i++) {
        printf("%s\n", banner[i]);
    }

    /* perform self-check of RC4 implementation */

    if (!RC4_selftest()) {
        printf("RC4 self test has failed.  Do not use this program.\n");
        exit(-1);
    }

    /* check the command line */

    if (argc != 4) {
        for (i = 0; usage[i] != 0; i++) {
            printf("%s\n", usage[i]);
        }
        getch();
        exit(-1);
    }

    infile = argv[2];
    outfile = argv[3];

    /* ask for the number of state array mixes required */

    printf ("Number of state array mixes (for CS1 enter 1) ? ");
    scanf("%s", str);
    sscanf(str, "%d", &CS2_count);

    /* check the first parameter, encrypt (-e) or decrypt (-d) */

    if (stricmp(argv[1], "-e") == 0) {
        printf("Encrypt %s into %s.\n", infile, outfile);
        cs_encryptfile(infile, outfile);

    } else {
        printf("Decrypt %s into %s.\n", infile, outfile);
        cs_decryptfile(infile, outfile);
    }
}

/*-------------------------------------------------------------------*/
/* Self test of RC4 encryption algorithm.                            */
/* It returns 0 if it is no good, returns 1 if it is okay.           */
/*-------------------------------------------------------------------*/

int RC4_selftest(void)
{
    int i;
    unsigned char work[RC4_TESTDATA];
    RC4_TEST *t = 0;
    int good = 1;

    for (i = 0; i < RC4_TESTSIZE; i++) {

        t = &RC4_test[i];
        CS2_count = t->statemixes;

        /* encrypt check */

        RC4_init(t->key, t->keysize);
        memcpy(work, t->plain, t->plainsize);
        RC4_cipher(work, t->plainsize);
        if (memcmp(work, t->cipher, t->ciphersize) != 0) {
            good = 0;
        }
        RC4_exit();

        /* decrypt check */

        RC4_init(t->key, t->keysize);
        RC4_cipher(work, t->ciphersize);
        if (memcmp(work, t->plain, t->plainsize) != 0) {
            good = 0;
        }
        RC4_exit();
    }
    return good;
}

/*-------------------------------------------------------------------*/
/* CipherSaber file encryption.                                      */
/*-------------------------------------------------------------------*/

void cs_encryptfile(char *infile, char *outfile)
{
    FILE *infp = 0;
    FILE *outfp = 0;
    unsigned char pass1[PASS+1];
    unsigned char pass2[PASS+1];
    unsigned char key[256];
    unsigned char random_ten[10];
    size_t rlen;
    int passlen;

    /* open input file */

    if ((infp = fopen(infile, "rb")) == NULL) {
        printf("Cannot find %s.\n", infile);
        return;
    }

    /* open output file */

    if ((outfp = fopen(outfile, "wb")) == NULL) {
        printf("Cannot create %s.\n", outfile);
        return;
    }

    /* get a password from user twice,   */
    /* and check if the two are the same */

    if (get_password("Enter  password: ", pass1, PASS) == 0) {
        goto err;
    }

    if (get_password("Repeat password: ", pass2, PASS) == 0) {
        goto err;
    }

    if (strcmp(pass1, pass2) != 0) {
        printf("The two passwords are not same.\n");
        goto err;
    }

    /* get randomized initial vector for CipherSaber */

    get_random(random_ten, 10);

    /* create CipherSaber key from                 */
    /* the password and randomized initial vector. */

    passlen = strlen(pass1);
    memcpy(key, pass1, passlen);
    memcpy(key + passlen, random_ten, 10);

    /* Read, encrypt and write. */

    RC4_init(key, passlen + 10);
    fwrite(random_ten, 1, 10, outfp);
    while ((rlen = fread(bigbuffer, 1, BIGBUFFER, infp)) > 0) {
        RC4_cipher(bigbuffer, rlen);
        fwrite(bigbuffer, 1, rlen, outfp);
    }
    RC4_exit();

err:
    /* close */

    if (infp) fclose(infp);
    if (outfp) fclose(outfp);

    burn_it(pass1, PASS+1);
    burn_it(pass2, PASS+1);
    burn_it(key, 256);
    burn_it(random_ten, 10);
    burn_it(bigbuffer, BIGBUFFER);
}

/*-------------------------------------------------------------------*/
/* CipherSaber file decryption.                                      */
/*-------------------------------------------------------------------*/

void cs_decryptfile(char *infile, char *outfile)
{
    FILE *infp = 0;
    FILE *outfp = 0;
    unsigned char pass1[PASS+1];
    unsigned char key[256];
    unsigned char random_ten[10];
    size_t rlen;
    int passlen;

    /* open input file */

    if ((infp = fopen(infile, "rb")) == NULL) {
        printf("Cannot find %s.\n", infile);
        return;
    }

    /* open output file */

    if ((outfp = fopen(outfile, "wb")) == NULL) {
        printf("Cannot create %s.\n", outfile);
        return;
    }

    /* get password from user */

    if (get_password("Enter  password: ", pass1, PASS) == 0) {
        goto err;
    }

    /* read initial vector from infile */

    if (fread(random_ten, 1, 10, infp) != 10) {
        printf("Cannot read enough data from %s.\n", infile);
        goto err;
    }

    /* create CipherSaber key from                */
    /* the password and initial vector from file. */

    passlen = strlen(pass1);
    memcpy(key, pass1, passlen);
    memcpy(key + passlen, random_ten, 10);

    /* Read, decrypt and write. */

    RC4_init(key, passlen + 10);
    while ((rlen = fread(bigbuffer, 1, BIGBUFFER, infp)) > 0) {
        RC4_cipher(bigbuffer, rlen);
        fwrite(bigbuffer, 1, rlen, outfp);
    }
    RC4_exit();

err:
    /* close */

    if (infp) fclose(infp);
    if (outfp) fclose(outfp);

    burn_it(pass1, PASS+1);
    burn_it(key, 256);
    burn_it(random_ten, 10);
    burn_it(bigbuffer, BIGBUFFER);
}

/*-------------------------------------------------------------------*/
/* getting password.                                                 */
/*-------------------------------------------------------------------*/

int get_password(unsigned char *prompt, unsigned char *buf, int len)
{
    printf("%s", prompt);
    scanf("%s", buf);
    
    if (strlen(buf) > len) {
        printf("Password is too long.\n");
        return -1;
    }
    
    return strlen(buf);
}

/*-------------------------------------------------------------------*/
/* get_random fills buf with (pseudo) random bytes.                  */
/*-------------------------------------------------------------------*/

void get_random(unsigned char *buf, int len)
{
    int i;

    srand(time(NULL));  /* You could choose better seed. */
    for (i = 0; i < len; i++) {
        buf[i] = (unsigned char)(rand() % 256);
    }
}

/*-------------------------------------------------------------------*/
/* Despose used variables.                                           */
/* You must call this function                                       */
/* after encryption or decryption ends.                              */
/*-------------------------------------------------------------------*/

void RC4_exit(void)
{
    RC4_init("burn", 4);
}

/*-------------------------------------------------------------------*/
/* burn it                                                           */
/*-------------------------------------------------------------------*/

void burn_it(unsigned char *buf, int len)
{
    memset(buf, '\0', len);
}

/*-------------------------------------------------------------------*/
/* Initialize RC4_... variables with the given key.                  */
/* You must call this function before calling RC4_cipher().          */
/*-------------------------------------------------------------------*/

void RC4_init(unsigned char *key, size_t len)
{
    int n;
    unsigned char ch;

    for (xi = 0; xi < 256; xi++) {
        state[xi] = xi;
    }
    xj = 0;
    for (n = 0; n < CS2_count; n++) {
        for (xi = 0; xi < 256; xi++) {
            xj = (xj + state[xi] + key[xi % len]) % 256;
            ch = state[xi];
            state[xi] = state[xj];
            state[xj] = ch;
        }
    }
    xi = 0;
    xj = 0;
}

/*-------------------------------------------------------------------*/
/* The core part of RC4.                                             */
/* You must call RC4_init() before using this.                       */
/* The given buffer is overwritten.                                  */
/*-------------------------------------------------------------------*/

void RC4_cipher(unsigned char *buffer, size_t len)
{
    int k, n;
    unsigned char ch;

    for (n = 0; n < len; n++) {
        xi = (xi + 1) % 256;
        xj = (xj + state[xi]) % 256;
        ch = state[xi];
        state[xi] = state[xj];
        state[xj] = ch;
        k = (state[xi] + state[xj]) % 256;
        buffer[n] = buffer[n] ^ state[k];
    }
}

/* End of CipherSaber-2 Source Code */
