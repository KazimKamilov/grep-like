Design document.

The idea of the program is to recursively search for a substring in a
given directory and output the names of the files in which the substring
is found. The program is written in C++20 and fully uses C++ modules
and CMake 3.28 minimum.

Let's go in order. The program does not contain OOP, only functions.
The program starts from the main() function, where it receives the
startup arguments. The arguments are passed to the run_search function,
inside which the appropriate behavior is selected based on the number of
argc parameters. The program implements several behaviors - with two
and three additional arguments, not counting the name of the executable file.
In addition to the two main arguments (substring and path to the directory),
the first argument can specify the number of program threads(jobs), for example:

grep-like -j4 substring path_to_dir

Where '-j4' is an argument with the number of jobs. If the '-j' argument
is not specified, the program will receive the maximum number of threads
from 'std::thread::hardware_concurrency()'. The '-j' argument from a string
to a digit is processed in the 'getJobCountFromArg()' function, which, in
case of incorrect input of the number, returns the maximum number of threads,
and in case of incorrect command input, returns 0. Now we are ready to search.

Next, we call the main search function - 'searchSubstr()', which does the
following: receives the parameters passed to it, then opens the directory given
to it, goes through all the files with a recursive iterator and creates a list
of their names. File system operations are performed via std::filesystem. Next,
we take the number of submitted jobs and divide the list of files between them.
If we have a remainder from the division, we add one file for each thread
evenly as long as there are unused files left.

Next, we create threads for each job and run them to execute their own file lists
in the 'processFile()' function, which opens the file for reading in binary mode,
reads all the byte data of the file into the buffer(std::vector<char>), then closes
the file. The substring is searched in the buffer using the standard 'std::search()'
algorithm. If a substring is found, we output the name of the file in which
it is located to the console. If the file has not been opened, we output a message
about it to the console.

If the number of files in the specified directory is less than the number of
specified jobs, we create as many threads as there are files and run them.

The last step is to call all threads that have been created and moved to
'std::vector<std::thread>' the join() method to wait for them to finish working.

All messages output to the console are output via the template function 'message()',
which is a variadic template and converts the message via 'std::vformat' and outputs
it to the console via 'std::cout'. The only place to synchronize threads is the
'message()' function, which uses the mutex lock via std::lock_guard.
