#ifndef TYPES_HPP
#define TYPES_HPP
#include <string>
using namespace std;

enum class TransactionType
{
    INCOME,
    EXPENSE
};

struct Transaction
{
    int id;
    string description;
    double amount;
    string date;
    TransactionType type;
};

#endif