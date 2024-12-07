#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

#define MAX_LINE_LENGTH 256
#define MAX_CITY_NAME 100

#define MAX_REVIEWS 100

#define REVIEW_FILE "./csv/spot_reviews.csv"

typedef struct node{
    char cityName[MAX_CITY_NAME];
    char spotName[MAX_CITY_NAME];
    float rating;
    char Longitude[20];
    char Latitude[20];
    struct node* next;
}node;

typedef node *SLL;

//review node
typedef struct review {
    char cityName[MAX_CITY_NAME];
    char spotName[MAX_CITY_NAME];
    float userRating;
    char reviewText[256];
} Review;


int length(SLL head);
void toLowerCase(char *str);
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude);
void addNode(SLL *head, SLL newNode);
int matchesType(const char *spotType, char spotTypes[][MAX_CITY_NAME], int numTypes);
void trimWhitespace(char *str);
SLL displayTouristSpots(SLL head, const char *cityName, const char *spotType, float Rating);
void removeSpot(SLL *head, const char *spotName);
void printTouristSpots(SLL head);
double calculateDistance(float lat1, float lon1, float lat2, float lon2);
SLL getNodeAt(SLL head, int index);
float** graphformation(SLL head);
SLL dijkstra_iterative(SLL head, const char *startSpotName, float **distanceMatrix);
void printShortestPath(SLL shortestPath);
int getNodeIndex(SLL touristSpots, SLL node);
int isSpotFound(SLL touristSpots, const char *startSpotName);
float calculateTotalDistance(SLL shortestPath, float **distanceMatrix, SLL touristSpots);
void displayTime(int hour, int minute);
void generateItinerary(SLL head, int days);

//review functions

void loadReviews(const char *filename, Review **reviews, int *reviewCount);

void displayReviewsForSpot(Review *reviews, int reviewCount, const char *spotName);

void addReview(const char *filename, Review **reviews, int *reviewCount, const char *cityName, const char *spotName);