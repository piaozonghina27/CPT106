#define  _CRT_SECURE_NO_WARNINGS 1
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
// Modify customer information
void modifyCustomer() {
    char usernameToModify[50];
    printf("Enter the username of the customer to modify:\n");
    scanf("%s", usernameToModify);

    // Traverse the list to find the customer information structure, assuming the list already exists

    // Initialize a pointer to the customer information linked list customerList
    Customer *currentCustomer = customerList;
    int found = 0;

    // Search for the customer information to be modified in the list
    while (currentCustomer != NULL) {
        if (strcmp(currentCustomer->username, usernameToModify) == 0) {
            found = 1;
            break;
        }
        currentCustomer = currentCustomer->next;
    }

    if (!found) {
        printf("Customer to modify not found.\n");
        return;
    }

    // Display and input the new customer password and type
    char newPassword[50];
    printf("Enter the new password:\n");
    scanf("%s", newPassword);

    // Update customer information
    strcpy(currentCustomer->password, newPassword);

    // Save the modified customer information to a file
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL) {
        printf("Unable to open the customer information file.\n");
        return;
    }

    currentCustomer = customerList;
    while (currentCustomer != NULL) {
        fprintf(file, "%s %s\n", currentCustomer->username, currentCustomer->password);
        currentCustomer = currentCustomer->next;
    }

    fclose(file);

    printf("Customer information modified successfully.\n");
}

// Delete customer information
void deleteCustomer() {
    char usernameToDelete[50];
    printf("Enter the username of the customer to delete:\n");
    scanf("%s", usernameToDelete);

    // Traverse the list to find the customer information structure, assuming the list already exists

    // Initialize a pointer to the customer information linked list customerList
    Customer *currentCustomer = customerList;
    Customer *prevCustomer = NULL;
    int found = 0;

    // Search for the customer information to be deleted in the list
    while (currentCustomer != NULL) {
        if (strcmp(currentCustomer->username, usernameToDelete) == 0) {
            found = 1;
            break;
        }
        prevCustomer = currentCustomer;
        currentCustomer = currentCustomer->next;
    }

    if (!found) {
        printf("Customer to delete not found.\n");
        return;
    }

    // Delete the customer information node
    if (prevCustomer == NULL) {
        // If the node to delete is at the beginning of the list
        customerList = currentCustomer->next;
    } else {
        prevCustomer->next = currentCustomer->next;
    }

    free(currentCustomer); // Free memory

    // Save the updated customer information to a file
    FILE *sourceFile = fopen("customers.txt", "r");
    FILE *tempFile = fopen("temp_customers.txt", "w");
    if (sourceFile == NULL || tempFile == NULL) {
        printf("Unable to open the customer information file.\n");
        return;
    }

    char username[50];
    char line[100];

    while (fgets(line, sizeof(line), sourceFile)) {
        sscanf(line, "%s", username);
        if (strcmp(username, usernameToDelete) != 0) {
            fputs(line, tempFile);
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    remove("customers.txt");
    rename("temp_customers.txt", "customers.txt");

    printf("Customer information deleted successfully.\n");
}

int isOrderInTransit(int orderID) {
    // Open the order file
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("Unable to open the order file.\n");
        return -1; // Return -1 for error
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char status[20];
        int currentOrderID;
        if (sscanf(line, "%*s %s %d", status, &currentOrderID) == 2) {
            if (currentOrderID == orderID) {
                fclose(file);
                // Check if the order status is "In Transit", return 1 if in transit, else return 0
                return strcmp(status, "In Transit") == 0 ? 1 : 0;
            }
        }
    }

    fclose(file);
    // Return -1 if the corresponding order ID is not found
    return -1;
}

// Cancel Order
void cancelOrder() {
    int orderID;
    printf("Enter the order ID to cancel: ");
    scanf("%d", &orderID);

    if (isOrderInTransit(orderID)) {
        printf("The order is in transit and cannot be canceled.\n");
        return;
    }

    // Implementation of order cancellation
}

// Print the sales quantity of each dish from the order file
void printDishSales() {
    // Open the order file
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("Unable to open the order file.\n");
        return;
    }

    // Initialize dish sales quantity list
    DishSales *salesList = NULL;

    // Read the order file line by line
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        // Read order information
        char customerName[50], deliveryStatus[20];
        int orderID;
        if (sscanf(line, "%s %s %d", customerName, deliveryStatus, &orderID) != 3) {
            printf("Order file format error.\n");
            fclose(file);
            return;
        }

        // Read dish information
        while (fgets(line, sizeof(line), file) != NULL && strcmp(line, "end\n") != 0) {
            // Parse dish information
            char itemName[50];
            int quantity;
            double price;
            if (sscanf(line, "%s %d %lf", itemName, &quantity, &price) == 3) {
                // Check if the dish already exists in the list
                DishSales *current = salesList;
                while (current != NULL) {
                    if (strcmp(current->name, itemName) == 0) {
                        // Dish exists, update quantity
                        current->quantity += quantity;
                        break;
                    }
                    current = current->next;
                }
                if (current == NULL) {
                    // Dish does not exist, add a new node
                    DishSales *newSales = (DishSales *)malloc(sizeof(DishSales));
                    strcpy(newSales->name, itemName);
                    newSales->quantity = quantity;
                    newSales->next = salesList;
                    salesList = newSales;
                }
            } else {
                printf("Order file format error.\n");
                fclose(file);
                return;
            }
        }
    }

    // Close the order file
    fclose(file);

    // Print the sales quantity of each dish
    printf("Sales quantity of each dish:\n");
    DishSales *current = salesList;
    while (current != NULL) {
        printf("%s: %d\n", current->name, current->quantity);
        current = current->next;
    }

    // Free memory of the list
    current = salesList;
    while (current != NULL) {
        DishSales *temp = current;
        current = current->next;
        free(temp);
    }
}

