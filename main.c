#include "header.h"

int main(){
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%[^\n]s", cityName);
    getchar(); // Consume the newline

    char spotType[MAX_CITY_NAME];
    printf("Enter the spot type (e.g. Historical Monument, Museum, Park, Religious Site, Shopping, etc.): ");
    scanf("%[^\n]s", spotType);
    getchar(); // Consume the newline

    float Rating;
    printf("Enter the rating above which you want spots: ");
    scanf("%f", &Rating);
    getchar();  // Consume the newline character left by scanf

    SLL touristSpots = filterTouristSpots(NULL, cityName, spotType, Rating);
    if (touristSpots != NULL) {
        printf("Tourist Spots in %s:\n", cityName);
        printTouristSpots(touristSpots);

        printf("\nCreating Distance Matrix...\n");
        float **distanceMatrix = graphformation(touristSpots); // Assuming this returns the distance matrix

        // Get the starting spot name from the user
        char startSpotName[MAX_CITY_NAME];
        printf("Enter the starting spot name: ");
        scanf("%[^\n]s", startSpotName);

        SLL shortestPath = dijkstra_iterative(touristSpots, startSpotName, distanceMatrix);

        // Display the shortest path
        printShortestPath(shortestPath);

    } else {
        printf("No tourist spots found for the city: %s\n", cityName);
    }

    return 0;
}