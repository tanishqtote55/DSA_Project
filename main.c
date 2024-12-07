// // int main() {
// //     char cityName[MAX_CITY_NAME];
// //     printf("Enter the city name: ");
// //     scanf("%[^\n]s", cityName);
// //     getchar(); // Consume the newline

// //     char spotTypeInput[MAX_CITY_NAME * 10];
// //     printf("Enter the spot types (separated by commas): ");
// //     scanf("%[^\n]s", spotTypeInput);
// //     getchar(); // Consume the newline

// //     float Rating;
// //     printf("Enter the rating above which you want spots: ");
// //     scanf("%f", &Rating);
// //     getchar();  // Consume the newline character left by scanf

// //     char spotTypes[10][MAX_CITY_NAME];
// //     int typeCount = 0;
// //     char *token = strtok(spotTypeInput, ",");
// //     while (token != NULL && typeCount < 10) {
// //         strcpy(spotTypes[typeCount++], token);
// //         token = strtok(NULL, ",");
// //     }

// //     // Create an empty linked list
// //     SLL touristSpots = NULL;

// //     // Process each spot type and append results to the linked list
// //     for (int i = 0; i < typeCount; i++) {
// //         char trimmedSpotType[MAX_CITY_NAME];
// //         strcpy(trimmedSpotType, spotTypes[i]);
// //         trimWhitespace(trimmedSpotType);

// //         SLL newSpots = displayTouristSpots(NULL, cityName, trimmedSpotType, Rating);

// //         // Append new spots to the main list
// //         SLL temp = newSpots;
// //         while (temp != NULL) {
// //             SLL newNode = createNode(temp->cityName, temp->spotName, temp->rating, temp->Longitude, temp->Latitude);
// //             addNode(&touristSpots, newNode);
// //             temp = temp->next;
// //         }
// //     }

// //     if (touristSpots != NULL) {
// //         printf("Tourist Spots in %s:\n", cityName);
// //         printTouristSpots(touristSpots);

// //         // Allow user to remove a spot
// //         char removeChoice;
// //         printf("Do you want to remove any spot? (y/n): ");
// //         scanf(" %c", &removeChoice); // Note the space before %c to consume leftover newline
// //         getchar(); // Consume the newline character

// //         while (removeChoice == 'y' || removeChoice == 'Y') {
// //             char removeSpotName[MAX_CITY_NAME];
// //             printf("Enter the name of the spot to remove: ");
// //             scanf("%[^\n]s", removeSpotName);
// //             getchar(); // Consume the newline character

// //             removeSpot(&touristSpots, removeSpotName);

// //             printf("Do you want to remove another spot? (y/n): ");
// //             scanf(" %c", &removeChoice);
// //             getchar(); // Consume the newline character
// //         }

// //         printf("\nCreating Distance Matrix...\n");
// //         float **distanceMatrix = graphformation(touristSpots); // Assuming this returns the distance matrix

// //         char startSpotName[MAX_CITY_NAME];
// //         printf("Enter the starting spot name: ");
// //         scanf("%[^\n]s", startSpotName);

// //         SLL shortestPath = dijkstra_iterative(touristSpots, startSpotName, distanceMatrix);

// //         printShortestPath(shortestPath);

// //         float totalDistance = calculateTotalDistance(shortestPath, distanceMatrix, touristSpots);
// //         printf("\nTotal distance traveled: %.2f km\n", totalDistance);

// //         int days;
// //         printf("Enter the number of days: ");
// //         scanf("%d", &days);
// //         generateItinerary(shortestPath, days);

// //     } else {
// //         printf("No tourist spots found for the city: %s\n", cityName);
// //     }

// //     return 0;
// // }

// #include "header.h"

// int main() {
//     char cityName[MAX_CITY_NAME];
//     printf("Enter the city name: ");
//     scanf("%[^\n]s", cityName);
//     getchar(); // Consume the newline

//     char spotTypeInput[MAX_CITY_NAME * 10];
//     printf("Enter the spot types (separated by commas): ");
//     scanf("%[^\n]s", spotTypeInput);
//     getchar(); // Consume the newline

//     float Rating;
//     printf("Enter the rating above which you want spots: ");
//     scanf("%f", &Rating);
//     getchar(); // Consume the newline character left by scanf

//     // Process spot types into an array
//     char spotTypes[10][MAX_CITY_NAME];
//     int typeCount = 0;
//     char *token = strtok(spotTypeInput, ",");
//     while (token != NULL && typeCount < 10) {
//         strcpy(spotTypes[typeCount++], token);
//         trimWhitespace(spotTypes[typeCount - 1]); // Remove leading/trailing spaces
//         token = strtok(NULL, ",");
//     }

//     // Create an empty linked list
//     SLL touristSpots = NULL;

//     // Fetch and append tourist spots based on spot types and ratings
//     for (int i = 0; i < typeCount; i++) {
//         SLL newSpots = displayTouristSpots(NULL, cityName, spotTypes[i], Rating);
//         while (newSpots != NULL) {
//             SLL newNode = createNode(newSpots->cityName, newSpots->spotName, newSpots->rating, 
//                                      newSpots->Longitude, newSpots->Latitude);
//             addNode(&touristSpots, newNode);
//             newSpots = newSpots->next;
//         }
//     }

