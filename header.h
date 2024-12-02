#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256
#define MAX_CITY_NAME 100

typedef struct node{
    char cityName[MAX_CITY_NAME];
    char spotName[MAX_CITY_NAME];
    float rating;
    char Longitude[20];
    char Latitude[20];
    struct node* next;
}node;

typedef node *SLL;


int length(SLL head);
void toLowerCase(char *str);
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude);
void addNode(SLL *head, SLL newNode);
SLL displayTouristSpots(const char* cityName, const char* spotType, float Rating);
void printTouristSpots(SLL head);
double calculateDistance(float lat1, float lon1, float lat2, float lon2);
SLL getNodeAt(SLL head, int index);
float** graphformation(SLL head);
SLL dijkstra_iterative(SLL head, const char *startSpotName, float **distanceMatrix);
void printShortestPath(SLL shortestPath);