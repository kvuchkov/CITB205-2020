# Synpsis
The goal of this exercise is to print all information about the invoice, i.e. complete the program. At the end, the program should build successfuly, run and output the following:
~~~~
+--------+----------------------------------------+----------+----------+
|       5|Super Mob                               |     12.90|     64.50|
+--------+----------------------------------------+----------+----------+
|      12|Tea Cup                                 |      5.30|     63.60|
+--------+----------------------------------------+----------+----------+
|       8|Red Wine Glass                          |      8.60|     68.80|
+--------+----------------------------------------+----------+----------+
                                                     Subtotal|    196.90|
                                                             +----------+
                                                        Taxes|     19.69|
                                                             +----------+
                                                        TOTAL|    216.59|
                                                             +----------+
~~~~

Note that the taxes are 10% flat, i.e. if a product costs 10 BGN, the taxes will be 1 BGN, so the cost for the total price for the customer will be 11 BGN.

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab4`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

The steps are similar to what we had in Lab #3. As we add code to the `TextPrinter::print` function, this will lead to cascading changes in the other classes. Remember - declarations go to the `.h` file, while definitions (i.e. the body) goes to the `.cpp` file of the class.


Steps (same as in lab #3):
1. Write a minimal, yet functional code. For example, make the `TextPrinter::print` function output the total of the invoice with taxes.
2. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see compilation errors.
3. Declare members of this and other classes so that the code added in step #1 compiles.
4. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see linking errors.
5. Define empty bodies of the member functions added in step #3 (i.e. linking errors in #4)
6. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. The build should be successful.

Now go to the next empty function body, implement it and repeat steps #1 to #6.

>Tip: You can commit your changes in the `master` branch after each run of steps 1 to 6, i.e. having a checkpoint of a compiling program. To do that: `git add .` and then `git commit -m "your message"`.

At the end, you should be able to run `./lab4` on Linux/Mac or `lab4.exe` on Windows and see all information on the invoice formatted nicely.

# Example Steps

At the starting point, everything builds successfuly and outputs the subtotal of the invoice.

Now, let's make a minimal functional change - print taxes. Go to `textprinter.cpp` and change the `print` function:
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << invoice.subtotal() << std::endl;
    out << invoice.taxes() << std::endl;
}
```

Build:
~~~
Scanning dependencies of target lab4
[ 16%] Building CXX object CMakeFiles/lab4.dir/src/textprinter.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab4/src/textprinter.cpp:6:20: error: no member named 'taxes' in 'Invoice'
    out << invoice.taxes() << std::endl;
           ~~~~~~~ ^
1 error generated.
make[2]: *** [CMakeFiles/lab4.dir/src/textprinter.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab4.dir/all] Error 2
make: *** [all] Error 2
~~~

Go to `invoice.h` and declare the new function.
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    double subtotal() const;
    double taxes() const;
private:
    vector<Item> items;
};
```

Remember when we discussed the `getPrice` in the previous lab? We said if a member function is not intended to modify the state of a class, we should be explicit about this. Both `total` and `subtotal` are not expected to change the invoice, only provide information, this is why we have added `const` at the end of the declaration.

Build:
~~~
Scanning dependencies of target lab4
[ 16%] Building CXX object CMakeFiles/lab4.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab4.dir/src/invoice.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab4/src/invoice.cpp:8:17: error: out-of-line definition of 'subtotal' does not match any declaration in 'Invoice'
double Invoice::subtotal() {
                ^~~~~~~~
/Users/kiril.vuchkov/uni/CITB205-2020/lab4/src/invoice.h:14:12: note: member declaration does not match because it is const qualified
    double subtotal() const;
           ^          ~~~~~
1 error generated.
make[2]: *** [CMakeFiles/lab4.dir/src/invoice.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab4.dir/all] Error 2
make: *** [all] Error 2
~~~

It doesn't compile, because subtotal definition (without `const`) doesn't match the declaration in the header (with `const`). Let's go to `invoice.cpp` and define both functions properly:
```c++
double Invoice::subtotal() const {
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }
    return sum;
}

