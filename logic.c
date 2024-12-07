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

// Function to Filter Tourist Spots Based on City, Spot Type, and Rating
SLL filterTouristSpots(SLL head, const char *cityName, const char *spotType, float Rating) {
    // Open the CSV file for reading
    FILE *file = fopen("./csv/tourist_spots.csv", "r");
    if (file == NULL) {
        printf("Error Opening File\n");
        return head;  // Return current list without changes
    }

    char line[MAX_LINE_LENGTH];
    char *token;

    // Convert city and spot type to lowercase for case-insensitive comparison
    char spotTypeLower[MAX_CITY_NAME], cityNameLower[MAX_CITY_NAME];

    strcpy(spotTypeLower, spotType);
    toLowerCase(spotTypeLower);

    strcpy(cityNameLower, cityName);
    toLowerCase(cityNameLower);

    // Skip the header line of the CSV
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Parse the CSV line into tokens
        char city[MAX_CITY_NAME], spotName[100], type[50], longitude[20], latitude[20];
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

        latitude[strcspn(latitude, "\n")] = '\0'; // Remove newline character

        // Convert city and type to lowercase for comparison
        char cityLower[MAX_CITY_NAME], typeLower[MAX_CITY_NAME];
        strcpy(cityLower, city);
        toLowerCase(cityLower);

        strcpy(typeLower, type);
        toLowerCase(typeLower);

        // Check if the city, type, and rating match the filter criteria
        if ((strcmp(cityLower, cityNameLower) == 0) &&
            (strcmp(typeLower, spotTypeLower) == 0) &&
            (Rating <= rating)) {

            // Create a new node for the matching tourist spot and add it to the list
            SLL newNode = createNode(city, spotName, rating, longitude, latitude);
            addNode(&head, newNode);
        }
    }

    fclose(file); // Close the CSV file
    return head;  // Return the updated list
}

// Function to Remove a Spot from the Linked List
void removeSpot(SLL *head, const char *spotName) {
    if (*head == NULL) {
        printf("The list is empty. Nothing to remove.\n");
        return;
    }

    SLL current = *head;
    SLL previous = NULL;
    char spotNameLower[MAX_CITY_NAME]; // Assuming MAX_SPOT_NAME_LENGTH is defined

    // Convert the spotName to lowercase for case-insensitive comparison
    strncpy(spotNameLower, spotName, MAX_CITY_NAME);
    toLowerCase(spotNameLower);

    // Traverse the list to find the node to remove
    while (current != NULL) {
        char currentSpotNameLower[MAX_CITY_NAME];
        
        // Convert the current node's spotName to lowercase for comparison
        strncpy(currentSpotNameLower, current->spotName, MAX_CITY_NAME);
        toLowerCase(currentSpotNameLower);

        if (strcmp(currentSpotNameLower, spotNameLower) == 0) {
            // Node found; remove it
            if (previous == NULL) {
                // Removing the head node
                *head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Tourist spot \"%s\" has been removed from the list.\n", spotName);
            return;
        }
        previous = current;
        current = current->next;
    }

    printf("Spot \"%s\" not found in the list.\n", spotName);
}

// Function to Print All Tourist Spots in the Linked List
void printTouristSpots(SLL head) {
    SLL temp = head;

    // Traverse and print each node in the list
    while (temp != NULL) {
        printf("%s, %s, %.2f\n", 
                temp->cityName, temp->spotName, temp->rating);
        temp = temp->next;
    }
}

// Function to Calculate the Haversine Distance Between Two Points on the Earth's Surface
double calculateDistance(float lat1, float lon1, float lat2, float lon2){
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

// Function to Access the Node at a Specific Index in the List
SLL getNodeAt(SLL head, int index){
    SLL temp = head;

    // Traverse the list until the specified index
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp -> next;
    }
    
    return temp;
}

