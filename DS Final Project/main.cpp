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

typedef struct MusicPlayerArtistSong {
    int songId;
    char* artistName[50];
    struct MusicPlayerArtistSong* NextArtistSongPair;
} MusicPlayerArtistSong;

typedef struct MusicPlayer {
    MusicPlayerArtistSong* Table[10];
} SongPlayer;

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
int GenerateHash(int studentid);
MusicPlayerArtistSong* InitializeKeyValuePair(int songid, char* artist[50]);
MusicPlayer* InitializeHashTable(void);
void InsertWithOverWrite(MusicPlayer* hashTable, int songid, char* artist[50]);
char* SearchWithOverWriteInCaseOfCollisionTechnique(MusicPlayer* hashTable, int songid);

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

//generates the hash value
int GenerateHash(int studentid) {
    int hash = 0;

    int asciiValue = studentid;
    hash = (hash + asciiValue) % 10;

    return hash;
}

//initializes the pair of the ID and Artist
MusicPlayerArtistSong* InitializeKeyValuePair(int songid, char* artist[50]) {
    MusicPlayerArtistSong* kvp = (MusicPlayerArtistSong*)malloc(sizeof(MusicPlayerArtistSong));
    if (kvp == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }

    kvp->songId = songid;
    kvp->artistName[50] = artist[50];
    kvp->NextArtistSongPair = NULL;
    return kvp;
}
//initializes the hashtable
MusicPlayer* InitializeHashTable(void) {
    MusicPlayer* hashTable = (MusicPlayer*)malloc(sizeof(MusicPlayer));
    if (hashTable == NULL) {
        printf("EOM");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; i++) {
        hashTable->Table[i] = NULL;
    }

    return hashTable;
}

void InsertWithOverWrite(MusicPlayer* hashTable, int songid, char* artist[50]) {
    MusicPlayerArtistSong* kvp = InitializeKeyValuePair(songid, artist);
    int hash = GenerateHash(songid);

    if (hashTable->Table[hash] == NULL) { //no collisions since the bucket is empty
        hashTable->Table[hash] = kvp;
        return;
    }

    //This means there is a collision, add the kvp to the head / tail of your linked list.
    MusicPlayerArtistSong* current = hashTable->Table[hash];
    free(current);
    hashTable->Table[hash] = kvp;
}

char* SearchWithOverWriteInCaseOfCollisionTechnique(MusicPlayer* hashTable, int songid) {
    int hash = GenerateHash(songid);

    if (hashTable->Table[hash] == NULL) {
        printf("ERROR: cannot find the Key in the table!");
        return 0;
    }

    MusicPlayerArtistSong* current = hashTable->Table[hash];
    if (current->songId == songid) {
        return current->artistName[50];
    }
    printf("ERROR: cannot find the Key in the table!");
    return 0;
}

void addFile(struct Node** headRef, struct CircularLinkedList** circularHeadRef, const char* filename) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Create a new node for the regular linked list
    // Convert the filename to wide-character string
    wchar_t wFilename[MAXFILENAMESIZE];
    if (MultiByteToWideChar(CP_UTF8, 0, filename, -1, wFilename, MAXFILENAMESIZE) == 0) {
        fprintf(stderr, "Failed to convert filename to wide-character string\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }

    wcscpy_s(newNode->filename, MAXFILENAMESIZE, wFilename);
    newNode->next = NULL;

    // Add the new node to the regular linked list
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

    // Create a new node for the circular linked list
    struct CircularLinkedList* circularNewNode = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
    if (circularNewNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the circular linked list node
    wcscpy_s(circularNewNode->filename, MAXFILENAMESIZE, wFilename);
    circularNewNode->next = *circularHeadRef; // Point to the current head of the circular linked list
    *circularHeadRef = circularNewNode; // Update the head of the circular linked list
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
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\source\\repos\\DS - Final - Project\\AudioDB\\Dark.wav");
            break;
        case 3:
            printf("If you want to go back to the previous song click p\n");
            printf("If you start to fall in love with this playlist and want to put it on a loop press L\n");
            play(playlist, roundPlaylist, &stack); // Assuming initially not using circular linked list
            break;
        case 4:
            freeList(playlist); // Assuming freeList() is defined elsewhere
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



