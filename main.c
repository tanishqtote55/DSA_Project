#include "header.h"

int main(){
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%[^\n]s", cityName);
    getchar(); // Consume the newline

    // char spotType[MAX_CITY_NAME];
    // printf("Enter the spot type (e.g. Historical Monument, Museum, Park, Religious Site, Shopping, etc.): ");
    // scanf("%[^\n]s", spotType);
    // getchar(); // Consume the newline

    char spotTypeInput[MAX_CITY_NAME * 10];
    printf("Enter the spot types (separated by commas): ");
    scanf("%[^\n]s", spotTypeInput);
    getchar(); // Consume the newline

    float Rating;
    printf("Enter the rating above which you want spots: ");
    scanf("%f", &Rating);
    getchar();  // Consume the newline character left by scanf

    char spotTypes[10][MAX_CITY_NAME];
    int typeCount = 0;
    char *token = strtok(spotTypeInput, ",");
    while (token != NULL && typeCount < 10) {
        strcpy(spotTypes[typeCount++], token);
        token = strtok(NULL, ",");
    }

    // Create an empty linked list
    SLL touristSpots = NULL;

    // Process each spot type and append results to the linked list
    for (int i = 0; i < typeCount; i++) {
        char trimmedSpotType[MAX_CITY_NAME];
        strcpy(trimmedSpotType, spotTypes[i]);
        trimWhitespace(trimmedSpotType); // Helper function to remove leading/trailing spaces
        // printf("%s", trimmedSpotType);

        // Call displayTouristSpots to get spots for this type
        SLL newSpots = displayTouristSpots(NULL, cityName, trimmedSpotType, Rating);
        // printTouristSpots(newSpots); 

        // Append new spots to the main list
        SLL temp = newSpots;
        while (temp != NULL) {
            SLL newNode = createNode(temp->cityName, temp->spotName, temp->rating, temp->Longitude, temp->Latitude);
            addNode(&touristSpots, newNode);
            temp = temp->next;
        }
    }

    // SLL touristSpots = displayTouristSpots(cityName, spotTypes, typeCount, Rating);
    if (touristSpots != NULL) {
        printf("Tourist Spots in %s:\n", cityName);
        printTouristSpots(touristSpots);

        printf("\nCreating Distance Matrix...\n");
        float **distanceMatrix = graphformation(touristSpots); // Assuming this returns the distance matrix

        // Get the starting spot name from the user
        char startSpotName[MAX_CITY_NAME];
        printf("Enter the starting spot name: ");
        scanf("%[^\n]s", startSpotName);
        
        // Use a function to validate the startSpotName
        // For example, you could implement:
        // if (!isSpotFound(touristSpots, startSpotName)) {
        //     printf("Starting spot \"%s\" not found in the list.\n", startSpotName);
        //     return 1; // Exit if not found
        // }
        
        // Start the modified Dijkstra's algorithm to visit all spots
        SLL shortestPath = dijkstra_iterative(touristSpots, startSpotName, distanceMatrix);

        // Display the shortest path
        printShortestPath(shortestPath);

        float totalDistance = calculateTotalDistance(shortestPath, distanceMatrix, touristSpots);
        printf("\nTotal distance traveled: %.2f km\n", totalDistance);
        int days;
        printf("Enter the number of days: ");
        scanf("%d", &days);
        generateItinerary(shortestPath, days);

    } else {
        printf("No tourist spots found for the city: %s\n", cityName);
    }

    return 0;
}