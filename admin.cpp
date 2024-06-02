#define  _CRT_SECURE_NO_WARNINGS 1
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Load administrator information from file
void loadAdminsFromFile() {
    FILE *file = fopen("admins.txt", "r");
    if (file == NULL) {
        // Failed to open the file
        return;
    }

    char username[50], password[50];
    while (fscanf(file, "%s %s", username, password) != EOF) {
        Admin *newAdmin = (Admin *)malloc(sizeof(Admin));
        strcpy(newAdmin->username, username);
        strcpy(newAdmin->password, password);
        newAdmin->next = adminList;
        adminList = newAdmin;
    }

    fclose(file);
}

// Save administrator information to file
void saveAdminsToFile() {
    FILE *file = fopen("admins.txt", "w");
    if (file == NULL) {
        // Failed to open the file, handle error
        return;
    }

    Admin *currentAdmin = adminList;
    while (currentAdmin != NULL) {
        fprintf(file, "%s %s\n", currentAdmin->username, currentAdmin->password);
        currentAdmin = currentAdmin->next;
    }

    fclose(file);
}

// Administrator function menu
void adminFunction() {
    int choice;
    printf("Welcome to the administrator interface\n");
    printf("Please select an operation\n");
    printf("1. Register\n");
    printf("2. Log in\n");
    printf("3. Return to previous level\n");
    printf("Please enter your choice and press Enter\n");

    scanf("%d", &choice);
    switch (choice) {
        case 1:
            system("cls"); // Clear screen, suitable for Windows systems
            adminSignUp(); // Register a new administrator account
            break;
        case 2:
            system("cls"); // Clear screen
            adminLogin(); // Log in to the administrator account
            break;
        case 3:
            system("cls"); // Clear screen
            break;
        default:
            printf("Invalid option, please choose again\n");
             
            system("cls"); // Clear screen
            adminFunction(); // Redisplay administrator function menu
    }
}

// Register a new administrator account
void adminSignUp() {
    char username[50], password[50];
    printf("Enter new administrator username\n");
    scanf("%s", username);
    printf("Enter new administrator password:\n");
    scanf("%s", password);

    // Check if the username is already taken
    Admin *currentAdmin = adminList;
    while (currentAdmin != NULL) {
        if (strcmp(currentAdmin->username, username) == 0) {
            printf("Username already exists, please enter a different one.\n");
             
            system("cls"); // Clear screen
            return;
        }
        currentAdmin = currentAdmin->next;
    }

    // Create a new administrator account
    Admin *newAdmin = (Admin *)malloc(sizeof(Admin));
    strcpy(newAdmin->username, username);
    strcpy(newAdmin->password, password);
    newAdmin->next = adminList;
    adminList = newAdmin;

    // Save updated administrator information to file
    saveAdminsToFile();

    printf("Registration successful\n");
    printf("Returning to previous level...\n");
      // Wait for user key press to confirm
    system("cls"); // Clear screen
    adminFunction(); // Return to administrator function menu
}

// Log in to the administrator account
void adminLogin() {
    char username[50], password[50];
    printf("Enter username:\n");
    scanf("%s", username);
    printf("Enter password:\n");
    scanf("%s", password);

    // Check the administrator list, match username and password
    Admin *currentAdmin = adminList;
    while (currentAdmin != NULL) {
        if (strcmp(currentAdmin->username, username) == 0 && strcmp(currentAdmin->password, password) == 0) {
            printf("Login successful\n");
            printf("Entering administrator functions...\n");
              // Wait for user key press
            system("cls"); // Clear screen
            // Save current username
            strcpy(currentUsername, username);
            adminSpecialFunction();
            return;
        }
        currentAdmin = currentAdmin->next;
    }

    printf("Incorrect username or password\n");
    printf("Please try again...\n");
      // Wait for user key press
    system("cls"); // Clear screen
    adminFunction(); // Return to administrator function menu
}

