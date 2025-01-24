#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <regex>
#include <limits>

struct Transaction {
    std::string date;
    std::string category;
    double amount;
    std::string type; // "Income" or "Expense"
};

class FinanceTracker {
private:
    std::vector<Transaction> transactions;
    double totalIncome = 0.0;
    double totalExpense = 0.0;

    void updateTotals(const Transaction& transaction) {
        if (transaction.type == "Income") {
            totalIncome += transaction.amount;
        }
        else {
            totalExpense += transaction.amount;
        }
    }

    bool isValidDate(const std::string& date) {
        std::regex datePattern("\\d{4}-\\d{2}-\\d{2}");
        if (!std::regex_match(date, datePattern)) return false;

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (month < 1 || month > 12) return false;

        const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        int maxDay = daysInMonth[month - 1];

        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
            maxDay = 29;
        }

        return day >= 1 && day <= maxDay;
    }

    bool isValidType(const std::string& type) {
        return type == "Income" || type == "Expense";
    }

public:
    void addTransaction() {
        std::string date, category, type;
        double amount;

        do {
            std::cout << "Enter date (YYYY-MM-DD): ";
            std::cin >> date;
            if (!isValidDate(date)) {
                std::cout << "Invalid date. Please ensure the format is YYYY-MM-DD and the date is valid.\n";
            }
        } while (!isValidDate(date));

        std::cout << "Enter category (e.g., Food, Rent, Salary): ";
        std::cin >> category;

        do {
            std::cout << "Enter amount: ";
            std::cin >> amount;
            if (std::cin.fail() || amount <= 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid amount. Please enter a positive number.\n";
            }
        } while (std::cin.fail() || amount <= 0);

        do {
            std::cout << "Enter type (Income/Expense): ";
            std::cin >> type;
            if (!isValidType(type)) {
                std::cout << "Invalid type. Please enter 'Income' or 'Expense'.\n";
            }
        } while (!isValidType(type));

        Transaction transaction = { date, category, amount, type };
        transactions.push_back(transaction);
        updateTotals(transaction);
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            return;
        }

        file << "Date,Category,Amount,Type\n";
        for (const auto& transaction : transactions) {
            file << transaction.date << "," << transaction.category << ","
                << transaction.amount << "," << transaction.type << "\n";
        }
        file.close();
        std::cout << "Transactions saved to " << filename << "\n";
    }

    void displaySummary() {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nFinancial Summary:\n";
        std::cout << "Total Income: $" << totalIncome << "\n";
        std::cout << "Total Expense: $" << totalExpense << "\n";
        std::cout << "Net Savings: $" << (totalIncome - totalExpense) << "\n";
    }

    void displayTransactions() {
        std::cout << "\nAll Transactions:\nDate       Category       Amount       Type\n";
        std::cout << "---------------------------------------------\n";
        for (const auto& t : transactions)
            std::cout << std::setw(10) << t.date << "  " << std::setw(12) << t.category << "  " << std::setw(8) << t.amount << "  " << std::setw(8) << t.type << "\n";
    }
};

int main() {
    FinanceTracker tracker;
    int choice;

    do {
        std::cout << "\nPersonal Finance Tracker\n";
        std::cout << "1. Add Transaction\n";
        std::cout << "2. Display Summary\n";
        std::cout << "3. Display Transactions\n";
        std::cout << "4. Save to File\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Please enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice) {
        case 1:
            tracker.addTransaction();
            break;
        case 2:
            tracker.displaySummary();
            break;
        case 3:
            tracker.displayTransactions();
            break;
        case 4: {
            std::string filename;
            std::cout << "Enter filename to save: ";
            std::cin >> filename;
            tracker.saveToFile(filename);
            break;
        }
        case 5:
            std::cout << "Exiting program. Goodbye!\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
