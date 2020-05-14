# Synpsis
In this exercise, we will explore the memory management aspect of our program. Thus far, we have completely ignore it, except the fact that we have eliminated unnecessary copying of the product objects. 

# Discussion
In C++, it is the developer's responsibility to manage the memory of the program. This can lead to efficiency, but at the cost of productivity.

Whenever we use static variables, they reside on the stack and are managed for us. The problem arises when we leverage the Heap with the `new` keyword. We've done this in the `Catalog` class - before we parse a product from the `product.dat` file, we allocate a new object on the heap. Later, we return the one and only copy of our product to callers of the `Catalog` class to, let's say, add it to the invoice. When we call `new`, memory is allocated on the heap. Until we call `delete` using a pointer to the same memory, this memory will be in use by our program.

Let's think of memory management in two terms - __ownership__ and __sharing__.

When we use static variables (the stack) __ownership__ is easy - the function owns the local variable. When the owner (i.e. the function) is destroyed (i.e. returns) all resources owned by it (i.e. local variables) are disposed. __Sharing__ is also straightforward: we can _share_ a local variable with another function by passing it as an argument. We can pass a copy, where the other function is the _owner_ of the copy, or a reference, in which case our first function remains the _owner_. In the later, sharing works because the caller function's lifespan is longer than that of the called function.

Now, with dynamic memory (the heap) we have the same situation. Consider the following example:
```c++

int sum(int arr[], int n) {
    int s = 0;
    for (int i=0; i<n; i++) {
        s += arr[i];
    }
    return s;
}

int main() {
    int n;
    cin >> n;
    int *arr = new int[n];
    for (int i=0; i<n; i++) {
        cin >> arr[i];
    }
    
    cout << sum(arr) << endl;

    delete [] arr;

    return 0;
}
```

As you can see, we are using dynamic memory, but the __ownership__ and __sharing__ are similar to the one we described for local variables. `main` is the owner of `arr`. It can share it with functions it calls, because each function `main` calls lives shorted than `main`. When the `main` has finished working with `arr`, it can decide to `delete` it, freeing the memory. We can have more code after the `delete [] arr` that deals with other stuff. But this is a very functional and artificial example - in real life, we are rarely in such simple context. In this example we even don't need to call the delete - our program will exit and all its memory will be free.

# Exercise

We will break this exercise in stages.

## 1. Catalog owns and deletes products
Let's look at our program for invoices. We have a `catalog` that allocates `new` products - it makes sense to extend the catalog with a _destructor_ that will delete the allocated memory when a catalog is deleted.

To monitor the lifecycle of products, let's first add a destructor that will print to `cerr` when the object is destroyed.
`product.h`
```c++
class Product {
public:
    Product();
    ~Product();
    Product(int id, string name, double price);
    Product(const Product &other);
    int getID() const;
    string getName() const;
    double getPrice() const;
    friend istream & operator>>(istream &in, Product &product);
    void setName(string name);
    void setPrice(double price);
private:
    int id;
    string name;
    double price;
};
```
`product.cpp`
```c++
Product::~Product()
{
    std::cerr << "~Product " << name << std::endl;
}
```

Now, let's add a destructor to the `Catalog`:
`catalog.h`
```c++
class Catalog
{
public:
    ~Catalog();
    void load(string path);
    Product *get(int id) const;
    vector<Product *> list() const;
    Catalog &operator+=(Product *product);
    void update(int id, string name, double price);
    void add(string name, double price);

private:
    vector<Product *> products;
    fstream file;
    void write(int pos, Product *product);
};
```
`catalog.cpp`
```c++
Catalog::~Catalog()
{
    cerr << "~Catalog" << endl;
    for (auto product : products)
    {
        delete product;
    }
}
```

Done. We now have the catalog owning the products.


## 2. Many products and invoices
Until now, our program has 1 catalog which loads all products and then it allows building a single invoice. Let's make some adjustments. 

Imagine we have 1,000,000 products and it would be impractical to load all of them - we want to load products _on demand_ (aka lazy), i.e. when someone calls `Catalog::get`.

