# MultiThread

@Author: Yifan Mei & Siyuan Ji

This project is focus on using multi threads to solve large scale file read and write. By utilizing C, we generated blocking Queue data structure by using monitor to ensure that when the queue size is over the capacity, monitor's wait is being used to prevent extra inputs to be put in the queue, while when the queue size is 0, monitor's wait is being used to prevent extra output to be extracted in the queue.
There are plenty of corner cases (invalid inputs) to deal with when read input files. 

module.c and queue.c are two versions to generate the blocking queue data structure using monitor, while 
