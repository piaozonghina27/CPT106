#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys.h"
#include <time.h>


Order *currentOrder = NULL;
UserType currentUserType;
DiscountNode *discountList = NULL;
Dish *menuList = NULL;
Admin *adminList = NULL;
Customer *customerList = NULL;
char currentUsername[50];
int ID;
int main() {
	loadAdminsFromFile();
    loadCustomersFromFile();
//    loadDeliveryPersonsFromFile();
    loadMenuFromFile();
    printf("OK\r\n");
    initializeDiscounts();
    
	while(1){
		//TODO
		welcomeScreen();
	}
    
    return 0;
}
// User welcome screen function
void welcomeScreen() {
    printf("Welcome to the Food Ordering Management System!\n");
    printf("Please select your role:\n");
    printf("1. Administrator\n");
    printf("2. Customer\n");
    printf("3. Exit\n");
    printf("Enter the option number and press Enter: \n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            system("cls"); // Clear screen
            // Call admin function
            adminFunction();
            break;
        case 2:
            system("cls"); // Clear screen
            // Call customer function
            customerFunction();
            break;
        case 3:
            printf("Thank you for using our system. Goodbye!\n");
            exit(0); // Exit the program
        default:
            printf("Invalid option. Please select again.\n");
              // Wait for user to press any key
            system("cls"); // Clear screen
    }
}

// Get menu item information (read prices from file)
MenuItem* getMenuItem() {
    MenuItem *head = NULL;
    char choice;
    do {
        MenuItem *newItem = (MenuItem *)malloc(sizeof(MenuItem));
        if (newItem == NULL) {
            printf("Memory allocation failed.\n");
            return NULL;
        }
        printf("Enter the name of the dish: \n");
        scanf("%s", newItem->name);

        // Read the dish price from the file
        double price = readPriceFromFile(newItem->name);
        if (price == -1) {
            printf("Price not found for \"%s\".\n", newItem->name);
            free(newItem);
            continue;
        }
        newItem->price = price;

        printf("Enter the quantity: \n");
        scanf("%d", &(newItem->quantity));

        newItem->next = NULL;

        if (head == NULL) {
            head = newItem;
        } else {
            MenuItem *temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newItem;
        }

        printf("Do you want to add another dish? (y/n)\n");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    return head;
}

// Read dish price from file
double readPriceFromFile(char *itemName) {
    FILE *file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("Unable to open menu file.\n");
        return -1;
    }

    char name[50];
    double price;
    double numIngredients;
    while (fscanf(file, "%s %lf %lf", name, &price, &numIngredients) != EOF) {
        if (strcmp(name, itemName) == 0) {
            fclose(file);
            return price;
        }
    }

    fclose(file);
    return -1;
}

// Initialize delivery status of order as undelivered
void initializeDeliveryStatus(Order *order) {
    strcpy(order->deliveryStatus, "Undelivered");
}

// Save menu information to file
void saveMenuToFile() {
    FILE *file = fopen("menu.txt", "w");
    if (file == NULL) {
        // Unable to create or open file
        return;
    }

    Dish *currentDish = menuList;
    while (currentDish != NULL) {
        fprintf(file, "%d %s %.2lf %s %.2lf\n", currentDish->id, 
		currentDish->name, currentDish->price, currentDish->category, currentDish->numIngredients);
        currentDish = currentDish->next;
    }

    fclose(file);
}


