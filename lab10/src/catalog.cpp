#include "catalog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::getline;
using std::ifstream;
using std::endl;
using std::cerr;
using std::stringstream;

void Catalog::load(string path) {
    ifstream fin(path);
    if (fin.fail()) {
        cerr << "Cannot open file " << path << endl;
    }
    string line;
    int id;
    string name;
    double price;
    string nop;
    int row = 0;
    while(getline(fin, line)) {
        row++;
        stringstream ss(line); // put the entire row in its own stream
        ss >> id; // read the ID
        getline(ss, nop, ','); // get rid of the comma
        getline(ss, name, ','); // read entire name with spaces - this will get rid of the separator
        ss >> price; // read the price

        if (ss.fail()) { // if any of the reads failed
            cerr << "Couldn't parse row " << row << endl;
            continue;
        }
        products.push_back(new Product(id, name, price));
    }
}

Product* Catalog::get(int id) const {
    for(auto product : products) {
        if (product->getID() == id) {
            return product;
        }
    }
    return NULL;
}

vector<Product*> Catalog::list() const {
    return products;
}

Catalog & Catalog::operator+=(Product *product) {
    this->products.push_back(product);
    return *this;
}

