#include "product.h"
#include <iostream>

Product::Product(string name, double price) {
    this->name = name;
    this->price = price;
}

Product::Product(const Product &other) {
    name = other.name;
    price = other.price;
    std::cerr << "copy of product (" << name << ")" << std::endl;
}

double Product::getPrice() const {
    return price;
}

string Product::getName() const {
    return name;
}
