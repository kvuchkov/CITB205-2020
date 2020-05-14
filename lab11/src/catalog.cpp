#include "catalog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

#ifdef _WIN32
const int NEWLINE_WIDTH = 2;
#else
const int NEWLINE_WIDTH = 1;
#endif

const int ID_WIDTH = 5;
const int NAME_WIDTH = 40;
const int PRICE_WIDTH = 10;
const int RECORD_WIDTH = ID_WIDTH + NAME_WIDTH + PRICE_WIDTH + NEWLINE_WIDTH;

void Catalog::load(string path) {
    file.open(path, ios_base::in | ios_base::out);
    if (file.fail()) {
        cerr << "Cannot open file " << path << endl;
    }

    int id;
    char name[NAME_WIDTH+1]; // name max plus the terminating character for the c string
    double price;

    int line = 0;
    while(file) {
        file.seekg(line * RECORD_WIDTH, ios_base::beg); // start from the beginning of the file
        file >> id;
        
        file.seekg(line * RECORD_WIDTH + ID_WIDTH, ios_base::beg); // we don't know the current position (id can be 1 to 5 characters), se we offset by the beginning of the file
        file.getline(name, NAME_WIDTH+1);
        if (file.fail() && file.gcount() == NAME_WIDTH) {
            file.clear(); // we've read the entire name, but since we didn't reach the new line character, file.getline sets the errro state
        }

        file >> price; // we don't need to seek the price - it starts immediately after the name and we have read the entire name width (40 characters)

        if (!file.fail()) { // if we have successfully read the record, file.fail will return false
            products.push_back(new Product(id, name, price));
        }

        line++; // increment the line so that the following reads start from the next record
    }
    file.clear();
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

void Catalog::add(string name, double price) {
    int id = 0;
    for(auto product : products) {
        id = max(id, product->getID());
    }
    *this += new Product(id+1, name, price);
}

Catalog & Catalog::operator+=(Product *product) {
    this->products.push_back(product);
    this->write(-1, product);
    return *this;
}

void Catalog::update(int id, string name, double price) {
    for (int i=0; i<products.size(); i++) {
        if (products[i]->getID() == id) { // we first find the product
            // Then we update the product in-memory
            auto product = products[i];
            product->setName(name);
            product->setPrice(price);
            
            // Finally, we write the updates to disk
            int pos = i * RECORD_WIDTH; 
            write(pos, product);
            return;
        }
    }
    cerr << "No product with ID #" << id << endl;
}

void Catalog::write(int pos, Product* product) {
    if (!file.is_open()) {
        cerr << "Catalog file is not open" << endl;
        return;
    }

    cerr << "write" << endl;

    if (pos < 0) {
        file.seekp(0, ios_base::end);
    } else {
        file.seekp(pos, ios_base::beg);
    }

    file << left << setw(ID_WIDTH) << product->getID();
    file << left << setw(NAME_WIDTH) << product->getName();
    file << right << setw(PRICE_WIDTH) << product->getPrice();
    file << endl;
    file.flush();
}


