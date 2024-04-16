#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

//Queue
typedef struct {
    Node* Front; // HEAD POINTER
    Node* Back; // TAIL POINTER
} Queue;


// Define a structure for the linked list node
struct Node {
    wchar_t filename[256];
    char* Name;
    struct Node* next;
};


struct Song {
    int songId;
    struct Song* NextSong;
};


// Prototypes //
Queue* InititalizeQueue(void);
bool IsQueueEmpty(Queue* queue);
Node* CreateNewNode(int data, char* name);
void EnQueue(Queue* queue, int elementToInsert, char* name);


// Need to change parameters accordingly 
Node* CreateNewNode(int data, char* name) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("No Memory");
        exit(EXIT_FAILURE);
    }

    newNode->Name = name;
    newNode->next = NULL;
    return newNode;
}

// Put items on queue, NEED TO CHANGE PARAMETERSE QCCORDINGLY 
void EnQueue(Queue* queue, int elementToInsert, char* name) {
    if (queue == NULL) {
        queue = InititalizeQueue();
    }

    Node* toEnqueue = CreateNewNode(elementToInsert, name);
    if (IsQueueEmpty(queue)) {
        queue->Front = toEnqueue;
        queue->Back = toEnqueue;
    }
    else {
        queue->Back->NextNode = toEnqueue;
        queue->Back = toEnqueue;
    }

}


// Initailize queue 
Queue* InititalizeQueue(void) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("No Memory");
        exit(EXIT_FAILURE);
    }

    queue->Front = NULL;
    queue->Back = NULL;
    return queue;
}

// CHeck if the queue is empty 
bool IsQueueEmpty(Queue* queue) {
    return queue->Front == NULL;
}

// Function to add a file to the playlist
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








int main(void) 
{
    bool helper = false;
    struct Node* playlist = NULL;
    int userInput = 0;

    printf("Welcome to the music player 1050\n");
    printf("Pick an option\n");
    printf("1: Create a playlist\n");
    printf("2: Add song to the playlist\n");
    printf("3: Run through playlist\n");
    printf("4: Delete song in playlist\n");
    printf("5: Delete playlist\n");

    printf("Enter your choice: ");
    scanf("%d", &userInput);


    if (userInput == 1) 
    {
        
    }
    else if (userInput == 2) 
    {
        do
        {
            printf("Go to previous song");
        }while (helper == false);
        addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
        addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\DS Final Project\\randomsound2.wav");
    }

    else if (userInput == 3) 
    {
        play(playlist);
        do
        {
            printf("Go to previous song");
        } while (helper == false);
    }
    else if (userInput == 4) 
    {
        // Call function to delete song in playlist
    }
    else if (userInput == 5) 
    {
        freeList(playlist);

    }
   
    else
    {
        printf("Invalid option\n");
        return 1;
    }

    // Add files to the playlist
    addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
    addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\DS Final Project\\randomsound2.wav");

    // Play the playlist
    play(playlist);

    // Free memory allocated for the linked list
    freeList(playlist);

    return 0;
}


