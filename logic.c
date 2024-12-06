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

// Updated function to filter tourist spots based on multiple spot types
SLL displayTouristSpots(SLL head, const char *cityName, const char *spotType, float Rating) {
    FILE *file = fopen("./csv/tourist_spots.csv", "r");
    if (file == NULL) {
        printf("Error Opening File\n");
        return head;  // Return current list without changes
    }

    char line[MAX_LINE_LENGTH];
    char *token;

    // Convert spotType and cityName to lowercase
    char spotTypeLower[MAX_CITY_NAME];
    strcpy(spotTypeLower, spotType);
    toLowerCase(spotTypeLower);

    char cityNameLower[MAX_CITY_NAME];
    strcpy(cityNameLower, cityName);
    toLowerCase(cityNameLower);

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Parse CSV line
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

        latitude[strcspn(latitude, "\n")] = '\0';

        // Lowercase comparison
        char cityLower[MAX_CITY_NAME], typeLower[MAX_CITY_NAME];
        strcpy(cityLower, city);
        toLowerCase(cityLower);

        strcpy(typeLower, type);
        toLowerCase(typeLower);

        if ((strcmp(cityLower, cityNameLower) == 0) &&
            (strcmp(typeLower, spotTypeLower) == 0) &&
            (Rating <= rating)) {
            SLL newNode = createNode(city, spotName, rating, longitude, latitude);
            addNode(&head, newNode);
        }
    }

    fclose(file);
    return head;  // Return the updated list
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
    int spotsPerDay = (totalSpots + 1)/ days;
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

// void addReviewToCSV(const char *fileName) {
//     FILE *file = fopen("./csv/tourist_spots.csv", "r+");
//     if (file == NULL) {
//         printf("Error Opening File\n");
//         return;  // Return current list without changes
//     }

//     char spotName[MAX_REVIEW_LENGTH];
//     char review[MAX_REVIEW_LENGTH];
//     char line[MAX_LINE_LENGTH];
//     long pos;
//     int found = 0;

//     // Get the spot name from the user
//     printf("Enter the name of the tourist spot you want to review: ");
//     fgets(spotName, sizeof(spotName), stdin);
//     spotName[strcspn(spotName, "\n")] = 0;  // Remove newline character

//     // Read the file line by line
//     while (fgets(line, sizeof(line), file)) {
//         pos = ftell(file);  // Get the current file position

//         // Extract city and spot name from the CSV line
//         char *city = strtok(line, ",");
//         char *spot = strtok(NULL, ",");  // This should be the spot name

//         // If the spot name in the line matches the user input
//         if (spot != NULL && strcmp(spot, spotName) == 0) {
//             // Prompt the user to enter a review for the spot
//             printf("Enter your review for '%s': ", spotName);
//             fgets(review, sizeof(review), stdin);
//             review[strcspn(review, "\n")] = 0;  // Remove newline character

//             // Go back to the beginning of the line in the file
//             fseek(file, pos - strlen(line), SEEK_SET);

//             // Append the review to the end of the original line
//             line[strcspn(line, "\n")] = 0;  // Remove the newline in the original line
//             fprintf(file, "|%s\n", review);
//             found = 1;
//             break;
//         }
//     }

//     // If the spot was not found, inform the user
//     if (!found) {
//         printf("The spot '%s' was not found in the CSV file.\n", spotName);
//     }

//     fclose(file);
//     printf("Review has been added successfully.\n");
// }


// void addReviewToCSV() {
//     FILE *file = fopen("./csv/tourist_spots.csv", "r+");
//     if (file == NULL) {
//         printf("Error Opening File\n");
//         return;
//     }

//     char spotName[MAX_REVIEW_LENGTH];
//     char review[MAX_REVIEW_LENGTH];
//     char line[MAX_LINE_LENGTH];
//     long writePosition;
//     int found = 0;

//     // Get the spot name from the user
//     printf("Enter the name of the tourist spot you want to review: ");
//     fgets(spotName, sizeof(spotName), stdin);
//     spotName[strcspn(spotName, "\n")] = 0;  // Remove newline character

//     // Read the file line by line
//     while (fgets(line, sizeof(line), file)) {
//         writePosition = ftell(file);  // Mark the position after reading the line

//         // Make a copy of the line for processing
//         char lineCopy[MAX_LINE_LENGTH];
//         strcpy(lineCopy, line);

//         // Extract city and spot name from the line copy
//         strtok(lineCopy, ",");  // Skip the city
//         char *spot = strtok(NULL, ",");  // Extract the spot name

//         // If the spot name matches
//         if (spot != NULL && strcmp(spot, spotName) == 0) {
//             // Prompt the user for the review
//             printf("Enter your review for '%s': ", spotName);
//             fgets(review, sizeof(review), stdin);
//             review[strcspn(review, "\n")] = 0;  // Remove newline character

//             // Ensure the line has a '|' separator for reviews
//             if (strchr(line, '|') == NULL) {
//                 line[strcspn(line, "\n")] = 0;  // Remove the existing newline
//                 strcat(line, "|");             // Add '|' at the end of the line
//             } else {
//                 line[strcspn(line, "\n")] = 0;  // Remove newline for appending
//             }

