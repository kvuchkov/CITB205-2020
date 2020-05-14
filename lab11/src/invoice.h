#ifndef LAB1_INVOICE_H
#define LAB1_INVOICE_H

#include <vector>
#include "product.h"
#include "item.h"
#include "discount.h"

using std::vector;

class Invoice
{
public:
    Invoice(); // decleare a default constructor
    Invoice(const Invoice &) = delete; // mark the copy constructor as deleted
    ~Invoice();
    void add(const Product *product, int quantity);
    void remove(const Product *product, int quantity);
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    const vector<Item *> getItems() const;
    void clearDiscounts();

private:
    vector<Item *> items;
    vector<Discount *> discounts;

    vector<Item *>::iterator find(const Product *product);
};

#endif //LAB1_INVOICE_H
