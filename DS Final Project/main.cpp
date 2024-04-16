#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

// Define a structure for the linked list node
struct Node {
    wchar_t filename[256];
    struct Node* next;
};

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



int main(void) {
    struct Node* playlist = NULL;

    // Add files to the playlist
    addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\AudioDB\\randomsound.wav");
    addFile(&playlist, "C:\\Users\\muham\\OneDrive\\Desktop\\DS Final Project\\DS Final Project\\randomsound2.wav");

    // Play the playlist
    play(playlist);

    // Free memory allocated for the linked list
    freeList(playlist);

    return 0;
}