//             // Append the review and add a newline
//             strcat(line, review);
//             strcat(line, "\n");

//             // // Go back to the start of the line's position in the file
//             fseek(file, writePosition - strlen(line), SEEK_SET);
//             fprintf(file, "%s", line);
//             // fflush(file);  // Ensure the changes are written to disk
//             found = 1;
//             break;
//         }
//     }

//     if (!found) {
//         printf("The spot '%s' was not found in the CSV file.\n", spotName);
//     }

//     fclose(file);
//     if (found) {
//         printf("Review has been added successfully.\n");
//     }
// }


void addReviewToCSV() {
    FILE *file = fopen("./csv/tourist_spots.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        printf("Error opening temporary file for writing.\n");
        fclose(file);
        return;
    }

    char spotName[MAX_REVIEW_LENGTH];
    char review[MAX_REVIEW_LENGTH];
    char line[MAX_LINE_LENGTH];
    int found = 0;

    // Get the spot name from the user
    printf("Enter the name of the tourist spot you want to review: ");
    fgets(spotName, sizeof(spotName), stdin);
    spotName[strcspn(spotName, "\n")] = 0;  // Remove newline character

    // Read each line and process
    while (fgets(line, sizeof(line), file)) {
        char originalLine[MAX_LINE_LENGTH];
        strcpy(originalLine, line);  // Copy the line for safe modification

        // Extract city and spot name from the line
        char *city = strtok(line, ",");
        char *spot = strtok(NULL, ",");

        // If the spot name matches
        if (spot != NULL && strcmp(spot, spotName) == 0) {
            // Prompt the user for the review
            printf("Enter your review for '%s': ", spotName);
            fgets(review, sizeof(review), stdin);
            review[strcspn(review, "\n")] = 0;  // Remove newline character

            // Check if there's already a review section (|) in the line
            if (strchr(originalLine, '|') == NULL) {
                // No existing reviews; add a separator before the review
                originalLine[strcspn(originalLine, "\n")] = 0;  // Remove existing newline
                strcat(originalLine, "|");  // Add separator
            } else {
                originalLine[strcspn(originalLine, "\n")] = 0;  // Remove newline for appending
            }

            // Append the new review and a newline
            strcat(originalLine, review);
            strcat(originalLine, "|");
            strcat(originalLine, "\n");

            // Write the updated line to the temporary file
            fputs(originalLine, tempFile);

            found = 1;
        } else {
            // Write the unmodified line to the temporary file
            fputs(originalLine, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated file
    if (remove("./csv/tourist_spots.csv") == 0) {
        if (rename("temp.csv", "./csv/tourist_spots.csv") == 0) {
            if (found) {
                printf("Review has been added successfully.\n");
            } else {
                printf("The spot '%s' was not found in the CSV file.\n", spotName);
            }
        } else {
            printf("Error renaming the temporary file.\n");
        }
    } else {
        printf("Error deleting the original file.\n");
    }
}


void readReviewsFromCSV() {
    FILE *file = fopen("./csv/tourist_spots.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char spotName[MAX_REVIEW_LENGTH];
    char line[MAX_LINE_LENGTH];
    char *reviews;
    int found = 0;

    // Get the spot name from the user
    printf("Enter the name of the tourist spot you want to read reviews for: ");
    fgets(spotName, sizeof(spotName), stdin);
    spotName[strcspn(spotName, "\n")] = 0;  // Remove newline character

    // Read each line and process
    while (fgets(line, sizeof(line), file)) {
        // Make a copy of the line for processing
        char lineCopy[MAX_LINE_LENGTH];
        strcpy(lineCopy, line);

        // Extract city and spot name from the line
        strtok(lineCopy, ",");  // Skip city
        char *spot = strtok(NULL, ",");  // Extract the spot name

        // If the spot name matches
        if (spot != NULL && strcmp(spot, spotName) == 0) {
            // Look for the reviews part (after '|')
            reviews = strchr(line, '|');
            if (reviews != NULL) {
                // Skip the '|' character to start reading reviews
                reviews++;

                // Display reviews
                printf("Reviews for '%s':\n", spotName);
                char *review = strtok(reviews, "|");  // Separate reviews by '|'
                while (review != NULL) {
                    // if (strlen(review) > 0) {  // Ensure review is not empty
                    //     printf("- %s\n", review);
                    // }
                    // review = strtok(NULL, "|");

                     // Trim newline and extra spaces from the review
                    review[strcspn(review, "\n")] = 0;  // Remove newline if present
                    
                    // If the review is not just an empty string
                    if (strlen(review) > 0) {
                        printf("- %s\n", review);
                    }

                    // Get next review
                    review = strtok(NULL, "|");
                }
            } else {
                printf("No reviews found for '%s'.\n", spotName);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("The spot '%s' was not found in the CSV file.\n", spotName);
    }

    fclose(file);
}