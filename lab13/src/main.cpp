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
