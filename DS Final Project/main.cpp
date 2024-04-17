#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <wchar.h>

const int MAXFILENAMESIZE = 256;

struct Node {
    wchar_t filename[MAXFILENAMESIZE];
    char* Name;
    struct Node* next;
    struct Node* prev;
};

typedef struct {
    struct Node* Front;
    struct Node* Back;
} Queue;

typedef struct {
    int top;
    struct Node* array[MAXFILENAMESIZE];
} Stack;

struct CircularLinkedList {
    wchar_t filename[MAXFILENAMESIZE];
    char* Name;
    struct CircularLinkedList* next;
    struct Node* current;
};

Queue* InitializeQueue(void);
bool IsQueueEmpty(Queue* queue);
struct Node* CreateNewNode(const wchar_t* filename);
void EnQueue(Queue* queue, const wchar_t* filename);
void addFile(struct Node** headRef, struct CircularLinkedList** circularHeadRef, const char* filename);
void play(struct Node* linkedList, Stack* stack);
void freeList(struct Node* head);
bool isEmpty(Stack* stack);
struct Node* pop(Stack* stack);
void push(Stack* stack, struct Node* node);
void printList(struct Node* head);
void printCircularList(struct CircularLinkedList* head);

struct Node* CreateNewNode(const wchar_t* filename) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    wcscpy_s(newNode->filename, MAXFILENAMESIZE, filename);
    newNode->next = NULL;
    return newNode;
}

void EnQueue(Queue* queue, const wchar_t* filename) {
    if (queue == NULL) {
        queue = InitializeQueue();
    }
    struct Node* toEnqueue = CreateNewNode(filename);
    if (IsQueueEmpty(queue)) {
        queue->Front = toEnqueue;
        queue->Back = toEnqueue;
    }
    else {
        queue->Back->next = toEnqueue;
        queue->Back = toEnqueue;
    }
}

Queue* InitializeQueue(void) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->Front = NULL;
    queue->Back = NULL;
    return queue;
}

bool IsQueueEmpty(Queue* queue) {
    return queue->Front == NULL;
}

void addFile(struct Node** headRef, struct CircularLinkedList** circularHeadRef, const char* filename) {
    wchar_t wFilename[MAXFILENAMESIZE];
    if (MultiByteToWideChar(CP_UTF8, 0, filename, -1, wFilename, MAXFILENAMESIZE) == 0) {
        fprintf(stderr, "Failed to convert filename to wide-character string\n");
        exit(EXIT_FAILURE);
    }

    struct Node* newNode = CreateNewNode(wFilename);
    if (*headRef == NULL) {
        *headRef = newNode;
    }
    else {
        struct Node* current = *headRef;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    if (*circularHeadRef == NULL) {
        *circularHeadRef = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
        if (*circularHeadRef == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        (*circularHeadRef)->current = newNode;
        (*circularHeadRef)->next = *circularHeadRef;
    }
    else {
        struct CircularLinkedList* circularCurrent = *circularHeadRef;
        while (circularCurrent->next != *circularHeadRef) {
            circularCurrent = circularCurrent->next;
        }
        struct CircularLinkedList* newCircularNode = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
        if (newCircularNode == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newCircularNode->current = newNode;
        newCircularNode->next = *circularHeadRef;
        circularCurrent->next = newCircularNode;
    }
}

void play(struct Node* linkedList, Stack* stack) {
    struct Node* current = linkedList;
    bool continuePlaying = true;

    while (current != NULL && continuePlaying) {
        printf("Playing: %ls\n", current->filename);
        if (PlaySoundW(current->filename, NULL, SND_FILENAME) == 0) {
            fprintf(stderr, "Failed to play audio file: %ls\n", current->filename);
            return;
        }

        if (_kbhit()) {
            char userInput = _getch();
            switch (userInput) {
            case 'p':
                if (!isEmpty(stack)) {
                    struct Node* prev = pop(stack);
                    if (prev != NULL) {
                        current = prev;
                    }
                    else {
                        printf("No previous song available.\n");
                    }
                }
                break;
            }
        }

        if (current->next != NULL) {
            push(stack, current);
            current = current->next;
        }
        else {
            continuePlaying = false;
        }
    }
}

void freeList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}

bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, struct Node* node) {
    if (stack->top < MAXFILENAMESIZE - 1) {
        stack->top++;
        stack->array[stack->top] = node;
    }
    else {
        fprintf(stderr, "Stack full\n");
    }
}

struct Node* pop(Stack* stack) {
    if (!isEmpty(stack)) {
        struct Node* node = stack->array[stack->top];
        stack->top--;
        return node;
    }
    else {
        fprintf(stderr, "Stack is empty\n");
        return NULL;
    }
}

void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("File: %ls\n", current->filename);
        current = current->next;
    }
}

void printCircularList(struct CircularLinkedList* head) {
    if (head == NULL) return;
    struct CircularLinkedList* current = head;
    do {
        printf("Circular File: %ls\n", current->current->filename);
        current = current->next;
    } while (current != head);
}

int main(void) {
    struct Node* playlist = NULL;
    struct CircularLinkedList* roundPlaylist = NULL;
    Stack stack;
    stack.top = -1; // Initialize stack
    int userInput = 0;

    printf("Welcome to the music player 1050\n");
    while (true) {
        printf("1: Create a playlist\n");
        printf("2: Add song to the playlist\n");
        printf("3: Play playlist\n");
        printf("4: Delete playlist\n");
        printf("5: Exit\n");
        printf("Enter your choice: ");
        scanf_s("%d", &userInput);

        switch (userInput) {
        case 1:
            freeList(playlist);
            playlist = NULL;
            roundPlaylist = NULL;
            printf("New playlist created. You can now add songs.\n");
            break;
        case 2:
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\source\\repos\DS-Final-Project\\AudioDB\\Dark.wav");
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\source\\repos\\DS-Final-Project\\AudioDB\\randomsound.wav");
            printf("Songs added to the playlist.\n");
            break;
        case 3:
            if (playlist == NULL) {
                printf("No songs in playlist. Add some songs first.\n");
            }
            else {
                printf("Playing playlist...\n");
                play(playlist, &stack);
            }
            break;
        case 4:
            freeList(playlist);
            playlist = NULL;
            roundPlaylist = NULL;
            printf("Playlist deleted.\n");
            break;
        case 5:
            freeList(playlist);
            return 0;
        default:
            printf("Invalid option. Please try again.\n");
        }
    }
    return 0;
}
