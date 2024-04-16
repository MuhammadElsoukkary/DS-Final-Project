#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

// Define a structure for the linked list node
struct Node {
    wchar_t filename[256];
    char* Name;
    struct Node* next;
};

// Queue
typedef struct {
    struct Node* Front; // HEAD POINTER
    struct Node* Back;  // TAIL POINTER
} Queue;

//struct for stack//
typedef struct Stack {
    int top;
    int stack;
    int array[256];
};
struct StackNode
{
    int data;
    struct StackNode* next;
};


// Prototypes
Queue* InitializeQueue(void);
bool IsQueueEmpty(Queue* queue);
struct Node* CreateNewNode(int data, char* name);
void EnQueue(Queue* queue, int elementToInsert, char* name);
bool isEmpty(Stack* stack);
int pop(Stack* stack);
void push(Stack* stack, int data);
// Create a new node
struct Node* CreateNewNode(int data, char* name) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("No Memory");
        exit(EXIT_FAILURE);
    }

    newNode->Name = name;
    newNode->next = NULL;
    return newNode;
}

// Put items on queue
void EnQueue(Queue* queue, int elementToInsert, char* name) {
    if (queue == NULL) {
        queue = InitializeQueue();
    }

    struct Node* toEnqueue = CreateNewNode(elementToInsert, name);
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
        printf("No Memory");
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
        exit(1);
    }

    // Convert the filename to wide-character string
    wchar_t wFilename[256];
    if (MultiByteToWideChar(CP_UTF8, 0, filename, -1, wFilename, 256) == 0) {
        fprintf(stderr, "Failed to convert filename to wide-character string\n");
        free(newNode);
        exit(1);
    }

    if (wcscpy_s(newNode->filename, 256, wFilename) != 0) {
        fprintf(stderr, "Failed to copy wide-character string\n");
        free(newNode);
        exit(1);
    }

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
void play(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("Playing: %ws\n", current->filename);
        if (PlaySoundW(current->filename, NULL, SND_FILENAME) == 0) {
            fprintf(stderr, "Failed to play audio file: %ws\n", current->filename);
            return;
        }
        current = current->next;
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
bool isEmpty(Stack* stack)
{
    return stack->top == NULL;
}
int pop(Stack* stack) {
    if (isEmpty(stack))
    {
        printf("Error: stack underflow exception\n");
        exit(EXIT_FAILURE);
    }
    return stack->array[stack->top];
}
void push(Stack* stack, int data)
{
    struct StackNode* newNode = (struct StackNode*)malloc(sizeof(struct Stack));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->data = stack->top;
    stack->top = newNode;
}


int main(void) {
    bool helper = false;
    struct Node* playlist = NULL;
    int userInput = 0;
    do
    {
        printf("Welcome to the music player 1050\n");
        printf("Pick an option\n");
        printf("1: Create a playlist\n");
        printf("2: Add song to the playlist\n");
        printf("3: Play playlist\n");
        printf("4: Delete song in playlist\n");
        printf("5: Delete playlist\n");
        printf("6: Play playlist on loop\n");

        printf("Enter your choice:\n ");
        scanf_s("%d", &userInput);


        if (userInput == 1)
        {
            Queue* myQueue = InitializeQueue();
        }
        else if (userInput == 2) 
        {

            addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
            //addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\DS Final Project\\randomsound2.wav");

            
        }
        else if (userInput == 3)
        {

            play(playlist);
            printf("Press p if you want the previous song");
            if (userInput == 'p')
            {
                // pop code
            }
        }
        else if (userInput == 4)
        {
            freeList(playlist);
        }
        else if (userInput == 5)
        {
            freeList(playlist);
        }
        else if (userInput == 6)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("Invalid option\n");
            return 1;
        }

    } while (helper == false);

    return 0;
}



