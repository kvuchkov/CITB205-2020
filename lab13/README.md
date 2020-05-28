# Synpsis
In this exercise, we will use _exceptions_ to control the unexpected situations in our program.

Until now, the only way we could deal with exceptional situations was to print an error message to the `cerr` for example, which is a side effect. In general, we would like to keep side effects in our program at a minimum and give the _caller_ of a function the responsibility to deal with a situation that we weren't able to deal with inside our function. Printing an error to `stderr` is only one possible way to _handle_ an _exceptional_ situation.

# Requirements
 * Change the `Catalog` class to _throw_ exceptions instead of printing error messages.
 * Change `main.cpp` to _catch_ the exceptions thrown by `Catalog` and report the errors to `cerr`
 * Go through the program and think of unexpected situations that might occur and introduce exceptions to handle them explicitly

# Discussion
Exceptions and error handling in general is a big topic in many languages. The concept of exceptions is only one of the ways to approach errors that happen in our programs and it is used across a number of languages (Java, C#, JavaScript, Python, etc.). It is - however - not a universal approach. `cin` for example in C++ doesn't throw an exception if the input is not read properly - it instead sets the `cin.fail` flag and the programmer can _query_ for the specific error that has happened.

When to throw an exception and when to do something else? This is a rather philosophical question, so take the next few guidelines with a pinch of salt:
 * Throwing an exception is forcing work on the caller of your function. Before doing this, try to figure out if there is a way you can handle the situation in your own code. Changing the function to _define the exception out of existance_ is sometimes an option.
 * Exceptions is for _exceptional_ situations (this is where the name comes from). For example, in a class or function that validates the user input, invalid input is not exceptional in any way - it is expected and the entire purpose of this class/function is to detect those situations. Opening the configuration file of a program, on the other hand, is expected to work and a lack of access permissions can be considered an _exceptional_ situation.
 * Handle exceptions/errors only ones. Catching an exception, logging and then re-throwing is handling it twice.


 In our case, we will use two exception classes:
 * `std::runtime_error` - use it for errors that couldn't have been prevented/detected before the program is executed. E.g. file is missing/inaccessible
 * `std::logic_error` - use it for errors that could have been prevented/detected before the program is executed, e.g. trying to read/write products to the catalog before the catalog has been loaded from disk.

 # Exercise

 First, let's change the `catalog.cpp` to use exceptions instead of printing errors.

`catalog.cpp`
```c++

```

Now, build and run the program. Try updating a non-existing product:
~~~
catalog update 15 ala 12
terminate called after throwing an instance of 'std::runtime_error'
  what():  No product with the specified ID
[1]    1865 abort (core dumped)  ./lab13 data/products.dat
~~~

As you see, our program terminates, but we see why (i.e. what happened). You can think of exceptions as an alternative return path - the exception is bubling up the call stack until it is catched by some of the callers or it hits the program bounderies, in which case the program terminates.

Now, let's change `main.cpp` to _catch_ the exception:
```c++
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "invoice.h"
#include "textprinter.h"
#include "discount.h"
#include "catalog.h"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::logic_error;
using std::right;
using std::runtime_error;
using std::setprecision;
using std::setw;

void executeCatalog(Catalog &catalog)
{
    string subCommand;
    cin >> subCommand;
    if (subCommand == "show")
    {
        for (auto id : catalog.list())
        {
            cout << id << endl;
        }
    }
    else if (subCommand == "add")
    {
        string name;
        double price;
        cin >> name >> price;
        catalog.add(name, price);
    }
    else if (subCommand == "update")
    {
        int id;
        string name;
        double price;
        cin >> id >> name >> price;
        catalog.update(id, name, price);
    }
    else
    {
        cerr << "Unrecognized catalog command " << subCommand << endl;
    }
}

void executeAdd(Invoice &invoice, Catalog &catalog)
{
    int id, qty;
    cin >> id >> qty;
    Product *product = catalog.get(id);
    if (!product)
    {
        cerr << "Cannot find product #" << id << endl;
        return;
    }
    if (qty < 1)
    {
        cerr << "Quantity should be > 0" << endl;
        return;
    }

    invoice.add(product, qty);
}

void executeRemove(Invoice &invoice, Catalog &catalog)
{
    int id, qty;
    cin >> id >> qty;
    Product *product = catalog.get(id);
    invoice.remove(product, qty);
}

void executeDiscount(Invoice &invoice, Inventory<Discount> &discounts)
{
    string subcmd;
    cin >> subcmd;
    if (subcmd == "fixed" || subcmd == "off")
    {
        Discount *discount;
        if (subcmd == "fixed")
        {
            double amount;
            cin >> amount;
            discount = new FixedDiscount(amount);
        }
        else
        {
            int perc;
            cin >> perc;
            discount = new PercentageDiscount(perc);
        }

        bool ok = discounts.remove(discount, 1);
        if (ok)
        {
            invoice.apply(discount);
        }
        else
        {
            cerr << "No discounts left." << endl;
        }
    }
    else if (subcmd == "clear")
    {
        invoice.clearDiscounts();
    }
    else
    {
        cerr << "Unrecognized discount command " << subcmd << endl;
        return;
    }
}

void executePrint(const Invoice &invoice)
{
    TextPrinter printer;
    printer.print(std::cout, invoice);
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cerr << "Invalid usage. Usage: ./lab9 path_to_catalog_file" << std::endl;
        return 1;
    }

    Invoice *invoice = new Invoice();
    Catalog catalog;
    Inventory<Discount> discounts;
    discounts.add(new FixedDiscount(10), 2);
    discounts.add(new FixedDiscount(50), 1);
    discounts.add(new PercentageDiscount(5), 1);
    discounts.add(new PercentageDiscount(10), 1);

    catalog.load(argv[1]);

    string cmd;
    while (cin >> cmd)
    {
        try
        {
            if (cmd == "catalog")
                executeCatalog(catalog);
            else if (cmd == "add")
                executeAdd(*invoice, catalog);
            else if (cmd == "remove")
                executeRemove(*invoice, catalog);
            else if (cmd == "discount")
                executeDiscount(*invoice, discounts);
            else if (cmd == "print")
                executePrint(*invoice);
            else if (cmd == "done")
            {
                delete invoice;
                invoice = new Invoice();
            }
            else
            {
                cerr << "Unrecognized command " << cmd << endl;
            }
        }
        catch (runtime_error &re)
        {
            cerr << "ERROR: " << re.what() << endl;
        }
    }

    return 0;
}
```

Now let's build and run the program again:
~~~
catalog update 15 ala 12
ERROR: No product with the specified ID
~~~
This time the program handles the exception, informs the user of the error and continues.

Let's try something else. Run the program with the following argument:
Linux/Mac:
~~~
./lab13 data/products.data
~~~
Windows
~~~
lab13.exe data\products.data
~~~

You will see another exception and the program will crash:
~~~
terminate called after throwing an instance of 'std::runtime_error'
  what():  Cannot open catalog file
[1]    2031 abort (core dumped)  ./lab13 data/products.data
~~~

It is the same `runtime_error`. However, we didn't catch this because it was thrown elsewhere in the `main`. Only exceptions happening inside the `try` block will be _catched_ by the `catch` block.

In this case, we decide to not handle the exception. If a wrong file was specified during program start, there is really nothing we can do about it - the program must be started again, this time with a correct argument. This is _fail fast_ approach - if there is no reasonable way to continue to operate under the exceptional situation, it is better to just crash, instead of risking more damage.


Let's try to produce a `logic_error`. We _throw_ logical if we try to interact with the catalog before loading it from disk, i.e. calling `Catalog::load`. The way our program is written, it is impossible to cause `logic_error`. This is the idea of this exception - it can be prevented by constructing the code appropriately. Having a logical error indicates a bug in our program that needs to be fixed. On the other hand, having a `runtime_error` is not something we can prevent or predict by just looking at the code.

Now, go ahead and introduce a bug in `main.cpp` by commenting out the line that loads the catalog from disk (i.e. `// catalog.load(argv[1]);`). Build and run the program:
~~~
add 1 2
Cannot find product #1
~~~

This was unfortunate. Notice that we are trying to add product with ID 1 from a catalog that hasn't been loaded. But the program is operational, as if the product is just not there. This is undesirable - we are confused, because we see the product in the catalog and yet our program reports it is missing. To fix this, go to `catalog.cpp` and check that the catalog file was loaded before answering inqueries:
```c++
add 1 2
terminate called after throwing an instance of 'std::logic_error'
  what():  Catalog file is not open
[1]    2247 abort (core dumped)  ./lab13 data/products.data
```

This is much better! We see a logical error. The program crashes, but we are immediately aware that there is a bug and we not exactly what to look for - we have forgotten to load the catalog from disk.

