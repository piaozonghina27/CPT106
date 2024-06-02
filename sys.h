#define  _CRT_SECURE_NO_WARNINGS 1
#ifndef __SYS_H__
#define __SYS_H__

#include <cstddef>
// Struct for representing item information in an order
typedef struct MenuItem {
    char name[50];          // Item name
    int quantity;           // Item quantity
    double price;           // Item price
    double numIngredients; // Number of ingredients
    struct MenuItem *next;  // Pointer to the next item node
} MenuItem;

// Struct for representing order information
typedef struct Order {
    char customerName[50];  // Customer name
    char deliveryStatus[20];// Delivery status
    int orderID;            // Order ID
    MenuItem *items;        // Pointer to the first item node in the order
    struct Order *next;     // Pointer to the next order node
} Order;

// Struct for representing customers
typedef struct Customer {
    char username[50];          // Username
    char password[50];          // Password
    char customer_type[50];     // Customer type, can be extended
    Order *orders;              // Head pointer of the order linked list
    struct Customer *next;      // Pointer to the next customer
} Customer;

// Struct for representing administrators
typedef struct Admin {
    char username[50];    // Username
    char password[50];    // Password
    struct Admin *next;   // Pointer to the next administrator
} Admin;

// Struct for representing dishes
typedef struct Dish {
    int id;             // Dish ID
    char name[50];      // Dish name
    double price;       // Dish price
    int quantity;       // Dish quantity
    char category[50];  // Dish category
    double numIngredients; // Number of ingredients
    struct Dish *next;  // Pointer to the next dish
} Dish;

// Enum type for user identity
typedef enum {
    ADMIN,
    CUSTOMER,
    DELIVERY_PERSON
} UserType;

// Struct for representing discount information node
typedef struct DiscountNode {
    char type[20];      // User type
    double discount;    // Discount
    struct DiscountNode *next; // Pointer to the next node
} DiscountNode;

// Struct for dish sales
typedef struct DishSales {
    char name[50];  // Dish name
    int quantity;   // Sales quantity
    struct DishSales *next;  // Pointer to the next dish sales node
} DishSales;


// Customer function declarations
void loadCustomersFromFile();
void saveCustomersToFile();
void customerFunction();
void customerSignUp();
void customerLogin();
void customerSpecificFunction();
void placeOrder();
void queryMenuByCategory();
void sortMenuByPrice();

// Administrator function declarations
void loadAdminsFromFile();
void saveAdminsToFile();
void adminFunction();
void adminSignUp();
void adminLogin();
void adminSpecialFunction();
void changeAdminPassword();
void displayAllOrders();
void displayCustomerOrders();
void initializeDiscounts();
void cancelOrder();
void modifyDiscounts();
void changeCustomerPassword();
void queryMenuByPriceRange();
void printDishSales();
void getMoney();

// Dish function declarations
void loadMenuFromFile();
void saveMenuToFile();
void addDish();
void deleteDish();
void displayMenu();
void modifyMenuItem();
double readPriceFromFile(char *itemName);
void welcomeScreen();

// Struct for representing discount information
typedef struct Discount {
    char customer_type[50]; // Customer type
    double discount_rate;   // Discount rate
    struct Discount *next;  // Pointer to the next discount information
} Discount;
double getDiscountRate(char *customerType);

// Global variables for storing the head pointer of the current order and the head pointer of the discount information linked list
extern Order *currentOrder;
extern DiscountNode *discountList;

// Global variables for storing the current login user's identity and username
extern UserType currentUserType;
extern char currentUsername[50];

// Global variable for storing the head pointer of the menu linked list
extern Dish *menuList;

// Global variables for storing the head pointer of the administrator linked list and the head pointer of the customer linked list
extern Admin *adminList;
extern Customer *customerList;
extern int ID;

// Function declarations for deleting and modifying customers, checking if an order is in transit, and getting menu items
void deleteCustomer();
void modifyCustomer();
int isOrderInTransit(int orderID);
MenuItem* getMenuItem();

#endif

