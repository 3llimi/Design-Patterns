//Ahmed Baha Eddine Alimi
#include <bits/stdc++.h>
 
using namespace std;
 
// Enum for account state
enum class AccountState { ACTIVE, INACTIVE };
 
// Transaction event
struct TransactionEvent {
    string eventDescription;
    double amount;
};
 
// Account interface
class Account {
 
public:
    Account(const string& name) : accountName(name), balance(0.0), state(AccountState::ACTIVE) {}
 
    // Virtual functions for deposit, initial deposit, withdraw and view details
    virtual void deposit(double amount) {
        balance += amount;
        history.push_back({"Deposit", amount});
    }
    virtual void intialDeposit(double amount) {
        balance += amount;
        history.push_back({"Initial Deposit", amount});
    }
    virtual bool withdraw(double amount) {
        if (balance >= amount && amount>=1 && amount<=5000) {
            balance -= amount;
            history.push_back({"Withdrawal", amount});
            return true;
        }
        return false;
    }
 
    virtual void viewDetails() const {
        cout << accountName << "'s Account: Type: " << getType() << ", Balance: $" << fixed << setprecision(3) << balance << ", State: " << (state == AccountState::ACTIVE ? "Active" : "Inactive") << ", Transactions: [";
        for (size_t i = 0; i < history.size(); ++i) {
            cout << history[i].eventDescription << " $" << fixed << setprecision(3) << history[i].amount;
            if (i != history.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]." << endl;
    }
 
    // Virtual function to get account type
    virtual string getType() const = 0;
 
    // Functions to deactivate and activate the account
    void deactivate() {
        state = AccountState::INACTIVE;
    }
 
    void activate() {
        state = AccountState::ACTIVE;
    }
 
    // Getters for balance, history, and state
    const vector<TransactionEvent>& getHistory() const {
        return history;
    }
 
    double getBalance() const {
        return balance;
    }
    double balance;
    AccountState state;
    vector<TransactionEvent> history;
 
    // Add clone method for Prototype pattern
    virtual Account* clone() const = 0;
 
    // Function to create a new instance by cloning
    Account* createClone() const {
        return this->clone();
    }
    string accountName;
};
 
// Concrete Account class
class ConcreteAccount : public Account {
private:
    string accountType;
 
public:
    ConcreteAccount(const string& name, const string& type) : Account(name), accountType(type) {
    }
 
    void deposit(double amount) override {
        Account::deposit(amount);
        cout << accountName << " successfully deposited $" << fixed << setprecision(3) << amount << ". New Balance: $" << balance << "." << endl;
    }
 
    bool withdraw(double amount) override {
        double transactionFee = amount * getTransactionFeePercentage();
        amount-=transactionFee;
        if (Account::withdraw(amount+transactionFee)) {
            cout << accountName << " successfully withdrew $" << fixed << setprecision(3) << amount<< ". New Balance: $" << balance << ". Transaction Fee: $" << fixed << setprecision(3) << transactionFee<< " (" << fixed << setprecision(1) << getTransactionFeePercentage() * 100 << "%) in the system." << endl;
            return true;
        } else {
            cout << "Error: Insufficient funds for " << accountName << "." << endl;
            return false;
        }
    }
 
    void viewDetails() const override {
        Account::viewDetails();
    }
 
    string getType() const override {
        return accountType;
    }
 
    // Virtual function to get transaction fee percentage
    virtual double getTransactionFeePercentage() const = 0;
 
    // Override clone method for Prototype pattern
    Account* clone() const override = 0;
};
 
// Savings Account class
class SavingsAccount : public ConcreteAccount {
public:
    SavingsAccount(const string& name) : ConcreteAccount(name, "Savings") {}
 
    double getTransactionFeePercentage() const override {
        return 0.015;
    }
 
    // Override clone method for Prototype pattern
    Account* clone() const override {
        return new SavingsAccount(*this);
    }
};
 
// Checking Account class
class CheckingAccount : public ConcreteAccount {
public:
    CheckingAccount(const string& name) : ConcreteAccount(name, "Checking") {}
 
