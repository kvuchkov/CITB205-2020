# Synpsis
In this exercise, we will use operator overloading to see how this could improve the syntax of our program. Note that we could go without overloading operators by just use functions, but the operators could improve the readability of our code in certain situations. Use with caution!

# Requirements
This is based on the program we had at the end of Lab 7. However, a few minor changes were done in `main.cpp` and you need to make adjustments in `Product` and `Catalog` classes to make everything works.

There is a new interactive command: `product`, e.g. `product 4 Mug 2.80` which adds a new product to the catalog on the fly. The work in `main` is already implemented. However:
* The product is added to the catalog using the `+=` operator, i.e. `catalog += product`. 
* The product is read from the standard input stream as if it were an int, i.e. `cin >> product`. We are using the `*` operator to dereference the pointer.

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab9`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

# Steps

