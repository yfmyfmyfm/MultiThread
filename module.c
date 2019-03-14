////////////////////////////////////////////////////////////////////////////////
// Main File:        thread.c
// This File:        module.c
// Other Files: 
// Semester:         CS 537 Fall 2018
//
// Author:           Siyuan Ji
// Email:            sji27@wisc.edu
// CS Login:         siyuan
//
// Author:           Yifan Mei
// Email:            ymei8@wisc.edu
// CS Login:         yifanmei
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NULL
//
// Online sources:   NULL
//
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "module.h"

// Initialize mutex and condition variables
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty  = PTHREAD_COND_INITIALIZER;

/*Dynamically allocate a new Queue structure and initialize it 
with an array of character points of length size. 
That means you'll malloc the queue structure and then 
malloc the char * array pointed to from that structure. 
Also remember to any state and synchronization variables 
used in this structure.
The function returns a pointer to the new queue structure.
For testing purposes, create your Queue's with a size of 10.
*/

Queue *CreateStringQueue(int size) {

        Queue *queue = (Queue*)malloc(sizeof(Queue)); // = {size,0, -1, (char **)malloc(size*sizeof(char*)), 0,0,0,0};
	queue->capacity = size;
	queue->size = 0;
	queue->first = 0;
	queue->last = -1;
	queue->elements = (char**) malloc(size*sizeof(char*));
	queue->enqueueCount = 0;
	queue->dequeueCount = 0;
	queue->enqueueBlockCount = 0;
	queue->dequeueBlockCount = 0;
        return queue;
}

/*This function places the pointer to the string at the end of queue q. 
If the queue is full, then this function blocks 
until there is space available.
*/
void EnqueueString(Queue *q, char *string) { 
	pthread_mutex_lock(&m);
	while(q->size == q->capacity) {
                q->enqueueBlockCount++;
                pthread_cond_wait(&full, &m);   
	}	
        q->size ++;
        q->last ++;
        if (string != NULL){
       	  q->enqueueCount++;
	}
        if (q->last == q->capacity) {
		q->last = 0;
        }

        //store string into the element array in the queue
	
         
	*(q->elements + q->last) = string ;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&m);
        return;
}

/*This function removes a pointer to a string 
from the beginning of queue q. 

If the queue is empty, then this function blocks 
until there is a string placed into the queue. 

This function returns the pointer that was removed from the queue.
*/
char * DequeueString(Queue *q) {
        pthread_mutex_lock(&m);
        while(q->size == 0) {
                q->dequeueBlockCount++;
                pthread_cond_wait(&empty, &m);
        }        
        char *ss = q->elements[q->first];
        q->size --;
        q->first ++;
        if(ss != NULL){
	  q->dequeueCount++;
	}
        if (q->first == q->capacity) {
                q->first = 0;
        }
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&m);
        return ss;
}

/*This function prints the statistics for this queue 
*/
void PrintQueueStats(Queue *q) {
        printf("the enqueueCount number is %d\n", q->enqueueCount);
        printf("the dequeueCount number is %d\n",q->dequeueCount);
        printf("the enqueueBlockCount number is %d\n",q->enqueueBlockCount);
        printf("the dequeueBlockCount number is %d\n",q->dequeueBlockCount);
        return;
}