// Administrator special function menu
void adminSpecialFunction() {
    int choice;
    printf("Please select an operation:\n");
    printf("0. Return to previous level\n");
    printf("1. Add dish information\n");
    printf("2. Delete dish information\n");
    printf("3. Modify dish information\n");
    printf("4. Add customer information\n");
    printf("5. Delete customer information\n");
    printf("6. Modify customer information\n");
    printf("7. Change your password\n");
    printf("8. Display all orders\n");
    printf("9. Reconfigure discounts\n");
    printf("10. Analyze dish sales\n");
    printf("11. View profit\n");
    printf("Please enter the option number and press Enter:\n");

    scanf("%d", &choice);

    switch (choice) {
        case 0:
            system("cls"); // Clear screen
            return;
        case 1:
            system("cls"); // Clear screen
            addDish();
            break;
        case 2:
            system("cls"); // Clear screen
            deleteDish();
            break;
        case 3:
            system("cls"); // Clear screen
            modifyMenuItem();
            break;
        case 4:
            system("cls"); // Clear screen
            customerSignUp();
            break;
        case 5:
            system("cls"); // Clear screen 
            deleteCustomer();
            break;
        case 6:
            system("cls"); // Clear screen
            modifyCustomer();
            break;
        case 7:
            system("cls"); // Clear screen
            changeAdminPassword();
            break;
        case 8:
            system("cls"); // Clear screen
            displayAllOrders();
            break;
        case 9:
            system("cls"); // Clear screen
            modifyDiscounts();
            break;
        case 10:
            system("cls"); // Clear screen
            printDishSales();
            break;
        case 11:
            system("cls"); // Clear screen
            getMoney();
            break;
        default:
            printf("Invalid option, please choose again\n");
              // Wait for user key press
            system("cls"); // Clear screen
            adminSpecialFunction(); // Return to administrator special function menu
    }
    adminSpecialFunction(); // Show administrator special function menu again
}

// Modify menu item information
void modifyMenuItem() {
    int idToModify;
    printf("Please enter the ID of the dish to modify:\n");
    scanf("%d", &idToModify);

    FILE *file = fopen("menu.txt", "r+");
    if (file == NULL) {
        printf("Unable to open menu file\n");
        return;
    }

    int id;
    char name[50];
    double price;
    char category[50];
    int found = 0;
    double numIngredients; 

    // Temporary file to store the updated menu
    FILE *tempFile = fopen("temp_menu.txt", "w");
    if (tempFile == NULL) {
        printf("Unable to create temporary file\n");
        fclose(file);
        return;
    }

    while (fscanf(file, "%d %s %lf %s %lf", &id, name, &price, category, &numIngredients) != EOF) {
        if (id == idToModify) {
            found = 1;
            printf("Please enter the new dish name:\n");
            scanf("%s", name);
            printf("Please enter the new dish price:\n");
            scanf("%lf", &price);
            printf("Please enter the new dish category:\n");
            scanf("%s", category);
            
            scanf("%lf", &numIngredients);
        }
        fprintf(tempFile, "%d %s %.2lf %s %.2lf\n", id, name, price, category, numIngredients);
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("Dish to modify not found\n");
        remove("temp_menu.txt");
        return;
    }

    remove("menu.txt");
    rename("temp_menu.txt", "menu.txt");
    
    menuList = NULL;
    loadMenuFromFile();

    printf("Dish information modified successfully\n");
}


// Initialize discount information
void initializeDiscounts() {
    FILE *file = fopen("discounts.txt", "r");
    if (file == NULL) {
        // Discount file not found, possibly first run
        printf("Unable to open discount information file\n");
        return;
    }

    char type[20];
    double discount;

    while (fscanf(file, "%s %lf", type, &discount) != EOF) {
        // Create a new discount node
        DiscountNode *newNode = (DiscountNode *)malloc(sizeof(DiscountNode));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            fclose(file);
            return;
        }
        strcpy(newNode->type, type);
        newNode->discount = discount;
        newNode->next = NULL;

        // Add the new node to the end of the discount list
        if (discountList == NULL) {
            discountList = newNode;
        } else {
            DiscountNode *currentNode = discountList;
            while (currentNode->next != NULL) {
                currentNode = currentNode->next;
            }
            currentNode->next = newNode;
        }
    }

    fclose(file);
}