    double getTransactionFeePercentage() const override {
        return 0.02;
    }
 
    // Override clone method for Prototype pattern
    Account* clone() const override {
        return new CheckingAccount(*this);
    }
};
 
// Business Account class
class BusinessAccount : public ConcreteAccount {
public:
    BusinessAccount(const string& name) : ConcreteAccount(name, "Business") {}
 
    double getTransactionFeePercentage() const override {
        return 0.025;
    }
 
    // Override clone method for Prototype pattern
    Account* clone() const override {
        return new BusinessAccount(*this);
    }
};
 
// Bank class
class Bank {
private:
    map<string, Account*> accounts;
 
public:
 
    // Destructor to delete dynamically allocated accounts
    ~Bank() {
        for (auto& pair : accounts) {
            delete pair.second;
        }
    }
 
    // Function to create an account
    void createAccount(const string& type, const string& name, double initialDeposit) {
        if (accounts.find(name) != accounts.end()) {
            cout << "Error: Account " << name << " already exists." << endl;
            return;
        }
 
        Account* accountPrototype = nullptr;
        if (type == "Savings") {
            accountPrototype = new SavingsAccount("PrototypeSavings");
        } else if (type == "Checking") {
            accountPrototype = new CheckingAccount("PrototypeChecking");
        } else if (type == "Business") {
            accountPrototype = new BusinessAccount("PrototypeBusiness");
        } else {
            cout << "Error: Invalid account type." << endl;
            return;
        }
 
        Account* account = accountPrototype->createClone();
        delete accountPrototype;
 
        account->intialDeposit(initialDeposit);
        account->accountName = name;
        accounts[name] = account;
        cout << "A new " << type << " account created for " << name << " with an initial balance of $" << fixed << setprecision(3) << initialDeposit << "." << endl;
    }
 
//Function to deposit money into an account
    void deposit(const string& accountName, double amount) {
        auto it = accounts.find(accountName);
        if (it != accounts.end()) {
            it->second->deposit(amount);
        } else {
            cout << "Error: Account " << accountName << " does not exist."<<endl ;
        }
    }
 
    //Function to withdraw money from an account
    void withdraw(const string& accountName, double amount) {
        auto it = accounts.find(accountName);
        if (it != accounts.end()) {
            if (it->second->state == AccountState::ACTIVE) { // Check if account is active
                it->second->withdraw(amount);
            } else {
                cout << "Error: Account " << accountName << " is inactive."<<endl ;
            }
        } else {
            cout << "Error: Account " << accountName << " does not exist."<<endl ;
        }
    }
 
    //Function to transfer money between two accounts
    void transfer(const string& fromAccountName, const string& toAccountName, double amount) {
        auto itFrom = accounts.find(fromAccountName);
        auto itTo = accounts.find(toAccountName);
 
        if (itFrom == accounts.end()) {
            cout << "Error: Account " << fromAccountName << " does not exist."<<endl ;
            return;
        }
 
        if (itTo == accounts.end()) {
            cout << "Error: Account " << toAccountName << " does not exist."<<endl ;
            return;
        }
 
        if (itFrom->second->state != AccountState::ACTIVE) {
            cout << "Error: Account " << fromAccountName << " is inactive."<<endl ;
            return;
        }
 
        ConcreteAccount* concreteFromAccount = dynamic_cast<ConcreteAccount*>(itFrom->second);
 
        if (concreteFromAccount) {
            double transactionFeePercentage = concreteFromAccount->getTransactionFeePercentage();
            double transactionFee = amount * transactionFeePercentage;
            double totalAmount = amount;
 
            if (concreteFromAccount->getBalance() >= totalAmount) {
                concreteFromAccount->balance -= totalAmount;
                itTo->second->balance += amount-transactionFee;
                // Record transfer in sender's history
                dynamic_cast<ConcreteAccount*>(itFrom->second)->history.push_back({"Transfer", amount});
 
                cout << fromAccountName << " successfully transferred $" << fixed << setprecision(3) << amount-transactionFee << " to " << toAccountName << ". New Balance: $" << concreteFromAccount->getBalance() << "." << " Transaction Fee: $" << fixed << setprecision(3) << transactionFee << " (" << fixed << setprecision(1) << transactionFeePercentage * 100 << "%) in the system."<<endl ;
            } else {
                cout << "Error: Insufficient funds for " << fromAccountName << "."<<endl ;
            }
        }
    }
 
 
    //Function to view account details
    void view(const string& accountName) {
        auto it = accounts.find(accountName);
        if (it != accounts.end()) {
            it->second->viewDetails();
        } else {
            cout << "Error: Account " << accountName << " does not exist."<<endl ;
        }
    }
 
