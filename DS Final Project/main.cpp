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
    struct Node* prev;
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
void printCircularList(struct CircularLinkedList* head);
void printList(struct Node* head);
void playCircular(struct CircularLinkedList* circularList, Stack* stack);


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
            current = current->next; // Traverse to the end of the list
        }
        current->next = newNode; // Append the new node at the end of the list
    }

    // Create or update the circular linked list
    if (*circularHeadRef == NULL) {
        *circularHeadRef = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
        if (*circularHeadRef == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        (*circularHeadRef)->current = newNode;
        (*circularHeadRef)->next = *circularHeadRef; // Make it circular
    }
    else {
        struct CircularLinkedList* circularCurrent = *circularHeadRef;
        while (circularCurrent->next != *circularHeadRef) {
            circularCurrent = circularCurrent->next; // Find the last node
        }
        struct CircularLinkedList* newCircularNode = (struct CircularLinkedList*)malloc(sizeof(struct CircularLinkedList));
        if (newCircularNode == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newCircularNode->current = newNode;
        newCircularNode->next = *circularHeadRef; // Maintain circular nature
        circularCurrent->next = newCircularNode; // Insert it into the list
    }
}

// Regular linked list play function
void play(struct Node* linkedList, Stack* stack) {
    struct Node* current = linkedList;
    bool continuePlaying = true;

    while (continuePlaying) {
        printf("Playing: %ls\n", current->filename);
        if (PlaySoundW(current->filename, NULL, SND_FILENAME) == 0) {
            fprintf(stderr, "Failed to play audio file: %ls\n", current->filename);
            return;
        }

        if (_kbhit()) {
            char userInput = _getch();

            switch (userInput) {
            case 'p':
                // Previous song logic (assuming stack contains indices)
                if (!isEmpty(stack)) {
                    int previousIndex = pop(stack);
                    struct Node* prev = NULL;
                    struct Node* temp = linkedList;
                    int currentIndex = 0;

                    while (temp != NULL && currentIndex < previousIndex) {
                        prev = temp;
                        temp = temp->next;
                        currentIndex++;
                    }

                    if (temp == NULL) {
                        fprintf(stderr, "Error: Unable to find previous song node.\n");
                        return;
                    }

                    current = prev;
                }
                else {
                    printf("No previous song available.\n");
                }
                break;
                // Case statements for other user inputs can remain here
            }
        }

        if (current->next != NULL) {
            current = current->next;
        }
        else {
            continuePlaying = false;
        }
    }
}


// Circular linked list play function
void playCircular(struct CircularLinkedList* circularList, Stack* stack) {
    struct CircularLinkedList* circularCurrent = circularList;
    bool continuePlaying = true;
    bool isLooping = false;

    while (continuePlaying) {
        printf("Playing: %ls\n", circularCurrent->current->filename);
        if (PlaySoundW(circularCurrent->current->filename, NULL, SND_FILENAME) == 0) {
            fprintf(stderr, "Failed to play audio file: %ls\n", circularCurrent->current->filename);
            return;
        }

        if (_kbhit()) {
            char userInput = _getch();

            switch (userInput) {
            case 'p':
                // Previous song logic (assuming stack contains indices)
                // This logic may need modification for circular lists
                // You might need to implement a stack specifically for circular lists
                break;

                // Case statements for other user inputs can remain here
            }
        }

        if (isLooping) {
            circularCurrent = circularCurrent->next;
        }
        else {
            if (circularCurrent->next != circularList) {
                circularCurrent = circularCurrent->next;
            }
            else {
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
        printf("Circular File: %ls\n", current->filename);
        current = current->next;
    } while (current != head);
}

// Call these functions after all addFile calls


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
            addFile(&playlist, &roundPlaylist,"C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
            addFile(&playlist, &roundPlaylist,"C:\\Users\\muham\\source\\repos\\DS-Final-Project\\AudioDB\\Dark.wav");
            addFile(&playlist, &roundPlaylist,"C:\\Users\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\birds.wav");
            break;
        case 3:
            printf("If you want to go back to the previous song click p\n");
            printf("If you start to fall in love with this playlist and want to put it on a loop press L\n");
            
            play(playlist,&stack); // Assuming initially not using circular linked list
            break;
        case 4:
            // code to play playlist on a loop 
        case 5:
            freeList(playlist); // Assuming freeList() is defined elsewhere
            break;
        case 6:
            freeList(playlist);
            break;
        case 7:
            exit(EXIT_SUCCESS);
        default:
            printf("Invalid option\n");
            return 1;
        }

    } while (helper == false);

    return 0;
}



