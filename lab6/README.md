# Synpsis
In this exercise, we will use the copy constructors of our classes to investigate how many times we are making copies and then eliminate unnessesary copying.

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
