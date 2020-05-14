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