// Load customer information from file
void loadCustomersFromFile() {
    FILE *file = fopen("customers.txt", "r");
    if (file == NULL) {
        // Unable to open the file
        return;
    }

    char username[50], password[50], customer_type[50];
    while (fscanf(file, "%s %s", username, password) != EOF) {
        Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
        strcpy(newCustomer->username, username);
        strcpy(newCustomer->password, password);
//        strcpy(newCustomer->customer_type, customer_type);
        newCustomer->orders = NULL;
        newCustomer->next = customerList;
        customerList = newCustomer;
    }

    fclose(file);
}

// Save customer information to file
void saveCustomersToFile() {
    FILE *file = fopen("customers.txt", "w");
    if (file == NULL) {
        // Error handling code
        return;
    }

    Customer *currentCustomer = customerList;
    while (currentCustomer != NULL) {
        fprintf(file, "%s %s %s\n", currentCustomer->username, currentCustomer->password, currentCustomer->customer_type);
        currentCustomer = currentCustomer->next;
    }

    fclose(file);
}

// Customer function
void customerFunction() {
    int choice;
    printf("Welcome, customer!\n");
    printf("Please select an option:\n");
    printf("1. Sign up\n");
    printf("2. Log in\n");
    printf("3. Go back\n");
    printf("Enter the option number and press Enter:\n");

    scanf("%d", &choice);
    switch (choice) {
        case 1:
            system("cls"); // Clear screen (Windows)
            customerSignUp(); // Sign up function
            customerFunction(); // Redisplay customer function
            break;
        case 2:
            system("cls"); // Clear screen
            customerLogin(); // Log in function
            break;
        case 3:
            system("cls"); // Clear screen
            break;
        default:
            printf("Invalid option, please choose again.\n");
             
            system("cls"); // Clear screen
            customerFunction(); // Redisplay customer function
    }
}

// Sign up function
void customerSignUp() {
    char username[50], password[50], customer_type[50];
    printf("Enter username:\n");
    scanf("%s", username);
    printf("Enter password:\n");
    scanf("%s", password);
//    printf("Enter customer type:\n");
//    scanf("%s", customer_type);

    // Check if the same username already exists
    Customer *currentCustomer = customerList;
    while (currentCustomer != NULL) {
        if (strcmp(currentCustomer->username, username) == 0) {
            printf("Username already exists, please choose another one.\n");
             
            system("cls"); // Clear screen
            return;
        }
        currentCustomer = currentCustomer->next;
    }

    // Create a new customer node
    Customer *newCustomer = (Customer *)malloc(sizeof(Customer));
    strcpy(newCustomer->username, username);
    strcpy(newCustomer->password, password);
//    strcpy(newCustomer->customer_type, customer_type);
    newCustomer->orders = NULL;
    newCustomer->next = customerList;
    customerList = newCustomer;

    // Save customer information to file
    saveCustomersToFile();

    printf("Sign up successful!\n");
    printf("Press any key to continue...\n");
      // Wait for user input
    system("cls"); // Clear screen
}

// Login function for customers
void customerLogin() {
    char username[50], password[50];
    printf("Enter username:\n");
    scanf("%s", username);
    printf("Enter password:\n");
    scanf("%s", password);

    // Check if the username and password match
    Customer *currentCustomer = customerList;
    while (currentCustomer != NULL) {
        if (strcmp(currentCustomer->username, username) == 0 && strcmp(currentCustomer->password, password) == 0) {
            printf("Login successful!\n");
            printf("Press any key to continue...\n");
              // Wait for user input
            system("cls"); // Clear screen
            // Set the current username
            strcpy(currentUsername, username);
            customerSpecificFunction();
            return;
        }
        currentCustomer = currentCustomer->next;
    }

    printf("Incorrect username or password.\n");
    printf("Press any key to continue...\n");
      // Wait for user input
    system("cls"); // Clear screen
    customerFunction(); // Redisplay customer function
}

