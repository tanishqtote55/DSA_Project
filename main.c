#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
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
int length(SLL head){
    int count = 0;
    node *temp = head;
    while(temp){
        count++;
        temp = temp -> next;
    }
    return count;
}
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);  
    }
}
// Function to create a new node
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude) {
    SLL newNode = (SLL)malloc(sizeof(node));
    strcpy(newNode->cityName, city);
    strcpy(newNode->spotName, spot);
    newNode->rating = rating;
    strcpy(newNode->Longitude, longitude);
    strcpy(newNode->Latitude, latitude);
    newNode->next = NULL;
    return newNode;
}
// Function to add a new node to the linked list
void addNode(SLL *head, SLL newNode) {
    if (*head == NULL) {
        *head = newNode;
    } else {
        SLL temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}
SLL displayTouristSpots(const char* cityName, const char* spotType, float Rating){
    FILE* file = fopen("./csv/tourist_spots.csv", "r");
    if(file == NULL){
        printf("Error Opening File\n");
        return NULL;
    }
    SLL head = NULL;
    char line[MAX_LINE_LENGTH];
    char *token;
    //Skip the header line
    fgets(line, sizeof(line), file);
    while(fgets(line, sizeof(line), file)){
        char city[MAX_CITY_NAME];
        char spotName[100], type[50], longitude[20], latitude[20];
        float rating;

        token = strtok(line, ","); 
        strcpy(city, token);

        token = strtok(NULL, ",");
        strcpy(spotName, token);

        token = strtok(NULL, ",");
        rating = atof(token);

        token = strtok(NULL, ",");
        strcpy(type, token); 

        token = strtok(NULL, ",");
        strcpy(longitude, token);

        token = strtok(NULL, ",");
        strcpy(latitude, token);

        latitude[strcspn(latitude, "\n")] = '\0';

        char cityLower[MAX_CITY_NAME];
        strcpy(cityLower, city);
        toLowerCase(cityLower);
        char cityNameLower[MAX_CITY_NAME];
        strcpy(cityNameLower, cityName);
        toLowerCase(cityNameLower);

        char typeLower[MAX_CITY_NAME];
        strcpy(typeLower, type);
        toLowerCase(typeLower);
        char spotTypeLower[MAX_CITY_NAME];
        strcpy(spotTypeLower, spotType);
        toLowerCase(spotTypeLower);
        

        if((strcmp(cityLower, cityNameLower) == 0) && (strcmp(typeLower, spotTypeLower) == 0) && Rating <= rating){
            SLL newNode = createNode(city, spotName, rating, longitude, latitude);
            addNode(&head, newNode);
        }
    }
    fclose(file);
    return head;
}
void printTouristSpots(SLL head) {
    SLL temp = head;
    while (temp != NULL) {
        printf("%s, %s, %.2f\n", 
                temp->cityName, temp->spotName, temp->rating);
        temp = temp->next;
    }
}

// Function to calculate the Haversine distance between two points on the Earth's surface
double calculateDistance(float lat1, float lon1, float lat2, float lon2) {
    lat1 = lat1 * 3.141592653589793 / 180.0;
    lon1 = lon1 * 3.141592653589793 / 180.0;
    lat2 = lat2 * 3.141592653589793 / 180.0;
    lon2 = lon2 * 3.141592653589793 / 180.0;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1) * cos(lat2) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return 6371.0 * c; // Earth radius in kilometers
}

// Function to access the node at a specific index in the list
SLL getNodeAt(SLL head, int index){
    SLL temp = head;
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp -> next;
    }
    return temp;
}

// Function to create the distance matrix (graph formation)
float** graphformation(SLL head) {
    int len = length(head);
    
    SLL spots[len];  // Allocate space for storing node pointers

    // Store all the nodes in the array for easier access
    SLL temp = head;
    for (int i = 0; i < len; i++) {
        spots[i] = temp;
        temp = temp->next;
    }

    // Initialize the distance matrix
    float **arr = malloc(len * sizeof(float *));
    for (int i = 0; i < len; i++) {
        arr[i] = malloc(len * sizeof(float));
    }

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            arr[i][j] = (i == j) ? 0.0 : -1.0;  // Initialize diagonals to 0 and others to -1 (uninitialized)
        }
    }

    // Compute distances only for i != j
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            SLL spot1 = spots[i];
            SLL spot2 = spots[j];
            
            float dist = calculateDistance(atof(spot1->Latitude), atof(spot1->Longitude), atof(spot2->Latitude), atof(spot2->Longitude));
            arr[i][j] = dist;
            arr[j][i] = dist;  // Symmetric matrix
        }
    }

    // Print the distance matrix
    printf("Distance Matrix:\n");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            if (arr[i][j] >= 0) {
                printf("%.2f ", arr[i][j]);
            } else {
                printf("N/A ");  // For uninitialized or invalid distances
            }
        }
        printf("\n");
    }

    return arr;  // Return the distance matrix
}

