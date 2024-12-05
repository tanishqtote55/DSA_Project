#include "header.h"

// Function to find the length of the Singly Linked List
int length(SLL head){
    int count = 0;
    node *temp = head;
    while(temp){
        count++;
        temp = temp -> next;
    }
    return count;
}

// Function to convert a given string to lowercase
void toLowerCase(char *str){
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);  
    }
}

// Function to create a new node
SLL createNode(char *city, char *spot, float rating, char *longitude, char *latitude){
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
void addNode(SLL *head, SLL newNode){
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

// Function to Filters tourist spots from a CSV file based on city, type, and minimum rating, and returns a linked list of matching spots.
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

// Function ro print the tourist spots in the Linked List
void printTouristSpots(SLL head){
    SLL temp = head;
    while (temp != NULL) {
        printf("%s, %s, %.2f\n", 
                temp->cityName, temp->spotName, temp->rating);
        temp = temp->next;
    }
}

// Function to calculate the Haversine distance between two points on the Earth's surface
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

// Function to access the node at a specific index in the list
SLL getNodeAt(SLL head, int index){
    SLL temp = head;
    for (int i = 0; i < index && temp != NULL; i++) {
        temp = temp -> next;
    }
    return temp;
}

// Function to create the distance matrix (graph formation)
float** graphformation(SLL head){
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

// This function implements Dijkstra's algorithm to find the shortest path from a specified starting spot to all other spots in a given list of tourist spots, and displays the farthest spot from the start.
SLL dijkstra_iterative(SLL head, const char *startSpotName, float **distanceMatrix){
    int len = length(head);

    if (len == 0) {
        printf("No spots to visit.\n");
        return NULL;
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
        return NULL;
    }

    // Initialize the visited array
    int visited[len];
    for (int i = 0; i < len; i++) {
        visited[i] = 0;  // Initially, all spots are unvisited
    }

    // Create a linked list for the shortest path
    SLL shortestPath = NULL;

    // Start from the user-defined starting spot
    int currentSpot = startIndex;
    visited[currentSpot] = 1;  // Mark the starting spot as visited
    printf("Starting at: %s\n", spots[currentSpot]->spotName);

    // Add the starting spot to the shortest path linked list
    SLL newNode = createNode(spots[currentSpot]->cityName, spots[currentSpot]->spotName, spots[currentSpot]->rating, spots[currentSpot]->Longitude, spots[currentSpot]->Latitude);
    addNode(&shortestPath, newNode);

    // Visit all spots one by one based on the shortest path
    for (int count = 0; count < len - 1; count++) {
        // Find the nearest unvisited spot
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
            break;
        }

        // Mark the next spot as visited and move there
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

    return shortestPath;
}

// Function to print the shortest path stored in the linked list
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


// Function to calculate the total distance of the shortest path
float calculateTotalDistance(SLL shortestPath, float **distanceMatrix, SLL touristSpots) {
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

// Function to get the index of a node in the shortest path linked list
int getNodeIndex(SLL touristSpots, SLL node) {
    int index = 0;
    SLL temp = touristSpots;
    while (temp != NULL) {
        if (strcmp(temp->spotName, node->spotName) == 0) {  // Compare based on spotName
            return index;
        }
        temp = temp->next;
        index++;
    }
    return -1;  // Node not found
}

// Function to check if the spot is found in the tourist spots list
int isSpotFound(SLL touristSpots, const char *startSpotName) {
    SLL temp = touristSpots;
    while (temp != NULL) {
        if (strcmp(temp->spotName, startSpotName) == 0) {
            return 1; // Spot found
        }
        temp = temp->next;
    }
    return 0; // Spot not found
}

// Helper function to display time in HH:MM format
void displayTime(int hour, int minute) {
    printf("%02d:%02d", hour, minute);
}

// Function to generate itinerary
void generateItinerary(SLL head, int days) {
    // Validate the number of days
    if (days > 5) {
        printf("Error: Not enough spots for more than 5 days of travel.\n");
        return;
    }

    // Total spots in the linked list
    int totalSpots = length(head);

    // Calculate spots per day (even distribution)
    int spotsPerDay = totalSpots / days;
    if (spotsPerDay == 0) {
        printf("Error: Not enough spots for the requested number of days.\n");
        return;
    }

    // Warning if spots exceed 5 in a day
    if (spotsPerDay > 5) {
        printf("Warning: More than 5 spots in a day. You may not fully enjoy all stops.\n");
    }

    SLL temp = head;
    for (int day = 1; day <= days; day++) {
        printf("Day %d Itinerary:\n", day);

        // Start time for the day
        int hour = 8, minute = 0;

        // Include breakfast at the start of the day
        displayTime(hour, minute);
        printf(" - ");
        displayTime(hour + 1, minute);
        printf(": Breakfast\n");

        // Update time to 9:00 AM after breakfast
        hour += 1;

        for (int i = 0; i < spotsPerDay && temp != NULL; i++) {
            float timeSpent;

            // Input time spent at the current spot
            printf("Enter the time (in hours) you want to spend at %s: ", temp->spotName);
            scanf("%f", &timeSpent);

            // Print the visit details
            displayTime(hour, minute);
            printf(" - ");
            hour += (int)timeSpent;
            minute += (int)((timeSpent - (int)timeSpent) * 60);
            if (minute >= 60) {
                hour += minute / 60;
                minute %= 60;
            }
            displayTime(hour, minute);
            printf(": Visit %s (Rating: %.2f) for %.2f hours\n", temp->spotName, temp->rating, timeSpent);

            // Check if there's another spot for travel
            if (temp->next != NULL && i < spotsPerDay - 1) {
                float travelTime = 1.25; // Example fixed travel time
                printf("  Travel from %s to %s: %.2f hours\n", temp->spotName, temp->next->spotName, travelTime);

                // Update time with travel time
                hour += (int)travelTime;
                minute += (int)((travelTime - (int)travelTime) * 60);
                if (minute >= 60) {
                    hour += minute / 60;
                    minute %= 60;
                }
            }

            // Move to the next spot
            temp = temp->next;
        }

        // Include dinner at the end of the day
        hour = 19; // 7:00 PM
        minute = 0;
        displayTime(hour, minute);
        printf(" - ");
        displayTime(hour + 1, minute);
        printf(": Dinner\n");

        printf("\n");
    }

    // If any spots remain, print them as "extra spots"
    if (temp != NULL) {
        printf("Extra spots not covered in the itinerary:\n");
        while (temp != NULL) {
            printf("  %s (Rating: %.2f)\n", temp->spotName, temp->rating);
            temp = temp->next;
        }
    } else {
        printf("Extra spots not covered in the itinerary:\n  None\n");
    }
}


// Function to add a review to a specific spot
void addReview(SLL spot, float rating, const char* reviewText) {
    if (spot == NULL) {
        printf("Error: Cannot add review to a null spot.\n");
        return;
    }

    // Create a new review node
    review* newReview = (review*)malloc(sizeof(review));
    newReview->rating = rating;
    strncpy(newReview->reviewText, reviewText, MAX_REVIEW_TEXT - 1);
    newReview->reviewText[MAX_REVIEW_TEXT - 1] = '\0';  // Ensure null-termination
    newReview->next = NULL;

    // Add the review to the spot's review list
    if (spot->reviews == NULL) {
        spot->reviews = newReview;
    } else {
        review* temp = spot->reviews;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newReview;
    }
}

// Function to display reviews for a specific spot
void displayReviews(SLL spot) {
    if (spot == NULL) {
        printf("Error: Spot is null.\n");
        return;
    }

    printf("Reviews for %s, %s:\n", spot->cityName, spot->spotName);
    
    if (spot->reviews == NULL) {
        printf("No reviews yet.\n");
        return;
    }

    review* temp = spot->reviews;
    int reviewCount = 0;
    while (temp != NULL) {
        reviewCount++;
        printf("Review %d:\n", reviewCount);
        printf("  Rating: %.1f/5.0\n", temp->rating);
        printf("  Comment: %s\n\n", temp->reviewText);
        temp = temp->next;
    }

    // Calculate and display average rating
    float avgRating = calculateAverageRating(spot);
    printf("Average Rating: %.2f/5.0\n", avgRating);
}

// Function to calculate average rating for a spot
float calculateAverageRating(SLL spot) {
    if (spot == NULL || spot->reviews == NULL) {
        return 0.0;
    }

    float totalRating = 0.0;
    int count = 0;
    review* temp = spot->reviews;
    
    while (temp != NULL) {
        totalRating += temp->rating;
        count++;
        temp = temp->next;
    }

    return (count > 0) ? (totalRating / count) : 0.0;
}

// Function to save reviews to a CSV file
void saveReviewsToCSV(SLL head) {
    FILE* file = fopen("./csv/spot-reviews-csv.csv", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing reviews.\n");
        return;
    }

    // Write CSV header
    fprintf(file, "City,Spot Name,Rating,Review Text\n");

    // Traverse through all spots
    SLL currentSpot = head;
    while (currentSpot != NULL) {
        review* currentReview = currentSpot->reviews;
        
        // Write all reviews for this spot
        while (currentReview != NULL) {
            // Escape any commas in the review text to prevent CSV parsing issues
            char escapedReview[MAX_REVIEW_TEXT];
            strcpy(escapedReview, currentReview->reviewText);
            for (int i = 0; escapedReview[i] != '\0'; i++) {
                if (escapedReview[i] == ',') {
                    escapedReview[i] = ';';
                }
            }

            fprintf(file, "%s,%s,%.1f,\"%s\"\n", 
                    currentSpot->cityName, 
                    currentSpot->spotName, 
                    currentReview->rating, 
                    escapedReview);
            
            currentReview = currentReview->next;
        }

        currentSpot = currentSpot->next;
    }

    fclose(file);
    printf("Reviews saved successfully.\n");
}

// Function to load reviews from CSV
SLL loadReviewsFromCSV(SLL head) {
    FILE* file = fopen("./csv/spot-reviews-csv.csv", "r");
    if (file == NULL) {
        printf("No existing reviews file found. Starting with empty reviews.\n");
        return head;
    }

    // Skip header line
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char city[MAX_CITY_NAME];
        char spotName[MAX_CITY_NAME];
        float rating;
        char reviewText[MAX_REVIEW_TEXT];

        // Parse CSV line
        char* token = strtok(line, ",");
        strcpy(city, token);

        token = strtok(NULL, ",");
        strcpy(spotName, token);

        token = strtok(NULL, ",");
        rating = atof(token);

        // Handle review text (which might contain commas)
        token = strtok(NULL, "\n");
        // Remove quotes if present
        if (token[0] == '"' && token[strlen(token)-1] == '"') {
            strncpy(reviewText, token + 1, strlen(token) - 2);
            reviewText[strlen(token) - 2] = '\0';
        } else {
            strcpy(reviewText, token);
        }
        
        // Replace semicolons back to commas
        for (int i = 0; reviewText[i] != '\0'; i++) {
            if (reviewText[i] == ';') {
                reviewText[i] = ',';
            }
        }

        // Find the corresponding spot in the linked list
        SLL currentSpot = head;
        while (currentSpot != NULL) {
            if (strcmp(currentSpot->cityName, city) == 0 && 
                strcmp(currentSpot->spotName, spotName) == 0) {
                // Add review to this spot
                addReview(currentSpot, rating, reviewText);
                break;
            }
            currentSpot = currentSpot->next;
        }
    }

    fclose(file);
    printf("Reviews loaded successfully.\n");
    return head;
}
