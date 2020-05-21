#ifndef LAB1_INVOICE_H
#define LAB1_INVOICE_H

#include <vector>
#include "product.h"
#include "item.h"
#include "discount.h"
#include "inventory.h"

using std::vector;

class Invoice : public Inventory<Product>
{
public:
    Invoice();                         // decleare a default constructor
    Invoice(const Invoice &) = delete; // mark the copy constructor as deleted
    ~Invoice();
    void apply(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double total(Item<Product> *item) const;
    double totalDiscount() const;
    void clearDiscounts();

private:
    Inventory<Product> items;
    vector<Discount *> discounts;
};

#endif //LAB1_INVOICE_H
