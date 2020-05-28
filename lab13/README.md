# Synpsis
In this exercise, we will use _exceptions_ to control the unexpected situations in our program.

Until now, the only way we could deal with exceptional situations was to print an error message to the `cerr` for example, which is a side effect. In general, we would like to keep side effects in our program at a minimum and give the _caller_ of a function the responsibility to deal with a situation that we weren't able to deal with inside our function. Printing an error to `stderr` is only one possible way to _handle_ an _exceptional_ situation.

# Requirements
 * Change the `Catalog` class to _throw_ exceptions instead of printing error messages.
 * Change `main.cpp` to _catch_ the exceptions thrown by `Catalog` and report the errors to `cerr`
 * Go through the program and think of unexpected situations that might occur and introduce exceptions to handle them explicitly

# Discussion
