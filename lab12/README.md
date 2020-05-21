# Synpsis
In this exercise, we will see how templates can help us model common behaviors. In the `Invoice` class, we have implemented an inventory. We have products with quantities, we can add and remove. When there are no more items left of a given type we remove the item completely. 

We can generalize this as an inventory class. It is pretty obvious how we can have an inventory of products, reusing most of the code we already have in `Invoice`. How about having an inventory of `Discount`? Imagine we have a limited amount of discounts a salesperson is allowed to give.

# Requirements

* Extract the inventory logic of the `Invoice` class into a common template class `Inventory`.
* Introduce `Discount::getID` member function. Use the amount or the percentage as the ID of discounts.
* Create a discounts inventory, which holds a finite number of discounts that can be applied. Hardcode the inventory for simplicity.
* Change the discount commands to take discounts out of the inventory before applying it on the invoice.

# Discussion

We are going to use the item class as a generic container of _something_ plus quantity. Therefore, we will need to change the `Item` interface a bit (exposing the contained object) and also make it a template class.

We will introduce a new class `Inventory`, which will be a template and will contain a vector of `Item` instances. We will use pointers - we want to preserve the efficiency in regards to avoid copying `Product`.

Note that for `Item` and `Inventory` we would only have `.h` files. Templates are not classes, they are templates used to generate classes compile-time.

# Exercise
* Make `Item` a tempate, delete the `item.cpp` and also remove it from the `CMakeLists.txt` file. Remove all the product-related member functions to make it truely generic - leave only `getQuantity`, `setQuantity` and introduce `get` that returns a constant reference to the contained object (e.g. product).
* Introduce the new `Inventory` template class. Use the `Invoice` interface for `add`, `remove` member functions. Also, add an `all` member function that can be used to iterate over all items in the inventory.
* Change `Invoice` to inherit `Inventory`. Rename `add` member function for discounts to `apply`, remove `add`, `remove` and `getItems` - those are inherited from `Inventory`, only `getItems` is renamed to `all`.
* Change `Discount` and work with the new generic `Item` class. Introduce a `getID` member function that returns a string ID containing the type of the discount plus the value, e.g. 15% off disocunt should have na ID "off:0.15".
* Change the `TextPrinter` to work with the generic `Item` class. You will need to add a new member function of the `Invoice` that can calculate the line item total (i.e. `price * quantity`) to avoid calculating this in the printer.
* Change `main.cpp` to create an `Inventory` of discounts. Before applying a discount on the invoice, first `remove` it from the inventory. If there is no such discount or there are no discounts left, print a message. Otherwise, apply the discount on the invoice (after removing it from the inventory).

Consult with the `solved` branch if anything is unclear.