To do this:
 - We will keep the `Catalog::load` function, but instead of loading the products in memory, it will build an index of the existing products. That way we will know what offset to use to read the actual product from the disk. This index will be just a vector of IDs.
 - We will introduce `Catalog::read` that reads a `Product` from a position (i.e. offset) in the file, similarly to the `Catalog::write` function we have.
 - We will change the `Catalog::list` to just return the IDs (for simplicity).

 `catalog.h`
 ```c++
class Catalog
{
public:
    ~Catalog();
    void load(string path);
    Product *get(int id);     // remove the const
    vector<int> list() const; // return ids and not products
    Catalog &operator+=(Product *product);
    void update(int id, string name, double price);
    void add(string name, double price);

private:
    vector<int> index; // replace products collection with a list of IDs
    fstream file;
    Product *read(int pos); // read a product from a position in the file
    void write(int pos, Product *product);
};
 ```

 `catalog.cpp`
 ```c++
 void Catalog::load(string path) // load only product IDs to be able to seek later
{
    file.open(path, ios_base::in | ios_base::out);
    if (file.fail())
    {
        cerr << "Cannot open file " << path << endl;
    }

    int id; // leave just the id, since we will not read anything else

    int line = 0;
    while (file)
    {
        file.seekg(line * RECORD_WIDTH, ios_base::beg);
        file >> id; // read just the ID
        if (!file.fail())
        {
            index.push_back(id);
        }

        line++;
    }
    file.clear();
}

Product *Catalog::get(int id)
{
    for (int i = 0; i < index.size(); i++)
    {
        if (index[i] == id) // find the line of the product with this ID
        {
            int pos = i * RECORD_WIDTH; // calculate offset using the line and the record width
            return read(pos);           // read the product from the position in the file
        }
    }
    return NULL;
}

vector<int> Catalog::list() const
{
    return index;
}

void Catalog::add(string name, double price)
{
    int lastID = 0;
    for (auto id : index)
    {
        lastID = max(lastID, id);
    }
    Product *product = new Product(lastID + 1, name, price);
    *this += product;
    delete product; // delete the product object - we no longer need it
}

Catalog &Catalog::operator+=(Product *product)
{
    this->index.push_back(product->getID());
    this->write(-1, product);
    return *this;
}

void Catalog::update(int id, string name, double price)
{
    for (int i = 0; i < index.size(); i++)
    {
        if (index[i] == id)
        {
            int pos = i * RECORD_WIDTH;
            auto product = read(pos);
            product->setName(name);
            product->setPrice(price);

            write(pos, product);

            delete product;

            return;
        }
    }
    cerr << "No product with ID #" << id << endl;
}

Product *Catalog::read(int pos)
{
    if (!file.is_open())
    {
        cerr << "Catalog file is not open" << endl;
        return NULL; // return NULL, there is no product
    }

    int id;
    char name[NAME_WIDTH + 1];
    double price;

    file.seekg(pos, ios_base::beg);
    file >> id; // read the ID from the beginning of the record

    file.seekg(pos + ID_WIDTH, ios_base::beg); // read the name immediately after the ID field ends
    file.getline(name, NAME_WIDTH + 1);
    if (file.fail() && file.gcount() == NAME_WIDTH)
    {
        file.clear();
    }

    file >> price;

    if (file.fail())
    {
        return NULL; // if we fail to read the product, return empty pointer
    }

    return new Product(id, name, price); // allocate and return a new product
}

void Catalog::write(int pos, Product *product)
{
    if (!file.is_open())
    {
        cerr << "Catalog file is not open" << endl;
        return;
    }

    cerr << "write" << endl;

    if (pos < 0)
    {
        file.seekp(0, ios_base::end);
    }
    else
    {
        file.seekp(pos, ios_base::beg);
    }

    file << left << setw(ID_WIDTH) << product->getID();
    file << left << setw(NAME_WIDTH) << product->getName();
    file << right << setw(PRICE_WIDTH) << product->getPrice();
    file << endl;
    file.flush();
}

Catalog::~Catalog()
{
    cerr << "~Catalog" << endl; // we no longer delete products
}
 ```

 `main.cpp`
 ```c++
 void executeCatalog(Catalog &catalog)
{
    string subCommand;
    cin >> subCommand;
    if (subCommand == "show")
    {
        for (auto id : catalog.list()) // list IDs only
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
void executeAdd(Invoice &invoice, Catalog &catalog) // remove const
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

void executeRemove(Invoice &invoice, Catalog &catalog) // remove const
{
    int id, qty;
    cin >> id >> qty;
    Product *product = catalog.get(id);
    invoice.remove(product, qty);
}
 ```

 A few things we've done:
  - We have removed the const when using the Catalog. Because `file` is a stream and a member of the `Catalog` class, now every operation either _reads_ or _writes_ to the stream, so every operation changes the `file` (updates the position in the stream), therefore every operation changes the `Catalog`.
  - `Catalog::add` allocates a new `Product` object, writes it to the disk and `index` and then disposes (i.e. deletes) the object. It is the sole owner of the object.
  - the `Catalog::~Catalog` destructor no longer deletes products, because products are not stored as members of the `Catalog` object. `Catalog` is no longer the owner of the products retrieved through `Catalog::get`, the caller of `Catalog::get` is.


