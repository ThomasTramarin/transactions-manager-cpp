#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <fstream>
#include <sstream>
#include "types.hpp"
#include "functions.cpp"

using namespace std;

int main()
{
    bool running = true;
    string choiceStr;
    int choice;
    string filename = "transactions.csv";
    vector<Transaction> transactions = loadTransactions(filename);

    do
    {
        displayMenu();
        cin >> choiceStr;

        try
        {
            choice = stoi(choiceStr);
        }
        catch (invalid_argument const &e)
        {
            cout << "Invalid input. Please enter a number.";
            continue;
        }
        catch (out_of_range const &e)
        {
            cout << "Number out of range. Please enter a valid number.";
            continue;
        }

        switch (choice)
        {
        case 1:
            addTransaction(transactions, filename);
            break;
        case 2:
            viewTransactions(transactions);
            break;
        case 3:
            deleteTransaction(transactions, filename);
            break;
        case 4:
            updateTransaction(transactions, filename);
            break;
        case 5:
            viewBalance(transactions);
            break;
        case 6:
            running = false;
            exit(0);
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 6." << endl
                 << endl;
        }
    } while (running);

    return 0;
}