// Customer-specific functionality
void customerSpecificFunction() {
    int choice;
    printf("Welcome, customer!\n");
    printf("Please select an option:\n");
    printf("0. Go back\n");
    printf("1. Display menu\n");
    printf("2. Place an order\n");
    printf("3. View my orders\n");
    printf("4. Cancel an order\n");
    printf("5. Change my password\n");
    printf("6. Query dishes by category\n");
    printf("7. Query dishes by price range\n");
    printf("Enter the option number and press Enter:\n");

    scanf("%d", &choice);
    switch (choice) {
        case 1:
            system("cls"); // Clear screen
            displayMenu(); // Display menu
             
            system("cls"); // Clear screen
            break;
        case 2:
            system("cls"); // Clear screen
            // Call the place order function
            placeOrder();
            break;
        case 3:
            system("cls"); // Clear screen
            displayCustomerOrders();
             
            system("cls"); // Clear screen
            break;
        case 4:
            system("cls"); // Clear screen
            cancelOrder();
             
            system("cls"); // Clear screen
            break;
        case 5:
            system("cls"); // Clear screen
            changeCustomerPassword();
             
            system("cls"); // Clear screen
            break;
        case 6:
            system("cls"); // Clear screen
            queryMenuByCategory();
             
            system("cls"); // Clear screen
            break;
        case 7:
            system("cls"); // Clear screen
            queryMenuByPriceRange();
             
            system("cls"); // Clear screen
            break;
        case 0:
            system("cls"); // Clear screen
            return;
            break;
        default:
            printf("Invalid option, please choose again.\n");
             
            system("cls"); // Clear screen
    }
    customerSpecificFunction(); // Redisplay customer-specific function
}

// Query dishes within a specified price range
void queryMenuByPriceRange() {
    double minPrice, maxPrice;
    printf("Enter the minimum and maximum values of the price range, separated by a space:\n");
    scanf("%lf %lf", &minPrice, &maxPrice);

    // Open the menu file
    FILE *file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("Unable to open the menu file.\n");
        return;
    }

    // Read and check the price of each dish
    char line[256];
    printf("Dishes within the price range of %.2lf to %.2lf are as follows:\n", minPrice, maxPrice);
    while (fgets(line, sizeof(line), file) != NULL) {
        int id;
        char name[50], category[50];
        double price;
        if (sscanf(line, "%d %s %lf %s", &id, name, &price, category) == 4) {
            if (price >= minPrice && price <= maxPrice) {
                printf("Dish ID: %d, Name: %s, Price: %.2lf, Category: %s\n", id, name, price, category);
            }
        }
    }

    fclose(file);
}

// Comparison function for sorting by price, used for qsort
int comparePrice(const void *a, const void *b) {
    const Dish *dishA = (const Dish *)a;
    const Dish *dishB = (const Dish *)b;
    if (dishA->price < dishB->price) return -1;
    if (dishA->price > dishB->price) return 1;
    return 0;
}

// Sort menu items by price
void sortMenuByPrice() {
    // Open the menu file
    FILE *file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("Unable to open the menu file.\n");
        return;
    }

    // Read menu items and store them in a linked list
    Dish *head = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        Dish *newDish = (Dish *)malloc(sizeof(Dish));
        if (newDish == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }
        if (sscanf(line, "%d %s %lf %s", &newDish->id, newDish->name, &newDish->price, newDish->category) == 4) {
            newDish->next = head;
            head = newDish;
        }
    }
    fclose(file);

    // Calculate the number of menu items
    int itemCount = 0;
    for (Dish *current = head; current != NULL; current = current->next) {
        itemCount++;
    }
    Dish *menuItems = (Dish *)malloc(itemCount * sizeof(Dish));
    if (menuItems == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    int i = 0;
    for (Dish *current = head; current != NULL; current = current->next) {
        menuItems[i++] = *current;
    }

    // Use quicksort to sort by price
    qsort(menuItems, itemCount, sizeof(Dish), comparePrice);

    // Reopen the menu file to write the sorted menu items
    file = fopen("menu.txt", "w");
    if (file == NULL) {
        printf("Unable to open the menu file.\n");
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d %s %.2lf %s\n", menuItems[i].id, menuItems[i].name, menuItems[i].price, menuItems[i].category);
    }
    fclose(file);

    // Free dynamically allocated memory
    while (head != NULL) {
        Dish *temp = head;
        head = head->next;
        free(temp);
    }

    printf("Menu items have been sorted by price and written back to the file.\n");
}

