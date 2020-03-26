# Synpsis
The goal of this exercise is to implement the empty member functions from Lab #2 and print the _subtotal_ of the invoice. At this point, do not waste time trying to format the output of the `TextPrinter` class, but focus on implementing the functions of the other classes.

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab3`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

The steps are similar to what we had in Lab #2. As we implement a member function (i.e. fill the empty function body with code), this will lead to cascading changes in other classes. For example, when we add a code in the `TextPrinter`'s `print` function to show the total of the invoice, we will have to go to the `Invoice` class and declare a new member function. Remember - declarations go to the `.h` file, while definitions (i.e. the body) goes to the `.cpp` file of the class.


Steps:
1. Write a minimal, yet functional function body. For example, make the `TextPrinter::print` function output the total of the invoice.
2. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see compilation errors.
3. Declare members of this and other classes so that the code added in step #1 compiles.
4. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see linking errors.
5. Define empty bodies of the member functions added in step #3 (i.e. linking errors in #4)
6. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. The build should be successful.

Now go to the next empty function body, implement it and repeat steps #1 to #6.

>Tip: You can commit your changes in the `master` branch after each run of steps 1 to 6, i.e. having a checkpoint of a compiling program. To do that: `git add .` and then `git commit -m "your message"`.

At the end, you should be able to run `./lab3` on Linux/Mac or `lab3.exe` on Windows and see the subtotal of the invoice.

# Example Steps

Build: `make` on Mac/Linux or `mingw32-make` on Windows. It should be OK.

Now, let's start simple. We will print the subtotal of the invoice - no taxes, we will just sum the line items, where each line item total is `quantity` x `product.price`. Go to `textprinter.cpp`:
```c++
void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << invoice.subtotal() << std::endl;
}
```
> Notice we use `out` and not `cout`. This is because the `cout` is passed as an argument by reference to the `print` function from `main`. This way, we can later replace `cout` with a file stream, and our `TextPrinter` class will be blissfully unaware of this.

Build by `make` on Mac/Linux or `mingw32-make` on Windows:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/textprinter.cpp:4:20: error: no member named 'subtotal' in 'Invoice'
    out << invoice.subtotal() << std::endl;
           ~~~~~~~ ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/textprinter.cpp:4:39: error: no member named 'endl' in namespace 'std'; did you mean 'end'?
    out << invoice.subtotal() << std::endl;
                                 ~~~~~^~~~
                                      end
/Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/initializer_list:109:1: note: 'end' declared here
end(initializer_list<_Ep> __il) _NOEXCEPT
^
2 errors generated.
make[2]: *** [CMakeFiles/lab3.dir/src/textprinter.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

Don't worry, this was expected and we've went through this whole process in lab #2. Let's make it compile first.

Go to `invoice.h` and declare the function:
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    double subtotal();
};
```

