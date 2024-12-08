#include "header.h"

int main(){
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%[^\n]s", cityName);
    getchar(); // Consume the newline

    char spotTypeInput[MAX_CITY_NAME * 10];
    printf("Enter the spot types Separated by Commas (e.g. Historical Monument, Museum, Park,Beach, Religious Site, Shopping, etc.): ");
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
        SLL newSpots = filterTouristSpots(NULL, cityName, trimmedSpotType, Rating);
        // printTouristSpots(newSpots);
        // Append new spots to the main list
        SLL temp = newSpots;
        while (temp != NULL) {
            SLL newNode = createNode(temp->cityName, temp->spotName, temp->rating, temp->Longitude, temp->Latitude);
            addNode(&touristSpots, newNode);
            temp = temp->next;
        }
    }

    if (touristSpots != NULL) {
        printf("Tourist Spots in %s:\n", cityName);
        printTouristSpots(touristSpots);

        // Review system
        Review *reviews = NULL;
        int reviewCount = 0;
        loadReviews(REVIEW_FILE, &reviews, &reviewCount);

        char reviewChoice;
        do {
            printf("\nWould you like to view or add reviews? (v: view, a: add, n: none): ");
            scanf(" %c", &reviewChoice);
            getchar(); // Consume newline

            if (reviewChoice == 'v' || reviewChoice == 'V') {
                char spotName[MAX_CITY_NAME];
                printf("Enter the name of the spot to view reviews: ");
                scanf("%[^\n]s", spotName);
                getchar(); // Consume newline
                displayReviewsForSpot(touristSpots, reviews, reviewCount, spotName);
            } else if (reviewChoice == 'a' || reviewChoice == 'A') {
                char spotName[MAX_CITY_NAME];
                printf("Enter the name of the spot to add a review: ");
                scanf("%[^\n]s", spotName);
                getchar(); // Consume newline
                addReview(touristSpots, REVIEW_FILE, &reviews, &reviewCount, cityName, spotName);
            }
        } while (reviewChoice != 'n' && reviewChoice != 'N');

        free(reviews); // Free memory allocated for reviews

        // Allow user to remove spots
        char removeChoice;
        do {
            printf("Do you want to remove any spot? (y/n): ");
            scanf(" %c", &removeChoice); // Note the space before %c
            getchar(); // Consume the newline character

            if (removeChoice == 'y' || removeChoice == 'Y') {
                char removeSpotName[MAX_CITY_NAME];
                printf("Enter the name of the spot to remove: ");
                scanf("%[^\n]s", removeSpotName);
                getchar(); // Consume the newline character

                removeSpot(&touristSpots, removeSpotName);

                printf("Updated List of Tourist Spots:\n");
                printTouristSpots(touristSpots);

                if (touristSpots == NULL) {
                    printf("All spots have been removed.\n");
                    return 0;
                }
            }
        } while (removeChoice == 'y' || removeChoice == 'Y');

        printf("\nCreating Distance Matrix...\n");
        float **distanceMatrix = graphformation(touristSpots); // Assuming this returns the distance matrix

        // Get the starting spot name from the user
        char startSpotName[MAX_CITY_NAME];
        printf("Enter the starting spot name: ");
        scanf("%[^\n]s", startSpotName);

        SLL shortestPath = dijkstra(touristSpots, startSpotName, distanceMatrix);

        // Display the shortest path
        printShortestPath(shortestPath);

        float totalDistance = calculateTotalDistance(shortestPath, distanceMatrix, touristSpots);
        printf("\nTotal distance traveled: %.2f km\n", totalDistance);

        // Generate and display an itinerary
        int days;
        printf("Enter the number of days for planning the trip: ");
        scanf("%d", &days);
        generateItinerary(shortestPath, days);

    } else {
        printf("No tourist spots found for the city: %s\n", cityName);
    }

    return 0;
}