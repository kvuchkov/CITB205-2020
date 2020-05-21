# Synpsis
In this exercise, we will see how templates can help us model common behaviors. In the `Invoice` class, we have implemented an inventory. We have products with quantities, we can add and remove. When there are no more items left of a given type we remove the item completely. 

We can generalize this as an inventory class. It is pretty obvious how we can have an inventory of products, reusing most of the code we already have in `Invoice`. How about having an inventory of `Discount`? Imagine we have a limited amount of discounts a salesperson is allowed to give.

# Requirements

* Extract the inventory logic of the `Invoice` class into a common template class `Inventory`.
* Introduce `Discount::getID` member function. Use the amount or the percentage as the ID of discounts.
* Create a discounts inventory, which holds a finite number of discounts that can be applied. Hardcode the inventory for simplicity.
* Change the discount commands to take discounts out of the inventory before applying it on the invoice.

# Discussion

# Exercise
