# Synpsis
In this exercise, we will use the copy constructors of our classes to investigate how many times we are making copies and then eliminate unnessesary copying. We want to focus on eliminating copying of the `Product` class, because it will potentially grow to contain an image of the product, description, links and specifications.

First, implement an explicit copy constructor for each class. Add a `cerr << "..."` message so that you can see whenever the copy constructor is called. Then, start using references and pointers to eliminate copying.

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab5`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

# Steps

Let's start with our product class. Just a reminder: for each class we define, there is an implicit copy constructor that just copies whatever data (memory) we have in the class. We can override this implicit constructor with an explicit one:
`product.h`:
```c++
class Product {
public:
    Product(string name, double price);
    Product(const Product &other); // added
    string getName() const;
    double getPrice() const;
private:
    string name;
    double price;
};
```
`product.cpp`:
```c++
#include <iostream> // we need to include the standard IO library to be able to log the copying

Product::Product(const Product &other) {
    name = other.name;
    price = other.price;
    std::cerr << "copy of product (" << name << ")" << std::endl;
}
```

Build the program, you should see no errors.

Now, let's run the program. This time, we want to split the standard output from the standard error stream (if you just run the program, you will see a very ugly output).:
Windows:
```
lab6.exe 2> log.txt
```
Linux/Mac:
```
./lab6 2> log.txt
```

This will execute the program, show you the same output, but in the log.txt file we will see a scary picture:
~~~
copy of product (Super Mob)
copy of product (Super Mob)
copy of product (Super Mob)
copy of product (Super Mob)
copy of product (Tea Cup)
copy of product (Tea Cup)
copy of product (Tea Cup)
copy of product (Tea Cup)
copy of product (Super Mob)
copy of product (Red Wine Glass)
copy of product (Red Wine Glass)
copy of product (Red Wine Glass)
copy of product (Red Wine Glass)
copy of product (Tea Cup)
copy of product (Super Mob)
...
~~~

You see that every product is copied multiple times. The copying happens when we pass a product by value and we do this in multiple places. Let's move through the code and try to limit this using references.

Let's follow the execution path of our program by starting from `main`. The line `invoice.add(superMob, 5);` passes the superMob as a value - notice that super mob is the first product to be copied 4 times before getting to the next product. This means that `Invoice:add` copies the product 4 times! 

Change the `Invoice:add`to accept a reference:
`invoice.h` 
```c++
class Invoice {
public:
    void add(const Product &product, int quantity); // add const and &, we don't want to change the product, just hold it
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
    vector<Discount*> discounts;
};
```

`invoice.cpp` 
```c++
void Invoice::add(const Product &product, int quantity) {
    Item item(product, quantity);
    items.push_back(item);
}
```

> Notice that this change doesn't require changing how the function is called.

OK, let't build and run. Look at the log.txt:
~~~
copy of product (Super Mob)
copy of product (Super Mob)
copy of product (Super Mob)
copy of product (Tea Cup)
copy of product (Tea Cup)
copy of product (Tea Cup)
copy of product (Super Mob)
copy of product (Red Wine Glass)
copy of product (Red Wine Glass)
copy of product (Red Wine Glass)
...
~~~

OK, we eliminated one unnecessary copying. There 3 more to go.

The second copying happens when we pass product as an argument of the `Item` constructor. Let's change it to accept a reference:
`item.h`
```c++
class Item {
public:
    Item(const Product &product, int quantity);
    double total();
    string getDescription() const;
    int getQuantity() const;
    double getPrice() const;
    double total() const;
private:
    Product product;
    int quantity;
};
```
`item.cpp`
```c++
Item::Item(const Product &product, int quantity) : product(product) {
    this->quantity = quantity;
}
```

Build and run, look at the log.txt:
~~~
opy of product (Super Mob)
copy of product (Super Mob)
copy of product (Tea Cup)
copy of product (Tea Cup)
copy of product (Super Mob)
copy of product (Red Wine Glass)
copy of product (Red Wine Glass)
...
~~~

One more copying goes away, but there are two more. Can you figure out where?

Spoiler alert! We are explicitly calling the copy constructor with the initializator syntax in the `Item::Item` constructor. We cannot really get rid of this by using reference. 

The second coping happens when we `push_back` the `item` to the vector. The `vector<Item>::push_back` uses value semantics and will copy the item, which in turn will copy the contained product.

Both problems can be addressed only if we use pointers, so let's do this. _Remember, we want to eliminate the copying of product._

Let's start from the `main.cpp`:
```c++
int main() {
    Invoice invoice;
    Product *superMob = new Product("Super Mob", 12.90); // we change the variables to pointers, allocated no the heap
    Product *teaCup = new Product("Tea Cup", 5.30); // same as above
    Product *redWineGlass = new Product("Red Wine Glass", 8.60); // same as above

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

Now, if you try to compile you will see there is no such `Invoice::add` function. This is becuase a pointer to `Product`, i.e. `Product*` is entirely different type than a `Product`. Now we need to change the `Invoice` class:
`invoice.h`
```c++
class Invoice {
public:
    void add(const Product *product, int quantity); // replace & with *, leave the const as we should not modify the product at any point
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
    vector<Discount*> discounts;
};
```
`invoice.cpp`
```c++
void Invoice::add(const Product *product, int quantity) { // we only replace & with *
    Item item(product, quantity);
    items.push_back(item);
}
```

So far - so good. Now this doesn't compile because Item doesn't have a constructor accepting a pointer. Let's change this:
`item.h`
```c++
class Item {
public:
    Item(const Product *product, int quantity); // change the constructor to accept a pointer
    double total();
    string getDescription() const;
    int getQuantity() const;
    double getPrice() const;
    double total() const;
private:
    const Product *product; // change the member variable to a pointer to avoid copying
    int quantity;
};
```
`item.cpp`
```c++
Item::Item(const Product *product, int quantity) : product(product) { // change the constructor
    this->quantity = quantity;
}

double Item::total() {
    return product->getPrice() * quantity; // change member access, because product is now a poitner
}

string Item::getDescription() const {
    return product->getName();  // change member access, because product is now a poitner
}

int Item::getQuantity() const {
    return quantity;  // change member access, because product is now a poitner
}

double Item::getPrice() const {
    return product->getPrice();  // change member access, because product is now a poitner
}

double Item::total() const {
    return product->getPrice() * quantity;  // change member access, because product is now a poitner
}
```

This is it! Build, run and look at the `log.txt` - we have eliminated 100% of the copying of `Product`, and the program still works and behaves as expected.

> The `Item` class is still copied, but it contains only an address of a product (the pointer) and an integer (quantity), so copying it is not a big deal for now.
