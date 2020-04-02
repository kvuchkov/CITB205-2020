#include "item.h"

Item::Item(Product product, int quantity) : product(product) {
    this->quantity = quantity;
}

double Item::total() {
    return product.getPrice() * quantity;
}

string Item::getDescription() const {
    return product.getName();
}

int Item::getQuantity() const {
    return quantity;
}

double Item::getPrice() const {
    return product.getPrice();
}

double Item::total() const {
    return product.getPrice() * quantity;
}
