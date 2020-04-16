# Synpsis
In this exercise, we will make our invoicing program use a catalog of products from a text file and allow users to add items on the invoice dynamically through interactive commands.

# Requirements
 - Parse the products catalog from `lab7/data/products.dat` file.
 - Implement interactive commands:
    - `show` - prints the catalog of products with ID, Name and Price
    - `add 1 2` - adds the product with ID 1 from the catalog two times on the invoice
    - `remove 1 4` - removes 4 items of the product with ID 1 from the invoice. If there are less than 4 just remove all of them. If there are no items of the product print a friendly message.
    - `discount fixed 10` - add a fixed amount discount of 10 BGN
    - `discount off 20` - add a percentage discount of 20%
    - `discount clear` - remove all discounts from the invoice.
    - `print` - prints the invoice with whatever line items we have

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab7`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

# Steps
TBD
