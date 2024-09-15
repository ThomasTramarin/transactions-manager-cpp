#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <vector>
#include <string>

#include "types.hpp"

void displayMenu();
TransactionType stringToTransactionType(const std::string& typeStr);
vector<Transaction> loadTransactions (const string &filename);
int getNextId(const vector<Transaction>& transactions);
bool isValidDate(const string& dateStr);
void addTransaction(vector<Transaction> &transactions, const string &filename);
void viewTransactions(const vector<Transaction> &transactions);
void deleteTransaction(vector<Transaction> &transactions, const string &filename);
void updateTransaction(vector<Transaction> &transactions, const string &filename);
void viewBalance(const vector<Transaction> &transactions);

#endif