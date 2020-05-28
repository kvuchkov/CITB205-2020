#ifndef LAB5_DISCOUNT_H
#define LAB5_DISCOUNT_H

#include "item.h"
#include "product.h"
#include <vector>

using std::vector;

class Discount
{
public:
    virtual string getID() const;
    virtual double total(const vector<Item<Product> *> items) const;
};

class FixedDiscount : public Discount
{
public:
    FixedDiscount(double amount);
    string getID() const;
    double total(const vector<Item<Product> *> items) const;

private:
    double amount;
};

class PercentageDiscount : public Discount
{
public:
    PercentageDiscount(int percentage);
    string getID() const;
    double total(const vector<Item<Product> *> items) const;

private:
    double rate;
};

#endif //LAB5_DISCOUNT_H
