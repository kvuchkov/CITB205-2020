#ifndef LAB5_DISCOUNT_H
#define LAB5_DISCOUNT_H

#include "item.h"
#include <vector>

using std::vector;

class Discount
{
public:
    virtual double total(const vector<Item *> items) const;
};

class FixedDiscount : public Discount
{
public:
    FixedDiscount(double amount);
    double total(const vector<Item *> items) const;

private:
    double amount;
};

class PercentageDiscount : public Discount
{
public:
    PercentageDiscount(int percentage);
    double total(const vector<Item *> items) const;

private:
    double rate;
};

#endif //LAB5_DISCOUNT_H