// Query dishes of a specified category
void queryMenuByCategory() {
    char category[50];
    printf("Enter the category of dishes to query:\n");
    scanf("%s", category);

    // Open the menu file
    FILE *file = fopen("menu.txt", "r");
    if (file == NULL) {
        printf("Unable to open the menu file.\n");
        return;
    }

    // Read and check the category of each dish
    char line[256];
    printf("The following are dishes in the category %s:\n", category);
    while (fgets(line, sizeof(line), file) != NULL) {
        int id;
        char name[50], cat[50];
        double price;
        if (sscanf(line, "%d %s %lf %s", &id, name, &price, cat) == 4) {
            if (strcmp(cat, category) == 0) {
                printf("Dish ID: %d, Name: %s, Price: %.2lf, Category: %s\n", id, name, price, cat);
            }
        }
    }

    fclose(file);
}

void displayCustomerOrders() {
    printf("Your order information:\n");

    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("Unable to open the order file.\n");
        return;
    }

    char line[256];
    double totalOrderPrice = 0.0;       // Total order price
    int orderCount = 0;                  // Order counter

    while (fgets(line, sizeof(line), file) != NULL) {
        char customerName[50], deliveryStatus[20];
        int orderID;

        // Parse order information
        if (sscanf(line, "%s %s %d", customerName, deliveryStatus, &orderID) == 3) {
            if (strcmp(customerName, currentUsername) == 0) {
                printf("Order ID: %d\n", orderID);
                printf("Delivery Status: %s\n", deliveryStatus);

                char itemName[50];
                int quantity;
                double price;
                double orderPrice = 0.0; // Current order price

                while (fgets(line, sizeof(line), file) != NULL) {
                    // Parse item information
                    if (sscanf(line, "%s %d %lf", itemName, &quantity, &price) == 3) {
                        printf("Item Name: %s, Quantity: %d, Price: %.2lf\n", itemName, quantity, price);
                        orderPrice += quantity * price; // Calculate current order price
                    } else if (strcmp(itemName, "end") == 0) {
                        printf("Current Order Price: %.2lf\n", orderPrice);
                        printf("-----------------\n");
                        break;
                    }
                }

                // Calculate total price
                totalOrderPrice += orderPrice;

                // Increment order count
                orderCount++;
            }
        }
    }

    fclose(file);

    // Print total price
    printf("Total price for your %d orders: %.2lf\n", orderCount, totalOrderPrice);

    printf("Checkout? (1/0)\n");
    int c;
    scanf("%d", &c);
    if (c == 1) {
        printf("Checkout successful.\n");
    } else {
        printf("Checkout next time.\n");
    }
}

// Write order information to file
void addOrderToFile(char *customerName, char *deliveryStatus, int orderID, MenuItem *items) {
    // Allocate memory for the order
    Order *newOrder = (Order *)malloc(sizeof(Order));
    if (newOrder == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    strcpy(newOrder->customerName, customerName);
    strcpy(newOrder->deliveryStatus, deliveryStatus);
    newOrder->orderID = orderID;
    newOrder->items = items;
    newOrder->next = NULL;

    // Open the order file
    FILE *file = fopen("orders.txt", "a");
    if (file == NULL) {
        printf("Unable to open the order file.\n");
        free(newOrder);
        return;
    }

    // Write order information to the file
    fprintf(file, "%s %s %d\n", customerName, deliveryStatus, orderID);
    MenuItem *currentItem = items;
    while (currentItem != NULL) {
        fprintf(file, "%s %d %.2lf\n", currentItem->name, currentItem->quantity, currentItem->price);
        currentItem = currentItem->next;
    }
    fprintf(file, "end\n");

    fclose(file);
}

// Place an order and write it to file
void placeOrder() {
    // Get menu item information
    MenuItem *items = getMenuItem();

    // Initialize order status as undelivered
    char deliveryStatus[20] = "Undelivered";

    // Get the current time as the order ID
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int orderID = (timeinfo->tm_year + 1900) * 10000000000 + (timeinfo->tm_mon + 1) * 100000000 + timeinfo->tm_mday * 1000000 + timeinfo->tm_hour * 10000 + timeinfo->tm_min * 100 + timeinfo->tm_sec;

    // Write the order to the file
    addOrderToFile(currentUsername, deliveryStatus, orderID, items);

    // Clear the current order
    currentOrder = NULL;

    // Free dynamically allocated memory
    MenuItem *temp;
    while (items != NULL) {
        temp = items;
        items = items->next;
        free(temp);
    }
}