Let's make one final adjustment to our `main.cpp`. So far, we have a single invoice that lives as long as our program is running. We will change that, giving the user the ability to `close` an invoice and start a new one.

We will create a new invoice object. When the user enters the `done` command, we will delete this object and create a new invoice:
`main.cpp`
```c++
int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cerr << "Invalid usage. Usage: ./lab9 path_to_catalog_file" << std::endl;
        return 1;
    }

    Invoice *invoice = new Invoice(); // invoice is now a pointer
    Catalog catalog;

    catalog.load(argv[1]);

    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "catalog")
            executeCatalog(catalog);
        else if (cmd == "add")
            executeAdd(*invoice, catalog); // we need to dereference the pointer to pass a reference to the object itself
        else if (cmd == "remove")
            executeRemove(*invoice, catalog);
        else if (cmd == "discount")
            executeDiscount(*invoice);
        else if (cmd == "print")
            executePrint(*invoice);
        else if (cmd == "done")
        {
            delete invoice; // we are done with this invoice
            invoice = new Invoice(); // we create a new one
        }
        else
        {
            cerr << "Unrecognized command " << cmd << endl;
        }
    }

    return 0;
}
```

Notice that products are no longer destroyed (unless we do `catalog add`).

Congratiolations! Our program now has memory leak. We allocate new products whenever we add them to the invoice, we delete the invoice, but the product remains in memory with no pointers to them and nobody to delete them. Open task manager to monitor the memory of the program and execute add several times - notice it grows, even after we do the `done` command.

First, we must ask who is the owner of those leaky products. And the answer is quite simple - the invoice. Let's modify the invoice class so that it deletes all associated products.

But it is not that simple. The invoice doesn't contain products, it contains items and each item has a product. So, the `Item` is the owner of the `Product`, while the `Invoice` is the owner of the `Item`. Let's try this

`item.h`
```c++
class Item {
public:
    Item(const Product *product, int quantity);
    ~Item(); // declare the destructor
    double total();
    int getProductID() const;
    string getDescription() const;
    int getQuantity() const;
    void setQuantity(int qty);
    double getPrice() const;
    double total() const;
private:
    const Product *product;
    int quantity;
};
```

`item.cpp`
```c++
Item::~Item()
{
    std::cerr << "~Item" << std::endl;
    delete product; // delete the product
}
```

`invoice.h`
```c++
class Invoice
{
public:
    ~Invoice(); // declare the destructor
    void add(const Product *product, int quantity);
    void remove(const Product *product, int quantity);
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    vector<Item> getItems() const;
    void clearDiscounts();

private:
    vector<Item> items;
    vector<Discount *> discounts;

    vector<Item>::iterator find(const Product *product);
};
```

`invoice.cpp`
```c++
Invoice::~Invoice()
{ 
    std::cerr << "~Invoice" << std::endl;
    // items are static members of the Invoice class, so they will be automatically deleted
}
```

Let's test it. Run the following commads once you start the program:
~~~
add 1 2
~Item
~Product Super Mob
print
-------------------------------------------------------------------------
|       2|�s���b                               |     11.90|     23.80|
-------------------------------------------------------------------------
~Item
~Product [1]    4360 segmentation fault (core dumped)  ./lab11 data/products.dat
~~~

