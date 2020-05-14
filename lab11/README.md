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