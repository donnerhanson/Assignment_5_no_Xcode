/*
Project: Semaphore Producer Consumer
Student Name: Donner Hanson
Student ID: 001276484
Student Email: hanso127@mail.chapman.edu
Date: 11/08/2020
File: prodcon.cpp
Description:
uses threads and semaphores to read and write to a shared data buffer

Compilation using user defined Shared Library ip_checksum:
g++ -c -Wall -Werror -fPIC ip_checksum.c;
g++ -shared -o libip_checksum.so ip_checksum.o;
g++ -L. -I. -std=c++0x -Wall -Wextra -g prodcon.cpp -o prodcon -lip_checksum -lpthread -lrt;
export LD_LIBRARY_PATH=($pwd):$LD_LIBRARY_PATH


Run: ./prodcon <nitems in buffer>

End: <ctrl + c>

Functionality:


REFERENCES
https://en.cppreference.com/w/cpp/utility/program/signal
https://stackoverflow.com/questions/36568672/returning-a-void-pointer-as-thread-return


*/

#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <random> // found in c++11 need the -std=c++11 or -std=c++0x flag for compilation
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // sleep - may need to see if this is unneeded
//Compile the code by linking with -lpthread -lrt
#include <semaphore.h>
#include <pthread.h>


#include "ip_checksum.h"


// atomic signal used for SIGINT
namespace {
    volatile std::sig_atomic_t gSignalStatus;
}

const int STR_SIZE = 34;
typedef struct packet {
    unsigned int in;
    unsigned int out;
    /* 16-bit Internet checksum    */
    unsigned short cksum;
    /* random generated data */
    unsigned char payload[STR_SIZE];
} packet;

/* Prototypes */
void write_packet_to_buff(int nitems);
int check_pkt_in_buff(int nitems, int curr_out);
//extern unsigned int ip_checksum(unsigned char *data, int length);
void showPacketContents(packet in);
void showPacketPayload(packet in);
void sig_handler(int sig);

// THREADS and mutex
void *thread_write(void *arg);
void *thread_read(void *arg);
static sem_t full;
static sem_t empty;
pthread_mutex_t  mutex;


/*
INITIALIZE GLOBALS
*/
// Packet Size
const unsigned int PACKET_SIZE = sizeof(packet);
// 64k in Bytes = 65536 Bytes - max bytes
const int MAX_BYTES = 65536;

packet pkt; // structure that holds the shared data
packet *buffer; // this is the buffer that will be read and written
int main(int argc, const char * argv[]) {
    int status;
    int nitems = 0; // user input adjusts this
    int max_len = 0;

    if (argc <= 1) {
        std::cout << "Usage: ./program <nitems>\n";
        return 0;
    }
    if (argc == 2) {
        // get the total number of packets to be in the buffer
        nitems = atoi(argv[1]);
        // (((int)PACKET_SIZE) + sizeof(Shareditem))
        max_len = (MAX_BYTES/((int)PACKET_SIZE)); // max length of bytes allocate-able to packet buffer
        if (!(nitems > 0 && nitems <= max_len)) {
            std::cout << "nitems must be between 0 and " << max_len << "\n";
            return -1;
        }
    }
    if (argc > 2) {
        std::cout << "Usage: ./program <nitems>\n";
        return 0;
    }

    int nitemsTable[2] = { nitems, nitems }; // need separate addresses so when passed as arg to pthread_create there isn't segfault

    // indexed from 0 -> nitems - 1
    // IN C
    // buffer = (item *)malloc (sizeof(item) * nitems);
    // str = (item *) malloc(nitems);
    buffer = new packet[nitems];

    // initialize packet structure
    pkt.in = 0;
    pkt.out = 0;
    pkt.cksum = 0;

    // handle signal
    signal(SIGINT, sig_handler);

    // greater than 0 for second arg means that the mutex is shared across PROCESSES
    // initial value is 0 - means that only a post can occur and a wait must wait to execute

    status = sem_init(&full, 0, 0); // id, not across processes, initial value
    if (status != 0) {
        perror("Semaphore full initialization failed\n");
        return -1;
    }

    status = sem_init(&empty, 0, nitems);
    if (status != 0) {
        perror("Semaphore empty initialization failed\n");
        return -1;
    }

    status = pthread_mutex_init  (&mutex, 0); // initial
    if (status != 0) {
        perror("Pthread mutex initialization failed\n");
        return -1;
    }

    pthread_t t1 ,t2; // two separate threads
    pthread_attr_t attr; /* set of thread attributes */
    pthread_attr_init(&attr);

    // two threads created - use the nitems as buffer count
    try {
        pthread_create(&t2,&attr,thread_read, (void *) &nitemsTable[0]); // wait until increment
        pthread_create(&t1,&attr,thread_write, (void *) &nitemsTable[1]); // increment and write - pass nitems as argument

        // threads run now
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);


        // Terminate program - MAIN
        if (gSignalStatus == SIGINT) {
            // look for kill threads
            sem_destroy(&empty); // destroy mutexes
            sem_destroy(&full);  // destroy mutexes
            pthread_mutex_destroy(&mutex); // destroy thread mutex
            delete [] buffer;
            exit(gSignalStatus);
        }

        delete [] buffer;
        return 0;
    } catch(...) {
        // try catch chksum error catch
        std::cout << "exception occured\n";
    }
    return -1;
}

