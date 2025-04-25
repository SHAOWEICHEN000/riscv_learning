#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Split the linked list into two parts
void splitList(Node *head, Node **firstHalf, Node **secondHalf)
{

    Node* slow = head;
    Node* fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    *firstHalf = head;
    *secondHalf = slow->next;
    slow->next = NULL; // Terminate the first half  
}

// Merge two sorted linked lists
Node *mergeSortedLists(Node *a, Node *b)
{
    Node *result = NULL;

    if (a == NULL) return b;
    if (b == NULL) return a;

    asm volatile(
        /* Block B (mergeSortedList), which merges two sorted lists into one */
        "lw t0, 0(%[a])\n\t"        // t0 = a->data
        "lw t1, 0(%[b])\n\t"        // t1 = b->data
        "ble t0, t1, 1f\n\t"        // if a->data <= b->data goto label 1
        "mv %[res], %[b]\n\t"       // result = b
        "j 2f\n\t"
        "1:\n\t"
        "mv %[res], %[a]\n\t"       // result = a
        "2:\n\t"
        : [res] "=r"(result)
        : [a] "r"(a), [b] "r"(b)
        : "t0", "t1", "memory"
   "" );

    if (result == a) {
        result->next = mergeSortedLists(a->next, b);
    } else {
        result->next = mergeSortedLists(a, b->next);
    }

    return result;
}

// Merge Sort function for linked list
Node *mergeSort(Node *head)
{
    if (!head || !head->next)
        return head; // Return directly if there is only one node

    Node *firstHalf, *secondHalf;
    splitList(head, &firstHalf, &secondHalf); // Split the list into two sublists

    firstHalf = mergeSort(firstHalf);   // Recursively sort the left half
    secondHalf = mergeSort(secondHalf); // Recursively sort the right half

    return mergeSortedLists(firstHalf, secondHalf); // Merge the sorted sublists
}

// Function to free the linked list
void freeList(Node *head)
{
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    Node* tmp; 
    int list_size;
    fscanf(input, "%d", &list_size);
    Node *head = (list_size > 0) ? (Node *)malloc(sizeof(Node)) : NULL;
    Node *cur = head;
    for (int i = 0; i < list_size; i++) {
        fscanf(input, "%d", &(cur->data));
        if (i + 1 < list_size)
            cur->next = (Node *)malloc(sizeof(Node));
        cur = cur->next;
    }
    fclose(input);

    // Linked list sort
    head = mergeSort(head);

    cur = head;
    while (cur) {
        printf("%d ", cur->data);
        // Block C (cur = cur->next)
        cur = cur->next;
/*asm volatile (
   "lw %[tmp], 4(%[cur])\n"   // tmp = cur->next
    "mv %[cur], %[tmp]\n"      // cur = tmp
    : [cur] "+r"(cur), [tmp] "=r"(tmp)
    :
    : "memory"
);*/
    }
    printf("\n");

    // Free the linked list
    freeList(head);
    return 0;
}