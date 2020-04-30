# Synpsis
In this exercise, we will use operator overloading to see how this could improve the syntax of our program. Note that we could go without overloading operators by just use functions, but the operators could improve the readability of our code in certain situations. Use with caution!

# Requirements
This is based on the program we had at the end of Lab 7. However, a few minor changes were done in `main.cpp`. To complete the exercise, you need to make adjustments in `Product` and `Catalog` classes to make the program compile and work correctly.

There is a new interactive command: `product`, e.g. `product 4 Mug 2.80` which adds a new product to the catalog on the fly. The work in `main` is already implemented. However:
* The product is added to the catalog using the `+=` operator, i.e. `catalog += product`. 
* The product is read from the standard input stream as if it were an int, i.e. `cin >> product`. We are using the `*` operator to dereference the pointer.

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab9`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

# Steps

The first complain of the compiler is that `Product` doesn't have a default constructor. Let's add it:

`product.h`
```c++
class Product {
public:
    Product(); // define a default constructor
    Product(int id, string name, double price);
    Product(const Product &other);
    int getID() const;
    string getName() const;
    double getPrice() const;
private:
    int id;
    string name;
    double price;
};

```
`product.cpp`
```c++
Product::Product() {
    this->id = 0;
    this->price = 0;
}
```

Next, `cin >> product` doesn't work, because there is no such function (i.e. operator). We need to declare it.

The `>>` operator is actually a function that accepts two arguments, think of `>>(x, y)` where `>>` is the function name. We have a lefthand operand of type `std::istream` and a righthand operand of type `Product`. Now, since we are going to modify both operands as part of the operation (reading from a stream is modifying it, because what is read from it cannot be read again; similarly we want to set id, price and name of the product object we are reading) we will pass those by reference. 

The tricky part is that we want this operator function to be able to write the private members of the Product class. We know that if something is private, only members of the class can access it. We will use a `friend` function for this. A `friend` function is not a member function, because it doesn't have the class scope, but it has access to private members if it gets its hands on an object of that class. It is like a member function that doesn't have `this`. 

`product.h`
```c++
#include <iostream>
#include <string>

using std::string;
using std::istream;

class Product {
public:
    Product();
    Product(int id, string name, double price);
    Product(const Product &other);
    int getID() const;
    string getName() const;
    double getPrice() const;
    friend istream & operator>>(istream &in, Product &product);
private:
    int id;
    string name;
    double price;
};
```
`product.cpp`
```c++
std::istream & operator>>(std::istream &in, Product &product) {
    int id;
    string name;
    double price;
    in >> product.id >> product.name >> product.price;
    return in;
}
```

This is it! We can now read products from streams (any stream, not just `cin`).

Now we move on to the next line: `catalog += product;`

Similarly to the `>>` operator, `+=` is a function with two argument. However, this time we can define this operator as a member function of the `Catalog` class. Why? Because the lefthand operand is of type `Catalog`. We couldn't do this for the `operator>>` because the lefthand operator was `istream` and we don't have access to the code of the class. Think of `catalog.+=(product)`. We could have just used an `add` function here (which is a better option in this case), but we want to illustrate the use of operator overloading.

`catalog.h`
```c++
class Catalog {
public:
    void load(string path);
    Product* get(int id) const;
    vector<Product*> list() const;
    Catalog & operator+=(Product *product); // notice we return a reference
private:
    vector<Product*> products;
};
```
`catalog.cpp`
```c++
Catalog & Catalog::operator+=(Product *product) {
    this->products.push_back(product);
    return *this;
}
```

Done. Go ahead and test it by using the `product` and the `show` commands.

> As a last exercise, think and experiment with other pieces of the program that could benefit from operator overloading. How about having a `product * 4` produce an `Item` instance? Or `item + discount` produce a discounted item? How would those operators look in the code?
