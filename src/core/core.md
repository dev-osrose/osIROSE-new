# Core library
The core library hosts all of the non-rose shared codebase between all projects (bot, char, login, map and node). It contains:
* google's mini-dump implementation (A mini dump that can be sent from users back to the devs to investigate eventual crashes
* networking library based on asio. The networking code is async with a thread pool that executes the work.
* database connection and database connection pool. To make database requests efficient, we reuse the same connections as soon as they become available.
* configuration file code. This code creates and reads the configuration file.
* a generic factory implementation
* a fuzzy matching code for fuzzy matching gm commands and character names
* a custom hash method for tuples (based on the boost implementation one)
* two traits to do meta programming: function_trait<> and is_container<>. function_trait<> gets the arity of a function and its parameter types. Also its return type. is_container<> returns true if the passed type can be iterated upon with a forward iterator
* a logging mechanism that can log to console, log file and database (the database is not currently set up)
* a custom implementation for working with tuples (iterations, index_of_type)
* a MWSR (multi-writer single reader) queue implementation (will be used later)
* a small random library that contains a few functions: get_uniform(min, max), get_normal(mean, deviation), random_in_unit_circle(), random_in_circle(x, y, radius)
* a custom random iterator implementation
* a generic singleton implementation
* a generic threadpool implementation
* a custom assert macro that throws the error instead of terminating the program

A lot of this code is documented using doxygen (it can be generated using make doc). If it isn't, please make an issue if you want docs about it!
