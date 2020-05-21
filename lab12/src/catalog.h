#ifndef LAB7_CATALOG_H
#define LAB7_CATALOG_H

#include <vector>
#include <fstream>
#include "product.h"

using std::fstream;
using std::vector;

class Catalog
{
public:
    ~Catalog();
    void load(string path);
    Product *get(int id);     // remove the const
    vector<int> list() const; // return ids and not products
    Catalog &operator+=(Product *product);
    void update(int id, string name, double price);
    void add(string name, double price);

private:
    vector<int> index; // replace products collection with a list of IDs
    fstream file;
    Product *read(int pos); // read a product from a position in the file
    void write(int pos, Product *product);
};

#endif //LAB7_CATALOG_H
