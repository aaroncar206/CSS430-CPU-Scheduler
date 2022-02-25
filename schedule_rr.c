/*
* schedule_rr.c
*
* By: Aaron Carbonell
*
* This program simulates a round robin cpu scheduler 
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
     
}

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

/*
* pickNextTask()
*
* This function picks the next task in the linked list that has
* least amount of runs to enable round robin scheduling. 
*/ 
Task *pickNextTask() {
    // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task; 
  int minRuns = temp->task->numRuns; 

  //find minimimum num runs
  while (temp != NULL) {

    //pick task that has least number of runs 
    if ( temp->task->numRuns <= minRuns){
      minRuns = temp->task->numRuns;
      best_sofar = temp->task;
    }
  
    temp = temp->next;
  }
  
  temp = g_head; 
  
  //find lex first task that has the min num of runs
  while(temp != NULL){
    if (temp->task->numRuns == minRuns)
      best_sofar = temp->task;
    temp = temp->next;
  }

  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

/*
* schedule()
*
* This function sends processes to scheduler in round robin fashion
* using a time quantum of 10. 
*/
void schedule(){
    int timeClock = 0; 
    int tq = 10; 

    struct node *temp;
    temp = g_head;

    //send each task to scheduler in order 
    while (temp != NULL) {
        Task* nextTask = pickNextTask(g_head); //pick next task to send to scheduler
        
        //check if need to run for entire tq
        if(nextTask->burst < tq){
          if(nextTask->burst == 0){
            continue; 
          }

          run(nextTask, nextTask->burst);
          nextTask->numRuns++;
          timeClock += nextTask->burst; 
          nextTask->burst -= nextTask->burst; 
        }
        else{
          run(nextTask, tq);
          nextTask->numRuns++;
          timeClock += tq; 
          nextTask->burst -= tq;

          if(nextTask->burst == 0){
            printf("Time is now: %d\n",timeClock);
            continue; 
          }
          insert(&g_head, nextTask); //problem: if putting back in the queue, will pick the same 
                                    //process everytime to send to scheduler. 

        }

        printf("Time is now: %d\n",timeClock);
        temp = temp->next; 


        //reset traverser to head when you reach a NULL task
        if(temp == NULL){
          if(g_head != NULL){
            temp = g_head; 
          }
        }

    }
    
  
}


