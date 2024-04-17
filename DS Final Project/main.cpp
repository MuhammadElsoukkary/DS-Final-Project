#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <windows.h>  // Include Windows.h with lowercase 'w' for portability
#include <wchar.h>    // Needed for wcscpy_s and wchar_t


const int MAXFILENAMESIZE = 256;
const int MAX = 50;
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
void printList(struct Node* head);

struct Node* convertToCircular(struct Node* head);



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
 
void addFile(struct Node** headRef, struct Node** circularHeadRef, const char* filename) {
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
        *circularHeadRef = newNode; // If the circular list is empty, make the new node the head
        newNode->next = newNode; // Make it circular by pointing to itself
    }
    else {
        // Traverse to the end of the circular list
        struct Node* circularCurrent = *circularHeadRef;
        while (circularCurrent->next != *circularHeadRef) {
            circularCurrent = circularCurrent->next; // Find the last node
        }
        circularCurrent->next = newNode; // Append the new node at the end of the circular list
        newNode->next = *circularHeadRef; // Make the new node point back to the head
    }
}


struct Node* convertToCircular(struct Node* head) {
    // If the list is empty or already circular, return head
    if (head == NULL || head->next == head)
        return head;

    struct Node* current = head;

    // Traverse to the end of the list
    while (current->next != NULL)
        current = current->next;

    // Make the last node point to the head, thus forming a circle
    current->next = head;

    return head;
}

bool playSound(const wchar_t* filename) {
    if (PlaySoundW(filename, NULL, SND_FILENAME) == 0) {
        fprintf(stderr, "Failed to play audio file: %ls\n", filename);
        return false;
    }
    return true;
}



void playNonCircular(struct Node** linkedListRef, Stack* stack) {
    struct Node* current = *linkedListRef;
    while (current != NULL) {
        printf("Playing: %ls\n", current->filename);
        if (!playSound(current->filename)) {
            return;
        }

        if (_kbhit()) {
            char userInput = _getch();
            if (userInput == 'p') {
                if (!isEmpty(stack)) {
                    int previousIndex = pop(stack);
                    struct Node* temp = *linkedListRef;
                    int currentIndex = 0;
                    while (temp != NULL && currentIndex < previousIndex) {
                        current = temp;
                        temp = temp->next;
                        currentIndex++;
                    }
                    if (temp == NULL) {
                        fprintf(stderr, "Error: Unable to find previous song node.\n");
                        return;
                    }
                }
                else {
                    printf("No previous song available.\n");
                }
            }
        }

        current = current->next;
    }
}


void playCircular(struct Node** linkedListRef) {
    struct Node* current = *linkedListRef;
    if (current == NULL) return;

    do {
        printf("Playing: %ls\n", current->filename);
        if (!playSound(current->filename)) {
            return;
        }

        if (_kbhit()) {
            char userInput = _getch();
            if (userInput == 'p') {
                // To move to the previous node, find the node whose `next` points to `current`
                struct Node* prev = current;
                while (prev->next != current) {
                    prev = prev->next;
                }
                current = prev;
            }
        }

        current = current->next;
    } while (current != *linkedListRef);
}





// Regular linked list play function
// Play function for both regular and circular linked lists
// Play function for both regular and circular linked lists
void play(struct Node** linkedListRef, Stack* stack) {
    struct Node* current = *linkedListRef;
    bool continuePlaying = true;
    bool isCircular = false;

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
                // For circular lists, add logic to move to the previous node
                if (isCircular) {
                    // Move to the previous node in the circular list
                    struct Node* prev = current;
                    while (prev->next != current) {
                        prev = prev->next;
                    }
                    current = prev;
                }
                else {
                    // For regular lists, pop from the stack to move to the previous node
                    if (!isEmpty(stack)) {
                        int previousIndex = pop(stack);
                        struct Node* temp = *linkedListRef;
                        int currentIndex = 0;

                        while (temp != NULL && currentIndex < previousIndex) {
                            current = temp;
                            temp = temp->next;
                            currentIndex++;
                        }

                        if (temp == NULL) {
                            fprintf(stderr, "Error: Unable to find previous song node.\n");
                            return;
                        }
                    }
                    else {
                        printf("No previous song available.\n");
                    }
                }
                break;
            case 'L':
            case 'l':
                // Convert to circular linked list
                if (*linkedListRef != NULL && (*linkedListRef)->next != NULL && (*linkedListRef)->next != *linkedListRef) {
                    *linkedListRef = convertToCircular(*linkedListRef);
                    current = *linkedListRef; // Reset current to the head of the circular list
                    isCircular = true;
                }
                break;
                // Case statements for other user inputs can remain here
            }
        }

        if (current->next != NULL) {
            current = current->next;
            if (current == *linkedListRef && isCircular) {
                continuePlaying = false; // If back to the start in a circular list, end the loop
            }
        }
        else {
            continuePlaying = false; // End the loop if there are no more nodes
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



// Call these functions after all addFile calls


int main(void) {
    bool helper = false;
    struct Node* playlist = NULL; // Head for regular linked list
    struct Node* circularPlaylist = NULL; // Head for circular linked list
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
            // No need to explicitly create a playlist, as it will be created when adding songs
            break;
        case 2:
            // Assuming addFile() is defined elsewhere
            addFile(&playlist, &circularPlaylist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
            addFile(&playlist, &circularPlaylist, "C:\\Users\\muham\\source\\repos\\DS-Final-Project\\AudioDB\\Dark.wav");
            addFile(&playlist, &circularPlaylist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\birds.wav");
            break;
        case 3:
            printf("If you want to go back to the previous song click p\n");
            printf("If you start to fall in love with this playlist and want to put it on a loop press L\n");

            play(&playlist, &stack); // Passing pointer to the regular linked list
            break;
        case 4:
            // Logic for deleting a song in the playlist (not implemented)
            break;
        case 5:
            freeList(playlist); // Free memory allocated for the playlist
            playlist = NULL; // Reset playlist pointer
            break;
        case 6:
            // Exit option (not implemented)
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




