# Synpsis
In this exercise, we will use _exceptions_ to control the unexpected situations in our program.

Until now, the only way we could deal with exceptional situations was to print an error message to the `cerr` for example, which is a side effect. In general, we would like to keep side effects in our program at a minimum and give the _caller_ of a function the responsibility to deal with a situation that we weren't able to deal with inside our function. Printing an error to `stderr` is only one possible way to _handle_ an _exceptional_ situation.

# Requirements
 * Change the `Catalog` class to _throw_ exceptions instead of printing error messages.
 * Change `main.cpp` to _catch_ the exceptions thrown by `Catalog` and report the errors to `cerr`
 * Go through the program and think of unexpected situations that might occur and introduce exceptions to handle them explicitly

# Discussion
Exceptions and error handling in general is a big topic in many languages. The concept of exceptions is only one of the ways to approach errors that happen in our programs and it is used across a number of languages (Java, C#, JavaScript, Python, etc.). It is - however - not a universal approach. `cin` for example in C++ doesn't throw an exception if the input is not read properly - it instead sets the `cin.fail` flag and the programmer can _query_ for the specific error that has happened.

When to throw an exception and when to do something else? This is a rather philosophical question, so take the next few guidelines with a pinch of salt:
 * Throwing an exception is forcing work on the caller of your function. Before doing this, try to figure out if there is a way you can handle the situation in your own code. Changing the function to _define the exception out of existance_ is sometimes an option.
 * Exceptions is for _exceptional_ situations (this is where the name comes from). For example, in a class or function that validates the user input, invalid input is not exceptional in any way - it is expected and the entire purpose of this class/function is to detect those situations. Opening the configuration file of a program, on the other hand, is expected to work and a lack of access permissions can be considered an _exceptional_ situation.
 * Handle exceptions/errors only ones. Catching an exception, logging and then re-throwing is handling it twice.


 In our case, we will use two exception classes:
 * `std::runtime_error` - use it for errors that couldn't have been prevented/detected before the program is executed. E.g. file is missing/inaccessible
 * `std::logic_error` - use it for errors that could have been prevented/detected before the program is executed, e.g. trying to read/write products to the catalog before the catalog has been loaded from disk.
 