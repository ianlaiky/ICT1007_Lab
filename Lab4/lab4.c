#include <stdio.h>
#include "buffer.h"
#include <stdlib.h>/* required for rand() */
#include <pthread.h>
#include <unistd.h>

// todo delete later
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
/* the buffer */
buffer_item buffer[BUFFERSIZE];

int insert_item(buffer_item item) {
    int returntype = -1;
/* insert item into buffer */
    printf("producer produced %d\n", item);
    for (int i = 0; i < BUFFERSIZE; ++i) {
        if (buffer[i] != 0) {
            buffer[i] = item;
            returntype = 0;
            break;
        }
    }
/* return 0 if successful, otherwise, return -1 indicating an error condition */
    return returntype;
}

int remove_item(buffer_item *item) {
/* remove an object from buffer placing it in item */
    int randomno = rand() % 50;
    printf("consumer consumed %d\n", randomno);
/* return 0 if successful, otherwise, return -1 indicating an error condition */
}

// producer thread
void *producer(void *param) {
    buffer_item item;
    while (1) {
        printf("test run\n");
        sleep(rand() % 50); /* sleep for a random period of time */
        item = rand(); /* generate a random number */

        if (insert_item(item) < 0)
            printf("…"); /* report error condition */
    }
}

////consumer thread
void *consumer(void *param) {
    buffer_item item;
    while (1) {
        sleep(rand()%50); /* sleep for a random period of time */
        if (remove item(&item) < 0)
        printf("…"); /* report error condition */
    }
}

// pthreads
void *thread_entry_producer(void *param) { /* entry point of a new thread */
    printf("thread created\n");

    producer(NULL);
}

int main(int argc, char *argv[]) {
/* 1. Get command line arguments argv[1], argv[2], argv[3] */

    int time_to_sleep = 0, no_producer_threads = 0, no_consumer_threads = 0;

    if (argc == 4) {

        time_to_sleep = atoi(argv[1]);
        no_producer_threads = atoi(argv[2]);
        no_consumer_threads = atoi(argv[3]);
    } else if (argc > 4) {
        printf("Too many arguments supplied.\n");
    } else {
        printf("Three argument expected.\n");
    }
    printf("The 1 argument supplied is %d\n", time_to_sleep);
    printf("The 2 argument supplied is %d\n", no_producer_threads);
    printf("The 3 argument supplied is %d\n", no_consumer_threads);

    /* 2. Initialize buffer, mutex, semaphores, other global vars */
    // buffer has been defined in header file
    int mutex = 1, full = 0, empty = BUFFERSIZE;

    // set each of index in buffer to 0
    for (int j = 0; j < BUFFERSIZE; ++j) {
        buffer[j] = 0;
    }

/* 3. Create producer thread(s) */
    for (int i = 0; i < no_producer_threads; ++i) {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr); /* get the default attribute */
        pthread_create(&tid, &attr, thread_entry_producer, NULL);
    }

/* 4. Create consumer thread(s) */
/* 5. Sleep */
    sleep(time_to_sleep);
/* 6. Release resources, e.g. destroy semaphores */
/* 7. Exit */
}

//todo delete later
#pragma clang diagnostic pop