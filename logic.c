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