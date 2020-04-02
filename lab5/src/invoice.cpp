#include "invoice.h"

const double TAX = 0.1; // 10% flat tax rate

void Invoice::add(Product product, int quantity) {
    Item item(product, quantity);
    items.push_back(item);
}

double Invoice::subtotal() const {
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }
    return sum;
}

double Invoice::taxes() const {
    return TAX * subtotal();
}

double Invoice::total() const {
    return subtotal() + taxes();
}

vector<Item> Invoice::getItems() const {
    return items;
}