//     if (touristSpots == NULL) {
//         printf("No tourist spots found for the city: %s\n", cityName);
//         return 0;
//     }

//     // Display the initial list of tourist spots
//     printf("Tourist Spots in %s:\n", cityName);
//     printTouristSpots(touristSpots);

//     // Allow user to remove spots
//     char removeChoice;
//     do {
//         printf("Do you want to remove any spot? (y/n): ");
//         scanf(" %c", &removeChoice); // Note the space before %c
//         getchar(); // Consume the newline character

//         if (removeChoice == 'y' || removeChoice == 'Y') {
//             char removeSpotName[MAX_CITY_NAME];
//             printf("Enter the name of the spot to remove: ");
//             scanf("%[^\n]s", removeSpotName);
//             getchar(); // Consume the newline character

//             removeSpot(&touristSpots, removeSpotName);

//             printf("Updated List of Tourist Spots:\n");
//             printTouristSpots(touristSpots);

//             if (touristSpots == NULL) {
//                 printf("All spots have been removed.\n");
//                 return 0;
//             }
//         }
//     } while (removeChoice == 'y' || removeChoice == 'Y');

//     // Generate the distance matrix
//     printf("\nCreating Distance Matrix...\n");
//     float **distanceMatrix = graphformation(touristSpots);

//     // Prompt user for the starting spot
//     char startSpotName[MAX_CITY_NAME];
//     printf("Enter the starting spot name: ");
//     scanf("%[^\n]s", startSpotName);
//     getchar(); // Consume the newline character

//     SLL shortestPath = dijkstra_iterative(touristSpots, startSpotName, distanceMatrix);

//     // Display the shortest path
//     printf("\nShortest Path:\n");
//     printShortestPath(shortestPath);

//     // Calculate and display the total distance traveled
//     float totalDistance = calculateTotalDistance(shortestPath, distanceMatrix, touristSpots);
//     printf("\nTotal distance traveled: %.2f km\n", totalDistance);

//     // Generate and display an itinerary
//     int days;
//     printf("Enter the number of days: ");
//     scanf("%d", &days);
//     generateItinerary(shortestPath, days);

//     return 0;
// }


#include "header.h"

int main() {
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%[^\n]s", cityName);
    getchar(); // Consume the newline

    char spotTypeInput[MAX_CITY_NAME * 10];
    printf("Enter the spot types separated by commas (e.g., Historical Monument, Museum, Park, etc.): ");
    scanf("%[^\n]s", spotTypeInput);
    getchar(); // Consume the newline

    float Rating;
    printf("Enter the rating above which you want spots: ");
    scanf("%f", &Rating);
    getchar(); // Consume the newline character left by scanf

    // Process spot types into an array
    char spotTypes[10][MAX_CITY_NAME];
    int typeCount = 0;
    char *token = strtok(spotTypeInput, ",");
    while (token != NULL && typeCount < 10) {
        strcpy(spotTypes[typeCount++], token);
        trimWhitespace(spotTypes[typeCount - 1]); // Remove leading/trailing spaces
        token = strtok(NULL, ",");
    }

    // Create an empty linked list for tourist spots
    SLL touristSpots = NULL;

    // Fetch and append tourist spots based on spot types and ratings
    for (int i = 0; i < typeCount; i++) {
        SLL newSpots = displayTouristSpots(NULL, cityName, spotTypes[i], Rating);
        while (newSpots != NULL) {
            SLL newNode = createNode(newSpots->cityName, newSpots->spotName, newSpots->rating, 
                                     newSpots->Longitude, newSpots->Latitude);
            addNode(&touristSpots, newNode);
            newSpots = newSpots->next;
        }
    }

    if (touristSpots == NULL) {
        printf("No tourist spots found for the city: %s\n", cityName);
        return 0;
    }

    // Display the initial list of tourist spots
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
            displayReviewsForSpot(reviews, reviewCount, spotName);
        } else if (reviewChoice == 'a' || reviewChoice == 'A') {
            char spotName[MAX_CITY_NAME];
            printf("Enter the name of the spot to add a review: ");
            scanf("%[^\n]s", spotName);
            getchar(); // Consume newline
            addReview(REVIEW_FILE, &reviews, &reviewCount, cityName, spotName);
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

    // Generate the distance matrix
    printf("\nCreating Distance Matrix...\n");
    float **distanceMatrix = graphformation(touristSpots);

    // Prompt user for the starting spot
    char startSpotName[MAX_CITY_NAME];
    printf("Enter the starting spot name: ");
    scanf("%[^\n]s", startSpotName);
    getchar(); // Consume the newline character

    SLL shortestPath = dijkstra_iterative(touristSpots, startSpotName, distanceMatrix);

    // Display the shortest path
    printf("\nShortest Path:\n");
    printShortestPath(shortestPath);

    // Calculate and display the total distance traveled
    float totalDistance = calculateTotalDistance(shortestPath, distanceMatrix, touristSpots);
    printf("\nTotal distance traveled: %.2f km\n", totalDistance);

    // Generate and display an itinerary
    int days;
    printf("Enter the number of days: ");
    scanf("%d", &days);
    generateItinerary(shortestPath, days);

    return 0;
}