// Function to create the distance matrix (graph formation)
float** graphformation(SLL head){
    // Get the number of tourist spots
    int len = length(head);
    
    // Allocate space for storing node pointers
    SLL spots[len];  

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


// Dijkstra's Algorithm Implementation ---
// - This function implements Dijkstra's shortest path algorithm for a graph represented by a distance matrix.
// - The graph nodes correspond to the tourist spots stored in a linked list. It identifies the shortest path
//   from a starting spot to all other spots and returns the linked list of the shortest path.
// - The function uses the distance matrix where `distanceMatrix[i][j]` holds the distance from spot `i` to spot `j`.
// - It operates iteratively, visiting the closest unvisited spot and updating the shortest known path.
// - After processing all reachable spots, it outputs the farthest spot from the starting point based on the distances.

// Parameters:
// - head: Pointer to the head of the linked list of tourist spots. Each node contains a spot's name, city, rating, etc.
// - startSpotName: Name of the starting spot for the shortest path computation (case-insensitive).
// - distanceMatrix: A 2D array representing distances between all pairs of tourist spots. A value of `-1` indicates no direct path between two spots.

// Returns:
// - A linked list containing the nodes (tourist spots) in the order of the shortest path found from the starting spot.
// - NULL if the starting spot is not found or the graph is empty.

SLL dijkstra_iterative(SLL head, const char *startSpotName, float **distanceMatrix){
    // Step 1: Get the number of tourist spots
    int len = length(head);

    // If the list is empty, there's no path to compute
    if (len == 0) {
        printf("No spots to visit.\n");
        return NULL;
    }

    // Step 2: Store all spots in an array for easier access later
    // Create an array to store pointers to the spots in the linked list
    SLL spots[len];
    SLL temp = head;
    for (int i = 0; i < len; i++) {
        spots[i] = temp;
        temp = temp->next;
    }

    // Step 3: Convert the starting spot name to lowercase for case-insensitive comparison
    char startSpotNameLower[MAX_CITY_NAME];
    strcpy(startSpotNameLower, startSpotName);
    toLowerCase(startSpotNameLower);

    // Step 4: Find the starting spot's index in the array of spots
    int startIndex = -1;
    for (int i = 0; i < len; i++) {
        char spotNameLower[MAX_CITY_NAME];
        strcpy(spotNameLower, spots[i]->spotName);
        toLowerCase(spotNameLower);

        // If the names match, we've found the starting spot
        if (strcmp(spotNameLower, startSpotNameLower) == 0) {
            startIndex = i;
            break;
        }
    }

    // If the starting spot is not found, return NULL
    if (startIndex == -1) {
        printf("Starting spot \"%s\" not found in the list.\n", startSpotName);
        return NULL;
    }

    // Step 5: Initialize the visited array to keep track of visited spots
    int visited[len];
    for (int i = 0; i < len; i++) {
        visited[i] = 0;  // Initially, all spots are unvisited
    }

    // Step 6: Create a linked list to store the shortest path
    SLL shortestPath = NULL;

    // Step 7: Start Dijkstra's algorithm from the starting spot
    int currentSpot = startIndex;
    visited[currentSpot] = 1;  // Mark the starting spot as visited
    printf("Starting at: %s\n", spots[currentSpot]->spotName);

    // Add the starting spot to the shortest path linked list
    SLL newNode = createNode(spots[currentSpot]->cityName, spots[currentSpot]->spotName, spots[currentSpot]->rating, spots[currentSpot]->Longitude, spots[currentSpot]->Latitude);
    addNode(&shortestPath, newNode);

    // Step 8: Visit all spots one by one, based on the shortest path
    for (int count = 0; count < len - 1; count++) {
        // Find the nearest unvisited spot
        float minDist = INT_MAX;
        int nextSpot = -1;

        // Loop through all spots to find the unvisited one with the minimum distance
        for (int i = 0; i < len; i++) {
            if (!visited[i] && distanceMatrix[currentSpot][i] != -1 && distanceMatrix[currentSpot][i] < minDist) {
                minDist = distanceMatrix[currentSpot][i];
                nextSpot = i;
            }
        }

        // If no unvisited spot is reachable, exit the loop
        if (nextSpot == -1) {
            // No more unvisited spots reachable
            break;
        }

        // Step 9: Mark the next spot as visited and move to that spot
        visited[nextSpot] = 1;
        printf("Next stop: %s (Distance: %.2f)\n", spots[nextSpot]->spotName, minDist);
        
        // Print the path from the current spot to the next spot
        printf("Path: %s -> %s\n", spots[currentSpot]->spotName, spots[nextSpot]->spotName);

        // Add the next spot to the shortest path linked list
        SLL newNode = createNode(spots[nextSpot]->  cityName, spots[nextSpot]->spotName, spots[nextSpot]->rating, spots[nextSpot]->Longitude, spots[nextSpot]->Latitude);
        addNode(&shortestPath, newNode);

        // Update the current spot to the next one
        currentSpot = nextSpot;
    }

    // Step 10: After all spots are visited, find and display the farthest spot from the start
    float maxDist = -1.0;
    int farthestSpot = -1;

    // Loop through all spots to find the farthest spot from the starting point
    for (int i = 0; i < len; i++) {
        if (distanceMatrix[startIndex][i] > maxDist) {
            maxDist = distanceMatrix[startIndex][i];
            farthestSpot = i;
        }
    }

    // If a farthest spot is found, print it
    if (farthestSpot != -1) {
        printf("\nFarthest spot from %s is %s at a distance of %.2f\n", spots[startIndex]->spotName, spots[farthestSpot]->spotName, maxDist);
    }

    // Step 11: Return the linked list of the shortest path
    return shortestPath;
}

// Function to Print the Shortest Path Stored in the Linked List
void printShortestPath(SLL shortestPath){
    if (shortestPath == NULL) {
        printf("No shortest path to display.\n");
        return;
    }

    SLL temp = shortestPath;
    printf("Shortest path: ");
    while (temp != NULL) {
        printf("%s -> ", temp->spotName); // You can modify this to print more details if needed.
        temp = temp->next;
    }
    printf("End\n");
}

// Function to Get the Index of a Node in the Shortest Path Linked List
int getNodeIndex(SLL touristSpots, SLL node){
    int index = 0;

    SLL temp = touristSpots;
    while (temp != NULL) {

        // Compare based on spotName
        if (strcmp(temp->spotName, node->spotName) == 0) {  
            // If Node Found
            return index;
        }

        temp = temp->next;
        index++;
    }
    // If Node not found
    return -1;  
}

// Function to Calculate the Total Distance of the Shortest Path
float calculateTotalDistance(SLL shortestPath, float **distanceMatrix, SLL touristSpots){
    float totalDistance = 0.0;
    
    if (shortestPath == NULL || shortestPath->next == NULL) {
        return totalDistance;  // If there's no valid path, the total distance is 0
    }
    
    SLL current = shortestPath;
    
    // Traverse the linked list and sum the distances between consecutive spots
    while (current != NULL && current->next != NULL) {
        int currentIndex = getNodeIndex(touristSpots, current);
        int nextIndex = getNodeIndex(touristSpots, current->next);
        
        if (currentIndex != -1 && nextIndex != -1) {
            totalDistance += distanceMatrix[currentIndex][nextIndex];
        }
        
        current = current->next;
    }

    return totalDistance;
}

// Function to load reviews from a file into a dynamically allocated array
// Parameters:
// - filename: Path to the file containing reviews
// - reviews: Pointer to an array of Review structures (allocated within the function)
// - reviewCount: Pointer to an integer to store the number of reviews loaded
void loadReviews(const char *filename, Review **reviews, int *reviewCount){
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open review file: %s\n", filename);
        return;
    }

    *reviewCount = 0;
    *reviews = malloc(MAX_REVIEWS * sizeof(Review));

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char cityName[MAX_CITY_NAME];
        char spotName[MAX_CITY_NAME];
        float userRating;
        char reviewText[256];

        // Parse each line, handling quoted review text
        int matched = sscanf(line, "%[^,],%[^,],%f,%[^\n]",
                             cityName, spotName, &userRating, reviewText);

        // Handle cases where the review text has quotes
        if (matched == 4) {
            // Remove quotes around the review text if present
            size_t len = strlen(reviewText);
            if (len > 1 && reviewText[0] == '"' && reviewText[len - 1] == '"') {
                reviewText[len - 1] = '\0'; // Remove trailing quote
                memmove(reviewText, reviewText + 1, len - 1); // Remove leading quote
            }

            // Populate the review structure
            strcpy((*reviews)[*reviewCount].cityName, cityName);
            strcpy((*reviews)[*reviewCount].spotName, spotName);
            (*reviews)[*reviewCount].userRating = userRating;
            strcpy((*reviews)[*reviewCount].reviewText, reviewText);

            (*reviewCount)++;
        }
    }

    fclose(file);
}