Notice that the item destructor is called immediately after we enter the `add` command. This is because we are copying the items around. We will need to change our program to use pointers to items, if we are to bind the lifecycle of products to them.


`discount.h`
```c++
class Discount
{
public:
    virtual double total(const vector<Item *> items) const;  // change signature
};

class FixedDiscount : public Discount
{
public:
    FixedDiscount(double amount);
    double total(const vector<Item *> items) const;  // change signature

private:
    double amount;
};

class PercentageDiscount : public Discount
{
public:
    PercentageDiscount(int percentage);
    double total(const vector<Item *> items) const; // change signature

private:
    double rate;
};
```
`discount.cpp`
```c++

double Discount::total(const vector<Item *> items) const  // change signature
{
    return 0;
}

FixedDiscount::FixedDiscount(double amount) : amount(amount) {}

double FixedDiscount::total(const vector<Item *> items) const // change signature
{
    return amount;
}

PercentageDiscount::PercentageDiscount(int percentage)
{
    rate = percentage / 100.0;
}

double PercentageDiscount::total(const vector<Item *> items) const // change signature
{
    double sum = 0;
    for (auto item : items)
    {
        sum += item->total(); // use pointer
    }

    return sum * rate;
}
```
`invoice.h`
```c++
class Invoice
{
public:
    ~Invoice();
    void add(const Product *product, int quantity);
    void remove(const Product *product, int quantity);
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    const vector<Item *> getItems() const; // change signature
    void clearDiscounts();

private:
    vector<Item *> items; // store pointers
    vector<Discount *> discounts;

    vector<Item *>::iterator find(const Product *product); // change signature
};
```
`invoice.cpp`
```c++
Invoice::~Invoice()
{
    std::cerr << "~Invoice" << std::endl;
    for (auto item : items)
    {
        delete item; // delete all items still in the collection
    }
}

void Invoice::add(const Product *product, int quantity)
{
    auto it = find(product);
    if (it == items.end())
    {
        items.push_back(new Item(product, quantity)); // allocate new item on the heap
    }
    else
    {
        auto item = *it; // dereference the iterator to get the pointer
        item->setQuantity(item->getQuantity() + quantity);
    }
}

void Invoice::remove(const Product *product, int quantity)
{
    auto it = find(product);
    if (it != items.end())
    {
        auto item = *it; // dereference the iterator to get the pointer
        int remaining = std::max(item->getQuantity() - quantity, 0);
        if (remaining > 0)
        {
            item->setQuantity(remaining);
        }
        else
        {
            items.erase(it);
            delete item; // delete the item before removing it from the collection, as we no longer need it
        }
    }
}
vector<Item *>::iterator Invoice::find(const Product *product) // change signature
{
    for (auto it = items.begin(); it != items.end(); it++)
    {
        if ((*it)->getProductID() == product->getID()) // dereference the iterator to access the pointer
        {
            return it;
        }
    }
    return items.end();
}
double Invoice::subtotal() const
{
    double sum = 0;
    for (auto item : items)
    {
        sum += item->total(); // use pointer
    }
    return sum - totalDiscount();
}
const vector<Item *> Invoice::getItems() const // change signature
{
    return items;
}
```
`textprinter.cpp`
```c++

void TextPrinter::print(std::ostream &out, Invoice invoice)
{
    out << std::fixed << std::setprecision(2);

    out << std::setfill('-') << setw(68 + 5) << "" << std::setfill(' ') << endl;

    for (auto item : invoice.getItems())
    {
        out << '|';
        out << setw(8) << right << item->getQuantity() << '|'; // use pointer
        out << setw(40) << left << item->getDescription() << '|'; // use pointer
        out << setw(10) << right << item->getPrice() << '|'; // use pointer
        out << setw(10) << right << item->total() << '|'; // use pointer
        out << endl;

        out << std::setfill('-') << setw(68 + 5) << "" << std::setfill(' ') << endl;
    }

    out << setw(58 + 3) << right << "Discount" << '|' << setw(10) << right << invoice.totalDiscount() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58 + 3) << right << "Subtotal" << '|' << setw(10) << right << invoice.subtotal() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58 + 3) << right << "Taxes" << '|' << setw(10) << right << invoice.taxes() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58 + 3) << right << "TOTAL" << '|' << setw(10) << right << invoice.total() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
}
```

