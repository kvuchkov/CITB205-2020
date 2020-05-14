#ifndef LAB1_PRODUCT_H
#define LAB1_PRODUCT_H

#include <iostream>
#include <string>

using std::string;
using std::istream;

class Product {
public:
    Product();
    Product(int id, string name, double price);
    Product(const Product &other);
    int getID() const;
    string getName() const;
    double getPrice() const;
    friend istream & operator>>(istream &in, Product &product);
    void setName(string name);
    void setPrice(double price);
private:
    int id;
    string name;
    double price;
};


#endif //LAB1_PRODUCT_H