// Display all order information
void displayAllOrders() {
    printf("Displaying all order information:\n");
    
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("Unable to open orders file\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line); // Print each line of order information directly from the file
    }

    fclose(file);
}

// Display the menu
void displayMenu() {
    printf("Menu List:\n");
    printf("ID\t\tName\tPrice\tCategory\tIngredient Cost\n");
    Dish *currentDish = menuList;
    while (currentDish != NULL) {
        printf("%d\t%8s\t%.2lf\t%s\t%lf\n", 
        currentDish->id, currentDish->name, currentDish->price, currentDish->category, currentDish->numIngredients);
        currentDish = currentDish->next;
    }
}

// Change admin password 
void changeAdminPassword() {
    char newPassword[50];
    printf("Enter the new admin password:\n");
    scanf("%s", newPassword);

    Admin *currentAdmin = adminList;
    while (currentAdmin != NULL) {
        if (strcmp(currentAdmin->username, currentUsername) == 0) {
            strcpy(currentAdmin->password, newPassword);
            // Save modified admin information
            saveAdminsToFile();
            printf("Password changed successfully\n");
            printf("Press any key to continue...\n");
              // Wait for user key press to continue
            system("cls"); // Clear screen 
            adminSpecialFunction(); // Return to administrator special function menu
            return;
        }
        currentAdmin = currentAdmin->next;
    }

    printf("Username does not exist, please try again\n");
    printf("Press any key to continue...\n");
      // Wait for user key press to continue
    system("cls"); // Clear screen
}


void changeCustomerPassword() {
    char newPassword[50];
    printf("Enter the new customer password:\n");
    scanf("%s", newPassword);

    Customer *currentCustomer = customerList;
    while (currentCustomer != NULL) {
        if (strcmp(currentCustomer->username, currentUsername) == 0) {
            strcpy(currentCustomer->password, newPassword);
            // Save modified customer information
            saveCustomersToFile();
            printf("Password changed successfully\n");
            printf("Press any key to continue...\n");
            return;
        }
        currentCustomer = currentCustomer->next;
    }

    printf("Username does not exist, please try again\n");
    printf("Press any key to continue...\n");
      // Wait for user key press to continue
    system("cls"); // Clear screen
}

int dishExists(int id, const char *name) {
    Dish *current = menuList;
    while (current != NULL) {
        if (current->id == id || strcmp(current->name, name) == 0) {
            return 1; // A dish with the same ID or name exists
        }
        current = current->next;
    }
    return 0; // No dish with the same ID or name exists
}

