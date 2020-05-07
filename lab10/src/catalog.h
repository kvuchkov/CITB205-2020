#ifndef LAB7_CATALOG_H
#define LAB7_CATALOG_H

#include <vector>
#include "product.h"

using std::vector;

class Catalog {
public:
    void load(string path);
    Product* get(int id) const;
    vector<Product*> list() const;
    Catalog & operator+=(Product *product);
private:
    vector<Product*> products;
};

#endif //LAB7_CATALOG_H
