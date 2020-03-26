# Synpsis
The goal of this exercise is to implement the empty member functions from Lab #2 and print the information on the invoice. At this point, do not waste time trying to format the output of the `TextPrinter` class, but focus on implementing the functions of the other classes.

# Clean
1. You first need to have clean repository, i.e. no changes. You have to commit everything you have worked on:
```
git add .
git commit -m "describe your changes"
```

2. Make sure you are on the master: `git checkout master`

# Prepare
1. Get the code. To get updates from github, you just need to run `git pull`. You should now see lab2 folder. 
2. Go to the folder (in the terminal, `cd lab3`)
3. Prepare the build:
  * For Linux/Mac, run: `cmake .`
  * For Windows, run: `cmake . -G "MinGW Makefiles"`

# Exercise

Don't forget to work in small iterations towards completing the exercise. I will illustrate the steps with a few examples, and then you should try on your own until you get to the final result.

The steps are similar to what we had in Lab #2. As we implement a member function (i.e. fill the empty function body with code), this will lead to cascading changes in other classes. For example, when we add a code in the `TextPrinter`'s `print` function to show the total of the invoice, we will have to go to the `Invoice` class and declare a new member function. Remember - declarations go to the `.h` file, while definitions (i.e. the body) goes to the `.cpp` file of the class.


Steps:
1. Write a minimal, yet functional function body. For example, make the `TextPrinter::print` function output the total of the invoice.
2. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see compilation errors.
3. Declare members of this and other classes so that the code added in step #1 compiles.
4. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. You will see linking errors.
5. Define empty bodies of the member functions added in step #3 (i.e. linking errors in #4)
6. Build the program with `make` on Mac/Linux or `mingw32-make` on Windows. The build should be successful.

Now go to the next empty function body, implement it and repeat steps #1 to #6.

>Tip: You can commit your changes in the `master` branch after each run of steps 1 to 6, i.e. having a checkpoint of a compiling program. To do that: `git add .` and then `git commit -m "your message"`.

At the end, you should be able to run `./lab3` on Linux/Mac or `lab3.exe` on Windows and see all information on the invoice (without the pretty format).

# Example Steps

Build: `make` on Mac/Linux or `mingw32-make` on Windows