double Invoice::taxes() const {
    return 0;
}
```

Build:
~~~
Scanning dependencies of target lab4
[ 16%] Building CXX object CMakeFiles/lab4.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab4.dir/src/invoice.cpp.o
[ 50%] Building CXX object CMakeFiles/lab4.dir/src/textprinter.cpp.o
[ 66%] Linking CXX executable lab4
[100%] Built target lab4
~~~

Run:
~~~
196.9
0
~~~

Taxes are `0`, we need to implement the `Invoice::taxes` function for real in `invoice.cpp`:
```c++
const double TAX = 0.1; // 10% flat tax rate

double Invoice::taxes() const {
    return TAX * subtotal();
}

```

Notice how we can use other member functions, similarly to how we can use member variables.

Build and run:
~~~
196.9
19.69
~~~

We are getting there! This is a good time to commit your changes. If you don't remember how - look at the previous lab for instructions.

Now, let's print the total of the invoice - what the customer needs to pay. It is tempting to just have `invoice.subtotal() + invoice.taxes()` in the `print` function. However, this will introduce calculation and business logic (i.e. how is the total calculated) in the printer. The sole purpose (i.e. responsibility) of the `TextPrinter` class is to print the information on the screen. We should keep it that way and introduce a `total` function in the invoice.

`textprinter.cpp`:
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << invoice.subtotal() << std::endl;
    out << invoice.taxes() << std::endl;
    out << invoice.total() << std::endl;
}
```

`invoice.h`:
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    double subtotal() const;
    double taxes() const;
    double total() const;
private:
    vector<Item> items;
};
```

`invoice.cpp`:
```c++
double Invoice::total() const {
    return subtotal() + taxes();
}
```

Build and run:
~~~
196.9
19.69
216.59
~~~

This was easy. Now we need to print the individual line items.

`textprinter.cpp`
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    for (auto item : invoice.getItems()) {
        out << item.getQuantity() << " ";
        out << item.getDescription() << " ";
        out << item.getPrice() << " ";
        out << item.total() << std::endl;
    }

    out << invoice.subtotal() << std::endl;
    out << invoice.taxes() << std::endl;
    out << invoice.total() << std::endl;
}
```

To make this work, we will need to define `Invoice::getItems` accessor and then we need to define a few member functions on the `Item` class.

> It is better if you do this in small steps (follow the algorithm described in the beginning of the exercise).

We will fast forward a bit:
`invoice.h` - declare `getItems` accessor
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    double subtotal() const;
    double taxes() const;
    double total() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
};
```

`invoice.cpp` - define `getItems` to return `items`
```c++
vector<Item> Invoice::getItems() const {
    return items;
}
```

> Note that by returning items directly we are not contradicting with the `const` keyword, because the caller of `getItems` will receive a copy of the `items` member, not a reference. Return values of functions have the same semantic as function parameters - by default, return value is passed by value, but can be explicitly passed by reference. In this case, we want a copy, so that the caller cannot modify the internal vector on its own.

`item.h` - declare the new functions
```c++
class Item {
public:
    Item(Product product, int quantity);
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

`item.cpp` - define the new functions
```c++

string Item::getDescription() const {
    return product.getName();
}

int Item::getQuantity() const {
    return quantity;
}

double Item::getPrice() const {
    return product.getPrice();
}

double Item::total() const {
    return product.getPrice() * quantity;
}
```

`product.h` - declare getName accessor and name member variable
```c++
class Product {
public:
    Product(string name, double price);
    string getName() const;
    double getPrice() const;
private:
    string name;
    double price;
};
```

`product.cpp` - define getName and set the name member variable in the constructor
```c++
Product::Product(string name, double price) {
    this->name = name;
    this->price = price;
}
string Product::getName() const {
    return name;
}

```

At the end, build and run:
~~~
5 Super Mob 12.9 64.5
12 Tea Cup 5.3 63.6
8 Red Wine Glass 8.6 68.8
196.9
19.69
216.59
~~~

OK, we have all the information we need exposed to the `TextPrinter` class. Now we just need to format it - all the work will be in `textprinter.cpp` file.

We start off by including the standard library for input/output manupulations:
```c++
#include <iomanip>
```

First, let's format the numbers. Since we are talking money here, we want ot format doubles to the second digit after the decimal point. To do this, we must configure the output stream:
```c++
out << std::fixed << std::setprecision(2);
```

Build and run:
~~~
5 Super Mob 12.90 64.50
12 Tea Cup 5.30 63.60
8 Red Wine Glass 8.60 68.80
196.90
19.69
216.59
~~~

Now, we want to get a table at the end. `<iomanip>` has a very useful function called `setw` - it will create a fixed-width field in which the next output will go. So, for example:
```c++
cout << std::setw(10) << 10 << std::endl;
```
Will output "        10", i.e. 10 padded with spaces on the left. We can change the alignment and get "10          " by combining `setw` with `right`. We want only the product description to be right aligned.

To produce the table from the example, let's put the data into columns of width 8, 40, 10 and 10. We want all cells right aligned except the product description, which will be left aligned. 
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << std::fixed << std::setprecision(2);

