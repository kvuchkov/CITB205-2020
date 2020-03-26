#include "invoice.h"

void Invoice::add(Product product, int quantity) {
    Item item(product, quantity);
    items.push_back(item);
}

double Invoice::subtotal() {
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }
    return sum;
}
