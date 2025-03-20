#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Class to represent a menu item
class MenuItem {
public:
    MenuItem() : name(""), price(0.0), quantity(0) {}
    MenuItem(const string& name, double price, int quantity) : name(name), price(price), quantity(quantity) {}

    // Getter functions
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    void useItem(int amount) { quantity -= amount; }

private:
    string name;
    double price;
    int quantity;
};

// Class to represent a category of menu items
class Category {
public:
    Category() : name("") {}
    Category(const string& name) : name(name) {}

    // Add a menu item to the category
    void addItem(const MenuItem& item) {
        items.push_back(item);
    }

    // Display the category and its items
    void displayCategory() const {
        cout << "Category: " << name << endl;
        cout << setw(20) << left << "Item" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
        cout << setfill('-') << setw(40) << "" << setfill(' ') << endl;

        for (const auto& item : items) {
            cout << setw(20) << left << item.getName() << setw(10) << item.getPrice() << setw(10) << item.getQuantity() << endl;
        }
    }

    MenuItem& getItem(size_t index) {
        return items.at(index);
    }

    size_t getSize() const {
        return items.size();
    }

private:
    string name;
    vector<MenuItem> items;
};

// Class to represent the menu, which contains categories of items
class Menu {
public:
    Menu() {
        // Initialize categories and items
        Category beverages("Beverages");
        beverages.addItem(MenuItem("Coffee", 2.5, 50));
        beverages.addItem(MenuItem("Tea", 2.0, 50));

        Category snacks("Snacks");
        snacks.addItem(MenuItem("Sandwich", 5.0, 30));
        snacks.addItem(MenuItem("Cake", 3.5, 20));

        Category fastFood("Fast Food");
        fastFood.addItem(MenuItem("Burger", 7.0, 25));
        fastFood.addItem(MenuItem("Pizza", 8.0, 15));

        categories["Beverages"] = beverages;
        categories["Snacks"] = snacks;
        categories["Fast Food"] = fastFood;
    }

    // Display all categories and their items
    void displayMenu() const {
        for (const auto& category : categories) {
            category.second.displayCategory();
            cout << endl;
        }
    }

    MenuItem& getItem(const string& category, size_t index) {
        return categories.at(category).getItem(index);
    }

    bool isValidCategory(const string& category) const {
        return categories.find(category) != categories.end();
    }

private:
    map<string, Category> categories;
};

// Class to represent a customer's order
class Order {
public:
    Order() : size(0) {}

    // Add an item to the order
    void addItem(const MenuItem& item) {
        if (size < maxItems) {
            items[size++] = item;
        }
    }

    // Calculate the total price of the order
    double calculateTotal() const {
        double total = 0.0;
        for (size_t i = 0; i < size; ++i) {
            total += items[i].getPrice();
        }
        return total;
    }

    // Display the order details
    void displayOrder() const {
        cout << "Your Order:" << endl;
        for (size_t i = 0; i < size; ++i) {
            cout << items[i].getName() << " - $" << items[i].getPrice() << endl;
        }
        cout << "Total: $" << fixed << setprecision(2) << calculateTotal() << endl;
    }

    size_t getSize() const {
        return size;
    }

    void clear() {
        size = 0;
    }

    bool isEmpty() const {
        return size == 0;
    }

private:
    static const size_t maxItems = 10;
    MenuItem items[maxItems];
    size_t size;
};

// Class to manage the cafe, including taking orders and processing payments
class Cafe {
public:
    Cafe() : orderCount(0) {}

    // Start the cafe management system
    void start() {
        int choice;
        do {
            displayOptions();
            cin >> choice;

            switch (choice) {
            case 1:
                menu.displayMenu();
                break;
            case 2:
                takeOrder();
                break;
            case 3:
                if (!currentOrder.isEmpty()) {
                    processPayment();
                }
                else {
                    cout << "No items in the current order." << endl;
                }
                break;
            case 4:
                viewOrders();
                break;
            case 5:
                cout << "Exiting the cafe management system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        } while (choice != 5);
    }

private:
    Menu menu;
    Order currentOrder;
    Order orders[10];
    size_t orderCount;

    // Display the main menu options
    void displayOptions() const {
        cout << "\nCafe Management System" << endl;
        cout << "1. Display Menu" << endl;
        cout << "2. Take Order" << endl;
        cout << "3. Process Payment" << endl;
        cout << "4. View Orders" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
    }

    // Take an order from the customer
    void takeOrder() {
        string category;
        int choice;
        do {
            menu.displayMenu();
            cout << "Enter the category of the item you want to order: ";
            cin.ignore();
            getline(cin, category);

            if (!menu.isValidCategory(category)) {
                cout << "Invalid category. Please try again." << endl;
                continue;
            }

            do {
                cout << "Enter the number of the item you want to order (0 to finish): ";
                cin >> choice;

                if (choice >= 1 && static_cast<size_t>(choice) <= menu.getItem(category, choice - 1).getQuantity()) {
                    currentOrder.addItem(menu.getItem(category, choice - 1));
                    menu.getItem(category, choice - 1).useItem(1);
                    cout << "Added " << menu.getItem(category, choice - 1).getName() << " to your order." << endl;
                }
                else if (choice != 0) {
                    cout << "Invalid choice. Please try again." << endl;
                }
            } while (choice != 0);

        } while (!menu.isValidCategory(category));
    }

    // Process the payment for the current order
    void processPayment() {
        double total = currentOrder.calculateTotal();
        cout << "Total amount to pay: $" << fixed << setprecision(2) << total << endl;
        currentOrder.displayOrder();

        if (orderCount < 10) {
            orders[orderCount++] = currentOrder;
        }
        else {
            cout << "Maximum order limit reached." << endl;
        }

        currentOrder.clear();
        cout << "Payment processed. Order cleared." << endl;
    }

    // View all previous orders
    void viewOrders() const {
        cout << "Previous Orders:" << endl;
        for (size_t i = 0; i < orderCount; ++i) {
            cout << "Order " << i + 1 << ":" << endl;
            orders[i].displayOrder();
        }
    }
};

// Main function to start the cafe management system
int main() {
    Cafe cafe;
    cafe.start();

    return 0;
}
