#include <iostream>
#include <map>
#include <vector>
using namespace std;

//  CashMachine Class
class CashMachine {
private:
    map<int, int, greater<int>> cashinventory; // Stores denominations in descending order using greater int comparator

public:
    CashMachine() {
        // Initialize available denominations 
        cashinventory[2000] = 0;
        cashinventory[500] = 0;
        cashinventory[200] = 0;
        cashinventory[100] = 0;
    }

    // Add money to the ATM
    void addMoney( map<int, int>& topUp) {
        for (const auto& entry : topUp) {
            int note = entry.first;
            int count = entry.second;

            if (cashinventory.find(note) != cashinventory.end()) {
                cashinventory[note] += count;
            } else {
                cout << "Invalid or banned denomination: " << note << endl;
            }
        }
    }

    // Display the total cash and denominations in the ATM
    void displayAmount() const {
        long totalAmount = 0;
        cout << "Current ATM Inventory:\n";
        for (auto entry : cashinventory) {
            cout << "Denomination: " << entry.first 
                 << ", Count: " << entry.second << endl;
            totalAmount += entry.first * entry.second;
        }
        cout << "Total Cash in ATM: ₹" << totalAmount << endl;
    }

    // Withdraw cash from the ATM
    bool getCash(int amount, map<int, int>& dispensedNotes) {
        if (amount <= 0) {
            cout << "Invalid amount. Please enter a positive value.\n";
            return false;
        }

        if (!isMultipleAvailable(amount)) {
            cout << "The amount is not a multiple of available denominations.\n";
            return false;
        }

        dispensedNotes.clear();
        int remainingAmount = amount;

        for (auto entry : cashinventory) {
            int note = entry.first;
            int availableNotes = entry.second;
            int requiredNotes = remainingAmount / note;

            if (requiredNotes > 0 && availableNotes > 0) {
                int notesToDispense = min(requiredNotes, availableNotes);
                dispensedNotes[note] = notesToDispense;
                remainingAmount -= notesToDispense * note;
                cashInventory[note] -= notesToDispense;
            }
        }

        if (remainingAmount > 0) {
            cout << "Insufficient balance in the ATM.\n";
            return false;
        }

        return true;
    }

private:
    // Check if the amount is a multiple of available denominations
    bool isMultipleAvailable(int amount) const {
        for ( auto& entry : cashInventory) {
            if (amount % entry.first == 0) {
                return true;
            }
        }
        return false;
    }
};

//  ATM Class
class ATM {
private:
    CashMachine* cashMachine;

public:
    ATM(CashMachine* machine) : cashMachine(machine) {}

    

    void withdrawAmount(int amount) {
        map<int, int> withdrawnNotes;
        if (cashMachine->getCash(amount, withdrawnNotes)) {
            cout << "Transaction Successful. Dispensed Notes:\n";
            for (const auto& entry : withdrawnNotes) {
                cout << "Denomination: " << entry.first 
                     << ", Count: " << entry.second << endl;
            }
        } else {
            cout << "Transaction Failed. Please check the error message above.\n";
        }
    }

    void displayAmount() const {
        cashMachine->displayAmount();
    }
};

// UserService Class
class UserService {
private:
    CashMachine cashMachine;
    ATM atm;

public:
    UserService() : atm(&cashMachine) {
        // Initialize the ATM with some cash
        map<int, int> initialCash = {
            {2000, 10}, // ₹2000 x 10
            {500, 10},  // ₹500 x 10
            {200, 11},  // ₹200 x 11
            {100, 20}   // ₹100 x 20
        };
        atm.addCashToMachine(initialCash);
    }

    void withdrawMoney(int amount) {
        atm.withdrawAmount(amount);
    }

    void showATMInventory()  {
        atm.displayAmount();
    }
};

//  Main Function
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
            userService.withdrawMoney(amount);
            break;
        }
        case 2:
            userService.showATMInventory();
            break;
        case 3:
            cout << "Exiting. Thank you for using the ATM!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    return 0;
}
