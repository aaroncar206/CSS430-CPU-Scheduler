/*
* schedule_priority.c 
*
* By: Aaron Carbonell 
*
* This program simulates a priority 
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
* hasMultiple()
*
* This function checks if there are multiple processes 
* with same priority. 
*
*/
bool hasMultiple(int num){
  int counter = 0; 

  if (!g_head)
    return NULL;

  struct node* temp;
  temp = g_head; 

  while(temp != NULL){
    if(temp->task->priority == num){
      counter++;
    }

    temp = temp->next; 
  }

  if(counter > 1){
    return true; 
  }

  return false; 
}


Task* pickLexFirst(int num){
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  //loop through list and find lex first out of the same bursts
  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name) && temp->task->priority == num)
      best_sofar = temp->task;
    temp = temp->next;
  }

  return best_sofar; 
}

/*
* pickNextTask()
*
* This function picks the next task in the linked list that has
* least amount of runs and highest priority to enable priority
* round robin scheduling. 
*/ 
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *maxTask = temp->task;
  int maxSoFar = temp->task->priority;

  //find highest priority task in list
  while (temp != NULL) {
    if (temp->task->priority >= maxSoFar){

        //if multiple processes with same priority, pick the lexicographical first
        //among each of them.
        if(hasMultiple(temp->task->priority)){
          maxTask = pickLexFirst(temp->task->priority);
          maxSoFar = temp->task->priority; 
        }
        else
        {
          maxTask = temp->task;
          maxSoFar = temp->task->priority;
        }
    }
    temp = temp->next;
  }

  // delete the node from list, Task will get deleted later
  delete (&g_head, maxTask);
  return maxTask;
}

/*
* schedule()
*
* This function sends processes to scheduler based on priority 
* in descending order. 
*/
void schedule(){
    int timeClock = 0; 

    struct node *temp;
    temp = g_head;
 
    //send each task to scheduler in order 
    while (temp != NULL) {
        Task* nextTask = pickNextTask(); //pick next task to send to scheuduler
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

