//Ahmed Baha Eddine Allimi
#include <bits/stdc++.h>
 
using namespace std;
 
// Book class representing a book entity
class Book {
private:
    string title;
    string author;
    double price;
 
public:
    Book(const string& title, const string& author, double price)
        : title(title), author(author), price(price) {}
 
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    double getPrice() const { return price; }
 
    // Function to update the price of the book
    void updatePrice(double newPrice) {
        price = newPrice;
    }
};
 
// Proxy class representing a proxy for the Book class
class BookProxy {
private:
    shared_ptr<Book> book;
    bool premiumAccess;
 
public:
    BookProxy(shared_ptr<Book> book, bool premiumAccess) : book(book), premiumAccess(premiumAccess) {}
 
    const string& getTitle() const { return book->getTitle(); }
    const string& getAuthor() const { return book->getAuthor(); }
    double getPrice() const { return book->getPrice(); }
 
    // Function to update the price of the book
    void updatePrice(double newPrice) {
        book->updatePrice(newPrice);
    }
 
    // Function to check if the user has premium access
    bool hasPremiumAccess() const {
        return premiumAccess;
    }
};
 
// User class representing a user entity
class User {
protected:
    string username;
 
public:
    User(const string& username) : username(username) {}
    virtual ~User() {}
    const string& getUsername() const { return username; }
};
 
// StandardUser class representing a standard user
class StandardUser : public User {
public:
    StandardUser(const string& username) : User(username) {}
};
 
// PremiumUser class representing a premium user
class PremiumUser : public User {
public:
    PremiumUser(const string& username) : User(username) {}
};
 
// UserFactory to create instances of User subclasses
class UserFactory {
public:
    static unique_ptr<User> createUser(const string& userType, const string& username) {
        if (userType == "standard") {
            return make_unique<StandardUser>(username);
        } else if (userType == "premium") {
            return make_unique<PremiumUser>(username);
        } else {
            return nullptr;
        }
    }
};
 
// SubscriptionNotifier interface for notifying users about book updates
class SubscriptionNotifier {
public:
    virtual ~SubscriptionNotifier() {}
    virtual void subscribe(const string& username) = 0;
    virtual void unsubscribe(const string& username) = 0;
    virtual void notifyAll(const string& bookTitle, const string& newPrice) = 0;
};
 
// Concrete implementation of SubscriptionNotifier using Observer Design Pattern
class BookUpdateNotifier : public SubscriptionNotifier {
private:
    unordered_map<string, vector<string>> subscriptions;
    queue<string> subscriberQueue; // Queue to maintain order of subscribers
 
public:
    void subscribe(const string& username) override {
        subscriptions[username] = {};
        subscriberQueue.push(username); // Add subscriber to the end of the queue
    }
 
    void unsubscribe(const string& username) override {
        subscriptions.erase(username);
        // Remove subscriber from the queue if they exist
        queue<string> tempQueue;
        while (!subscriberQueue.empty()) {
            string user = subscriberQueue.front();
            subscriberQueue.pop();
            if (user != username) {
                tempQueue.push(user);
            }
        }
        subscriberQueue = tempQueue;
    }
 
    void notifyAll(const string& bookTitle, const string& priceStr) override {
        queue<string> tempQueue = subscriberQueue; // Create a copy of the subscriber queue
        while (!tempQueue.empty()) {
            const string& username = tempQueue.front();
            // Output the price exactly as provided
            cout << username << " notified about price update for " << bookTitle << " to " << priceStr << endl;
            tempQueue.pop();
        }
    }
 
    bool isSubscribed(const string& username) const {
        return subscriptions.find(username) != subscriptions.end();
    }
};
 
// Bookstore class managing books and users
class Bookstore {
private:
    unordered_map<string, shared_ptr<BookProxy>> books;
    unordered_map<string, unique_ptr<User>> users;
    unique_ptr<SubscriptionNotifier> notifier;
 
public:
    Bookstore() : notifier(make_unique<BookUpdateNotifier>()) {}
 
    // Function to create a new book
    void createBook(const string& title, const string& author, double price) {
        if (books.find(title) != books.end()) {
            cout << "Book already exists" << endl;
        } else {
            auto book = make_shared<Book>(title, author, price);
            books[title] = make_shared<BookProxy>(book, false); // By default, no premium access
        }
    }
 
    // Function to create a new user
    void createUser(const string& userType, const string& username) {
        if (users.find(username) != users.end()) {
            cout << "User already exists" << endl;
        } else {
            auto user = UserFactory::createUser(userType, username);
            if (user) {
                users[username] = move(user);
            } else {
                cout << "Invalid user type" << endl;
            }
        }
    }
 
    // Function to subscribe a user for book updates
    void subscribe(const string& username) {
        if (users.find(username) != users.end()) {
            // Check if the user is already subscribed
            if (dynamic_cast<BookUpdateNotifier*>(notifier.get())->isSubscribed(username)) {
                cout << "User already subscribed" << endl;
            } else {
                notifier->subscribe(username);
            }
        }
    }
 
    // Function to unsubscribe a user from book updates
    void unsubscribe(const string& username) {
        if (users.find(username) != users.end()) {
            // Check if the user is subscribed before attempting to unsubscribe
            if (dynamic_cast<BookUpdateNotifier*>(notifier.get())->isSubscribed(username)) {
                notifier->unsubscribe(username);
            } else {
                cout << "User is not subscribed" << endl;
            }
        }
    }
 
    // Function to update the price of a book and notify subscribed users
    void updatePrice(const string& title, const string& priceStr) {
        if (books.find(title) != books.end()) {
            shared_ptr<BookProxy> bookProxy = books[title];
            // Convert the string price to double if necessary
            double newPrice = stod(priceStr);
            bookProxy->updatePrice(newPrice);
            notifier->notifyAll(title, priceStr);
        }
    }
 
    // Function to read a book
    void readBook(const string& username, const string& title) {
        if (books.find(title) != books.end() && users.find(username) != users.end()) {
            cout << username << " reading " << title << " by " << books[title]->getAuthor() << endl;
        }
    }
 
    // Function to listen to a book (only available for premium users)
    void listenBook(const string& username, const string& title) {
        if (books.find(title) != books.end() && users.find(username) != users.end()) {
            if (dynamic_cast<PremiumUser*>(users[username].get())) {
                cout << username << " listening " << title << " by " << books[title]->getAuthor() << endl;
            } else {
                cout << "No access" << endl;
            }
        }
    }
};
 
int main() {
    Bookstore bookstore;
 
    string command;
    while (getline(cin, command)) {
        if (command == "end") {
            break;
        }
 
        // Parsing the input command
        stringstream ss(command);
        string operation;
        ss >> operation;
 
        if (operation == "createBook") {
            string title, author;
            double price;
            ss >> title >> author >> price;
            bookstore.createBook(title, author, price);
        } else if (operation == "createUser") {
            string userType, username;
            ss >> userType >> username;
            bookstore.createUser(userType, username);
        } else if (operation == "subscribe") {
            string username;
            ss >> username;
            bookstore.subscribe(username);
        } else if (operation == "unsubscribe") {
            string username;
            ss >> username;
            bookstore.unsubscribe(username);
        } else if (operation == "updatePrice") {
            string title;
            string price;
            ss >> title >> price;
            bookstore.updatePrice(title, price);
        } else if (operation == "readBook") {
            string username, title;
            ss >> username >> title;
            bookstore.readBook(username, title);
        } else if (operation == "listenBook") {
            string username, title;
            ss >> username >> title;
            bookstore.listenBook(username, title);
        }
    }
 
    return 0;
}