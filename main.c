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

    SLL touristSpots = displayTouristSpots(cityName, spotType, Rating);
    
    // Load existing reviews
    touristSpots = loadReviewsFromCSV(touristSpots);
    
    if (touristSpots != NULL) {
        printf("Tourist Spots in %s:\n", cityName);
        printTouristSpots(touristSpots);

        // Review System Menu
        int choice;
        do {
            printf("\nReview System Menu:\n");
            printf("1. Add a Review\n");
            printf("2. View Reviews for a Spot\n");
            printf("3. Continue with Trip Planning\n");
            printf("4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); // Consume newline

            switch(choice) {
                case 1: {
                    // Add Review
                    char spotName[MAX_CITY_NAME];
                    printf("Enter the spot name to review: ");
                    scanf("%[^\n]s", spotName);
                    getchar();

                    // Find the spot
                    SLL spot = touristSpots;
                    while (spot != NULL) {
                        if (strcmp(spot->spotName, spotName) == 0) {
                            float reviewRating;
                            char reviewText[MAX_REVIEW_TEXT];

                            printf("Enter your rating (0-5): ");
                            scanf("%f", &reviewRating);
                            getchar();

                            printf("Enter your review: ");
                            scanf("%[^\n]s", reviewText);
                            getchar();

                            addReview(spot, reviewRating, reviewText);
                            saveReviewsToCSV(touristSpots);
                            printf("Review added successfully!\n");
                            break;
                        }
                        spot = spot->next;
                    }
                    
                    if (spot == NULL) {
                        printf("Spot not found.\n");
                    }
                    break;
                }
                case 2: {
                    // View Reviews
                    char spotName[MAX_CITY_NAME];
                    printf("Enter the spot name to view reviews: ");
                    scanf("%[^\n]s", spotName);
                    getchar();

                    // Find the spot
                    SLL spot = touristSpots;
                    while (spot != NULL) {
                        if (strcmp(spot->spotName, spotName) == 0) {
                            displayReviews(spot);
                            break;
                        }
                        spot = spot->next;
                    }
                    
                    if (spot == NULL) {
                        printf("Spot not found.\n");
                    }
                    break;
                }
                case 3:
                    // Continue with existing trip planning logic
                    printf("\nCreating Distance Matrix...\n");
                    float **distanceMatrix = graphformation(touristSpots);

                    // Get the starting spot name from the user
                    char startSpotName[MAX_CITY_NAME];
                    printf("Enter the starting spot name: ");
                    scanf("%[^\n]s", startSpotName);
                    
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
                    break;
                case 4:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 4);
    } else {
        printf("No tourist spots found for the city: %s\n", cityName);
    }


    // review system ammendments
    

    return 0;
}
