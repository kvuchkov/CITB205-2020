#include "product.h"

Product::Product(string name, double price) {
    this->price = price;
}
double Product::getPrice() const {
    return price;
}
