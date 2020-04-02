# Synpsis
The goal of this excercise is to add a discount feature to the program. We will use inheritence and polymorphism to achieve this, so we will have to define 3 new classes.
~~~~
+--------+----------------------------------------+----------+----------+
|       5|Super Mob                               |     12.90|     64.50|
+--------+----------------------------------------+----------+----------+
|      12|Tea Cup                                 |      5.30|     63.60|
+--------+----------------------------------------+----------+----------+
|       8|Red Wine Glass                          |      8.60|     68.80|
+--------+----------------------------------------+----------+----------+
                                                    Discounts|     10.00|
                                                             +----------+
                                                     Subtotal|    196.90|
                                                             +----------+
                                                        Taxes|     19.69|
                                                             +----------+
                                                        TOTAL|    216.59|
                                                             +----------+
~~~~

# Requirements
We will have a bit more of business logic to implement in this exercise.

We would like to have two kinds of discounts:
 - Fixed amount - the discount is for a fixed amount of money, e.g. $10
 - Percentage - the discount is a percentage of the subtotal of the invoice, e.g. 5%

The discounts can be added to the invoice, in which case they should be immediately applied to everything on the invoice (existing line item). 
When items are added to the invoice, the discounts should be updated.

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

The steps should be familiar by know. 
1. Write a minimal, yet functional code.
2. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see compilation errors.
3. Declare members of this and other classes so that the code added in step #1 compiles.
4. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see linking errors.
5. Define empty bodies of the member functions added in step #3 (i.e. linking errors in #4)
6. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. The build should be successful.

Now go to the next empty function body, implement it and repeat steps #1 to #6.

>Tip: You can commit your changes in the `master` branch after each run of steps 1 to 6, i.e. having a checkpoint of a compiling program. To do that: `git add .` and then `git commit -m "your message"`.

At the end, you should be able to run `./lab5` on Linux/Mac or `lab5.exe` on Windows and see invoice with discounts applied.

# Adding the new file

We will start off by introducing 2 new files to our program that will hold the new classes: `discount.h` and `discount.cpp`.
* Create both files under the `src` folder (similar to `item.cpp`, etc.)
* Open `CMakeLists.txt` file and add the new `.cpp` file to the line #6, where the other cpp files are included:
```
set(SOURCE_FILES src/main.cpp src/product.cpp src/item.cpp src/invoice.cpp src/textprinter.cpp src/discount.cpp)
```
* Create the include guards. Those are the `#ifndef XXX_H` lines at the top of the `.h` files that protect it from being included more than once. Go to `discount.h` and add the following:
```
#ifndef LAB5_DISCOUNT_H
#define LAB5_DISCOUNT_H

#endif //LAB5_DISCOUNT_H
```

That's it! You are now ready to start hacking the exercise.

# Changing the main

This time, we will also have to change the main function to incorporate the discounts.

To properly demonstrate/test the business logic, we will add two discounts - one fixed and one percentage - and we will do this after we have added two items to the invoice, but before we have added the last.

Go to `main.cpp`:
```c++
#include <iostream>
#include "invoice.h"
#include "textprinter.h"
#include "discount.h"

int main() {
    Invoice invoice;
    Product superMob("Super Mob", 12.90);
    Product teaCup("Tea Cup", 5.30);
    Product redWineGlass("Red Wine Glass", 8.60);

    FixedDiscount fiveOff(5.0);
    PercentageDiscount tenPercentOff(10);

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

> Notice we have included our new header file.

Let's try to build it:
~~~
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/kiril.vuchkov/uni/CITB205-2020/lab5
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/main.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/main.cpp:12:5: error: unknown type name 'FixedDiscount'
    FixedDiscount fiveOff(5.0);
    ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/main.cpp:13:5: error: unknown type name 'PercentageDiscount'
    PercentageDiscount tenPercentOff(10);
    ^
2 errors generated.
make[2]: *** [CMakeFiles/lab5.dir/src/main.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab5.dir/all] Error 2
make: *** [all] Error 2
~~~

Expectedly, the main doesn't compile. Now we will need to declare all things to make it compile.

First, go to `discount.h`:
```c++
#ifndef LAB5_DISCOUNT_H
#define LAB5_DISCOUNT_H