Now build by running `make` on Mac/Linux or `mingw32-make` on Windows:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
[ 50%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/textprinter.cpp:4:9: error: invalid operands to binary expression ('std::ostream' (aka 'basic_ostream<char>') and 'double')
    out << invoice.subtotal() << std::endl;
    ~~~ ^  ~~~~~~~~~~~~~~~~~~
/Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:5393:1: note: candidate template ignored: could not match 'shared_ptr<type-parameter-0-2>' against 'double'
operator<<(basic_ostream<_CharT, _Traits>& __os, shared_ptr<_Yp> const& __p);
^
/Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/string:4239:1: note: candidate template ignored: could not match 'basic_string<type-parameter-0-0, type-parameter-0-1, type-parameter-0-2>' against 'double'
operator<<(basic_ostream<_CharT, _Traits>& __os,
^
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/textprinter.cpp:4:39: error: no member named 'endl' in namespace 'std'; did you mean 'end'?
    out << invoice.subtotal() << std::endl;
                                 ~~~~~^~~~
                                      end
/Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/initializer_list:109:1: note: 'end' declared here
end(initializer_list<_Ep> __il) _NOEXCEPT
^
2 errors generated.
make[2]: *** [CMakeFiles/lab3.dir/src/textprinter.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

Compilation is successful, but linking is not, because we are still missing the body of the new function `subtotal`. Go to `invoice.cpp` and define it as empty as possible:
```c++
double Invoice::subtotal() {
    return 0;
}
```

Now, let's build again by running `make` on Mac/Linux or `mingw32-make` on Windows:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
[ 33%] Linking CXX executable lab3
[100%] Built target lab3
~~~ 

Build is successful, let's run our program by `./lab3` on Linux/Mac or `lab3.exe` on Windows:
~~~
0
~~~

Congratulations! \o/ Our program does something for a change - it prints `0`, returned by `Invoice::subtotal`.

Next, we repeat the process, i.e. back to step #1. This time, we will implement `Invoice::subtotal` for real. To do this, we need to think __abstract__ (i.e. the OOP way). What is the simplest,
 most abstract way we can calculate the subtotal of the invoice? It comes from the definition of the domain problem: an invoice sub total is the sum of each line item's total. Let's focus
 on that and not deal with what is line item's total (this is a problem for later).

Go to `invoice.cpp`:
```c++
void Invoice::add(Product product, int quantity) {}

double Invoice::subtotal() {
    double sum = 0;
    for (auto item : items) {
        sum += item.total();
    }
    return sum;
}
```

OK, for this to work, we will need to add our first member variable - a collection of items on the invoice. Let's use vector for this. Go to `invoice.h`:
```c++
class Invoice {
public:
    void add(Product product, int quantity);
    double subtotal();
private:
    vector<Item> items;
};
```
> Notice we have placed the `items` member variable in the `private` section. This is called __encapsulation__ - we hide and protect the internal representation of our object's state 
from the outside world. This gives us the ability to control modifications and to change the implementation (e.g. use linked list instead of a vector) later without affecting other code in the program.

Done! Let's build it by running `make` on Mac/Linux or `mingw32-make` on Windows:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/invoice.cpp:7:21: error: no member named 'total' in 'Item'
        sum += item.total();
               ~~~~ ^
1 error generated.
make[2]: *** [CMakeFiles/lab3.dir/src/invoice.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

Notice we are again in the same loop? Let's go to `item.h` and declare the missing function:
```c++
class Item {
public:
    double total();
};
```

Build it by running `make` on Mac/Linux or `mingw32-make` on Windows:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
[ 50%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
[ 66%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
[ 83%] Linking CXX executable lab3
Undefined symbols for architecture x86_64:
  "Item::total()", referenced from:
      Invoice::subtotal() in invoice.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [lab3] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

Go to `item.cpp` to define the function:
```c++
double Item::total() {
    return 0;
}
```

Build it by running `make` on Mac/Linux or `mingw32-make` on Windows:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
[ 33%] Linking CXX executable lab3
[100%] Built target lab3
~~~

Built is successful, let's run it (`./lab3` on Linux/Mac or `lab3.exe`)
~~~
0
~~~

It is `0` again, but this zero was calculated by iterating over the items collection in the invoice.

We did one more go of steps 1 to 6. It is a good time to commit our changes: `git add .` and then `git commit -m "your message"`.

Now we need to deal with the problem of calculating line item total. Notice how we can now focus on this problem alone, without thinking about the invoice or the printer. This is one of the many benefits of __abstraction__ or __separation of concern__ - having to deal with one problem at a time.

Open `item.cpp` and implement the total method for real:
```c++
double Item::total() {
    return product.getPrice() * quantity;
}
```

For this to work, we will need to member variables: `product`, which is an object of the `Product` class and `quantity`, which indicates how many of the product we have on the line item. Add those to the `item.h`:
```c++
class Item {
public:
    double total();
private:
    Product product;
    int quantity;
};
```

OK, let's build it (don't remember how? look at the instructions above):
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/item.cpp:4:20: error: no member named 'getPrice' in 'Product'
    return product.getPrice() * quantity;
           ~~~~~~~ ^
1 error generated.
make[2]: *** [CMakeFiles/lab3.dir/src/item.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

__Question__: Why do we need a function `getPrice` on the `Product` class? Can't we just have a member variable that is public? 

__Answer__: No, especially if we want to control the access to the price:
 1. First, this is the main difference between a structure of data (i.e. `struct`) and a `class`. By class, we usually mean a set
of behaviours attached to a state. With classes, behaviour is more important - this is why we have public methods, we have `public` inheritence where we inherit the `behaviour`. Therefore, it usually makes sense to hide and protect the state. This is the reason why in classes the default accessibility is `private`, while in `struct` the default is `public`.
2. Second, not every double is a valid price. We cannot have `negative` prices. The language doesn't allow us to enforce this _invariance_ (i.e. rule), so by _encapsulation_ we can make sure that 
there is no `Product` objects with negative price.

Go to `product.h` and declare the `getPrice` member function:
```c++
class Product {
public:
    Product(string name, double price);
    double getPrice() const;
};
```

Functions like `getPrice` are sometimes called accessor functions - they don't accept any arguments and expose some state of the object without modifying it. Notice the `const` keyword __after__ the function parameters. This indicates that the function will be called on a `const` reference of the object, i.e. the function will not modify the internal state of the class. It is a good idea
to annotate functions like this whenever possible - this way the compiler will protect you from accidentally modifying the state when you don't want to.

Build the program:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/product.cpp.o
[ 50%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
[ 66%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
[ 83%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
[100%] Linking CXX executable lab3
Undefined symbols for architecture x86_64:
  "Product::getPrice() const", referenced from:
      Item::total() in item.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [lab3] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

Go to `product.cpp` and define `getPrice`:
```c++
double Product::getPrice() const {
    return 0;
}
```

Build:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/product.cpp.o
[ 33%] Linking CXX executable lab3
[100%] Built target lab3
~~~

Now, let's fast forward a bit and implement the getPrice for real. To do this, we will need to also implement the constructor of the `Product` class and add member variables to preserve the state.

Go to `product.h`:
```c++
class Product {
public:
    Product(string name, double price);
    double getPrice() const;
private:
    double price;
};
```

Then go to `product.cpp`:
```c++
Product::Product(string name, double price) {
    this->price = price;
}
double Product::getPrice() const {
    return price;
}
```

> Notice the use of the `this` keyword. `this` is a pointer to the object on which we are calling the function. For example, in the main we have a variable `superMob`. When we call a member function on the `suberMob` object, (e.g. `superMob.foo()`), inside the `foo` function we will have access to all member variables (e.g. `price`) as well as `this`, and `this` will be a pointer to `superMob`. In this particular case, we need `this` in the constructor to distinquish between `price` as a parameter of the constructor and `price` as a member variable in the class. Remember, that the inner-most scope wins when we have `shadowing` of variable names, so in the constructor just `price` will actually refer to the argument, not the member.

Build:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/product.cpp.o
[ 50%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
[ 66%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
[ 83%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
[100%] Linking CXX executable lab3
[100%] Built target lab3
~~~

Run the program (don't remember how? look at the instructions above),

Our program still outputs `0`, but we are close. __Can you figure out what is missing?__

`Invoice::add` doesn't do anything and when we call `subtotal`, there is an empty vector of `items`. Let's fix this by implementing the `add` function in `invoice.cpp`:
```c++
void Invoice::add(Product product, int quantity) {
    Item item(product, quantity);
    items.push_back(item);
}
```
We construct a new item object (statically, on the _stack_) by passing product and quantity to the constructor and then we add this new item object at the end of the `items` vector.

Build:
~~~
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/invoice.cpp:4:10: error: no matching constructor for initialization of 'Item'
    Item item(product, quantity);
         ^    ~~~~~~~~~~~~~~~~~
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/item.h:7:7: note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 2 were provided
class Item {
      ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/item.h:7:7: note: candidate constructor (the implicit move constructor) not viable: requires 1 argument, but 2 were provided
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/item.h:7:7: note: candidate constructor (the implicit default constructor) not viable: requires 0 arguments, but 2 were provided
1 error generated.
make[2]: *** [CMakeFiles/lab3.dir/src/invoice.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

True, we haven't declared any constructors of the `Item` class. Let's do that in `item.h`:
```c++
class Item {
public:
    Item(Product product, int quantity);
    double total();
private:
    Product product;
    int quantity;
}
```

Build:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/main.cpp.o
[ 33%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
[ 50%] Building CXX object CMakeFiles/lab3.dir/src/invoice.cpp.o
[ 66%] Building CXX object CMakeFiles/lab3.dir/src/textprinter.cpp.o
[ 83%] Linking CXX executable lab3
Undefined symbols for architecture x86_64:
  "Item::Item(Product, int)", referenced from:
      Invoice::add(Product, int) in invoice.cpp.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make[2]: *** [lab3] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

No body for the constructor, define it in `item.cpp`:
```c++
Item::Item(Product product, int quantity) {
    this->product = product;
    this->quantity = quantity;
}
```

Build:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/item.cpp:3:7: error: constructor for 'Item' must explicitly initialize the member 'product' which does not have a default constructor
Item::Item(Product product, int quantity) {
      ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/item.h:12:13: note: member is declared here
    Product product;
            ^
/Users/kiril.vuchkov/uni/CITB205-2020/lab3/src/product.h:8:7: note: 'Product' declared here
class Product {
      ^
1 error generated.
make[2]: *** [CMakeFiles/lab3.dir/src/item.cpp.o] Error 1
make[1]: *** [CMakeFiles/lab3.dir/all] Error 2
make: *** [all] Error 2
~~~

Now this is a tricky error. The problem is that when we explicitly declared a constructor of `Product`, the _implicit_ default constructor (i.e. without arguments) is no longer generated by the compiler. Remember how when we write everything in the `main`, primitive types like `int` are not initialized, but classes are (e.g. if we declare a string variable, it will be an empty string, ergo initialized)? Same applies when dealing with classes. Because `Item` has a static member of type `Product` (i.e. it is not a pointer), this static member needs to be initialized before we enter the constructor of `Item` - it is as if we have declared a variable of type `Product` at the beginning of the constructor function. We have two ways to make this work:
1. Define a default constructor of `Product`. Classes can have multiple constructors as long as they accept different parameters. You should already know how to do this.
2. Use _member initializer list_ on the `Item`'s constructor and initialize the `product` member directly with an existing non-default constructor.

We will go for option #2 - go to the `item.cpp`:
```c++
Item::Item(Product product, int quantity) : product(product) {
    this->quantity = quantity;
}
```

__But wait!__ We haven't defined a `Product(Product p)` constructor anywhere. True, but by default the compiler will generate the so called _copy constructor_, which accepts a _reference_ of the same class (i.e. signature is `Product(Product &product)`). This implicit constructor will copy all members of product to the new object. 

__Wait, wait, why is there such a thing?__ Remember that function parameters are __passed by value__ by default? Well, for this to work with our classes, the _copy constructor_ is invloved whenever we pass our classes by value. Sometimes, you will need to override the implicit _copy constructor_, e.g. when your class has pointers.

Build:
~~~
Scanning dependencies of target lab3
[ 16%] Building CXX object CMakeFiles/lab3.dir/src/item.cpp.o
[ 33%] Linking CXX executable lab3
[100%] Built target lab3
~~~

Run:
~~~
196.9
~~~

Voila! We have our program actually doing something!

# The End
Now that our program is outputing the actual subtotal of the invoice, we are done with this exrecise and can move to Lab #4

