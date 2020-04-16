# Synpsis
In this exercise, we will make our invoicing program use a catalog of products from a text file and allow users to add items on the invoice dynamically through interactive commands.

# Requirements
 - Parse the products catalog from `lab7/data/products.dat` file.
 - Implement interactive commands:
    - `show` - prints the catalog of products with ID, Name and Price
    - `add 1 2` - adds the product with ID 1 from the catalog two times on the invoice
    - `remove 1 4` - removes 4 items of the product with ID 1 from the invoice. If there are less than 4 just remove all of them. If there are no items of the product print a friendly message.
    - `discount fixed 10` - add a fixed amount discount of 10 BGN
    - `discount off 20` - add a percentage discount of 20%
    - `discount clear` - remove all discounts from the invoice.
    - `print` - prints the invoice with whatever line items we have

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab7`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

# Steps

## Parse Products Catalog

Let's start with the first problem the new requirements present, i.e. loading product details from a file.

We will have to make some changes in the main. But since we are working and thinking in object-oriented fashion, we can introduce a new object - `ProductCatalog` that returns objects by ID.

Create two files under `src`:
`catalog.h`
```c++
#ifndef LAB7_CATALOG_H
#define LAB7_CATALOG_H

class CATALOG {

};

#endif //LAB7_CATALOG_H
```
`catalog.cpp`
```c++
#include "catalog.h"
```

And add the `catalog.cpp` to the `CMakeLists.txt` (similarly to what we did for discount):
~~~
cmake_minimum_required(VERSION 3.5)
project(lab7)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/product.cpp src/item.cpp src/invoice.cpp src/textprinter.cpp src/discount.cpp src/catalog.cpp)
add_executable(lab7 ${SOURCE_FILES})
~~~


Now it should be easy to change the main so that items are not hardcoded, but taken from the catalog:
`main.cpp`
```c++
#include <iostream>
#include "invoice.h"
#include "textprinter.h"
#include "discount.h"
#include "catalog.h" // include the header of the new class

int main() {
    Invoice invoice;
    Catalog catalog; // create a catalog object

    Product *superMob = catalog.get(1); // get products by ID (see the example data in products.dat)
    Product *teaCup = catalog.get(2); // same
    Product *redWineGlass = catalog.get(3); // same

    FixedDiscount *fiveOff = new FixedDiscount(5.0);
    PercentageDiscount *tenPercentOff = new PercentageDiscount(10);

    invoice.add(superMob, 5);
    invoice.add(teaCup, 12);

    invoice.add(fiveOff);
    invoice.add(tenPercentOff);

    invoice.add(redWineGlass, 8);

    TextPrinter printer;
    printer.print(std::cout, invoice);
    return 0;
}
```

OK, now we have offloaded the concept of initializing products away from main. 

We might need one additional thing - the catalog needs to know where to load the data from, it makes sense main to to pass it to it. But how would it know? We the user should tell it. We will use a command line argument to pass the location on disk of the catalog file.
`main.cpp`
```c++
int main(int argc, char *argv[]) { // change main signature to the full one

    if (argc != 2) { // check how many arguments are provided and print an error
        std::cerr << "Invalid usage. Usage: ./lab7 path_to_catalog_file" << std::endl;
        return 1;
    }

    Invoice invoice;
    Catalog catalog;
    
    catalog.load(argv[1]); // load the catalog from the file. Remember that argv[0] is the path to the executable, so argv[1] is the first actual argument

    Product *superMob = catalog.get(1);
    Product *teaCup = catalog.get(2);
    Product *redWineGlass = catalog.get(3);

    FixedDiscount *fiveOff = new FixedDiscount(5.0);
    PercentageDiscount *tenPercentOff = new PercentageDiscount(10);

    invoice.add(superMob, 5);
    invoice.add(teaCup, 12);

    invoice.add(fiveOff);
    invoice.add(tenPercentOff);

    invoice.add(redWineGlass, 8);

    TextPrinter printer;
    printer.print(std::cout, invoice);
    return 0;
}
```

Try to build the program - it will complain, since catalog doesn't have the methods. Let's make it compile first and then implement the methods. Start by hardcoding the products in the load member function.

`catalog.h`
```c++
#ifndef LAB7_CATALOG_H
#define LAB7_CATALOG_H

#include <vector>
#include "product.h"

using std::vector;

class Catalog {
public:
    void load(string path);
    Product* get(int id) const;
private:
    vector<Product*> products;
};

#endif //LAB7_CATALOG_H
```
`catalog.cpp`
```c++
#include "catalog.h"

void Catalog::load(string path) {
    products.push_back(new Product(1, "Super Mob", 12.90));
    products.push_back(new Product(2, "Tea Cup", 5.30));
    products.push_back(new Product(3, "Red Wine Glass", 8.60));
}
Product* Catalog::get(int id) const {
    for(auto product : products) {
        if (product->getID() == id) {
            return product;
        }
    }
    return NULL;
}
```

We will also need to change the `Product` class a bit:
`product.h`
```c++
class Product {
public:
    Product(int id, string name, double price); // add the id to the constructor
    Product(const Product &other);
    int getID() const; // add an accessor for the id
    string getName() const;
    double getPrice() const;
private:
    int id; // introduce id member variable
    string name;
    double price;
};
```
`product.cpp`
```c++
Product::Product(int id, string name, double price) { // add id to the constructor
    this->id = id;
    this->name = name;
    this->price = price;
}
int Product::getID() const { // implement the accessor
    return id;
}
```

Build and run:
~~~
Invalid usage. Usage: ./lab7 path_to_catalog_file
~~~

From now on, we will have to pass an argument when we run the program. On Linux/Mac `./lab7 data/products.dat` and on Windows `lab7.exe data\products.dat`:
~~~
-------------------------------------------------------------------------
|       5|Super Mob                               |     12.90|     64.50|
-------------------------------------------------------------------------
|      12|Tea Cup                                 |      5.30|     63.60|
-------------------------------------------------------------------------
|       8|Red Wine Glass                          |      8.60|     68.80|
-------------------------------------------------------------------------
                                                     Discount|     24.69|
                                                             ------------
                                                     Subtotal|    172.21|
                                                             ------------
                                                        Taxes|     17.22|
                                                             ------------
                                                        TOTAL|    189.43|
                                                             ------------
~~~

We still get the same result, because we have just transfered the hardcoded values to another class. But the good news is that we can now focus on the actual parsing in `Catalog`.

`catalog.cpp`
```c++
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
```

Done! Products are now read from the `.dat` file. Try it out by changing some of the data, e.g. the price of the mob.

## Interactive program

In this part you need to make the program integractive.
