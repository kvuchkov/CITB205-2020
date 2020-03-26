#include "product.h"

Product::Product(string name, double price) {
    this->name = name;
    this->price = price;
}
double Product::getPrice() const {
    return price;
}

string Product::getName() const {
    return name;
}
