#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TASKS 200
#define MAX_TEXT  200
#define DATA_FILE "tasks.txt"

typedef struct {
    int done;                 // 0 = not done, 1 = done
    char text[MAX_TEXT];
} Task;

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static int read_int(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    // skip leading spaces
    char *p = buf;
    while (isspace((unsigned char)*p)) p++;
    return (int)strtol(p, NULL, 10);
}

static void press_enter(void) {
    printf("\nPress Enter to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int load_tasks(Task tasks[], int capacity) {
    FILE *f = fopen(DATA_FILE, "r");
    if (!f) return 0; // no file yet is fine

    int count = 0;
    char line[512];

    while (count < capacity && fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        // Format: done|text
        char *sep = strchr(line, '|');
        if (!sep) continue;

        *sep = '\0';
        int done = atoi(line);
        char *text = sep + 1;

        tasks[count].done = (done != 0);
        strncpy(tasks[count].text, text, MAX_TEXT - 1);
        tasks[count].text[MAX_TEXT - 1] = '\0';
        count++;
    }

    fclose(f);
    return count;
}

static void save_tasks(const Task tasks[], int count) {
    FILE *f = fopen(DATA_FILE, "w");
    if (!f) {
        printf("Error: could not open %s for writing.\n", DATA_FILE);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(f, "%d|%s\n", tasks[i].done, tasks[i].text);
    }

    fclose(f);
}

static void list_tasks(const Task tasks[], int count) {
    if (count == 0) {
        printf("\nNo tasks yet.\n");
        return;
    }

    printf("\nYour tasks:\n");
    for (int i = 0; i < count; i++) {
        printf("%3d) [%c] %s\n", i + 1, tasks[i].done ? 'x' : ' ', tasks[i].text);
    }
}

static void add_task(Task tasks[], int *count) {
    if (*count >= MAX_TASKS) {
        printf("Task list is full.\n");
        return;
    }

    char buf[MAX_TEXT];
    printf("Enter task: ");
    if (!fgets(buf, sizeof(buf), stdin)) return;
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
    printf("Added!\n");
}

static void toggle_done(Task tasks[], int count) {
    if (count == 0) {
        printf("No tasks to update.\n");
        return;
    }

    list_tasks(tasks, count);
    printf("\nEnter task number to toggle done: ");
    int idx = read_int() - 1;

    if (idx < 0 || idx >= count) {
        printf("Invalid task number.\n");
        return;
    }

    tasks[idx].done = !tasks[idx].done;
    save_tasks(tasks, count);
    printf("Updated!\n");
}

static void delete_task(Task tasks[], int *count) {
    if (*count == 0) {
        printf("No tasks to delete.\n");
        return;
    }

    list_tasks(tasks, *count);
    printf("\nEnter task number to delete: ");
    int idx = read_int() - 1;

    if (idx < 0 || idx >= *count) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = idx; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    (*count)--;

    save_tasks(tasks, *count);
    printf("Deleted!\n");
}

static void show_menu(void) {
    printf("\n=== To-Do List (C) ===\n");
    printf("1) List tasks\n");
    printf("2) Add task\n");
    printf("3) Toggle done\n");
    printf("4) Delete task\n");
    printf("5) Quit\n");
    printf("> ");
}

int main(void) {
    Task tasks[MAX_TASKS];
    int count = load_tasks(tasks, MAX_TASKS);

    while (1) {
        show_menu();
        int choice = read_int();

        switch (choice) {
            case 1: list_tasks(tasks, count); break;
            case 2: add_task(tasks, &count); break;
            case 3: toggle_done(tasks, count); break;
            case 4: delete_task(tasks, &count); break;
            case 5:
                printf("Bye!\n");
                return 0;
            default:
                printf("Choose 1-5.\n");
                break;
        }
        press_enter();
    }
}
