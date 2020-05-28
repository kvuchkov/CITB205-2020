#include "invoice.h"

const double TAX = 0.1; // 10% flat tax rate

Invoice::Invoice() {}
Invoice::~Invoice()
{
    std::cerr << "~Invoice" << std::endl;
}

double Invoice::subtotal() const
{
    double sum = 0;
    for (auto item : items.all())
    {
        sum += total(item); // use pointer
    }
    return sum - totalDiscount();
}

double Invoice::taxes() const
{
    return TAX * subtotal();
}

double Invoice::total() const
{
    return subtotal() + taxes();
}

double Invoice::total(Item<Product> *item) const
{
    return item->getQuantity() * item->get().getPrice();
}

void Invoice::apply(Discount *discount)
{
    discounts.push_back(discount);
}

double Invoice::totalDiscount() const
{
    double sum = 0;
    for (auto discount : discounts)
    {
        sum += discount->total(items.all());
    }
    return sum;
}

void Invoice::clearDiscounts()
{
    while (!discounts.empty())
    {
        discounts.pop_back();
    }
}