#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list, implement this ourselves 
void add(char *name, int priority, int burst);

// invoke the scheduler, executes each task according to specified scheduling algo 
void schedule();