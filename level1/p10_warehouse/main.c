#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int data;
    char *name;
    struct node *next;
    struct node *prev;
};

struct list {
    struct node *head;
};

struct node* find(const struct list *my_list, const char *name) {
    struct node *current = my_list->head;
    while (current != NULL) {
        if (!strcmp(name, current->name)) {
            return current;
        } else {
            current = current->next;
        }
    }
    return NULL;
}

void insert(struct list *my_list, char *name, int data) {
    if (my_list->head == NULL) {
        struct node *new_node = malloc(sizeof(struct node));
        new_node->data = data;
        new_node->name = name;
        new_node->next = NULL;
        new_node->prev = NULL;

        my_list->head = new_node;
    }
    else {
        struct node *tmp = find(my_list, name);
        if (tmp == NULL) {
            struct node *new_node = malloc(sizeof(struct node));
            new_node->data = data;
            new_node->name = name;
            new_node->next = my_list->head;
            my_list->head -> prev = new_node;
            new_node->prev = NULL;

            my_list->head = new_node;
        } else {
            tmp -> data += data;
        }
    }
}

void erase(const struct list *my_list, const char *name, const int data) {
    struct node *tmp = find(my_list, name);
    if (tmp == NULL) {
        printf("Not found.\n");
        return;
    }
    tmp -> data -= data;
    if (tmp -> data < 0) {
        printf("Illegal Num.\n");
    }
    else if(tmp -> data == 0) {
        tmp -> prev -> next = tmp -> next;
        tmp -> next -> prev = tmp -> prev;
        free(tmp -> name);
        free(tmp);
    }
}

void display(struct list *my_list) {
    struct node *current = my_list->head;
    while (current != NULL) {
        printf("%s %d\n", current->name, current->data);
        current = current->next;
    }
}

struct list load() {
    int n;
    FILE *f = fopen("data.txt", "r");
    fscanf(f, "%d\n", &n);

    printf("%d\n", n);

    struct list my_list;
    my_list.head = NULL;

    for (int i = 0; i < n; i++) {
        int data;
        char *name = malloc(sizeof(char) * 50);
        fscanf(f, "%s %d\n", name, &data);
        printf("%s %d\n", name, data);

        insert(&my_list, name, data);
    }
    fclose(f);

    return my_list;
}

void save(const struct list *my_list) {
    FILE *f = fopen("data.txt", "w");

    const struct node *current = my_list->head;

    int counter = 0;

    while (current != NULL) {
        ++counter;
        current = current->next;
    }
    current = my_list->head;

    fprintf(f, "%d\n", counter);
    while (current != NULL) {
        fprintf(f, "%s %d\n", current->name, current->data);
        current = current->next;
    }
    fclose(f);
}

int main() {

    struct list my_list = load();

    while(1) {
        int choice;
        printf("1. Display\n2. Insert\n3. Erase\n4. Exit\n");
        printf("Enter your choice:");
        scanf("%d", &choice);
        if (choice == 1) {
            putchar('\n');
            display(&my_list);
            putchar('\n');
        }
        else if (choice == 2) {
            char *name = malloc(sizeof(char) * 50);
            int data;
            scanf("%s %d", name, &data);
            insert(&my_list, name, data);
        }
        else if (choice == 3) {
            char *name = malloc(sizeof(char) * 50);
            int data;
            scanf("%s %d", name, &data);
            erase(&my_list, name, data);
        }
        else {
            save(&my_list);
            break;
        }
    }

    return 0;
}