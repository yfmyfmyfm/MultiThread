# ifndef _queue_
# define _queue_

typedef struct Queue{
        int capacity, size, first, last;
        char **elements;
        //A count of the number of strings enqueued on this queue.
        int enqueueCount;
        //A count of the number of strings dequeued on this queue.
        //We would expect that when the program exits,
        //the two count values are equal.
        int dequeueCount;
        //A count of the number of times that
        //an enqueue was attempted but blocked.
        int enqueueBlockCount;
        //A count of the number of times that
        //a dequeue was attempted but blocked.
        int dequeueBlockCount;
}Queue;



  Queue* CreateStringQueue(int size);

  void EnqueueString(Queue *q, char *string);

  char * DequeueString(Queue *q);

  void PrintQueueStats(Queue *q);




# endif
