#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

int find(struct node *head, int data) {
    struct node *current = head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        } else {
            current = current->next;
        }
    }
    return 0;
}

int main() {

    int n;
    scanf("%d", &n);
    struct node *head = NULL;
    for (int i = 0; i < n; i++) {
        struct node *node = (struct node *)malloc(sizeof(struct node));
        scanf("%d", &node->data);
        node->next = NULL;
        if (head == NULL) {
            head = node;
        }
        else {
            node->next = head;
            head = node;
        }
    }
    struct node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

    if (find(head, 5) == 1) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }

    current = head;
    struct node *prev = NULL, *after = NULL;
    while (current != NULL) {
        after = current->next;
        current->next = prev;
        prev = current;
        current = after;
    }
    head = prev;
    current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }

    return 0;
}