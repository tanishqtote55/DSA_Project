#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char line[MAX_LINE_LENGTH];
    char *token;

    fgets(line, sizeof(line), file);

    SLL head = NULL;

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
            // printf("%s", city);
            // printf(" %s", spotName);
            // printf(" %.2f", rating);
            // printf(" %s", type);
            // printf(" %s", longitude);
            // printf(" %s\n", latitude);
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
void graphformation(SLL head){
    int len = length(head);
    float arr[len][len];
    for(int i = 0; i < len; i++){
        for(int j = i + 1; j < len; j++){
            SLL spot1 = getNodeAt(head, i);
            SLL spot2 = getNodeAt(head, j);
            if(i == j){
                arr[i][j] = 0.0;
            }else{
                arr[i][j] = -1;
            }       
        }
    }

    for(int i = 0; i < len; i++){
        for(int j = i + 1; j < len; j++){
            SLL spot1 = getNodeAt(head, i);
            SLL spot2 = getNodeAt(head, j);
            arr[i][j] = calculateDistance(atof(spot1->Latitude), atof(spot1->Longitude), atof(spot2->Latitude), atof(spot2->Longitude));
            arr[j][i] = arr[i][j];
        }
    }

    printf("Distance Matrix:\n");
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            printf("%.2f ", arr[i][j]);
        }
        printf("\n");
    }
}

int main(){
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%[^\n]s", cityName);
    getchar();

    char spotType[MAX_CITY_NAME];
    printf("Enter the spot type (e.g. Historical Monument, Museum, Park, Religious Site, Shopping, etc.): ");
    scanf("%[^\n]s", spotType);
    getchar();

    float Rating;
    printf("Enter the rating above which you want spots: ");
    scanf("%f", &Rating);

    SLL touristSpots = displayTouristSpots(cityName, spotType, Rating);
    if(touristSpots != NULL){
        printf("Tourist Spots in %s:\n", cityName);
        printTouristSpots(touristSpots);

        // printf("\nCreating Distance Matrix...\n");
        // graphformation(touristSpots);
    }else{
        printf("No tourist spots found for the city: %s\n", cityName);
    }

    // Free the linked list memory (optional but good practice)
    SLL current = touristSpots;
    while (current != NULL) {
        SLL temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}