// Function to display all reviews for a specific spot.
// Parameters:
// - reviews: Array of Review structures containing review data.
// - reviewCount: Total number of reviews in the array.
// - spotName: Name of the spot for which reviews should be displayed.
void displayReviewsForSpot(Review *reviews, int reviewCount, const char *spotName){
    int found = 0; // Flag to track if any reviews are found

    for (int i = 0; i < reviewCount; i++) {
        if (strcmp(reviews[i].spotName, spotName) == 0) {
            if (!found) {
                printf("Reviews for '%s':\n", spotName);
                found = 1;
            }
            printf("City: %s\n", reviews[i].cityName);
            printf("Rating: %.1f\n", reviews[i].userRating);
            printf("Review: %s\n\n", reviews[i].reviewText);
        }
    }

    if (!found) {
        printf("No reviews found for the spot: %s\n", spotName);
    }
}

// Function to add a new review to the reviews array and save it to the file.
// Parameters:
// - filename: Path to the file where the review should be saved.
// - reviews: Pointer to the array of Review structures (dynamic array).
// - reviewCount: Pointer to the integer tracking the current number of reviews.
// - cityName: Name of the city for the new review.
// - spotName: Name of the spot for the new review.
void addReview(const char *filename, Review **reviews, int *reviewCount, const char *cityName, const char *spotName){
    if (*reviewCount >= MAX_REVIEWS) {
        printf("Maximum review limit reached.\n");
        return;
    }

    Review newReview;
    strcpy(newReview.cityName, cityName);
    strcpy(newReview.spotName, spotName);

    printf("Enter your review: ");
    scanf("%[^\n]s", newReview.reviewText);
    getchar(); // Consume newline

    printf("Enter your rating (0.0 - 5.0): ");
    scanf("%f", &newReview.userRating);
    getchar(); // Consume newline

    (*reviews)[*reviewCount] = newReview;
    (*reviewCount)++;

    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Could not open review file: %s\n", filename);
        return;
    }

    fprintf(file, "%s,%s,%.1f,%s\n", newReview.cityName, newReview.spotName, newReview.userRating, newReview.reviewText);
    fclose(file);

    printf("Review added successfully!\n");
}