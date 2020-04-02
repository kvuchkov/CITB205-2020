#include "textprinter.h"
#include <iostream>
#include <iomanip>

using std::setw;
using std::left;
using std::right;
using std::endl;

void TextPrinter::print(std::ostream &out, Invoice invoice) {
    out << std::fixed << std::setprecision(2);
    
    out << std::setfill('-') << setw(68+5) << "" << std::setfill(' ') << endl;

    for (auto item : invoice.getItems()) {
        out << '|';
        out << setw(8) << right << item.getQuantity() << '|';
        out << setw(40) << left << item.getDescription() << '|';
        out << setw(10) << right << item.getPrice() << '|';
        out << setw(10) << right << item.total() << '|';
        out << endl;
        
        out << std::setfill('-') << setw(68+5) << "" << std::setfill(' ') << endl;
    }

    out << setw(58+3) << right << "Discount" << '|' << setw(10) << right << invoice.totalDiscount() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "Subtotal" << '|' << setw(10) << right << invoice.subtotal() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "Taxes" << '|' << setw(10) << right << invoice.taxes() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
    out << setw(58+3) << right << "TOTAL" << '|' << setw(10) << right << invoice.total() << '|' << endl;
    out << setw(58+3) << "" << std::setfill('-') << setw(12) << "" << std::setfill(' ') << endl;
}
