#ifndef LAB1_ITEM_H
#define LAB1_ITEM_H


#include "product.h"

class Item {
public:
    Item(Product product, int quantity);
    double total();
private:
    Product product;
    int quantity;
};


#endif //LAB1_ITEM_H
