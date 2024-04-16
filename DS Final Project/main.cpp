#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <windows.h>  // Include Windows.h with lowercase 'w' for portability
#include <wchar.h>    // Needed for wcscpy_s and wchar_t


const int MAXFILENAMESIZE = 256;
// Define a structure for the linked list node
struct Node {
    wchar_t filename[MAXFILENAMESIZE];
    char* Name;
    struct Node* next;
    struct Node* head;
};

// Queue
typedef struct {
    struct Node* Front; // HEAD POINTER
    struct Node* Back;  // TAIL POINTER
} Queue;

// Stack
typedef struct {
    int top;
    int array[MAXFILENAMESIZE];
} Stack;


struct CircularLinkedList {
    wchar_t filename[MAXFILENAMESIZE];
    char* Name;
    struct CircularLinkedList* next;
    struct CircularLinkedList* head;
    struct Node* current;
};

// Prototypes
Queue* InitializeQueue(void);
bool IsQueueEmpty(Queue* queue);
struct Node* CreateNewNode(const wchar_t* filename);
void EnQueue(Queue* queue, const wchar_t* filename);
void addFile(struct Node** headRef, struct CircularLinkedList** circularHeadRef, const char* filename);
void play(struct Node* linkedList, struct CircularLinkedList* circularList, Stack* stack);
void freeList(struct Node* head);
bool isEmpty(Stack* stack);
int pop(Stack* stack);
void push(Stack* stack, int data);

// Create a new node
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

// Put items on queue
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

// Initialize queue
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

// Check if the queue is empty
bool IsQueueEmpty(Queue* queue) {
    return queue->Front == NULL;
}



void addFile(struct Node** headRef, struct CircularLinkedList** circularHeadRef, const char* filename) {
    wchar_t wFilename[MAXFILENAMESIZE];
    if (MultiByteToWideChar(CP_UTF8, 0, filename, -1, wFilename, MAXFILENAMESIZE) == 0) {
        fprintf(stderr, "Failed to convert filename to wide-character string\n");
        exit(EXIT_FAILURE);
    }

    // Create a new node for the regular linked list
    struct Node* newNode = CreateNewNode(wFilename);

    // Add the new node to the end of the regular linked list
    if (*headRef == NULL) {
        *headRef = newNode; // If the list is empty, make the new node the head
    }
    else {
        struct Node* current = *headRef;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode; // Add the new node to the end of the list
    }

    // Create or update the circular linked list
    if (*circularHeadRef == NULL) {
        // If the circular linked list is empty, create the first node
        *circularHeadRef = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
        if (*circularHeadRef == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        (*circularHeadRef)->current = newNode; // Point current to the new node
        (*circularHeadRef)->next = *circularHeadRef; // Make it circular
    }
    else {
        // Find the last node in the circular linked list
        struct CircularLinkedList* circularCurrent = *circularHeadRef;
        while (circularCurrent->next != *circularHeadRef) {
            circularCurrent = circularCurrent->next;
        }
        // Create a new circular node
        struct CircularLinkedList* newCircularNode = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
        if (newCircularNode == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newCircularNode->current = newNode; // Link it to the new regular node
        newCircularNode->next = *circularHeadRef; // Maintain circular nature
        circularCurrent->next = newCircularNode; // Insert it into the list
    }
}





void play(struct Node* linkedList, struct CircularLinkedList* circularList, Stack* stack) {
    struct Node* current = linkedList; // Start with the regular linked list
    struct CircularLinkedList* circularCurrent = circularList; // Handle to circular linked list
    bool continuePlaying = true;
    bool isLooping = false;

    while (continuePlaying) {
        printf("Playing: %ws\n", current->filename);
        if (PlaySoundW(current->filename, NULL, SND_FILENAME) == 0) {
            fprintf(stderr, "Failed to play audio file: %ws\n", current->filename);
            return;
        }

        // Check for user input to toggle loop mode or quit
        if (_kbhit()) {
            char userInput = _getch();
            switch (userInput) {
            case 'p': // Previous song logic
                // Implement stack logic to go back to the previous song
                break;
            case 'l': // Loop toggle
                isLooping = !isLooping;
                if (isLooping) {
                    // Find the corresponding circular node to the current song
                    while (circularCurrent->current != current && circularCurrent->next != circularList) {
                        circularCurrent = circularCurrent->next;
                    }
                }
                break;
            }
        }

        if (isLooping) {
            // In loop mode, move through the circular linked list
            circularCurrent = circularCurrent->next;
            current = circularCurrent->current; // Sync regular list node with circular list node
        }
        else {
            // Move to next song in the regular linked list
            if (current->next != NULL) {
                current = current->next;
            }
            else {
                // Optionally stop playing at the end if not looping
                continuePlaying = false;
            }
        }
    }
}





// Function to free the memory allocated for the linked list
void freeList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}

// Check if the stack is empty
bool isEmpty(Stack* stack) {
    return stack->top == -1;
}

// Pop operation
int pop(Stack* stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Error: stack underflow exception\n");
        exit(EXIT_FAILURE);
    }
    return stack->array[stack->top--];

}

// Push operation
void push(Stack* stack, int data) {
    if (stack->top >= 255) {
        fprintf(stderr, "Error: stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->array[++stack->top] = data;
}

int main(void) {
    bool helper = false;
    struct Node* playlist = NULL;
    struct CircularLinkedList* roundPlaylist = NULL;
    Stack stack; // Initialize the stack
    stack.top = -1; // Set top to -1 to indicate an empty stack
    int userInput = 0;
    do {
        printf("Welcome to the music player 1050\n");
        printf("Pick an option\n");
        printf("1: Create a playlist\n");
        printf("2: Add song to the playlist\n");
        printf("3: Play playlist\n");
        printf("4: Delete song in playlist\n");
        printf("5: Delete playlist\n");
        printf("Enter your choice: ");
        scanf_s("%d", &userInput);

        switch (userInput) {
        case 1:
            // Assuming InitializeQueue() is defined elsewhere
            InitializeQueue();
            break;
        case 2:
            // Assuming addFile() is defined elsewhere
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\dark - future - logo - 196217.wav");
            break;
        case 3:
            play(playlist, roundPlaylist, &stack); // Assuming initially not using circular linked list
            printf("If you want to go back to the previous song click p\n");
            printf("If you start to fall in love with this playlist and want to put it on a loop press L\n");
            break;
        case 4:
            freeList(playlist); // Assuming freeList() is defined elsewhere
            break;
        case 5:
            freeList(playlist); // Assuming freeList() is defined elsewhere
            break;
        case 6:
            exit(EXIT_SUCCESS);
        default:
            printf("Invalid option\n");
            return 1;
        }

    } while (helper == false);

    return 0;
}




