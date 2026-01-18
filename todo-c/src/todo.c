#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "todo.h"

/* ---------- private helper functions ---------- */

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static int read_int(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin))
        return -1;

    char *p = buf;
    while (isspace((unsigned char)*p))
        p++;

    return (int)strtol(p, NULL, 10);
}

/* ---------- file handling ---------- */

int load_tasks(Task tasks[], int capacity) {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file)
        return 0;   /* no file yet */

    int count = 0;
    char line[512];

    while (count < capacity && fgets(line, sizeof(line), file)) {
        trim_newline(line);

        if (line[0] == '\0')
            continue;

        /* format: done|text */
        char *sep = strchr(line, '|');
        if (!sep)
            continue;

        *sep = '\0';
        tasks[count].done = atoi(line) != 0;

        strncpy(tasks[count].text, sep + 1, MAX_TEXT - 1);
        tasks[count].text[MAX_TEXT - 1] = '\0';

        count++;
    }

    fclose(file);
    return count;
}

void save_tasks(const Task tasks[], int count) {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) {
        printf("Error: could not save tasks.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%s\n", tasks[i].done, tasks[i].text);
    }

    fclose(file);
}

/* ---------- task operations ---------- */

void list_tasks(const Task tasks[], int count) {
    if (count == 0) {
        printf("\nNo tasks found.\n");
        return;
    }

    printf("\nYour tasks:\n");
    for (int i = 0; i < count; i++) {
        printf("%3d) [%c] %s\n",
               i + 1,
               tasks[i].done ? 'x' : ' ',
               tasks[i].text);
    }
}

void add_task(Task tasks[], int *count) {
    if (*count >= MAX_TASKS) {
        printf("Task list is full.\n");
        return;
    }

    char buf[MAX_TEXT];
    printf("Enter task: ");

    if (!fgets(buf, sizeof(buf), stdin))
        return;

    trim_newline(buf);

    if (buf[0] == '\0') {
        printf("Task cannot be empty.\n");
        return;
    }

    tasks[*count].done = 0;
    strncpy(tasks[*count].text, buf, MAX_TEXT - 1);
    tasks[*count].text[MAX_TEXT - 1] = '\0';

    (*count)++;
    save_tasks(tasks, *count);

    printf("Task added.\n");
}

void toggle_done(Task tasks[], int count) {
    if (count == 0) {
        printf("No tasks to update.\n");
        return;
    }

    list_tasks(tasks, count);
    printf("\nEnter task number to toggle: ");

    int index = read_int() - 1;
    if (index < 0 || index >= count) {
        printf("Invalid task number.\n");
        return;
    }

    tasks[index].done = !tasks[index].done;
    save_tasks(tasks, count);

    printf("Task updated.\n");
}

void delete_task(Task tasks[], int *count) {
    if (*count == 0) {
        printf("No tasks to delete.\n");
        return;
    }

    list_tasks(tasks, *count);
    printf("\nEnter task number to delete: ");

    int index = read_int() - 1;
    if (index < 0 || index >= *count) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    (*count)--;
    save_tasks(tasks, *count);

    printf("Task deleted.\n");
}
