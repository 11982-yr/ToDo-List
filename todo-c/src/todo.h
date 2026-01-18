#ifndef TODO_H
#define TODO_H

#define MAX_TASKS 200
#define MAX_TEXT  200
#define DATA_FILE "tasks.txt"

typedef struct {
    int done;
    char text[MAX_TEXT];
} Task;

/* core functions */
int load_tasks(Task tasks[], int capacity);
void save_tasks(const Task tasks[], int count);

/* task actions */
void list_tasks(const Task tasks[], int count);
void add_task(Task tasks[], int *count);
void toggle_done(Task tasks[], int count);
void delete_task(Task tasks[], int *count);

#endif
