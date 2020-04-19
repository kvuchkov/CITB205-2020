#include "invoice.h"

const double TAX = 0.1; // 10% flat tax rate

void Invoice::add(const Product *product, int quantity) {
    
    for (int i=0; i < items.size(); i++) { // using for (auto item : items) will make a copy of item[i], whereas we want to update it
        if (items[i].getProductID() == product->getID()) {
            items[i].setQuantity(items[i].getQuantity() + quantity);
            return;
        }
    }

    Item item(product, quantity);
    items.push_back(item);
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
