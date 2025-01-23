#include <iostream>
#include <map>
using namespace std;

// Machine that manages cash storage and transactions
class CashManager {
private:
    map<int, int, greater<int>> cashInventory; // Keeps track of cash denominations in descending order

public:
    CashManager() {
        // Initialize the cash inventory with supported denominations
        cashInventory[2000] = 0;
        cashInventory[500] = 0;
        cashInventory[200] = 0;
        cashInventory[100] = 0;
    }

    // Add cash to the ATM
    void addCash(map<int, int>& topUp) {
        for (const auto& entry : topUp) {
            int denomination = entry.first;
            int count = entry.second;

            if (cashInventory.find(denomination) != cashInventory.end()) {
                cashInventory[denomination] += count;
            } else {
                cout << "Invalid denomination: " << denomination << ". Ignored.\n";
            }
        }
    }

    // Display the total cash and inventory
    void displayCashInventory() const {
        long totalCash = 0;
        cout << "ATM Cash Inventory:\n";
        for (const auto& entry : cashInventory) {
            cout << "Denomination: ₹" << entry.first 
                 << ", Count: " << entry.second << "\n";
            totalCash += entry.first * entry.second;
        }
        cout << "Total Cash in ATM: ₹" << totalCash << "\n";
    }

    // Withdraw the specified amount, returning notes in dispensedNotes
    bool withdrawCash(int amount, map<int, int>& dispensedNotes) {
        if (amount <= 0) {
            cout << "Invalid amount. Please enter a positive value.\n";
            return false;
        }

        if (!isMultipleOfDenominations(amount)) {
            cout << "The requested amount cannot be dispensed due to unavailable denominations.\n";
            return false;
        }

        dispensedNotes.clear();
        int remainingAmount = amount;

        for (auto& entry : cashInventory) {
            int denomination = entry.first;
            int availableNotes = entry.second;
            int requiredNotes = remainingAmount / denomination;

            if (requiredNotes > 0 && availableNotes > 0) {
                int notesToDispense = min(requiredNotes, availableNotes);
                dispensedNotes[denomination] = notesToDispense;
                remainingAmount -= notesToDispense * denomination;
                cashInventory[denomination] -= notesToDispense;
            }
        }

        if (remainingAmount > 0) {
            cout << "ATM does not have sufficient cash for this transaction.\n";
            return false;
        }

        return true;
    }

private:
    // Check if the amount can be formed using available denominations
    bool isMultipleOfDenominations(int amount) const {
        for (const auto& entry : cashInventory) {
            if (amount % entry.first == 0) {
                return true;
            }
        }
        return false;
    }
};

// A user-facing interface for interacting with the ATM
class ATM {
private:
    CashManager* cashManager;

public:
    ATM(CashManager* manager) : cashManager(manager) {}

    // Withdraw money from the ATM
    void withdrawMoney(int amount) {
        map<int, int> dispensedNotes;
        if (cashManager->withdrawCash(amount, dispensedNotes)) {
            cout << "Transaction Successful! Dispensed Notes:\n";
            for (const auto& entry : dispensedNotes) {
                cout << "Denomination: ₹" << entry.first 
                     << ", Count: " << entry.second << "\n";
            }
        } else {
            cout << "Transaction Failed. Please check the error above.\n";
        }
    }

    // Display the ATM's current inventory
    void showCashInventory() const {
        cashManager->displayCashInventory();
    }
};

// A service to initialize and manage ATM operations
class UserService {
private:
    CashManager cashManager;
    ATM atm;

public:
    UserService() : atm(&cashManager) {
        // Preload ATM with some cash
        map<int, int> initialCash = {
            {2000, 10}, // 2000 x 10
            {500, 10},  // 500 x 10
            {200, 10},  // 200 x 10
            {100, 20}   // 100 x 20
        };
        cashManager.addCash(initialCash);
    }

    void withdrawFromATM(int amount) {
        atm.withdrawMoney(amount);
    }

    void displayATMInventory() {
        atm.showCashInventory();
    }
};

// Main function to drive the ATM application
int main() {
    UserService userService;
    int choice;

    do {
        cout << "\n===== ATM Menu =====\n";
        cout << "1. Withdraw Money\n";
        cout << "2. Show ATM Inventory\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int amount;
                cout << "Enter the amount to withdraw: ";
                cin >> amount;
                userService.withdrawFromATM(amount);
                break;
            }
            case 2:
                userService.displayATMInventory();
                break;
            case 3:
                cout << "Thank you for using the ATM. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
