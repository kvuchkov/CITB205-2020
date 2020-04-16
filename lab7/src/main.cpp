#include <iostream>
#include "invoice.h"
#include "textprinter.h"
#include "discount.h"

int main() {
    Invoice invoice;
    Product *superMob = new Product("Super Mob", 12.90);
    Product *teaCup = new Product("Tea Cup", 5.30);
    Product *redWineGlass = new Product("Red Wine Glass", 8.60);

    FixedDiscount *fiveOff = new FixedDiscount(5.0);
    PercentageDiscount *tenPercentOff = new PercentageDiscount(10);

    invoice.add(superMob, 5);
    invoice.add(teaCup, 12);

    invoice.add(fiveOff);
    invoice.add(tenPercentOff);

    invoice.add(redWineGlass, 8);

    TextPrinter printer;
    printer.print(std::cout, invoice);
    return 0;
}
