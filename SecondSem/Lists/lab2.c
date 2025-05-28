#include <stdio.h>
#include <stdlib.h>

// Структура узла
struct node {
    int data;
    struct node *next;
    struct node *link;  // Указатель для связи с другим списком
};

// Функция для добавления узла в конец списка
void append(struct node **head_ref, int new_data) {
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    struct node *last = *head_ref;

    new_node->data = new_data;
    new_node->next = NULL;
    new_node->link = NULL;

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
}

// Функция для вывода списка на экран
void printList(struct node *node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

// Функция для связывания двух списков по схеме варианта 8
void linkLists(struct node *head1, struct node *head2) {
    struct node *temp1 = head1;
    struct node *temp2 = head2;

    while (temp1 != NULL && temp2 != NULL) {
        temp1->link = temp2;
        temp1 = temp1->next;
        temp2 = temp2->next;
    }

    // Если первый список длиннее, оставшиеся link устанавливаем в NULL
    while (temp1 != NULL) {
        temp1->link = NULL;
        temp1 = temp1->next;
    }
}
// Функция для навигации по списку
void navigateList(struct node *head) {
    struct node *current = head;
    char input;

    while (1) {
        printf("Current node: %d\n", current->data);
        printf("Press 'D' or '6' to move right, 'S' or '2' to move down, 'Q' to quit: ");
        scanf(" %c", &input);

        if (input == 'D' || input == '6') {
            if (current->next != NULL) {
                current = current->next;
            } else {
                printf("End of list reached. Do you want to go back to the start? (Y/N): ");
                scanf(" %c", &input);
                if (input == 'Y' || input == 'y') {
                    current = head;
                }
            }
        } else if (input == 'S' || input == '2') {
            if (current->link != NULL) {
                current = current->link;
            } else {
                printf("No linked node below. Do you want to go back to the start? (Y/N): ");
                scanf(" %c", &input);
                if (input == 'Y' || input == 'y') {
                    current = head;
                }
            }
        } else if (input == 'Q' || input == 'q') {
            break;
        } else {
            printf("Invalid input. Try again.\n");
        }
    }
}

int main() {
    struct node *head1 = NULL;  // Голова первого списка
    struct node *head2 = NULL;  // Голова второго списка
    int input;

    // Ввод первого списка
    printf("Enter elements for the first list (0 to stop):\n");
    while (1) {
        scanf("%d", &input);
        if (input == 0) break;
        append(&head1, input);
    }

    // Ввод второго списка
    printf("Enter elements for the second list (0 to stop):\n");
    while (1) {
        scanf("%d", &input);
        if (input == 0) break;
        append(&head2, input);
    }

    // Связывание списков по схеме варианта 8
    linkLists(head1, head2);

    // Вывод списков
    printf("First list: ");
    printList(head1);
    printf("Second list: ");
    printList(head2);

    // Навигация по списку
    printf("Starting navigation from the first list:\n");
    navigateList(head1);

    return 0;
}