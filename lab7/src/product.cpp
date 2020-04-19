#include "product.h"
#include <iostream>

Product::Product(int id, string name, double price) {
    this->id = id;
    this->name = name;
    this->price = price;
}

Product::Product(const Product &other) {
    name = other.name;
    price = other.price;
    std::cerr << "copy of product (" << name << ")" << std::endl;
}

int Product::getID() const {
    return id;
}

double Product::getPrice() const {
    return price;
}

string Product::getName() const {
    return name;
}
