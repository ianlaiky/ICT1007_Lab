#include <stdio.h>
#include "buffer.h"
#include <stdlib.h>/* required for rand() */
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

/* the buffer */
buffer_item buffer[BUFFERSIZE];

//creating the semaphore as global var
sem_t sem_mutex;
sem_t sem_full;
sem_t sem_empty;

// producer and consumer counter
int producer_id = 0, consumer_id = 0;

int insert_item(buffer_item item, int pid) {
    int returntype = -1;
/* insert item into buffer */
    printf("producer %d produced %d\n", pid, item);

    // loop to insert data when there is no 0 in the index
    for (int i = 0; i < BUFFERSIZE; ++i) {
        if (buffer[i] == 0) {
            // set the buffer[i] to random data
            buffer[i] = item;
            // set to success
            returntype = 0;
            break;
        }
    }
/* return 0 if successful, otherwise, return -1 indicating an error condition */
    return returntype;
}

int remove_item(buffer_item *item, int pid) {
    int returntype = -1;
    int tosave = -1;
/* remove an object from buffer placing it in item */
// loop the buffer to get the resource
    for (int i = 0; i < BUFFERSIZE; ++i) {
        if (buffer[i] != 0) {

            printf("Buffer: ");
            for (int k = 0; k < BUFFERSIZE; ++k) {
                printf("%d ", buffer[k]);
            }
            printf("\n");

            // save resource to tosave
            tosave = buffer[i];
            // set the index value to 0
            buffer[i] = 0;

            // move all data left by 1
            for (int j = 0; j < BUFFERSIZE-1; ++j) {
                buffer[j]=buffer[j+1];
            }
            buffer[BUFFERSIZE-1]=0;


            // set return to success
            returntype = 0;
            break;
        }
    }
    item = &tosave;
    if (*item != -1) {
        printf("consumer %d consumed %d\n", pid, *item);
    }

    // display buffer len
    int emptycounter=0;
    for (int l = 0; l < BUFFERSIZE; ++l) {
        if(buffer[l]!=0){
            emptycounter++;
        }
    }
    printf("Buffer Len: %d\n",emptycounter);

/* return 0 if successful, otherwise, return -1 indicating an error condition */
    return returntype;
}


// producer thread
void *producer(int pid) {
    buffer_item item;
    while (1) {

        sleep(rand() % 50); /* sleep for a random period of time */

        sem_wait(&sem_empty); // acquire semaphore for the buffersize
        sem_wait(&sem_mutex);/* create the semaphore */
        item = rand(); /* generate a random number */

        if (insert_item(item, pid) < 0)
            printf("Error: Buffer is full\n"); /* report error condition */

        sem_post(&sem_mutex);/* release the semaphore */
        sem_post(&sem_full); /* add one to the full for the consumer */
    }


}

//consumer thread
void *consumer(int pid) {
    buffer_item item;
    while (1) {

        sleep(rand() % 50); /* sleep for a random period of time */


        sem_wait(&sem_full); /* acquire the semaphore from full which is added by producer*/
        sem_wait(&sem_mutex); /* acquire the semaphore for accessing critical section*/
        if (remove_item(&item, pid) < 0)
            printf("Error: NO item to consume\n"); /* report error condition */

        sem_post(&sem_mutex); /* release the semaphore */
        sem_post(&sem_empty); /* add one to empty which is the buffer empty space */

    }


}

// pthreads
void *thread_entry_producer(void *param) { /* entry point of a new thread */
    // saving pid
    int local_pid = producer_id;
    producer_id++;
    printf("thread created producer %d\n", local_pid);
    producer(local_pid);
}

void *thread_entry_consumer(void *param) { /* entry point of a new thread */
    // saving pid
    int local_pid = consumer_id;
    consumer_id++;
    printf("thread created consumer %d\n", local_pid);
    consumer(local_pid);
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
    printf("Time to sleep is %d\n", time_to_sleep);
    printf("No Producer threads is %d\n", no_producer_threads);
    printf("No Consumer threads is %d\n", no_consumer_threads);

    /* 2. Initialize buffer, mutex, semaphores, other global vars */
    // buffer has been defined in header file
    int mutex = 1, full = 0, empty = BUFFERSIZE;
    // init the semaphore
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_empty, 0, BUFFERSIZE);

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
    for (int i = 0; i < no_consumer_threads; ++i) {
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr); /* get the default attribute */
        pthread_create(&tid, &attr, thread_entry_consumer, NULL);
    }
/* 5. Sleep */
    sleep(time_to_sleep);
/* 6. Release resources, e.g. destroy semaphores */
    sem_destroy(&sem_mutex);
/* 7. Exit */
    exit(0);
}