class FixedDiscount {
public:
    FixedDiscount(double amount);
};

class PercentageDiscount {
public:
    PercentageDiscount(int percentage);
};

#endif //LAB5_DISCOUNT_H
```

We have defined two classes with their constructors. No inheritence or anything yet.

Build:
~~~
canning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/main.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/main.cpp:18:24: error: too few arguments to function call, expected 2, have 1
    invoice.add(fiveOff);
    ~~~~~~~~~~~        ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/invoice.h:13:5: note: 'add' declared here
    void add(Product product, int quantity);
    ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/main.cpp:19:30: error: too few arguments to function call, expected 2, have 1
    invoice.add(tenPercentOff);
    ~~~~~~~~~~~              ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/invoice.h:13:5: note: 'add' declared here
    void add(Product product, int quantity);
    ^
2 errors generated.
make[2]: *** [CMakeFiles/lab5.dir/src/main.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab5.dir/all] Error 2
make: *** [all] Error 2
~~~

Now we need to implement an `Invoice::add` overload (i.e. same function name with different parameters) that accepts discounts. We could
do this by defining two overloads - `void Invoice::add(FixedDiscount discount)` and `void Invoice::add(PercentageDiscount discount)`. However,
we don't want the `Invoice` class to know about all the different kinds of discounts we might have in the future.

> Remember, the __Single responsibility principle__ dictates that a class should have only one reason to change. Supporting a new kind of discount is not a reason for the `Invoice` class to change.

We will use inheritence. Both `FixedDiscount` and `PercentageDiscount` are discounts, so we might as well code it this way. Go to `discount.h` and make both classes inherit from a common base class
```c++
class Discount {
};

class FixedDiscount : public Discount {
public:
    FixedDiscount(double amount);
};

class PercentageDiscount : public Discount {
public:
    PercentageDiscount(int percentage);
};
```

> Notice both `FixedDiscount` and `PercentageDiscount` inherit `Discount` publicly. This is very specific to C++ - if you want the inheritence to mean `is a` you need to put the keyword public. By default, inheritence in C++ is private (we will discuss this later in the course). In our case, both discount types are discounts.

> Check out the __Liskov substitution principle__. In summary, it says that any child class should be able to substitute the parent class in any situation. This is not so straight forward as it seems on prima vista. 

Now, let's add a new overload of `Invoice::add` in `invoice.h`:
```c++
#include <vector>
#include "product.h"
#include "item.h"
#include "discount.h" // YOU NEED TO INLCUDE THIS

using std::vector;

