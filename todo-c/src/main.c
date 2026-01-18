#include <stdio.h>
#include "todo.h"

static int read_int(void) {
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return -1;
    return (int)strtol(buf, NULL, 10);
}

static void press_enter(void) {
    printf("\nPress Enter to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
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
            case 5: return 0;
            default: printf("Choose 1-5.\n");
        }
        press_enter();
    }
}
