#include "header.h"

// Function to Find the Length of the Singly Linked List
int length(SLL head){
    int count = 0;
    node *temp = head;

    // Traverse the list to count the number of nodes
    while(temp){
        count++;
        temp = temp -> next;
    }
    return count;
}

// Function to Convert a Given String to Lowercase
void toLowerCase(char *str){
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);  
    }
}

// Function to create a new node
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude){
    // Allocate memory for a new node
    SLL newNode = (SLL)malloc(sizeof(node));
    if (!newNode) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    // Initialize the node's data
    strcpy(newNode->cityName, city);
    strcpy(newNode->spotName, spot);
    newNode->rating = rating;
    strcpy(newNode->Longitude, longitude);
    strcpy(newNode->Latitude, latitude);
    newNode->next = NULL;

    return newNode;
}

// Function to Add a New Node to the Linked List
void addNode(SLL *head, SLL newNode){
    // If the list is empty, make the new node the head
    if (*head == NULL) {
        *head = newNode;
    } else {
        SLL temp = *head;

        // Traverse to the end of the list
        while (temp->next != NULL) {
            temp = temp->next;
        }

        // Add the new node to the end of the list
        temp->next = newNode;
    }
}

// Function to Remove Leading and Trailing Whitespace From a String
void trimWhitespace(char* str) {
    char* start = str; // Pointer to the beginning of the string
    char* end;

    // Move the start pointer to the first non-space character
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // If the string is entirely spaces, set it to an empty string
    if (*start == 0) {
        str[0] = '\0';
        return;
    }

    // Find the end of the string
    end = start + strlen(start) - 1;

    // Move the end pointer back to the last non-space character
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    // Copy the trimmed string back to the original pointer
    memmove(str, start, end - start + 2); // +2 to include null terminator
}
