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

// Maximum number of reviews the program can handle.
#define MAX_REVIEWS 100

// Filepath for the reviews CSV file.
#define REVIEW_FILE "./csv/spot_reviews.csv"

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

//review node
typedef struct review {
    char cityName[MAX_CITY_NAME];
    char spotName[MAX_CITY_NAME];
    float userRating;
    char reviewText[256];
} Review;

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

// Function to Remove a Spot from the Linked List
void removeSpot(SLL *head, const char *spotName);

// Prints All Tourist Spots in the List
void printTouristSpots(SLL head);

// Calculates the Great-Circle Distance Between Two Geographical Coordinates
double calculateDistance(float lat1, float lon1, float lat2, float lon2);

// Retrieves the Node at a Specific Index in the List
SLL getNodeAt(SLL head, int index);

// Forms a Graph Representation (Distance Matrix) for the Tourist Spots
float** graphformation(SLL head);

// Finds the shortest path using Dijkstra's algorithm (iterative approach)
SLL dijkstra_iterative(SLL head, const char *startSpotName, float **distanceMatrix);

// Prints the Shortest Path from the Starting Spot to All Others
void printShortestPath(SLL shortestPath);

// Function to Get the Index of a Node in the Shortest Path Linked List
int getNodeIndex(SLL touristSpots, SLL node);

// Function to Calculate the Total Distance of the Shortest Path
float calculateTotalDistance(SLL shortestPath, float **distanceMatrix, SLL touristSpots);

// Function to load reviews from a file into a dynamically allocated array
void loadReviews(const char *filename, Review **reviews, int *reviewCount);

// Function to display all reviews for a specific spot.
void displayReviewsForSpot(SLL touristSpots, Review *reviews, int reviewCount, const char *spotName);

// Function to add a new review to the reviews array and save it to the file.
void addReview(SLL touristSpots, const char *filename, Review **reviews, int *reviewCount, const char *cityName, const char *spotName);

// Helper function to display time in HH:MM format
void displayTime(int hour, int minute);

// Function to generate an itinerary for a given number of days.
void generateItinerary(SLL head, int days);


int isSpotFound(SLL touristSpots, const char *startSpotName);