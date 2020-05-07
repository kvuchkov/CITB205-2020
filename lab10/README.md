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
* `Catalog::operator+=` should assign an ID to the new product based on the IDs it already has, i.e. the next number
* `Catalog::operator+=` should write the new product to the end of the file
* `Catalog::update` should update the product information and write it to the file (overriding the exisitng information in the file)
* `Catalog::~Catalog` should close the catalog file if it is opened.

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


