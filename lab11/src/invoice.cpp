#include "invoice.h"

const double TAX = 0.1; // 10% flat tax rate

void Invoice::add(const Product *product, int quantity) {
    auto item = find(product);
    if (item == items.end()) {
        Item item(product, quantity);
        items.push_back(item);
    } else {
        item->setQuantity(item->getQuantity() + quantity);
    }
}

void Invoice::remove(const Product *product, int quantity) {
    auto item = find(product);
    if (item != items.end()) {
        int remaining = std::max(item->getQuantity() - quantity, 0);
        if (remaining > 0) {
            item->setQuantity(remaining);
        } else {
            items.erase(item);
        }
    }
}

vector<Item>::iterator Invoice::find(const Product *product) {
    for (auto it = items.begin(); it != items.end(); it++) {
        if (it->getProductID() == product->getID()) {
            return it;
        }
    }
    return items.end();
}

double Invoice::subtotal() const {
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }
    return sum - totalDiscount();
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

void Invoice::add(Discount *discount) {
    discounts.push_back(discount);
}

double Invoice::totalDiscount() const {
    double sum = 0;
    for (auto discount : discounts) {
        sum += discount->total(items);
    }
    return sum;   
}

void Invoice::clearDiscounts() {
    while(!discounts.empty()) {
        discounts.pop_back();
    }
}
