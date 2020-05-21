#include "invoice.h"

const double TAX = 0.1; // 10% flat tax rate

Invoice::Invoice() {}
Invoice::~Invoice()
{
    std::cerr << "~Invoice" << std::endl;
    for (auto item : items)
    {
        delete item; // delete all items still in the collection
    }
}

void Invoice::add(const Product *product, int quantity)
{
    auto it = find(product);
    if (it == items.end())
    {
        items.push_back(new Item(product, quantity)); // allocate new item on the heap
    }
    else
    {
        auto item = *it; // dereference the iterator to get the pointer
        item->setQuantity(item->getQuantity() + quantity);
    }
}

void Invoice::remove(const Product *product, int quantity)
{
    auto it = find(product);
    if (it != items.end())
    {
        auto item = *it; // dereference the iterator to get the pointer
        int remaining = std::max(item->getQuantity() - quantity, 0);
        if (remaining > 0)
        {
            item->setQuantity(remaining);
        }
        else
        {
            items.erase(it);
            delete item; // delete the item before removing it from the collection, as we no longer need it
        }
    }
}

vector<Item *>::iterator Invoice::find(const Product *product) // change signature
{
    for (auto it = items.begin(); it != items.end(); it++)
    {
        if ((*it)->getProductID() == product->getID()) // dereference the iterator to access the pointer
        {
            return it;
        }
    }
    return items.end();
}

double Invoice::subtotal() const
{
    double sum = 0;
    for (auto item : items)
    {
        sum += item->total(); // use pointer
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

const vector<Item *> Invoice::getItems() const
{
    return items;
}

void Invoice::add(Discount *discount)
{
    discounts.push_back(discount);
}

double Invoice::totalDiscount() const
{
    double sum = 0;
    for (auto discount : discounts)
    {
        sum += discount->total(items);
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
