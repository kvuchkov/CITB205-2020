#include "catalog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <cstring>

using namespace std;

#ifdef _WIN32
const int NEWLINE_WIDTH = 2;
#else
const int NEWLINE_WIDTH = 1;
#endif

const int ID_WIDTH = 5;
const int NAME_WIDTH = 40;
const int PRICE_WIDTH = 10;
const int RECORD_WIDTH = ID_WIDTH + NAME_WIDTH + PRICE_WIDTH + NEWLINE_WIDTH;

void Catalog::load(string path) // load only product IDs to be able to seek later
{
    file.open(path, ios_base::in | ios_base::out);
    if (file.fail())
    {
        throw runtime_error("Cannot open catalog file");
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
    if (!file.is_open())
    {
        throw logic_error("Catalog file is not open");
    }

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
    delete product;
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
    throw runtime_error("No product with the specified ID");
}

Product *Catalog::read(int pos)
{
    if (!file.is_open())
    {
        throw logic_error("Catalog file is not open");
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
        throw logic_error("Catalog file is not open");
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