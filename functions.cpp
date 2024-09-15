#include "functions.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

void displayMenu()
{
    cout << endl;
    cout << "********** Menu **********" << endl;
    cout << "1. Add Transaction" << endl;
    cout << "2. View Transactions" << endl;
    cout << "3. Delete Transaction" << endl;
    cout << "4. Update Transaction" << endl;
    cout << "5. View Balance" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose an option: ";
}

TransactionType stringToTransactionType(const string &typeStr)
{
    if (typeStr == "Income")
    {
        return TransactionType::INCOME;
    }
    else if (typeStr == "Expense")
    {
        return TransactionType::EXPENSE;
    }
    else
    {
        throw std::invalid_argument("Invalid transaction type");
    }
}

std::vector<Transaction> loadTransactions(const std::string &filename)
{
    std::vector<Transaction> transactions;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open())
    {
        ofstream newFile(filename);
        if (!newFile)
        {
            cerr << "Error creating file: " << filename << endl;
            return transactions; // empty vector
        }
        newFile << "id,description,amount,date,type\n";
        newFile.close();
        return transactions; // empty vector
    }

    // Skip header line
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, description, amountStr, date, typeStr;
        int id;
        double amount;

        getline(ss, idStr, ',');
        id = std::stoi(idStr);

        getline(ss, description, ',');

        getline(ss, amountStr, ',');
        amount = stod(amountStr);

        getline(ss, date, ',');

        getline(ss, typeStr, ',');

        Transaction transaction;
        transaction.id = id;
        transaction.description = description;
        transaction.amount = amount;
        transaction.date = date;
        transaction.type = stringToTransactionType(typeStr);

        transactions.push_back(transaction);
    }

    file.close();
    return transactions;
}

int getNextId(const vector<Transaction> &transactions)
{
    int maxId = 0;
    for (const auto &transaction : transactions)
    {
        if (transaction.id > maxId)
        {
            maxId = transaction.id;
        }
    }
    return maxId + 1;
}