void addDish() {
    Dish *newDish = (Dish *)malloc(sizeof(Dish));
    if (!newDish) {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Enter the dish ID:\n");
    scanf("%d", &newDish->id);

    printf("Enter the dish name:\n");
    scanf("%s", newDish->name);

    printf("Enter the dish price:\n");
    scanf("%lf", &newDish->price);

    printf("Enter the dish category:\n");
    scanf("%s", newDish->category);
    
    printf("Enter the dish ingredient cost:\n");
    scanf("%lf", &newDish->numIngredients);

    if (dishExists(newDish->id, newDish->name)) {
        printf("Dish ID or name already exists, addition failed\n");
        free(newDish); // Free allocated memory
        return;
    }

    // Add the new dish to the beginning of the list
    newDish->next = menuList;
    menuList = newDish;

    // Save the menu to file
    saveMenuToFile();

    printf("Dish added successfully\n");
}

// Delete dish information
void deleteDish() {
    int id;
    printf("Enter the ID of the dish to delete:\n");
    scanf("%d", &id);

    Dish *currentDish = menuList;
    Dish *prevDish = NULL;

    while (currentDish != NULL && currentDish->id != id) {
        prevDish = currentDish;
        currentDish = currentDish->next;
    }

    if (currentDish == NULL) {
        printf("No dish with the specified ID found\n");
        return;
    }

    if (prevDish == NULL) {
        menuList = currentDish->next;
    } else {
        prevDish->next = currentDish->next;
    }

    free(currentDish);

    // Update the menu file
    saveMenuToFile();

    printf("Dish deleted successfully\n");
}
// Modify discount information
void modifyDiscounts() {
    double platinumDiscount, goldDiscount, silverDiscount;
    printf("Enter the discount for platinum members: ");
    scanf("%lf", &platinumDiscount);
    printf("Enter the discount for gold members: ");
    scanf("%lf", &goldDiscount);
    printf("Enter the discount for silver members: ");
    scanf("%lf", &silverDiscount);

    // Open the discounts file
    FILE *file = fopen("discounts.txt", "w");
    if (file == NULL) {
        printf("Unable to open the discounts file\n");
        return;
    }

    // Write the updated discount information to the file
    fprintf(file, "%.2lf %.2lf %.2lf\n", platinumDiscount, goldDiscount, silverDiscount);
    
    fclose(file);

    printf("Discount information modified successfully\n");
}

// Search for the corresponding dish in menuList by dish name and calculate its cost
double getCostPrice(const char *itemName) {
    Dish *current = menuList;
    while (current != NULL) {
        if (strcmp(current->name, itemName) == 0) {
            return current->numIngredients;
        }
        current = current->next;
    }
    return 0.0; // Return 0 cost if the corresponding dish is not found
}

// TODO
void getMoney() {

    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("Unable to open the orders file.\n");
        return;
    }

    char line[256];
    double totalOrderPrice = 0.0;        // Total order price
    double totalDiscountedPrice = 0.0;   // Total discounted price
    double totalProfit = 0.0;            // Total profit
    int orderCount = 0;                  // Order counter

    while (fgets(line, sizeof(line), file) != NULL) {
        char customerName[50], deliveryStatus[20];
        int orderID;

        // Parse order information
        if (sscanf(line, "%s %s %d", customerName, deliveryStatus, &orderID) == 3) {
            printf("Order ID: %d\n", orderID);
            printf("Delivery Status: %s\n", deliveryStatus);

            char itemName[50];
            int quantity;
            double price;
            double orderPrice = 0.0;     // Current order price
            double orderCost = 0.0;      // Current order cost
            double orderProfit = 0.0;    // Current order profit

            while (fgets(line, sizeof(line), file) != NULL) {
                // Parse dish information
                if (sscanf(line, "%s %d %lf", itemName, &quantity, &price) == 3) {
                    double costPrice = getCostPrice(itemName);
                    double itemProfit = (price - costPrice) * quantity;
                    
                    printf("Dish Name: %s, Quantity: %d, Price: %.2lf, Cost: %.2lf, Profit: %.2lf\n",
                           itemName, quantity, price, costPrice, itemProfit);

                    orderPrice += quantity * price;      // Calculate current order price
                    orderCost += quantity * costPrice;   // Calculate current order cost
                    orderProfit += itemProfit;           // Calculate current order profit
                } else if (strcmp(line, "end\n") == 0) {
                    printf("Current Order Price: %.2lf\n", orderPrice);
                    printf("Current Order Profit: %.2lf\n", orderProfit);
                    printf("-----------------\n");
                    break;
                }
            }

            // Calculate total price and total profit 
            totalOrderPrice += orderPrice;
            totalProfit += orderProfit;

            // Increment order count
            orderCount++;
            
        }
    }

    // Output total order information
    printf("Total Orders: %d\n", orderCount);
    printf("Total Order Price: %.2lf\n", totalOrderPrice);
    printf("Total Profit: %.2lf\n", totalProfit);

    fclose(file);
}

