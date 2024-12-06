#include "header.h"

// Function to Find the Length of the Singly Linked List
int length(SLL head){
    int count = 0;
    node *temp = head;
    while(temp){
        count++;
        temp = temp -> next;
    }
    return count;
}