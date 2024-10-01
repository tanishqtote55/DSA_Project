#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_CITY_NAME 100

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);  
    }
}

void displayTouristSpots(const char* cityName){
    FILE* file = fopen("./csv/tourist_spots.csv", "r");
    if(file == NULL){
        printf("Error Opening File\n");
        return;
    }
    char line[MAX_LINE_LENGTH];
    char *token;

    fgets(line, sizeof(line), file);
    
    while(fgets(line, sizeof(line), file)){
        char city[MAX_CITY_NAME];
        char spotName[100], rating[10], type[50], longitude[20], latitude[20];

        token = strtok(line, ","); 
        strcpy(city, token);

        token = strtok(NULL, ",");
        strcpy(spotName, token);

        token = strtok(NULL, ",");
        strcpy(rating, token);

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

        if(strcmp(cityLower, cityNameLower) == 0){
            printf("%s", city);
            printf(" %s", spotName);
            printf(" %s", rating);
            printf(" %s", type);
            printf(" %s", longitude);
            printf(" %s\n", latitude);
        }
    }
    fclose(file);
}

int main(){
    char cityName[MAX_CITY_NAME];
    printf("Enter the city name: ");
    scanf("%99s", cityName);
    displayTouristSpots(cityName);
    return 0;
}