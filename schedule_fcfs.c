/*
* schedule_fcfs.c 
*
* By: Aaron Carbonell 
*
* This program simulates a first come first serve 
* CPU scheduler 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "cpu.h"
#include "task.h" 
#include "schedulers.h"
#include <stdbool.h>


struct node *g_head = NULL;
int taskCounter = 0;

/*
* add()
*
* This function creates a new task to send to the 
scheduler
*/
void add(char *name, int priority, int burst){

    //create new task
    Task* newTask = (Task*) malloc(sizeof(Task));
    newTask->name = name; 
    newTask->priority = priority; 
    newTask->burst = burst;  
    insert(&g_head, newTask); 
    taskCounter++;
}


bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

/*
* This method sends processes to the scheduler in 
* lexicographical order. 
*/
void schedule(){
    int timeClock = 0; 

    struct node *temp;
    temp = g_head;

    //send each task to scheduler in order 
    while (temp != NULL) {
        Task* nextTask = pickNextTask(); 
        run(nextTask, nextTask->burst);
        timeClock += nextTask->burst; 
        printf("Time is now: %d\n",timeClock);
        temp = temp->next; 

        if(temp == NULL){
          if(g_head != NULL)
            temp = g_head; 
        }
    }
    
}