bool isValidDate(const string &dateStr)
{
    if (dateStr.length() != 10 || dateStr[4] != '-' || dateStr[7] != '-')
        return false;

    int year, month, day;
    try
    {
        year = stoi(dateStr.substr(0, 4));
        month = stoi(dateStr.substr(5, 2));
        day = stoi(dateStr.substr(8, 2));
    }
    catch (const invalid_argument &)
    {
        return false;
    }

    if (month < 1 || month > 12)
        return false;

    if (month == 2)
    { // February
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        {
            return day >= 1 && day <= 29; // Leap year
        }
        else
        {
            return day >= 1 && day <= 28; // Non-leap year
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    { // April, June, September, November
        return day >= 1 && day <= 30;
    }
    else
    { // January, March, May, July, August, October, December
        return day >= 1 && day <= 31;
    }
}

void addTransaction(vector<Transaction> &transactions, const string &filename)
{
    Transaction transaction;
    transaction.id = getNextId(transactions);

    // Description
    cout << "Enter description: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, transaction.description);
    if (transaction.description.empty())
    {
        cout << "Description cannot be empty." << endl;
        return;
    }

    // Amount
    cout << "Enter amount: ";
    while (!(cin >> transaction.amount) || transaction.amount <= 0)
    {
        cout << "Invalid amount. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input buffer
    }

    // Date
    string dateStr;
    while (true)
    {
        cout << "Enter date (YYYY-MM-DD): ";
        cin.ignore(); // Clear the input buffer
        getline(cin, dateStr);
        if (isValidDate(dateStr))
        {
            transaction.date = dateStr;
            break;
        }
        else
        {
            cout << "Invalid date format or value. Please use YYYY-MM-DD and ensure the date is correct." << endl;
        }
    }

    // Type
    string typeStr;
    cout << "Enter type (Income/Expense): ";
    cin >> typeStr;
    try
    {
        transaction.type = stringToTransactionType(typeStr);
    }
    catch (const invalid_argument &)
    {
        cout << "Invalid transaction type. Please enter 'Income' or 'Expense'." << endl;
        return;
    }

    transactions.push_back(transaction);

    // Save to file
    ofstream file(filename, ios::app);
    if (file.is_open())
    {
        file << transaction.id << ","
             << transaction.description << ","
             << transaction.amount << ","
             << transaction.date << ","
             << (transaction.type == TransactionType::INCOME ? "Income" : "Expense") << "\n";
        file.close();
    }
    else
    {
        cout << "Error opening file for writing." << endl;
    }

    cout << "Transaction added successfully." << endl;
}

void viewTransactions(const vector<Transaction> &transactions)
{
    string choice;
    cout << "Do you want to view all transactions or search by id? (all/id): ";
    cin >> choice;
    if (choice == "all")
    {
        if (transactions.empty())
        {
            cout << "No transactions found." << endl;
            return;
        }
        else
        {
            cout << endl;
            cout << "********** Transactions **********" << endl;
            for (const Transaction transaction : transactions)
            {
                cout << "ID: " << transaction.id << endl;
                cout << "Description: " << transaction.description << endl;
                cout << "Amount: $" << transaction.amount << endl;
                cout << "Date: " << transaction.date << endl;
                cout << "Type: " << (transaction.type == TransactionType::INCOME ? "Income" : "Expense") << endl;
                cout << "---------------------------------" << endl;
            }
        }
    }
    else if (choice == "id")
    {
        int id;
        bool found = false;
        cout << "Enter transaction ID: ";
        cin >> id;

        for (Transaction transaction : transactions)
        {
            if (transaction.id == id)
            {
                cout << endl;
                cout << "ID: " << transaction.id << endl;
                cout << "Description: " << transaction.description << endl;
                cout << "Amount: $" << transaction.amount << endl;
                cout << "Date: " << transaction.date << endl;
                cout << "Type: " << (transaction.type == TransactionType::INCOME ? "Income" : "Expense") << endl;
                cout << "---------------------------------" << endl;
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Transaction not found." << endl;
        }
    }
    else
    {
        cout << "Invalid choice. Please enter 'all' or 'id'." << endl;
    }
}

void deleteTransaction(vector<Transaction> &transactions, const string &filename)
{
    int id;
    cout << "Enter the id of the transaction you want to delete: ";
    cin >> id;

    auto it = remove_if(transactions.begin(), transactions.end(), [id](const Transaction &t)
                        { return t.id == id; });

    if (it != transactions.end())
    {
        transactions.erase(it, transactions.end()); // Remove the element from the vector

        // Update the file
        ofstream file(filename);
        if (file.is_open())
        {
            file << "id,description,amount,date,type\n"; // Write header

            // Write remaining transactions
            for (const auto &transaction : transactions)
            {
                file << transaction.id << ","
                     << transaction.description << ","
                     << transaction.amount << ","
                     << transaction.date << ","
                     << (transaction.type == TransactionType::INCOME ? "Income" : "Expense") << "\n";
            }
            file.close();

            cout << "Transaction with id " << id << " deleted successfully." << endl;
        }
        else
        {
            cout << "Error opening file for writing." << endl;
        }
    }
    else
    {
        cout << "Transaction with id " << id << " not found." << endl;
    }
}

void updateTransaction(vector<Transaction> &transactions, const string &filename)
{
    int id;
    cout << "Enter the id of the transaction you want to update: ";
    cin >> id;

    // Find the transaction by id
    auto it = find_if(transactions.begin(), transactions.end(), [id](const Transaction &t)
                      { return t.id == id; });

    if (it != transactions.end())
    {
        Transaction &transactionToUpdate = *it;

        // Update description
        cout << "Enter new description (current: " << transactionToUpdate.description << "): ";
        cin.ignore(); // Clear the input buffer
        getline(cin, transactionToUpdate.description);
        if (transactionToUpdate.description.empty())
        {
            cout << "Description cannot be empty." << endl;
            return;
        }

        // Update amount
        cout << "Enter new amount (current: $" << transactionToUpdate.amount << "): ";
        while (!(cin >> transactionToUpdate.amount) || transactionToUpdate.amount <= 0)
        {
            cout << "Invalid amount. Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input buffer
        }

        // Update date
        string dateStr;
        while (true)
        {
            cout << "Enter new date (YYYY-MM-DD) (current: " << transactionToUpdate.date << "): ";
            cin.ignore(); // Clear the input buffer
            getline(cin, dateStr);
            if (isValidDate(dateStr))
            {
                transactionToUpdate.date = dateStr;
                break;
            }
            else
            {
                cout << "Invalid date format or value. Please use YYYY-MM-DD and ensure the date is correct." << endl;
            }
        }

        // Update type
        string typeStr;
        cout << "Enter new type (Income/Expense) (current: " << (transactionToUpdate.type == TransactionType::INCOME ? "Income" : "Expense") << "): ";
        cin >> typeStr;
        try
        {
            transactionToUpdate.type = stringToTransactionType(typeStr);
        }
        catch (const invalid_argument &)
        {
            cout << "Invalid transaction type. Please enter 'Income' or 'Expense'." << endl;
            return;
        }

        // Save updated transactions to file
        ofstream file(filename);
        if (file.is_open())
        {
            file << "id,description,amount,date,type\n"; // Write header

            // Write all transactions including updated one
            for (const auto &transaction : transactions)
            {
                file << transaction.id << ","
                     << transaction.description << ","
                     << transaction.amount << ","
                     << transaction.date << ","
                     << (transaction.type == TransactionType::INCOME ? "Income" : "Expense") << "\n";
            }
            file.close();

            cout << "Transaction with id " << id << " updated successfully." << endl;
        }
        else
        {
            cout << "Error opening file for writing." << endl;
        }
    }
    else
    {
        cout << "Transaction with id " << id << " not found." << endl;
    }
}

void viewBalance(const vector<Transaction> &transactions){
    int totalIncome = 0;
    int totalExpense = 0;
    int balance = 0;

    for(Transaction transaction: transactions){
        if(transaction.type == TransactionType::INCOME){
            totalIncome += transaction.amount;
        }else{
            totalExpense += transaction.amount;
        }
    }

    balance = totalIncome - totalExpense;

    cout << endl;
    cout << "********** Balance *********" << endl;
    cout << "Your total income is $" << totalIncome << endl;
    cout << "Your total expense is $" << totalExpense << endl;
    cout << "Your current balance is $" << balance << endl;
    cout << "----------------------------" << endl;
}