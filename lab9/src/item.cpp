#include "item.h"

Item::Item(const Product *product, int quantity) : product(product) {
    this->quantity = quantity;
}

double Item::total() {
    return product->getPrice() * quantity;
}

int Item::getProductID() const {
    return product->getID();
}

string Item::getDescription() const {
    return product->getName();
}

int Item::getQuantity() const {
    return quantity;
}

void Item::setQuantity(int qty) {
    quantity = qty;
}

double Item::getPrice() const {
    return product->getPrice();
}

double Item::total() const {
    return product->getPrice() * quantity;
}
