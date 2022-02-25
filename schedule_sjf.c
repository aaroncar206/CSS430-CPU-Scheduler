/*
* schedule_sjf.c 
*
* By: Aaron Carbonell 
*
* This program simulates a shortest-job-first
* priority scheduler. 
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

/*
* hasMultiple()
*
* This function checks if there are multiple processes 
* with the same burst time. 
*
*/
bool hasMultiple(int num){
  int counter = 0; 

  if (!g_head)
    return NULL;

  struct node* temp;
  temp = g_head; 

  while(temp != NULL){
    if(temp->task->burst == num){
      counter++;
    }

    temp = temp->next; 
  }

  if(counter > 1){
    return true; 
  }

  return false; 
}

/*
* pickNextTask()
*
* This function picks the next task in the linked list that
* has the next lowest burst time. 
*/ 
Task* pickLexFirst(int num){
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  //loop through list and find lex first out of the same bursts
  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name) && temp->task->burst == num)
      best_sofar = temp->task;
    temp = temp->next;
  }

  return best_sofar; 
}

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *minTask = temp->task;
  int minSoFar = temp->task->burst;

  //find minimum burst task in list
  while (temp != NULL) {
    if (temp->task->burst <= minSoFar){
      if(hasMultiple(temp->task->burst)){
        minTask = pickLexFirst(temp->task->burst);
        minSoFar = temp->task->burst; 
      }
      else{
        minTask = temp->task;
        minSoFar = temp->task->burst;
      }

    }
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&g_head, minTask);
  return minTask;
}

/*
* schedule()
*
* This function sends processes to scheduler based on 
* burst time in ascending order. 
*/
void schedule(){
    int timeClock = 0; 

    struct node *temp;
    temp = g_head;
 
    //send each task to scheduler in order 
    while (temp != NULL) {
        Task* nextTask = pickNextTask(); //does not get to go all the way through list because deletes as you go 
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