Let's test again:
~~~
add 1 2
print
-------------------------------------------------------------------------
|       2|Super Mob                               |     11.90|     23.80|
-------------------------------------------------------------------------
                                                     Discount|      0.00|
                                                             ------------
                                                     Subtotal|     23.80|
                                                             ------------
                                                        Taxes|      2.38|
                                                             ------------
                                                        TOTAL|     26.18|
                                                             ------------
~Invoice
~Item
~Product Super Mob
~~~

We see the invoice destructor being called - not OK. This is because we are copying the invoice somewhere. Let's defend against that by forbidding the copy constructor:
`invoice.h`
```c++
class Invoice
{
public:
    Invoice(); // decleare a default constructor
    Invoice(const Invoice &) = delete; // mark the copy constructor as deleted
    ~Invoice();
    void add(const Product *product, int quantity);
    void remove(const Product *product, int quantity);
    void add(Discount *discount);
    double subtotal() const;
    double taxes() const;
    double total() const;
    double totalDiscount() const;
    const vector<Item *> getItems() const;
    void clearDiscounts();

private:
    vector<Item *> items;
    vector<Discount *> discounts;

    vector<Item *>::iterator find(const Product *product);
};
```

Execute `make` and you will see exactly where is the problem:
~~~
[ 12%] Building CXX object CMakeFiles/lab11.dir/src/main.cpp.o
/home/kiko/uni/CITB205-2020/lab11/src/main.cpp: In function ‘void executePrint(const Invoice&)’:
/home/kiko/uni/CITB205-2020/lab11/src/main.cpp:108:37: error: use of deleted function ‘Invoice::Invoice(const Invoice&)’
     printer.print(std::cout, invoice);
                                     ^
In file included from /home/kiko/uni/CITB205-2020/lab11/src/main.cpp:4:0:
/home/kiko/uni/CITB205-2020/lab11/src/invoice.h:14:5: note: declared here
     Invoice(const Invoice &) = delete;
     ^~~~~~~
In file included from /home/kiko/uni/CITB205-2020/lab11/src/main.cpp:5:0:
/home/kiko/uni/CITB205-2020/lab11/src/textprinter.h:8:10: note:   initializing argument 2 of ‘void TextPrinter::print(std::ostream&, Invoice)’
     void print(std::ostream &out, Invoice invoice);
          ^~~~~
~~~


Indeed, we are not passing a reference to the invoice in our print function.

Let's use a reference so that we don't copy the invoice:
`textprinter.h`
```c++
class TextPrinter
{
public:
    void print(std::ostream &out, const Invoice &invoice); // change signature
};
```

`textprinter.cpp`
```c++
void TextPrinter::print(std::ostream &out, const Invoice &invoice) // change signature
{
    out << std::fixed << std::setprecision(2);

    out << std::setfill('-') << setw(68 + 5) << "" << std::setfill(' ') << endl;

    for (auto item : invoice.getItems())
    {
        out << '|';
        out << setw(8) << right << item->getQuantity() << '|';    // use pointer
        out << setw(40) << left << item->getDescription() << '|'; // use pointer
        out << setw(10) << right << item->getPrice() << '|';      // use pointer
        out << setw(10) << right << item->total() << '|';         // use pointer
        out << endl;

        out << std::setfill('-') << setw(68 + 5) << "" << std::setfill(' ') << endl;
    }

    out << setw(58 + 3) << right << "Discount" << '|' << setw(10) << right << invoice.totalDiscount() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58 + 3) << right << "Subtotal" << '|' << setw(10) << right << invoice.subtotal() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58 + 3) << right << "Taxes" << '|' << setw(10) << right << invoice.taxes() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58 + 3) << right << "TOTAL" << '|' << setw(10) << right << invoice.total() << '|' << endl;
    out << setw(58 + 3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
}
```

Voila! Go ahead and play a bit with the program. Notice that products are allocated on demand and their lifecycle is bound to that of their invoice item.