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





