#ifndef LAB7_CATALOG_H
#define LAB7_CATALOG_H

#include <vector>
#include <fstream>
#include "product.h"

using std::vector;
using std::fstream;

class Catalog {
public:
    void load(string path);
    Product* get(int id) const;
    vector<Product*> list() const;
    Catalog & operator+=(Product *product);
    void update(int id, string name, double price);
    void add(string name, double price);
private:
    vector<Product*> products;
    fstream file;
    void write(int pos, Product* product);
};

#endif //LAB7_CATALOG_H