    //Function to deactivate account
    void deactivate(const string& accountName) {
        auto it = accounts.find(accountName);
        if (it != accounts.end()) {
            if (it->second->state == AccountState::ACTIVE) {
                it->second->state = AccountState::INACTIVE;
                cout << accountName << "'s account is now deactivated."<<endl ;
            } else {
                cout << "Error: Account " << accountName << " is already deactivated."<<endl ;
            }
        } else {
            cout << "Error: Account " << accountName << " does not exist."<<endl ;
        }
    }
 
    //Function to activate account
    void activate(const string& accountName) {
        auto it = accounts.find(accountName);
        if (it != accounts.end()) {
            if (it->second->state == AccountState::INACTIVE) {
                it->second->state = AccountState::ACTIVE;
                cout << accountName << "'s account is now activated."<<endl ;
            } else {
                cout << "Error: Account " << accountName << " is already activated."<<endl ;
            }
        } else {
            cout << "Error: Account " << accountName << " does not exist."<<endl ;
        }
    }
};
 
// Façade class
class BankFacade {
private:
    Bank bank;
 
public:
    void createAccount(const string& accountType, const string& accountName, double initialDeposit) {
        bank.createAccount(accountType, accountName, initialDeposit);
    }
 
    void deposit(const string& accountName, double amount) {
        bank.deposit(accountName, amount);
    }
 
    void withdraw(const string& accountName, double amount) {
        bank.withdraw(accountName, amount);
    }
 
    void transfer(const string& fromAccountName, const string& toAccountName, double amount) {
        bank.transfer(fromAccountName, toAccountName, amount);
    }
 
    void view(const string& accountName) {
        bank.view(accountName);
    }
 
    void deactivate(const string& accountName) {
        bank.deactivate(accountName);
    }
 
    void activate(const string& accountName) {
        bank.activate(accountName);
    }
};
 
int main() {
    int n;
    cin >> n;
 
    BankFacade bankFacade;
 
    // Loop to perform bank operations
    for (int i = 0; i < n; ++i) {
        string operation, accountType, accountName, toAccountName;
        double amount, initialDeposit;
        cin >> operation;
 
        if (operation == "Create") {
            string createCommand;
            cin >> createCommand >> accountType >> accountName >> initialDeposit;
            if (createCommand == "Account") {
                if (accountType == "Savings" || accountType == "Checking" || accountType == "Business") {
                    bankFacade.createAccount(accountType, accountName, initialDeposit);
                } else {
                    cout << "Error: Invalid account type." << endl;
                }
            } else {
                cout << "Error: Invalid command for creating account." << endl;
            }
        } else if (operation == "Deposit") {
            cin >> accountName >> amount;
            bankFacade.deposit(accountName, amount);
        } else if (operation == "Withdraw") {
            cin >> accountName >> amount;
            bankFacade.withdraw(accountName, amount);
        } else if (operation == "Transfer") {
            cin >> accountName >> toAccountName >> amount;
            bankFacade.transfer(accountName, toAccountName, amount);
        } else if (operation == "View") {
            cin >> accountName;
            bankFacade.view(accountName);
        } else if (operation == "Deactivate") {
            cin >> accountName;
            bankFacade.deactivate(accountName);
        } else if (operation == "Activate") {
            cin >> accountName;
            bankFacade.activate(accountName);
        }
    }
    return 0;
}