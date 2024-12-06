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