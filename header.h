#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

// Maximum line length for input processing
#define MAX_LINE_LENGTH 256

// Maximum length for city or spot names
#define MAX_CITY_NAME 100

// Definition of a node in the singly linked list
typedef struct node {
    char cityName[MAX_CITY_NAME];  // City name
    char spotName[MAX_CITY_NAME];  // Tourist spot name
    float rating;                  // Rating of the tourist spot
    char Longitude[20];            // Longitude of the spot
    char Latitude[20];             // Latitude of the spot
    struct node* next;             // Pointer to the next node
} node;

// Typedef for simplicity
typedef node *SLL;

// Function prototypes =>

// Returns the length of the singly linked list
int length(SLL head);

// Converts a string to lowercase
void toLowerCase(char *str);

// Creates a new node with the given data
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude);

// Adds a new node to the end of the list
void addNode(SLL *head, SLL newNode);

// Trims whitespace from the beginning and end of a string
void trimWhitespace(char *str);