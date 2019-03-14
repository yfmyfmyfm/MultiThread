////////////////////////////////////////////////////////////////////////////////
// Main File:        thread.c
// This File:        thread.c
// Other Files:      queue.c
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


#include<pthread.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<ctype.h> 
#include "module.h"
 

typedef struct ARG{ 
	Queue* q1; 
	Queue* q2; 
}ARG;


/*Description: Reader thread which get user input and store into queue1
 */
void* fReader(void* arg){
 
  //record length of each line of input
  int count = 0; 
  char c;
  int size = 1024; 
  char *buffer = malloc(sizeof(char) * size);
  
  //read all user inputs
  while((c = fgetc(stdin)) != EOF){
    //check if user input string is longer than buffersize
    if(c != '\n' && count == (size-1)){ 
      count = 0; 
      fprintf(stderr, "Error: Input line is too long. \n");
      while((c = fgetc(stdin)) != '\n'){ 
	if (c == EOF){ 
	  break; 
	} 
      } 
      // alloc new memory space for another input
      buffer = malloc(sizeof(char)*size); 
    }
    //one line of input has been readed
    else if (c == '\n'){
     
      *(buffer + count)  = '\0'; 
  //    printf("This is to test the output: %s\n", buffer); 
      
      EnqueueString((Queue*)arg, buffer);
      //reset counter to read new line
      count = 0; 
      // alloc new memeory space for another input
      buffer = malloc(sizeof(char) * size); 
    } 
    else{
      *(buffer + count)  = c; 
      count++; 
    }
  }

  //for the last line of the file without \n, add into the queue
  if (count) {
    buffer[count] = '\n';
    EnqueueString((Queue*)arg, buffer);
  }

  //NULL means the end of the file
  EnqueueString((Queue*)arg, NULL);
  
  //exit thread
  pthread_exit(NULL);
}


/*Description: fMunch1 processes the string from queue1 by replacing space by *
 * and enqueue into queue2
 */ 

void* fMunch1(void* arg){  
  
  Queue *inQ = ((ARG*)arg)->q1; 
  Queue *outQ = ((ARG*)arg)->q2; 
  char* buffer; 
 
  //dequeue from q1 
  while((buffer = DequeueString(inQ)) != NULL){  
    //replace space character with *
    int len = strlen(buffer);
    for(int i=0; i<len; i++){
      if (buffer[i] == ' ')
        buffer[i] = '*';
    }	 
    //enqueue to queue2
    EnqueueString(outQ, buffer);    
  }
  //NULL means previous threads are done
  EnqueueString(outQ, NULL);
  
  //exit thread
  pthread_exit(NULL);
}

/*Description:
 *  fMunch2 process the string from queue2 by convert all letters
 *  into upper case and enqueue into queue3
 */

void* fMunch2(void* arg){

  Queue *inQ = ((ARG*)arg)->q1; 
  Queue *outQ = ((ARG*)arg)->q2; 
  char* buffer;
  //deque form queue2  
  while((buffer = DequeueString(inQ)) != NULL){
    //convert all letters to upper case
    int len = strlen(buffer);
    for(int i=0; i<len; i++){
      buffer[i] = toupper(buffer[i]);
    }
  //enqueue to queue3
  EnqueueString(outQ, buffer);
  } 

  //NULL means previous threads are done
  EnqueueString(outQ, NULL);
  //exit the thread
  pthread_exit(NULL);
}


void* fWriter(void* arg){
  char* buffer;
  Queue* inQ = (Queue*)arg;
  
  //dequeu from queue3
  while((buffer = DequeueString(inQ)) != NULL){
    //print out the final result to standard output
    fprintf(stdout, "The final result is %s\n", buffer);

    //free all pointers
    free(buffer);
    buffer = NULL;
  }	  
  pthread_exit(NULL);
}

int main(void){

  //create three queues to store and transfer user input string	
  Queue* queue1 = CreateStringQueue(10);
  Queue* queue2 = CreateStringQueue(10);
  Queue* queue3 = CreateStringQueue(10);
 
  //argument into fMunch1 function 
  ARG *arg1 = malloc(sizeof(ARG));
  arg1->q1 = queue1;
  arg1->q2 = queue2;
 
  //argument into fMuch2 function
  ARG *arg2 = malloc(sizeof(ARG));
  arg2->q1 = queue2;
  arg2->q2 = queue3;
 
  //name for each thread
  pthread_t Reader,Munch1,Munch2,Writer; 
 
  //create four threads 
  pthread_create(&Reader,NULL,fReader,queue1);
 
  pthread_create(&Munch1,NULL,fMunch1,arg1);

  pthread_create(&Munch2,NULL,fMunch2,arg2);
  
  pthread_create(&Writer,NULL,fWriter,queue3);

 //wait until all threads are done
 pthread_join(Reader,NULL);

 pthread_join(Munch1,NULL);

 pthread_join(Munch2,NULL);

 pthread_join(Writer,NULL);
 
 fprintf(stdout,"************Queue Stats************\n");
 
 fprintf(stdout, "queue1 stat: \n");
 PrintQueueStats(queue1);
 fprintf(stdout, "\nqueue2 stat: \n");
 PrintQueueStats(queue2);
 fprintf(stdout, "\nqueue3 stat: \n");
 PrintQueueStats(queue3);

 return(0);
}
