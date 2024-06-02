#define  _CRT_SECURE_NO_WARNINGS 1
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Load dish information from the menu file
void loadMenuFromFile() {
    FILE *file = fopen("menu.txt", "r");
    if (file == NULL) {
        // Unable to open the file
        return;
    }

    int id;
    char name[50], category[50];
    double price;
    double numIngredients; // Ingredients
    while (fscanf(file, "%d %s %lf %s %lf", &id, name, &price, category, &numIngredients) != EOF) {
        Dish *newDish = (Dish *)malloc(sizeof(Dish));
        newDish->id = id;
        strcpy(newDish->name, name);
        newDish->price = price;
        strcpy(newDish->category, category);
        newDish->next = menuList;
        newDish->numIngredients = numIngredients;
        menuList = newDish;
    }

    fclose(file);
}

