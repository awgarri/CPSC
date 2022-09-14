// Name: Drew Garrison
// Date: 10/12/2021

#include "mythreads.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include <ucontext.h>

typedef struct thread {
  int id;
  ucontext_t context;
  thFuncPtr func;
  void *arg;
  void *result;
  int done;
} thread_t;

typedef struct node {
  thread_t *t;
  struct node *next;
} node_t;

node_t *start, *current;

int locks[NUM_LOCKS];
int conditions[NUM_LOCKS][CONDITIONS_PER_LOCK];

int thread_count = 1;
int interruptsAreDisabled = 0;

static void disableInterrupt() {
  interruptsAreDisabled = 1;
}

static void enableInterrupt() {
  interruptsAreDisabled = 0;
}

void addNode(node_t *node) {
  node_t *tmp2 = current->next;
  current->next = node;
  node->next = tmp2;
}

void removeNode(node_t *node) {
  node_t *tmp = start;
  while (tmp->next != node) {
    tmp = tmp->next;
  }
  tmp = tmp->next;
}

// Wrapper function for running the user function
void runThread(thread_t *t) {
  enableInterrupt();
  t->result = t->func(t->arg);
  t->done = 1;
  disableInterrupt();
}

// Advance to the next unfinished thread
void threadYieldInternal() {
  node_t *old = current;
  do {
    current = current->next;
  } while (current->t->done);

  // Switch contexts the the next thread
  if (old != current) {
    swapcontext(&old->t->context, &current->t->context);
  } 
}

extern void threadYield() {
  disableInterrupt();
  threadYieldInternal();
  enableInterrupt();
}


extern void threadInit() {
  // Create a thread structure for the main thread
  thread_t *main_thread = malloc(sizeof(thread_t));
  getcontext(&main_thread->context);
  main_thread->id = 1;
  main_thread->done = 0;

  node_t *main_node = malloc(sizeof(node_t));
  main_node->t = main_thread;
  main_node->next = main_node;

  start = current = main_node;
}

extern int threadCreate(thFuncPtr funcPtr, void *argPtr) {
  disableInterrupt();

  // Create a thread structure for the user function
  thread_t *t = (thread_t *) malloc(sizeof(thread_t));
  getcontext(&t->context);
  // Make a stack for it
  t->context.uc_stack.ss_sp = malloc(STACK_SIZE);
  t->context.uc_stack.ss_size = STACK_SIZE;
  t->context.uc_stack.ss_flags = 0;
  t->context.uc_link = &start->t->context;
  // Add other details
  t->func = funcPtr;
  t->arg = argPtr;
  t->done = 0;
  t->id = ++thread_count;
  makecontext(&t->context, (void (*)(void)) runThread, 1, t);

  // Create the node structure
  node_t *newnode = (node_t *) malloc(sizeof(node_t));
  newnode->t = t;
  // Add it to the list
  addNode(newnode);
  // Run it (or something else)
  threadYieldInternal();

  enableInterrupt();
  return newnode->t->id;
}

extern void threadJoin(int thread_id, void **result) {
  disableInterrupt();

  // Find the thread
  node_t *tmp = start;
  while (tmp->t->id != thread_id) {
    tmp = tmp->next;
  }

  // Switch threads until it's done
  while (!tmp->t->done) {
    threadYieldInternal();
  }

  // Store the result
  if (result)
    *result = tmp->t->result;

  enableInterrupt();
}

extern void threadExit(void *result) {
  disableInterrupt();
  // Store the result and mark thread as complete
  current->t->result = result;
  current->t->done = 1;
  // Remove it from the list
  removeNode(current);
  // Move on to other threads
  threadYieldInternal();
  enableInterrupt();
}

void threadLockInternal(int lockNum) {
  // Attempt to grab the lock
  int gotlock = 0;
  while (!gotlock) {
    if (!locks[lockNum]) {
      // Lock has been established
      locks[lockNum] = 1;
      gotlock = 1;
    } else {
      // Switch threads until it can be grabbed
      threadYield();
    }
  }
}

extern void threadLock(int lockNum) {
  disableInterrupt();
  threadLockInternal(lockNum);
  enableInterrupt();
}

extern void threadUnlock(int lockNum) {
  // Release the lock
  locks[lockNum] = 0;
}

extern void threadWait(int lockNum, int conditionNum) {
  disableInterrupt();

  // Prevent erroroneous calls
  if (!locks[lockNum]) {
    printf("unlocked mutex\n");
    exit(1);
  }

  threadUnlock(lockNum);
  // Take the current thread out of the rotation
  node_t *old = current;
  removeNode(current);
  // Wait for a condition variable signal
  while (!conditions[lockNum][conditionNum]) {
    threadYield();
  }
  // Put the thread back into the rotation
  addNode(old);
  threadLockInternal(lockNum);

  enableInterrupt();
}

// Signal the condition variable
extern void threadSignal(int lockNum, int conditionNum) {
  disableInterrupt();
  conditions[lockNum][conditionNum]++;
  enableInterrupt();
}