    for (auto item : invoice.getItems()) {
        out << setw(8) << right << item.getQuantity();
        out << setw(40) << left << item.getDescription();
        out << setw(10) << right << item.getPrice();
        out << setw(10) << right << item.total();
        out << endl;
    }

    out << setw(58) << right << "Subtotal" << setw(10) << right << invoice.subtotal() << endl;
    out << setw(58) << right << "Taxes" << setw(10) << right << invoice.taxes() << endl;
    out << setw(58) << right << "TOTAL" << setw(10) << right << invoice.total() << endl;
}
```

Build and run:
~~~
       5Super Mob                                    12.90     64.50
      12Tea Cup                                       5.30     63.60
       8Red Wine Glass                                8.60     68.80
                                                  Subtotal    196.90
                                                     Taxes     19.69
                                                     TOTAL    216.59
~~~

This is close. We now need to put the vertical separators.
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << std::fixed << std::setprecision(2);

    for (auto item : invoice.getItems()) {
        out << '|';
        out << setw(8) << right << item.getQuantity() << '|';
        out << setw(40) << left << item.getDescription() << '|';
        out << setw(10) << right << item.getPrice() << '|';
        out << setw(10) << right << item.total() << '|';
        out << endl;
    }

    out << setw(58+3) << right << "Subtotal" << '|' << setw(10) << right << invoice.subtotal() << endl;
    out << setw(58+3) << right << "Taxes" << '|' << setw(10) << right << invoice.taxes() << endl;
    out << setw(58+3) << right << "TOTAL" << '|' << setw(10) << right << invoice.total() << endl;
}
```

Note that because we have separators, the width of the summary rows (i.e. subtotal, taxes and total) needs to be increased with the number of vertical separators - in this case 3.

Finally, we want to put the horizontal separators - we will use the `std::setfill` function which sets the padding character when using `std:setw` (`' '` by default):
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << std::fixed << std::setprecision(2);
    
    out << std::setfill('-') << setw(68+5) << "" << std::setfill(' ') << endl;

    for (auto item : invoice.getItems()) {
        out << '|';
        out << setw(8) << right << item.getQuantity() << '|';
        out << setw(40) << left << item.getDescription() << '|';
        out << setw(10) << right << item.getPrice() << '|';
        out << setw(10) << right << item.total() << '|';
        out << endl;
        
        out << std::setfill('-') << setw(68+5) << "" << std::setfill(' ') << endl;
    }

    out << setw(58+3) << right << "Subtotal" << '|' << setw(10) << right << invoice.subtotal() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "Taxes" << '|' << setw(10) << right << invoice.taxes() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "TOTAL" << '|' << setw(10) << right << invoice.total() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
}

```
Build and run:
~~~
-------------------------------------------------------------------------
|       5|Super Mob                               |     12.90|     64.50|
-------------------------------------------------------------------------
|      12|Tea Cup                                 |      5.30|     63.60|
-------------------------------------------------------------------------
|       8|Red Wine Glass                          |      8.60|     68.80|
-------------------------------------------------------------------------
                                                     Subtotal|    196.90|
                                                             ------------
                                                        Taxes|     19.69|
                                                             ------------
                                                        TOTAL|    216.59|
                                                             ------------
~~~

Voila! Could be prittier, but works for now.

# The End

We have all the information of the invoice presented as a table. 
