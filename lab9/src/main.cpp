#include <iostream>
#include <iomanip>

#include "invoice.h"
#include "textprinter.h"
#include "discount.h"
#include "catalog.h"


using std::endl;
using std::cerr;
using std::cin;
using std::cout;
using std::setw;
using std::right;
using std::left;
using std::fixed;
using std::setprecision;

void executeShow(const Catalog &catalog) {
    for(auto product : catalog.list()) {
        cout << left << setw(5) << product->getID();
        cout << left << setw(40) << product->getName();
        cout << right << setw(10) << fixed << setprecision(2) << product->getPrice();
        cout << endl;
    }
}

void executeProduct(Catalog &catalog) {
    Product *product = new Product();
    cin >> *product;
    catalog += product;
}


void executeAdd(Invoice &invoice, const Catalog &catalog) {
    int id, qty;
    cin >> id >> qty;
    Product * product = catalog.get(id);
    if (!product) {
        cerr << "Cannot find product #" << id << endl;
        return;
    }
    if (qty < 1) {
        cerr << "Quantity should be > 0" << endl;
        return;
    }

    invoice.add(product, qty);
}

void executeRemove(Invoice &invoice, const Catalog &catalog) {
    int id, qty;
    cin >> id >> qty;
    Product * product = catalog.get(id);
    invoice.remove(product, qty);
}

void executeDiscount(Invoice &invoice) {
    string discount;
    cin >> discount;
    if (discount == "fixed") {
        double amount; 
        cin >> amount;
        invoice.add(new FixedDiscount(amount));
    } else if(discount == "off") {
        int percentage;
        cin >> percentage;
        invoice.add(new PercentageDiscount(percentage));
    } else if (discount == "clear") {
        invoice.clearDiscounts();
    } else {
        cerr << "Unrecognized discount command " << discount << endl;
        return;
    }
}

void executePrint(const Invoice &invoice) {
    TextPrinter printer;
    printer.print(std::cout, invoice);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Invalid usage. Usage: ./lab9 path_to_catalog_file" << std::endl;
        return 1;
    }

    Invoice invoice;
    Catalog catalog;
    
    catalog.load(argv[1]);

    string cmd;
    while (cin >> cmd) {
        if (cmd == "show") executeShow(catalog);
        else if (cmd == "product") executeProduct(catalog);
        else if (cmd == "add") executeAdd(invoice, catalog);
        else if (cmd == "remove") executeRemove(invoice, catalog);
        else if (cmd == "discount") executeDiscount(invoice);
        else if (cmd == "print") executePrint(invoice);
        else {
            cerr << "Unrecognized command " << cmd << endl;
        }
    }

    return 0;
}
