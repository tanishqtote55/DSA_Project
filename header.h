#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

// Maximum Line Length for Input Processing
#define MAX_LINE_LENGTH 256

// Maximum Length for City or Spot Names
#define MAX_CITY_NAME 100

// Definition of a Node in the Singly Linked List
typedef struct node {
    char cityName[MAX_CITY_NAME];  // City name
    char spotName[MAX_CITY_NAME];  // Tourist spot name
    float rating;                  // Rating of the tourist spot
    char Longitude[20];            // Longitude of the spot
    char Latitude[20];             // Latitude of the spot
    struct node* next;             // Pointer to the next node
} node;

// Typedef for Simplicity
typedef node *SLL;

// Function prototypes =>

// Returns the Length of the Singly Linked List
int length(SLL head);

// Converts a String to Lowercase
void toLowerCase(char *str);

// Creates a New Node with the Given Data
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude);

// Adds a New Node to the End of the List
void addNode(SLL *head, SLL newNode);

// Trims Whitespace From the Beginning and End of a String
void trimWhitespace(char *str);

// Displays Tourist Spots Filtered by City, Spot Type, and Rating
SLL filterTouristSpots(SLL head, const char *cityName, const char *spotType, float Rating);