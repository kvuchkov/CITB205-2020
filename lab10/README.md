# Synpsis
In this exercise, we will add catalog management to our program. At the end of the exercise, each product we add will be stored in the file we pass as a command line argument (i.e. `products.dat`).

# Requirements
We already can add products from the catalog and we can add products to the catalog on the fly, but we still cannot persist our new products on the disk.

* Adding a product to the catalog with `catalog add` command should immediately persist it in the file
* Print the catalog with `catalog show` command
* Updating a product should be supported through `catalog update` command. It is already implemented in `main`.
* We will keep the catalog file open as long as the catalog exists. We will use the destructor to close the file.
* The product class will assign product IDs automatically - allowing the user to enter those is dangerous, because they could accidentaly duplicate an existing ID.
* The format of the `product.dat` file needs to change. We want to have fixed-width columns so that we can `seek` individual records (i.e. products) and individual columns. Use the following sizes:
  * ID - 5
  * Name - 40
  * Price - 10 
  * Record - 56/57 (55 + new line). _Note that on Windows, the new line is 2 characters_


Changes you need to do:
* `Catalog::load` should keep the file open
* `Catalog::load` should read the new format - not CSV, but fixed-width columns
* `Catalog::add` should assign an ID to the new product based on the IDs it already has, i.e. the next number
* `Catalog::operator+=` should write the new product to the end of the file
* `Catalog::update` should update the product information and write it to the file (overriding the exisitng information in the file)

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab10`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

# Steps

As always, we start by making the main compile. We need to declare the `Catalog::update` member function:
`catalog.h`
```c++
class Catalog {
public:
    void load(string path);
    Product* get(int id) const;
    vector<Product*> list() const;
    Catalog & operator+=(Product *product);
    void update(int id, string name, double price);
private:
    vector<Product*> products;
};
```

For now, let's define an empty body. To make our program functional, we first need to make the changes to the `Catalog::load`, because our products.dat cannot be parsed at the moment
`catalog.cpp`
```c++
void Catalog::update(int id, string name, double price) {}
```

Let's make `Catalog::load` work with the new format of the product.dat:
`catalog.cpp`
```c++
using namespace std; // it is simpler to use the entire namespace

// Preprocessor can help us use a different value for windows and for linux/mac.
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
    ifstream fin(path);
    if (fin.fail()) {
        cerr << "Cannot open file " << path << endl;
    }

    int id;
    char name[NAME_WIDTH+1]; // name max plus the terminating character for the c string
    double price;

    int line = 0;
    while(fin) {
        fin.seekg(line * RECORD_WIDTH, ios_base::beg); // start from the beginning of the file
        fin >> id;
        
        fin.seekg(line * RECORD_WIDTH + ID_WIDTH, ios_base::beg); // we don't know the current position (id can be 1 to 5 characters), se we offset by the beginning of the file
        fin.getline(name, NAME_WIDTH+1);
        if (fin.fail() && fin.gcount() == NAME_WIDTH) {
            fin.clear(); // we've read the entire name, but since we didn't reach the new line character, fin.getline sets the errro state
        }

        fin >> price; // we don't need to seek the price - it starts immediately after the name and we have read the entire name width (40 characters)

        if (!fin.fail()) { // if we have successfully read the record, fin.fail will return false
            products.push_back(new Product(id, name, price));
        }

        line++; // increment the line so that the following reads start from the next record
    }
}
```

We can test with `catalog show` command - it should display all products.

Before we move on, let's try to plan ahead. To complete the persistance of the catalog we need a function that writes a record. For a new record, it will write it at the end of the file. For updating an existing record, we need to overwrite its row in the file.

Let's define all the functions we need in `catalog.h`
```c++
#include <vector>
#include <fstream> // we need to add the library
#include "product.h"

using std::vector;
using std::fstream; // and use the symbol

class Catalog {
public:
    void load(string path);
    Product* get(int id) const;
    vector<Product*> list() const;
    Catalog & operator+=(Product *product); // Add an entire product to the catalog
    void update(int id, string name, double price); // Update an existing product
    void add(string name, double price); // Add a new product and assign it an ID, will use the operator
private:
    vector<Product*> products;
    fstream file; // Move the file as a member so it remains open until we destroy the Catalog object
    void write(int pos, Product* product); // Helper that will write a product record to the file
};
```

We need to change the `Catalog::load` to use the `file` member:
```c++
void Catalog::load(string path) {
    file.open(path, ios_base::in | ios_base::out); // we call open instead of using the constructor
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
    file.clear(); // we need to clear the eof state after reading, because it will prevent us from writing
}
```

Next, we can implement the helper function `write` in `catalog.cpp`. It will receive a `pos` argument to indicate where to write in the file. A negative value (e.g. `-1`) will mean we should write at the end of the file:
```c++
void Catalog::write(int pos, Product* product) {
    if (!file.is_open()) { // we check if the file is open. Note that if the Catalog::load is not called, the file will not be open.
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
    file.flush(); // this will write the changes to the disk immediately
}
```

Now we can use the `Catalog::write` to implement the `Catalog::add`:
```c++
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
```

Lastly, we implement the update:
```c++
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
```

For the update to work, we need to add `Product::setName` and `Product::setPrice`:
`product.h`
```c++
class Product {
public:
    Product();
    Product(int id, string name, double price);
    Product(const Product &other);
    int getID() const;
    string getName() const;
    double getPrice() const;
    friend istream & operator>>(istream &in, Product &product);
    void setName(string name);
    void setPrice(double price);
private:
    int id;
    string name;
    double price;
};
```

`product.cpp`
```c++
void Product::setName(string name) {
    this->name = name;
}
void Product::setPrice(double price) {
    this->price = price;
}
```

This is it. You can test by using the `catalog` commands:
* Add a product: `catalog add Test 2.20` - this will append a new line at the end of the file and have the product directly in-memory
* Update a product: `catalog update 3 Glass 5.50` - this will update the product #3 (wine glass) to just glass


