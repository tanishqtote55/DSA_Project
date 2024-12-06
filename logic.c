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
