#include "textprinter.h"
#include <iostream>
#include <iomanip>

using std::endl;
using std::left;
using std::right;
using std::setw;

void TextPrinter::print(std::ostream &out, const Invoice &invoice)
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
