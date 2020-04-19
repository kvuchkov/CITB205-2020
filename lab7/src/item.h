#ifndef LAB1_ITEM_H
#define LAB1_ITEM_H


#include "product.h"

class Item {
public:
    Item(const Product *product, int quantity);
    double total();
    int getProductID() const;
    string getDescription() const;
    int getQuantity() const;
    void setQuantity(int qty);
    double getPrice() const;
    double total() const;
private:
    const Product *product;
    int quantity;
};


#endif //LAB1_ITEM_H