/* FUNCTION DEFINITIONS */

void showPacketContents(packet in) {
    using namespace std;
    printf("CheckSum :0x%x \n", in.cksum);
    showPacketPayload(in);
    cout << "in: "<< in.in << "\n";
    cout << "out: " << in.out << "\n";
    cout << "\n";
}  /* end showPacketContents */


void showPacketPayload(packet in) {

    printf("payload:{ ");
    for (int i = 0; i < STR_SIZE; i++) {
        if(i < (STR_SIZE - 1))
        printf("%d, ", (int)in.payload[i]);
        else
        printf("%d ", (int)in.payload[i]);
    }
    printf("}\n");
}  /* end showPacketPayload */


void sig_handler(int sig) {
    std::cout << "\nCaught signal " << sig << "\n";
    gSignalStatus = sig;
}  /* end sig_handler */


// THREADS
void *thread_write(void *arg)
{
    // critical section
    int nitems = *((int *) arg); // cast the void * arg to int
    // WRITE LOOP
    do {
        /* sem_wait() call---decreases by 1*/
        sem_wait(&empty);

        if (gSignalStatus == SIGINT) {
            // look for kill threads
            sem_destroy(&empty); // destroy mutexes
            sem_destroy(&full);  // destroy mutexes
            pthread_mutex_destroy(&mutex); // destroy thread mutex
            delete [] buffer;
            exit(gSignalStatus);
        }
        /* pthread_mutex_lock call*/
        pthread_mutex_lock(&mutex);

        // this will increment pkt.in and write a single packet to buffer
        write_packet_to_buff(nitems);

        /* increment semaphore */
        sem_post(&full);
        /* pthread_mutex_unlock call*/
        pthread_mutex_unlock(&mutex);

    }  while (true);/* end while(true) */


    static const long ok_return = 1;
    return (void*)&ok_return;
}

void *thread_read(void *arg)
{
    int nitems = *((int *) arg);
    int curr_out = 0;
    packet pkt_read; // intermediate packet to store data to read

    pkt_read.out = curr_out; // set init out

    do {

        pkt_read.out = ++curr_out;
        // critical section
        // wait - decrement
        sem_wait(&full);

        // pthread_mutex_lock() call
        pthread_mutex_lock(&mutex);

        // in the signal handler set a flag that both the consumer/producer
        // can loop on then both threads just exit if the flag is set.
        if (gSignalStatus == SIGINT) {
            // look for kill threads
            sem_destroy(&empty); // destroy mutexes
            sem_destroy(&full);  // destroy mutexes
		    pthread_mutex_destroy(&mutex); // destroy thread mutex
            delete [] buffer;
            exit(gSignalStatus);
        }

        //curr_out = check_pkt_in_buff(nitems, curr_out); // check if curr_out == -1 error



        // curr_out is passed as an int and returned as a counter
        // dest_pkt, src, pkt_size
        memcpy((void *)&pkt_read, (void *)&buffer[((pkt_read.out - 1) % nitems)], PACKET_SIZE);

        // Calculate the 16-bit checksum (cksum)
        int length = STR_SIZE;
        unsigned short cksum_2 = (unsigned short)ip_checksum(&pkt_read.payload[0], length);
        printf("cksum_2: 0x%04x\n",cksum_2);
        if (cksum_2 == pkt_read.cksum) {
            pkt_read.out = curr_out; // set the number out to curr out
            showPacketContents(pkt_read);
        }
        else {
            curr_out = -1;
            printf("\nCHECKSUM ERROR\n");
        }
        /* sem_post() call---increment*/
        sem_post(&empty);
        //printf("\nRead from Buffer\n");
        try {
            if (curr_out == -1) {
                throw -1;
            }
        } catch (...) {
            throw -1;
        }

        /* pthread_mutex_unlock call*/
        pthread_mutex_unlock(&mutex);
    } while (true); /* end do while (true) */

    static const long ok_return = 1;
    return (void*)&ok_return;
}  /* end thread_read */


void write_packet_to_buff(int nitems) {

    for (int n = 0; n < STR_SIZE; n++) {
        pkt.payload[n] = (unsigned char) rand() % 256;
    }

    //Calculate the 16-bit checksum (cksum)
    int length = STR_SIZE;
    pkt.cksum = (unsigned short)ip_checksum(&pkt.payload[0], length);
    pkt.in++; // first packet.in label is 1 but inserted at buff[0]

    // pkt.in starts at 1
    // (pkt.in - 1) % nitems will cycle from 0 --> nitems
    // pkt is the global structure used for writing
    memcpy((void *)&buffer[((pkt.in - 1) % nitems)], (void *)&pkt, PACKET_SIZE);

}  /* end write_to_packet_buff */
