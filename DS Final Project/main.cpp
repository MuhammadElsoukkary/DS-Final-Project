
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

typedef struct MusicPlayerArtistSong {
    int songId;
    char* artistName[50];
    struct MusicPlayerArtistSong* NextArtistSongPair;
} MusicPlayerArtistSong;

typedef struct MusicPlayer {
    MusicPlayerArtistSong* Table[10];
} SongPlayer;


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
int GenerateHash(int studentid);
MusicPlayerArtistSong* InitializeKeyValuePair(int songid, char* artist[50]);
MusicPlayer* InitializeHashTable(void);
void InsertWithOverWrite(MusicPlayer* hashTable, int songid, char* artist[50]);
char* SearchWithOverWriteInCaseOfCollisionTechnique(MusicPlayer* hashTable, int songid);

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
struct Node* makeCircular(struct Node** headRef) {
    if (*headRef == NULL) return NULL; // If the list is empty, return NULL

    struct Node* last = *headRef;

    // Traverse to the end of the list
    while (last->next != NULL) {
        last = last->next;
    }

    // Link the last node back to the head to make the list circular
    last->next = *headRef;

    // Return the head of the list
    return *headRef;
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

void playCircular(struct Node* head, Stack* stack) {
    if (head == NULL) {
        printf("The playlist is empty.\n");
        return;
    }

    struct Node* current = head;
    bool continuePlaying = true;

    // Ensure we start the playback loop
    while (continuePlaying) {
        printf("Playing: %ls\n", current->filename);
        if (PlaySoundW(current->filename, NULL, SND_FILENAME | SND_ASYNC) == 0) {
            fprintf(stderr, "Failed to play audio file: %ls\n", current->filename);
            continuePlaying = false; // Stop playing if an error occurs
        }

        push(stack, current);  // Save current node for possible "previous" action

        int loopCheck = 0; // A counter to prevent infinite loop on user input checks
        while (_kbhit() && loopCheck < 5) {
            char userInput = _getch();
            switch (userInput) {
            case 'p':  // If 'p' is pressed, go to the previous song
                if (!isEmpty(stack)) {
                    pop(stack); // Remove current song from stack first
                    if (!isEmpty(stack)) {
                        current = pop(stack);  // Move to the previous song
                    }
                    else {
                        printf("At the start of the playlist.\n");
                        push(stack, current);  // Re-push the current song since there's no previous
                    }
                }
                else {
                    printf("No previous song available.\n");
                }
                break;
            case 'q':  // Let's add an option to quit playback
                continuePlaying = false;
                break;
            default: // If other keys are pressed, do nothing
                break;
            }
            loopCheck++;
        }

        if (continuePlaying) {
            current = current->next;  // Move to the next song in the circular list
            Sleep(1000);  // Small delay to allow for keypresses and prevent CPU spike
        }
    }
}



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
    Stack stack{};
    stack.top = -1; // Initialize stack
    int userInput = 0;
    char option = 'A';

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
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\source\\repos\\DS-Final-Project\\AudioDB\\Dark.wav");
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\source\\repos\\DS-Final-Project\\AudioDB\\randomsound.wav");
            addFile(&playlist, &roundPlaylist, "C:\\Users\\muham\\source\\repos\\DS-Final-Project\\AudioDB\\large-underwater-explosion-190270.wav");

            printf("Songs added to the playlist.\n");
            break;
        case 3:
            if (playlist == NULL) {
                printf("No songs in playlist. Add some songs first.\n");
            }
            else {
                printf("Playing playlist...\n");
                printf("You want your songs to loop press L\n");
                printf("You want your songs to not loop press S\n");
                scanf_s("%c", &option);
                if (option == 'L')
                {

                    playCircular(makeCircular(&playlist), &stack);
                }
                else if (option == 'S')
                {
                    play(playlist, &stack);
                }



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