void dijkstra_iterative_recursive(SLL head, const char *startSpotName, float **distanceMatrix) {
    int len = length(head);

    if (len == 0) {
        printf("No spots to visit.\n");
        return;
    }

    // Store all the spots in an array for easier access
    SLL spots[len];
    SLL temp = head;
    for (int i = 0; i < len; i++) {
        spots[i] = temp;
        temp = temp->next;
    }

    // Convert the starting spot name to lowercase for case-insensitive comparison
    char startSpotNameLower[MAX_CITY_NAME];
    strcpy(startSpotNameLower, startSpotName);
    toLowerCase(startSpotNameLower);

    // Find the starting index based on the spot name
    int startIndex = -1;
    for (int i = 0; i < len; i++) {
        char spotNameLower[MAX_CITY_NAME];
        strcpy(spotNameLower, spots[i]->spotName);
        toLowerCase(spotNameLower);

        if (strcmp(spotNameLower, startSpotNameLower) == 0) {
            startIndex = i;
            break;
        }
    }

    if (startIndex == -1) {
        printf("Starting spot \"%s\" not found in the list.\n", startSpotName);
        return;
    }

    // Initialize the visited array
    int visited[len];
    for (int i = 0; i < len; i++) {
        visited[i] = 0;  // Initially, all spots are unvisited
    }

    // Helper function to recursively visit all nodes
    void visitNodes(int currentSpot) {
        visited[currentSpot] = 1;  // Mark the current spot as visited

        printf("Visiting: %s\n", spots[currentSpot]->spotName);

        // Find the nearest unvisited spot from the current spot
        float minDist = INT_MAX;
        int nextSpot = -1;

        for (int i = 0; i < len; i++) {
            if (!visited[i] && distanceMatrix[currentSpot][i] != -1 && distanceMatrix[currentSpot][i] < minDist) {
                minDist = distanceMatrix[currentSpot][i];
                nextSpot = i;
            }
        }

        if (nextSpot == -1) {
            // No more unvisited spots reachable
            printf("All spots visited or no more reachable spots.\n");
            return;
        }

        // Print the path and distance from the current spot to the next spot
        printf("Next stop: %s (Distance: %.2f)\n", spots[nextSpot]->spotName, minDist);
        printf("Path: %s -> %s\n", spots[currentSpot]->spotName, spots[nextSpot]->spotName);

        // Call Dijkstra again starting from the next spot recursively
        visitNodes(nextSpot);
    }

    // Start visiting nodes from the user-defined starting spot
    visitNodes(startIndex);

    // After all spots are visited, find and display the farthest spot
    float maxDist = -1.0;
    int farthestSpot = -1;
    for (int i = 0; i < len; i++) {
        if (distanceMatrix[startIndex][i] > maxDist) {
            maxDist = distanceMatrix[startIndex][i];
            farthestSpot = i;
        }
    }

    if (farthestSpot != -1) {
        printf("\nFarthest spot from %s is %s at a distance of %.2f\n", spots[startIndex]->spotName, spots[farthestSpot]->spotName, maxDist);
    }
}

int main() {
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%[^\n]s", cityName);
    getchar();  // Consume newline

    char spotType[MAX_CITY_NAME];
    printf("Enter the spot type (e.g., Historical Monument, Museum, Park, Religious Site, Shopping, etc.): ");
    scanf("%[^\n]s", spotType);
    getchar();  // Consume newline

    float rating;
    printf("Enter the rating above which you want spots: ");
    scanf("%f", &rating);
    getchar();  // Consume the newline character left by scanf

    // Display tourist spots based on the user inputs (city, spot type, rating)
    SLL touristSpots = displayTouristSpots(cityName, spotType, rating);
    if (touristSpots != NULL) {
        printf("Tourist Spots in %s:\n", cityName);
        printTouristSpots(touristSpots);  // Print the list of tourist spots

        printf("\nCreating Distance Matrix...\n");
        float **distanceMatrix = graphformation(touristSpots);  // Generate the distance matrix from the list

        // Get the starting spot name from the user
        char startSpotName[MAX_CITY_NAME];
        printf("Enter the starting spot name: ");
        scanf("%[^\n]s", startSpotName);
        getchar();  // Consume newline

        // Call the recursive Dijkstra function to visit all tourist spots
        dijkstra_iterative_recursive(touristSpots, startSpotName, distanceMatrix);

    } else {
        printf("No tourist spots found for the city: %s\n", cityName);
    }

    return 0;
}