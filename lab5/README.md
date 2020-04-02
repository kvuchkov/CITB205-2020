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




