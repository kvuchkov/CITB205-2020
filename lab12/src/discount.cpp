#include "discount.h"

double Discount::total(const vector<Item *> items) const
{
    return 0;
}

FixedDiscount::FixedDiscount(double amount) : amount(amount) {}

double FixedDiscount::total(const vector<Item *> items) const
{
    return amount;
}

PercentageDiscount::PercentageDiscount(int percentage)
{
    rate = percentage / 100.0;
}

double PercentageDiscount::total(const vector<Item *> items) const
{
    double sum = 0;
    for (auto item : items)
    {
        sum += item->total();
    }

    return sum * rate;
}