class Invoice {
public:
    void add(Product product, int quantity);
    void add(Discount discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
};
```

Let's build:
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/main.cpp.o
[ 28%] Building CXX object CMakeFiles/lab5.dir/src/product.cpp.o
[ 42%] Building CXX object CMakeFiles/lab5.dir/src/item.cpp.o
[ 57%] Building CXX object CMakeFiles/lab5.dir/src/invoice.cpp.o
[ 71%] Building CXX object CMakeFiles/lab5.dir/src/textprinter.cpp.o
[ 85%] Building CXX object CMakeFiles/lab5.dir/src/discount.cpp.o
[100%] Linking CXX executable lab5
Undefined symbols for architecture x86_64:
  "FixedDiscount::FixedDiscount(double)", referenced from:
      _main in main.cpp.o
  "PercentageDiscount::PercentageDiscount(int)", referenced from:
      _main in main.cpp.o
  "Invoice::add(Discount)", referenced from:
      _main in main.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [lab5] Error 1
make[1]: *** [CMakeFiles/lab5.dir/all] Error 2
make: *** [all] Error 2
~~~


Voila! It comiles successfully. The invoice supports discounts (any discount, really), at least at compile time. We still have work to do on the implementation front.

Go to `discount.cpp` and define the constructors of our discount classes:
```c++
#include "discount.h"

FixedDiscount::FixedDiscount(double amount) {}

PercentageDiscount::PercentageDiscount(int percentage) {}
```

Build:
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/main.cpp.o
[ 28%] Building CXX object CMakeFiles/lab5.dir/src/invoice.cpp.o
[ 42%] Building CXX object CMakeFiles/lab5.dir/src/textprinter.cpp.o
[ 57%] Building CXX object CMakeFiles/lab5.dir/src/discount.cpp.o
[ 71%] Linking CXX executable lab5
Undefined symbols for architecture x86_64:
  "Invoice::add(Discount)", referenced from:
      _main in main.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [lab5] Error 1
make[1]: *** [CMakeFiles/lab5.dir/all] Error 2
make: *** [all] Error 2
~~~

Almost there! Now let's go to `invoice.cpp` and define the new overload of the `Invoice::add` member function:
```c++
void Invoice::add(Discount discount) { }
```

Build:
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/invoice.cpp.o
[ 28%] Linking CXX executable lab5
[100%] Built target lab5
~~~

Linking is successful, let's run it:
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

Nothing has changed. This is good, though - we haven't broken our program. __This is a good time to commit your changes (see previous labs for instructions).__

---

We will start with the implementation from the text printer. Rembember, we always try to make the minimum functional change possible.

Go to `textprinter.cpp` and print the discount of the invoice:
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

    out << setw(58+3) << right << "Discount" << '|' << setw(10) << right << invoice.totalDiscount() << '|' << endl; // ADDED
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl; // ADDED
    out << setw(58+3) << right << "Subtotal" << '|' << setw(10) << right << invoice.subtotal() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "Taxes" << '|' << setw(10) << right << invoice.taxes() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "TOTAL" << '|' << setw(10) << right << invoice.total() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
}
```

OK, this was easy. We needed a new public function of the invoice to return the total discounts.
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/textprinter.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab5/src/textprinter.cpp:26:85: error: no member named 'totalDiscount' in 'Invoice'
    out << setw(58+3) << right << "Subtotal" << '|' << setw(10) << right << invoice.totalDiscount() << '|' << endl;
                                                                            ~~~~~~~ ^
1 error generated.
make[2]: *** [CMakeFiles/lab5.dir/src/textprinter.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab5.dir/all] Error 2
make: *** [all] Error 2
~~~

Go ahead, declare and define the new member function:
`invoice.h`
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    void add(Discount discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
};
```
`invoice.cpp`
```c++
double Invoice::totalDiscount() const {
     return 0;    
}
```

Build:
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/invoice.cpp.o
[ 28%] Linking CXX executable lab5
[100%] Built target lab5
~~~

Success! Let's run it:
~~~
-------------------------------------------------------------------------
|       5|Super Mob                               |     12.90|     64.50|
-------------------------------------------------------------------------
|      12|Tea Cup                                 |      5.30|     63.60|
-------------------------------------------------------------------------
|       8|Red Wine Glass                          |      8.60|     68.80|
-------------------------------------------------------------------------
                                                     Discount|      0.00|
                                                             ------------
                                                     Subtotal|    196.90|
                                                             ------------
                                                        Taxes|     19.69|
                                                             ------------
                                                        TOTAL|    216.59|
                                                             ------------
~~~

Nice! We have the discount showing on the screen. Proceed to implementing the `Invoice::totalDiscount` function, as it will help us define the behaviour of the base `Discount` class.
`invoice.cpp`
```c++
double Invoice::totalDiscount() const {
    double sum = 0;
    for (auto discount : discounts) {
        sum += discount.total();
    }
    return sum;   
}
```
This was easy, but it means we will need two more things: a member variable `discounts` of the `Invoice` class, and a `Discount::total` member function of the `Discount` class:
`invoice.h`
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    void add(Discount discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
    vector<Discount> discounts; // ADDED
};
```
`discount.h`
```c++
class Discount {
public:
    double total() const;
};
```

Build:
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/textprinter.cpp.o
[ 28%] Linking CXX executable lab5
Undefined symbols for architecture x86_64:
  "Discount::total() const", referenced from:
      Invoice::totalDiscount() const in invoice.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [lab5] Error 1
make[1]: *** [CMakeFiles/lab5.dir/all] Error 2
make: *** [all] Error 2
~~~

Now that we need to define the body of `Discount::total`, you should be confused. The base class `Discount` is meaningless. Maybe we can have it return `0` (it will certainly work), but this isn't exactly what we mean. The base class exists only so that there can be children classes that have actual discount logic in them. This is an example of an `abstract` class - one that exists only to be ihnerited from concrete, meaningful other classes.

> Side note: In our case, `Discount` will define only a set of empty behaviours, without state. In other languages (e.g. Java) this is modeled as an `interface`.

For now, let's just make the `Discount::total` return `0`:
`discount.cpp`
```c++
double Discount::total() const {
    return 0;
}
```

This function will be inherited publically (i.e. imagine the same member function appears in the public section of the child classes) by both `FixedDiscount` and `PercentageDiscount`.

Build:
~~~
Scanning dependencies of target lab5
[ 14%] Building CXX object CMakeFiles/lab5.dir/src/discount.cpp.o
[ 28%] Linking CXX executable lab5
[100%] Built target lab5
~~~

Run:
~~~
-------------------------------------------------------------------------
|       5|Super Mob                               |     12.90|     64.50|
-------------------------------------------------------------------------
|      12|Tea Cup                                 |      5.30|     63.60|
-------------------------------------------------------------------------
|       8|Red Wine Glass                          |      8.60|     68.80|
-------------------------------------------------------------------------
                                                     Discount|      0.00|
                                                             ------------
                                                     Subtotal|    196.90|
                                                             ------------
                                                        Taxes|     19.69|
                                                             ------------
                                                        TOTAL|    216.59|
                                                             ------------
~~~

We see `0` again, but this time it comes from the `Invoice::totalDiscount` function, which sums all discounts on the invoice. However, we never add to the `discounts` vector, member of the invoice.
`invoice.cpp`
```c++
void Invoice::add(Discount discount) {
    discounts.push_back(discount);
}
```

Build and run - you should see `0.00` again for the discounts, but this is returned by each discount object added to the invoice.

Let's implement the fixed discount's total - it is much easier:
`discount.h`
```c++
class FixedDiscount : public Discount {
public:
    FixedDiscount(double amount);
    double total() const; // note we redeclare the function in the child class
private:
    double amount; // we also add a member variable to hold the fixed amount
};
```

`discount.cpp`
```c++
FixedDiscount::FixedDiscount(double amount) : amount(amount) {} // we use initializer syntax to initialize the amount member variable with the amount argument of the constructor

double FixedDiscount::total() const {
    return amount;
}
```

Let's build and run. You should get `0.00` discount again and you are probably confused.

The reason for this is that we are currently using polymorphism - we expect the function `total` to do different things depending on the actual class of the object. 
This is only possible when we use pointers. The reason for this is that when we call a function on an object, e.g. `x.total()`, C++ invokes the function of the type (i.e. class) of `x`. In our case, inside the `Invoice::totalDiscount` we call `total` over the `Discount` class, while we actually want to call `FixedDiscount::total`. We need to give that information to the runtime.

The way do do this is to use `virtual` function and pointer. When we call a virtual member function on a pointer, the runtime will check what is the actual member function for this particular object, instead of relying solely on the type (i.e. `Discount*`) do determine it.

We will make a few changes to make polymorphism work in our code:
`main.cpp`
```c++
int main() {
    Invoice invoice;
    Product superMob("Super Mob", 12.90);
    Product teaCup("Tea Cup", 5.30);
    Product redWineGlass("Red Wine Glass", 8.60);

    FixedDiscount *fiveOff = new FixedDiscount(5.0); // we use pointers now
    PercentageDiscount *tenPercentOff = new PercentageDiscount(10); // for both discounts

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

Since we call `Invoice:add` with a pointer now, we need to change the `Invoice` class:
`invoice.h`
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    vector<Item> getItems() const;
private:
    vector<Item> items;
    vector<Discount*> discounts; // we now have vector of pointers to Discount
};
```

`invoice.cpp`
```c++
void Invoice::add(Discount *discount) { // the function accepts a pointer and adds it to the vector of pointers
    discounts.push_back(discount);
}

double Invoice::totalDiscount() const {
    double sum = 0;
    for (auto discount : discounts) {
        sum += discount->total(); // we `->` instead of `.` when we access members of pointers
    }
    return sum;   
}
```

And finally, to instruct the runtime to keep track of the `total` function actual implementation:
`discount.h`
```c++
class Discount {
public:
    virtual double total() const; // add virtual keyword before the return type
};
```

That's it! Build and run:
~~~
-------------------------------------------------------------------------
|       5|Super Mob                               |     12.90|     64.50|
-------------------------------------------------------------------------
|      12|Tea Cup                                 |      5.30|     63.60|
-------------------------------------------------------------------------
|       8|Red Wine Glass                          |      8.60|     68.80|
-------------------------------------------------------------------------
                                                     Discount|      5.00|
                                                             ------------
                                                     Subtotal|    196.90|
                                                             ------------
                                                        Taxes|     19.69|
                                                             ------------
                                                        TOTAL|    216.59|
                                                             ------------
~~~


Voila! 5.00 fixed discount. 

There is a problem, though - the discount doesn't affect the totals. The discount needs to be applied before taxes, so we need to change `Invoice::subtotal`. Let's make it shine!
`invoice.cpp`
```c++
double Invoice::subtotal() const {
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }
    return sum - totalDiscount();
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
                                                     Discount|      5.00|
                                                             ------------
                                                     Subtotal|    191.90|
                                                             ------------
                                                        Taxes|     19.19|
                                                             ------------
                                                        TOTAL|    211.09|
                                                             ------------
~~~

Done! Fixed discount is fully operational.

> Take a moment to appriciate __single responsibility principle__. We had only one reason to change the `TextPrinter` class - to display new information. This is it. We did quite a few changes to how the information (i.e. discount) is calculated and implemented without needing to touch the `TextPrinter`

Now the only thing we need to do is to implement the `PercentageDiscount`. It needs to know the subtotal of the invoice, however we don't want the `Discount` to depend on the `Invoice` - it is the other way around (avoid creating circular dependencies!).

Let's think a minute about the discount abstraction and what could the future bring. We can have a discount that works on a specific set of items, or a discount where you buy 2 and get 1 free. In all those cases, the discount will need to we aware of the line items on the invoice. What we can do is to "inform" discounts about what items are on the invoice, i.e. add the items as a parameter to the `Discount:total` function.
`discount.h`
```c++
#include "item.h" // we depend on the item class
#include <vector> // also on the vector class from STL

using std::vector; // let's declare we will be using vector and not have to put `std::` prefix every time

class Discount {
public:
    virtual double total(vector<Item> items) const; // change total to accept vector of items
};

class FixedDiscount : public Discount {
public:
    FixedDiscount(double amount);
    double total(vector<Item> items) const; // change the child override, otherwise it will be considered a different function and we will loose the polymorphism
private:
    double amount;
};

class PercentageDiscount : public Discount {
public:
    PercentageDiscount(int percentage);
    double total(vector<Item> items) const; // override the total function
private:
    double rate; // add a member function that will hold the rate, computed by the percentage, i.e. rate = percentage/100 (0.1 = 10/100)
};
```

`discount.cpp`
```c++
double Discount::total(vector<Item> items) const { // only the signature changes, the body still returns 0
    return 0;
}

FixedDiscount::FixedDiscount(double amount) : amount(amount) {}

double FixedDiscount::total(vector<Item> items) const { // only the signature changes, the body still returns fixed amount
    return amount;
}

PercentageDiscount::PercentageDiscount(int percentage) {
    rate = percentage/100.0; // initialize the new member variable
}

double PercentageDiscount::total(vector<Item> items) const { // define total in the child class
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }

    return sum * rate;
}
```

Last but not least - don't forget to change the `invoice.cpp`, where we use the `Discount::total` function:
```c++
double Invoice::totalDiscount() const {
    double sum = 0;
    for (auto discount : discounts) {
        sum += discount->total(items);
    }
    return sum;   
}
```

Done! With a few modifications, without touching the `Invice` class a lot, we were able to introduce a new discount into our program. If we need to add another discount, let's say 50% every second item we can do that by just defining another child class of the `Discount` class and adding an object to the `Invoice` - the invoice will never need to know or change.

Build and run:
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

# The End
This concludes the exercise.
