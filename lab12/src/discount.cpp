#include "discount.h"
#include <sstream>
#include <iomanip>

string Discount::getID() const
{
    return "";
}

double Discount::total(const vector<Item<Product> *> items) const
{
    return 0;
}

FixedDiscount::FixedDiscount(double amount) : amount(amount) {}

double FixedDiscount::total(const vector<Item<Product> *> items) const
{
    return amount;
}
string FixedDiscount::getID() const
{
    std::ostringstream id;
    id << "fixed:" << std::fixed << std::setprecision(2) << amount;
    return id.str();
}

PercentageDiscount::PercentageDiscount(int percentage)
{
    rate = percentage / 100.0;
}

double PercentageDiscount::total(const vector<Item<Product> *> items) const
{
    double sum = 0;
    for (auto item : items)
    {
        sum += item->getQuantity() * item->get().getPrice();
    }

    return sum * rate;
}

string PercentageDiscount::getID() const
{
    std::ostringstream id;
    id << "off:" << std::fixed << std::setprecision(2) << rate;
    return id.str();
}