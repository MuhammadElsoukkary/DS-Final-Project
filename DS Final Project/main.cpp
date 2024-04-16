#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>


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
    struct Node* head;
};

// Prototypes
Queue* InitializeQueue(void);
bool IsQueueEmpty(Queue* queue);
struct Node* CreateNewNode(const wchar_t* filename);
void EnQueue(Queue* queue, const wchar_t* filename);
void addFile(struct Node** headRef, const char* filename);
void play(struct Node* head);
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

// Function to add a file to the playlist
void addFile(struct Node** headRef, const char* filename) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Convert the filename to wide-character string
    wchar_t wFilename[256];
    if (MultiByteToWideChar(CP_UTF8, 0, filename, -1, wFilename, MAXFILENAMESIZE) == 0) {
        fprintf(stderr, "Failed to convert filename to wide-character string\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }

    wcscpy_s(newNode->filename, MAXFILENAMESIZE, wFilename);
    newNode->next = NULL;

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
}

// Function to play the playlist
void play(struct Node* linkedList, struct CircularLinkedList* circularList, Stack* stack) {
    struct Node* current = linkedList->head; // Start with the regular linked list
    int currentIndex = 0; // Initialize index counter
    bool loop = true; // Variable to control the loop

    while (loop) {
        do {
            printf("Playing: %ws\n", current->filename);
            if (PlaySoundW(current->filename, NULL, SND_FILENAME) == 0) {
                fprintf(stderr, "Failed to play audio file: %ws\n", current->filename);
                return;
            }

            // Check for user input
            if (_kbhit()) {
                char userInput = _getch();
                if (userInput == 'p' && !isEmpty(stack)) {
                    // Pause current song
                    PlaySound(NULL, NULL, SND_ASYNC);

                    // Play previous song
                    int prevIndex = pop(stack);
                    struct Node* prevSong = linkedList->head; // Start with the regular linked list
                    // Traverse the playlist to find the previous song
                    for (int i = 0; i < prevIndex; i++) {
                        prevSong = prevSong->next;
                    }
                    printf("Playing previous song: %ws\n", prevSong->filename);
                    PlaySoundW(prevSong->filename, NULL, SND_FILENAME);
                    continue; // Skip the increment step
                }
                else if (userInput == 'l') {
                    // Use circular linked list if 'L' is pressed
                    current = circularList->head;
                    break;
                }
            }

            push(stack, currentIndex); // Push the index onto the stack
            current = current->next; // Move to the next node
            currentIndex++; // Move to the next index

        } while (current != NULL && loop); // Continue until end of list or user quits
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
            InitializeQueue();
           
            break;
        case 2:
            addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
            addFile(&playlist, "C:\\Users\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\dark - future - logo - 196217.wav");
            break;
        case 3:
            play(playlist, roundPlaylist, &stack); // Assuming initially not using circular linked list
            printf("If you want to go back to the previous song click p\n");
            printf("If you start to fall in love with this playlist and want to put it on a loop press L\n");
            break;
        case 4:
            freeList(playlist);
            break;
        case 5:
            freeList(playlist);
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



