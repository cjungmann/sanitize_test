# PROJECT SANITIZE_TEST

Lacking destructor methods that C++ provides, C language programming
requires discipline to ensure that certain deinitialize code is run
appropriately.

It should be possible to signal a missed deinitialization by deliberately
leaking memory that would then be reported by the `-fsanitize=address`
compiler and linker options.

In my [termintel][termintel] project, I save terminfo escape sequences
to an array using malloced memory.  I have a deinitialize function that
should be called before exiting the program that frees the memory.  I
expected that I would see a memory leak warning if the deinitialize
function is not called.  However, despite using the `-fsanitize=address`
compiler and linker options, my program **does not** generate a memory
leak warning in that case.

This project is an attempt to identify conditions that duplicate the
failure of `-fsanitize=address` to report memory leaks.

## SIMULATION STRATEGY

To somewhat duplicate the *termintel* scenario, this project employs
several methods of creating an empty **target array** of pointers to
char pointers, the elements of which will be assigned a malloced
string.  The program will exit *without* freeing the memory in the
hope of generating a memory leak warning,

The **target array** will be filled from elements of a **source array**
of strings that will stand in for escape sequences provided by the
terminfo database.

## SOURCE FILES

- **test.c** was my first effort, and I implemented a line-by-line
  file reading interface for supplying the test lines to copy to the
  array of char*.  I cleverly wasted much time writing the file
  reading functions, then realized that it was much too long and
  distracting to submit along with a question to [Stack Overflow][stackoverflow].

- **test2.c** is a better example, using a simple array of strings
  as the source for filling the array.  There are five different
  implementations of the basic idea (and I may add further).

- **Makefile** is a simple make file where the **gcc** options are
  split over two statements to make it easier to eliminate options
  that might interfere with `-fsanitize` operation.

## SUMMARY OF METHODS

There are several different methods, broken into separate functions.
The differences between the methods is primarily where the memory
is allocated.

In **assign_method_one** and **assign_method_two**, the **target array**
is created with `malloc`.  The difference between the two methods is
that in **assign_method_one**, the elements' assignments are done in
the same stack frame as the **target_array**, while **assign_method_two**
calls function **copy_arrays** to assign malloced strings to the
**target array** elements.

**assign_method_three** and **assign_method_four** both populate a
**target array** that is created in the function that calls them.  Given
that, **assign_method_three** assigns the elements of the **target array**
directly, and **assign_method_four** uses the **copy_array** function
like **assign_method_two**.

There is a **assign_method_five** that mallocs the **target array**
and returns it to the calling function through a char*** (doubly-indirect
pointer), using **copy_array** to populate the **target array**.

Each of these functions is called from **main()**, where appropriate
allocations are made according to the design of the method function
called.  In the case of **assign_method_five**, the contents of the
generated array are printed to prove that the indirection is correct.

## OBSERVATIONS

Keep in mind that, in service to the idea of leveraging memory leak
warnings to alert the programmer to coding errors, the objective here
is to find the conditions where leaked memory does not generate memory
leak warnings.

Although all of the methods deliberately leak memory, the only
method where the memory leaks are not reported is **assign_method_one**,
and then only when it is run by itself.  All of the other methods
result in memory leak reports.

Surprisingly, the memory leaks in **assign_method_one** WILL be
reported if at least one of the other methods is run as well.

Sadly, none of this helps me solve the problem in [termintel][termintel].
The model used there mostly matches **assign_method_four**.  And while
here, that method generates a memory leak warning, no such warning is
being generated for deliberate memory leaks in that project.


[termintel]:     "https://www.github.com/cjungmann/termintel.git"
[stackoverflow]: "https://www.stackoverflow.com"