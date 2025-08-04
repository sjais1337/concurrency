# Concurrency and Parallelism

The main resource we are following is **C++ Concurrency in Action** (Referred as CCIA) by Anthony Williams. It is expected, that after going through this guide in its entirety you will be able to look at new design patterns and concepts in concurrency and understand the "Why?"s of what is happening with a lot less friction. You'll learn the core knowledge required to design and build concurrent applications.

IMPORTANT: If you think you are really stuck at a certain point, and not able to understand something, you are free to contact the person who wrote this guide!

## Why concurrency? 
Nowadays, almost every system is multi-core, and exploiting the pre-existing compute is one of they key skills out there that one can learn.

One thing which catches everyone's attention is HFT, and this skill is very valuable to such firms. Gaming studios require it for building high performance game engines. In general, processing large amounts of data, or handling a lot of operations requires you to make your code more efficient, and utilise resources which it has available already.

## Why this guide?
As it presently stands, you won't find any resource on the internet which follows such a pattern. And it isn't without reason. It is generally assumed that someone dealing with concurrency is already capable of designing applications by themselves, and mostly the focus is on design patterns and other choices. 

As it presently stands, most resources on concurrency lack excercises, assuming the reader to be familiar with writing utilities and applications in C++. However, we are assuming the contrary. We assume that you know how to write basic applications, and have a decent idea about the STL. We focus on giving you sufficient code-examples, and a birdseye view of how concurrency is used in real utilities that you might use frequently.

> Although we recommend learning concurrency in C++, there is an exceptional resource by Anton Zhiyanov in Go language for learning concurrency (for experienced Go programmers), whose philosphy (of using examples) I have tried to replicate, alas, in a very limited fashion due to time and format restrictions. [Here](https://antonz.org/go-concurrency/) is the link if you wish to use it.

### How to follow?
[//]: <> (TODO: Re-write or Remove)
A pretty important pre-requisite is to be familiar with C++ to a certain degree. Whether you are familiar enough or not can be tested by Assignment 1 of this guide. If you are not, then here are some resources for learning C++ itself.

If you got time on your hands and are interested in this stuff, read and do everything mentioned, if not more! If you wish to build a good understanding of multi-threading and concurrency, but a little bit short on time, and want to just understand how to use concurrency, skip the reading assignments marked optional, and if you somehow are even busier, then you may skip the project (not recommended though, this is they key aspect of the guide), but still, do all the other assignments. 

Having established the "Why"s let us start!

## Random resources for further sections (Not final stuff, will put in relevant sections later)

### Some coded up examples

Stuff from the author himself https://github.com/anthonywilliams/ccia_code_samples

Excercises for multithreading in C++ https://github.com/thanhit95/multi-threading/tree/main/cpp/

Small projects kind of stuff, will likely be very helpful https://github.com/IvanFernande/Cpp-concurrence-examples

Must include excercises, Problems 61 to 66 of https://github.com/rudenko-gold/bancila-modern-cpp-challenge/blob/master/Bancila%20M.%20-The%20Modern%20C%2B%2B%20Challenge%20-%202018.pdf

Some stuff from the book Modern C++ Concurrency maybe, if time permits.

Some more advanced stuff, in modern C++, i.e. C++20 again, most likely for reference, will add in later iterations https://github.com/MericLuc/Modern-cpp-concurrency

Examples grouped by topic, not particularly helpful i suppose since the book kinda has a good amount https://j040222.github.io/concurrency-tutorial/index.html

Shit in GO, will probably go through if time permits, and adopt some stuff https://antonz.org/go-concurrency/ 

Solved concurrency excercises in Java, can probably adopt some of them in C++ https://github.com/leticiamazzoportela/concurrent-programming

Something about semaphores, need to research more, might include later https://github.com/AaronEnnis/CDDLabs


#### (Likely useless)
Some coded up examples, 
https://github.com/wasimusu/cpp_multithreading

not sure what this dude was doing https://github.com/MatiasCarabella/multithreading


### Youtube Videos

https://www.youtube.com/watch?v=wXBcwHwIt_I - How C++ handles threads, waiting and stuff

### Talks 
Too many talks, will have to sift through them. Most of them talk about how stuff works different in the newer versions of C++, and design practices. 

[Get Off My Thread](https://www.youtube.com/watch?v=iUKxvEg0zdk&ab_channel=CppCon) -> Design choices again.

# The Basics

<ins>Reading Assignment</ins>: 
- [Why Threads?](https://pages.mtu.edu/~shene/NSF-3/e-Book/FUNDAMENTALS/threads.html)
- CCIA: Chapter 1, Sections 1.1 to 1.4. *Optional*: 1.3.

<ins>Helpful Videos:</ins> 
- [On Processes](https://www.youtube.com/watch?v=LDhoD4IVElk) (Can skip, not that relevant to concurrency, but you'll gain understanding on CPU instructions on a lower level)
- [Why Are Threads Needed in Single Core Processors](https://www.youtube.com/watch?v=M9HHWFp84f0)
- [Threads on Multicore Processors](https://www.youtube.com/watch?v=5sw9XJokAqw) (Can skip, already explained nicely in reading assignment, watch if not clear) 

Explores different methods of concurrency, single-threading and multi-threading, and discusses why should one use concurrency and establishes that the main motivation behind using it is either separation of concerns or performance. 

Here's an experiment you can try. Build and run the code in the [given file](https://github.com/sjais1337/concurrency/blob/master/tests/time_comparison.cpp). If you wish to look at the code, you may, but at this stage you don't have to, but essentially the task is to sum the first billion natural numbers, by using a simple for loop.

Considering that most of you will be having at most 16 threads, all even multiples of threads have been added till 16 so you can notice the pattern yourself! Once you run it, you'll find results similar to this (assuming you have a 16 threaded CPU, the results would be similar to mine, however try running it yourself). 

```
Available threads 16

(No Threading) Time over single run: 439.338ms

(1 Threads) Average time over 15 runs: 1605.68 ms
(2 Threads) Average time over 15 runs: 811.696 ms
(4 Threads) Average time over 15 runs: 433.742 ms
(6 Threads) Average time over 15 runs: 308.542 ms
(8 Threads) Average time over 15 runs: 232.459 ms
(10 Threads) Average time over 15 runs: 186.434 ms
(12 Threads) Average time over 15 runs: 162.535 ms
(14 Threads) Average time over 15 runs: 141.65 ms
(16 Threads) Average time over 15 runs: 127.17 ms
(20 Threads) Average time over 15 runs: 110.081 ms
(32 Threads) Average time over 15 runs: 114.502 ms
(64 Threads) Average time over 15 runs: 107.696 ms
(128 Threads) Average time over 15 runs: 106.636 ms
(256 Threads) Average time over 15 runs: 106.176 ms
(1024 Threads) Average time over 15 runs: 115.011 ms
(4096 Threads) Average time over 15 runs: 142.688 ms
```
Can you infer why do you see the results that you see here?

## Assignment 1 (a)
Right now you just know how to create a thread. Cute, right? To design concurrent programs, you need be comfortable with vanilla programs. This assignment involves no use of concurrency. You are tasked with building a find and replace utility. We will be working with this base for some sections of the guide, mostly to understand how concurrency is utilised in an actual tool, and how much it can affect speeds.

Here is what we expect the output for `./grep --help` (assuming your build file is called "grep"). Make sure that all the functionalities mentioned here are implemented.
```
A grep-like tool with replacing capabilities. 
 
USAGE:
  ./grep [OPTIONS] <pattern> <file1> [file2]...
  ./grep [OPTIONS] -r <replacement> <pattern> <file1> [file2]...
OPTIONS:
  -r, --replace <TEXT>    Enable find-and-replace mode.
  -i, --ignore-case       Perform case-insensitive matching.
  -n, --line-number       Prefix each line of output with its line number.
  -v, --invert-match      Select non-matching lines.
  -h, --help              Display this help message.
```

Refer to our code [here](https://github.com/sjais1337/concurrency/blob/master/assignment1) if you face difficulties making it, but make sure you do not just copy-paste stuff.

For benchmarking our tool, we will need two separate data-sets, generated using this [utility_small](https://github.com/sjais1337/concurrency/blob/master/dataset/generator_small.cpp) which creates multiple small files, and [utility_large](https://github.com/sjais1337/concurrency/blob/master/dataset/generator_large.cpp). which creates multiple large files. This is to standardize the testing. Both the utilites use subsets of different lengths from 10,000 most used words on google dataset.

Note: In case your computer has a weaker CPU, you may want to reduce the file size in the larger utility (by default, 8 files of size 500MB), and the number of files (I used 8, I suggest using less than 8 for reasons you'll understand later). 

# Managing Threads
<ins>Reading/Watching Assignment:</ins> (In order as mentioned) 
- [Basic Thread Management Theory](https://pages.mtu.edu/~shene/NSF-3/e-Book/FUNDAMENTALS/thread-management.html), Leave Yield, Suspend and Resume for now (or read if you're interested).
- CCIA: Sections 2.1.1 - 2.1.3 => [Using .join() practically](https://www.youtube.com/watch?v=wXBcwHwIt_I) => Section 2.1.4, 2.2, 2.3
 

Joining threads is mostly for coordination, not necessirily parallelism or efficiency. There is a difference between referencing to a member function in a struct, and referencing to a free function. The name of a free-function decays to a pointer to the function, whereas the name of a member function does not strictly decay to it's pointer, since it is not well-defined without the object it is assosciated with, however `&` can be used to get the address of the member function.

## Assignment 1 (b)
Take inspiration from the technique used in Listing 2.7 from the book, to implement a way to process multiple files concurrently. Think about the design choices you have learnt in this chapter, and figure out one key modification you'll need to ensure safety. Here are the results I found with my implementation:

**No Threading**
```
[sjais ~]$ ./a.out "hello" 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt
--- Processed 1.txt in 349.182 ms. ---
--- Processed 2.txt in 347.961 ms. ---
--- Processed 3.txt in 347.135 ms. ---
--- Processed 4.txt in 346.712 ms. ---
--- Processed 5.txt in 350.587 ms. ---
--- Processed 6.txt in 347.18 ms. ---
--- Processed 7.txt in 348.428 ms. ---
--- Processed 8.txt in 345.747 ms. ---
Finished processing files in 2783.06 ms.
```
**With Threading**
```
[sjais ~]$ ./a.out "hello" 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt
--- Processed 8.txt in 417.541 ms. ---
--- Processed 7.txt in 418.521 ms.  ---
--- Processed 4.txt in 419.33 ms.  ---
--- Processed 3.txt in 419.488 ms.  ---
--- Processed 5.txt in 420.017 ms.  ---
--- Processed 6.txt in 420.865 ms.  ---
--- Processed 1.txt in 492.593 ms.  ---
--- Processed 2.txt in 493.279 ms.  ---
Finished processing files in 493.677 ms.
```

When testing yourself, I would highly recommend to not print the lines, since there will be way too many of them. **(Optional)** Implement a feature to print the total number of occurrences found. 

However, there are a couple of problems with this. In one of my runs, the output was like 

```
[sjais ~]$ ./a.out "hello" 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt
--- Processed 8.txt in 419.799 ms. ---
--- Processed 7.txt in 420.15 ms. ---
--- Processed 5.txt in 421.128 ms. ---
--- Processed 3.txt in 421.811 ms. ---
--- Processed 6.txt in --- Processed 4.txt in 422.597 ms. ---
422.507 ms. ---
--- Processed 1.txt in 504.27 ms. ---
--- Processed 2.txt in 506.959 ms. ---
Finished processing files in 507.347 ms.
```
Can you figure why is that? In the next chapter you'll learn about ways to fix this issue. 

<ins>Reading/Watching Assignment:</ins> (In order as mentioned) 
- CCIA: Section 2.4, 2.5

Once done reading, and with the assignment, spend some time revising the listings, and make sure you understand things at least to a certain degree. The excercise given isn't tough, but uses every topic you have learnt till now, and will be a longer problem. I recommend you play around with the *Listings* in the book as the book suggests, which can be found [here](https://github.com/anthonywilliams/ccia_code_samples).

### Excercise 1
A *Central Dispatcher* recieves a continuous stream of unique *Delivery Orders*. These orders cannot be duplicated. The Dispatcher sends the orders to another centre, let's say the *Post Office*. The Post Office is supposed to find (let's say create) an available *Driver* (thread basically), assign the order to the driver, and then return the driver back to the Central Dispatcher, and the driver is then added back to the ready-to-go fleet (the thread should be free after delivery). The drivers all work in parallel. 

Upon finishing its work, each driver must return a one-line summary of their work in a central logbook, and it is written as soon as the driver finishes its work. You do not have to worry as of now about the conflicts between drivers when they are entering their work in the logbook. This is a known flaw, and you'll fix this in the next chapter.

**Technical Requirements/Design Pattern**
1. struct `DeliveryOrder` - Contains `order_id`, `destination` and a method `execute` which returns the log. You may simulate the work using any function you like, I suggest, you just let the program sleep for a few (random number of) milliseconds (google how to put a thread to sleep for this). The log can just return that the delivery was completed.  
2. class `Driver` - This will be your thread manager, which manages a *single* thread. The driver should be moveable, but not copyable (unique_ptr). Implement the relevant safegaurds. The constructor takes the `DeliverOrder` and *reference* to `logBook`. You must directly invoke the `execute` method of `DeliveryOrder`. The destructor must also make sure that the thread has been properly shutdown. You should also make sure that your Driver object is moveable (this is a crucial part, think about what makes an object moveable, HINT: If a variable contains the Driver object, and you do `var = Driver(order, book)`, what is a necessary condition for this to go smoothly?)
3. `create_driver` - Or `find_driver`. You do you. Helper function to create a driver, kinda optional.
4. `main` (Dispatcher) - The logbook is a vector of `std::string`. The fleet of drivers should be in `std::vector<Driver>`. Use good practices to initiate these strings. Find the optimal number of drivers (section 2.4), or just hardcode it your choice. Create a loop to create and add the drivers to your vector. The `main` function should wait for all the tasks to be completed. After which, print the contents of the logbook.

**BONUS**: As of now, the Driver is moveable, but you don't really use it's moving capability much. Try running your code without the Driver being moveable. Also, think of how you can put the moveability of it into practice.

**Expected Output**: Depending on how you actually write your logs, it will look different. But the output should likely be different everytime you run it, and end with a `Segmentation fault (core dumped)` error. Think about why do you get the error that you get here, and fix the segmentation fault too. Here is the output in one of my runs.

```
Starting Program...

--- Creating 16 orders ---
Creating driver for order 0.
Creating driver for order 1.
Creating driver for order 2.
Driver for order 2 delivering to Sector 2.
Driver for order 2 delivering to Sector 2.
Creating driver for order 3.
Creating driver for order 4.
Driver for order 4 delivering to Sector 4.
Creating driver for order 5.
Driver for order 5 delivering to Sector 5.
Creating driver for order 6.
Driver for order 6 delivering to Sector 6.
Driver for order 6 delivering to Sector 6.
Driver for order Creating driver for order 7.
Segmentation fault (core dumped)
```

[Solution](https://github.com/sjais1337/concurrency/blob/master/excercises/excercise1.cpp)

### Commentary on the solution
I'll start by saying, the reason for using random times is a surprise for you, for the later sections. Next, re-iterating the design choices. The driver class initializes the thread and has its destructor called upon whenever the class is going out of scope. To revise, first the main function reaches its closing brace, after which the `drivers` vector is being cleaned. Since each individual entry of the `drivers` vector is an instance of the `Driver` class, it's destructor is called, which joins the thread into the main function, causing it to wait for the completion of the thread. This kind of resource acquisiton pattern which handles the resource without you having to do anything is known as RAII (Resouce Acquisiton is Initialization). 

> As a sidenote, anyone who has worked with rust before probably understands RAII already. Rust heavily utilises the RAII design pattern to manage memory, file handles, network connections and of course, threads. Rust also ensures that each resource has only a single owner, which makes it memory safe by default. These checks have to be added in C++ as and where needed (spoiler alert, they're not needed everywhere). 

Next, we have made the threads (which in this case are `Drivers`) moveable, by defining `Driver(Driver&&)` (the move constructor) and deleting `Driver(const Driver&)` (the copy constructor). It is necessary to make them moveable, since a vector either needs the stored data type to be moveable or copyable by the standard, since during resizing, or invocation of methods like emplace_back, the data present in the vector needs to be moved, [CPP Ref](https://en.cppreference.com/w/cpp/named_req/MoveInsertable). Here are some videos for the constructors in case you are not familiar with them [Default Constructor](https://www.youtube.com/watch?v=FXhALMsHwEY), [(Wiki) Copy Constructor](http://en.wikipedia.org/wiki/Copy_constructor_(C%2B%2B)), and just look up the move constructor somewhere. 

Another important feature that we have used here are lambda function. People who have used Python before might find this familiar, although it is a tad bit more complicated than the one you possibly know from Python. Here is a video for [Lambdas](https://www.youtube.com/watch?v=mWgmBBz0y8c) and I would recommend going through the [CPP Ref](https://en.cppreference.com/w/cpp/language/lambda.html) for the same, (if you have not heard of function pointers before, it might be beneficial to watch this video before the one for lambdas, [Function Pointers](https://www.youtube.com/watch?v=p4sDgQ-jao4&)). It was not necessary to use lambdas in this particular problem, and it could have been done without lambdas too, which should be sufficiently easy to figure out, although there is something tricky going on with the `logbook` if you do it.   

I mention about an optional design pattern, which is the use of the `friend` declaration. In the given scenario, the constructor itself has been made private! This means that you cannot call it from any given function. Only specific functions, which are a "friend" of the Class can call it's constructor, which in this case is `find_driver`. This kind of a design pattern is known as "Factory Patterns". This is completely optional for now, but is a interesting design pattern to know. The reason it has been used here is, it has the effect of verbosity, and keeps the program safe. The more important reason is that such patterns are very important in programs in which you are constantly delegating tasks to other functions to manage them safely, a strong example of which is a threadpool, which we will be exploring in later sections. Lastly, if you have any questions about the `make_unique` usage, just google it but it basically prevents memory leaks, and wraps the newly created object in a unique pointer.

# Sharing Data Between Threads 

## Part 1
In this section we will start first by solving the race conditions you might have encountered in the previous sections. Later we will build up on the grep tool by developing more sophisticated logging tools.

<ins>Reading/Watching Assignment (In Order): </ins>
- [[YT] On Sharing of Data](https://www.youtube.com/watch?v=7ENFeb-J75k) => [Race Conditions Overview](https://www.baeldung.com/cs/race-conditions) => CCIA: Section 3.1 to 3.2.3 => [[YT] Mutexes on Low Level](https://www.youtube.com/watch?v=1tZhmTnk-vc) (Answers how multiple threads accessing a mutex isn't a race condition in itself) => [[YT] Data Race & Mutex (Optional)](https://www.youtube.com/watch?v=3ZxZPeXPaM4)

The bug you must have encountered previously, while attempting the grep problem is also an example of a race condition, albeit not that severe. Notice how the bug isn't easily replicable. Similarly, the issue with the Dispatcher problem is another example of a race condition, with higher severity. The output you get in each of these programs feels essentially random, and irreproducible. This is one of the main reasons why is it so hard to diagnose race conditions and fix them. Thus you should always make sure that your data structures must have protection mechanisms, predominantly, _mutexes_, standing for mutually exclusive. 

#### Mini Task (Fix for Dispatcher Problem)
As you probably must have figured out in the earlier assignment that the `Segmentation Fault` originated from multiple threads trying to call `push_back` on the `logbook` vector simultaneously. Take appropriate steps to fix this.

## Assignment 1 (c)
First you should do some preliminary tasks to get used to mutexes. 

1. Fix Garbled Output: Create a shared mutex to protect the out stream, and use a lock gaurd to make the processed file message atomic.
2. Data Aggregation: Implement the optional feature from earlier, i.e. count the total number of occurrences found accross all the files. Create a shared integer containing that number and also protect it using a mutex. A thread should lock the mutex after processing a file, and add the count of the number of occurrences from a single file to the total count.

Now we will aim to demonstrate a new type of bug that can be created. We will do this by adding a "Status Reporting" feature. This feature will run in it's own thread, and periodically fire up reporting the total number of matches found thus far. This is particularly simple to do, and here's the outline of how you will have to do it.

1. Create a new thread in `main` which runs a status reporting loop.
2. Inside of this loop, the thread should
    - Lock the mutex controlling the output stream to print a message which reports the progress.
    - Lock the mutex controlling the number of matches, reading the `total_matches` andd printing its value.
    - Sleep for 100ms.
  

You will notice that the program you have made sometimes runs to completion, but other times it just hangs and never finishes. Can you figure out why it never finishes?

## Part 2
<ins>Reading/Watching Assignment (In Order): </ins>
- Understanding the Deadlock: [[YT] Deadlock](https://www.youtube.com/watch?v=_N0B5ua7oN8) => CCIA: Sections 3.2.4 to 3.2.7 => [Dining Philosphers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) and the other linked problems therein
- Other Tools: CCIA: [[YT] Lazy Initialization and `unique_lock`](https://www.youtube.com/watch?v=IBu5ka1MQ7w&ab_channel=BoQian) => Section 3.3 => [Reader Writer Locks](https://www.modernescpp.com/index.php/reader-writer-locks/)

In this section, we will solve the deadlock problem faced in the previous part, solve some inefficiencies, only stopping for processes for which it is necessary to stop. For the status reporting from part 1, 

As you must have figured out after reading about deadlocks, the reason that your program hangs in some iterations is because it enters a "Dead Lock", which occurs when two or more threads are blocked forever, each waiting for a resource held by another thread in the cycle. 

In our particular case, worker thread might lock the mutex gaurding the result, and then try to lock the outstream mutex, whereas the new status thread that is running might try to lock out the outstream mutex and lock the then lock the mutex gaurding the result. If the timing is right, both the threads are waiting for the other one and neither can proceed, freezing the program. This wouldn't happen if the order of locking the mutexes was same in both the threads.

Moreover, many threads access the `total_matches`, but the status reporting thread access it pretty frequently, atleast for large files which take multiple seconds to process. We used 100ms as the interval but if the interval was lesser, then the number of times it is accessed would also increase. A standard mutex prevents the status thread from reading the value from it if another thread is also reading it, which is one of the causes of bottleneck. This problem is solved by using a `shared_mutex`. Since for readers a lock doesn't matter, a shared lock allows multiple threads to acquire the shared lock simultaneously. 

## Assignment 1 (d)
- Refactor the grep tool to fix the deadlock, and use the unique lock. 
- Make the logger thread safe, using call once, and use a shared mutex for the status reporter.

## Excercise 2 (Bank Account)

## Excercise 3 (Many Readers One Writer)

# Synchronizing Concurrent Operations
So far our operations have been really simple. If you watched the Computerphile video on Multithreading Code, we basically have just covered what they explained in that single video. Gaurding a resource using a lock, and unlocking it when necessary. In it's essence, that's what it is all about. Just gaurding data. But our interactions have been really simple. This section introduces the tools required for more sophisticated coordination, such as waiting for specific conditions and getting back results from background tasks.

## Part 1: Waiting using condition variables
<ins>Reading/Watching Assignment (In Order):</ins>
- [[YT] Condition Variable](https://www.youtube.com/watch?v=13dFggo4t_I) => CCIA: Sections 4.1 to 4.1.2

## Assignment 1 (e)
Refactor the grep tool to use a producer consumer pattern, where the worker threads are the producers, that find matches, pushing them into a thread-safe queue. After this, a new consumer thread will wait on this queue and handle the printing to outstream, essentially decoupling the file processing from I/O.

## Excercise 4: Thread Safe Queue from Scratch

## Part 2: Futures, Promises and Asynchronous Tasks
<ins>Reading/Watching Assignment (In Order):</ins>
- [[YT] Asynchronous Programming in Game Engines](https://www.youtube.com/watch?v=5HWCsmE9DrE) => [[YT] Future, Promise and async()](https://www.youtube.com/watch?v=SZQ6-pf-5Us) => CCIA: Section 4.2.1 to 4.2.5 

## Assignment 1 (f)
Refactor the `grep` tool to parallelize the search within a single large file. A large file will be divided into a bunch of byte-range chunks launching a worker using `async` for each chunk, with the main thread collecting a `future<int>` from each worker, finally using `.get()` to combine the final count of matches.

## Excercise 5: Parallel Calculator

## Multi-threading in Python
Although we have previously discussed about parallelism, multi-threading and concurrency. Till now these terms have been used inter changebly. In python a siginifcant difference occurs. The standard implementation of python (CPython) has a mechanism called the [Global Interpreter Lock (GIL)](https://en.wikipedia.org/wiki/Global_interpreter_lock), which basically synchronizes the operations so that only one thread can execute operations at time. The GIL is absent in C++ thus, whatever number of threads are created can be allocated to different cores and thus can run truly in _parallel_. Although tasks can still be _concurrent_ they can never be truly _parallel_ (the GIL itself is a mutex lock!). Actually, you can have truly parallel systems, but that won't be due to multi-threading. 

Multi-threading can be utilised for tasks such as reading from the I/O while still processing information in parallel since context-switching still is allowed. However, to run CPU bound tasks in parallel,you will have to utilise _multi-processing_, which implies you run multiple processes, instead of running multiple threads on the same process. This method of paralleism has significant resource over-head over multi-threading, since each independent process starts with its own memory space, has it's own instance of a Python interpreter and thus is a heavy operation for a system to do. Moreover, since threads share the same memory space, communication between two threads is straight-forward, which is not the case with processes. Passing data requires one to serialize the data, and send it through a pipe, which has to then be deserialized by the receiving process.

The official python documentation does a great job explaining the usage of these modules, [Thread Based Parallelism](https://docs.python.org/3/library/threading.html), [Process Based Parallelism](https://docs.python.org/3/library/multiprocessing.html), and should be referred to if you want to learn how to do this.

You might question [why does python have a GIL?](https://realpython.com/python-gil/). You should read the linked article in that case which discussess in detail all the issues with GIL and why is it still a part of the Python programming language. Still, another question rises, why does python have to use reference counting? How do other languages handle that particular issue?

Python uses reference counting because it aligns with its philosophy of simplicity and predictability. Since the objects are removed as soon as their references, it makes it deterministic. This makes managing resources straightforward and reduces cognitive overload for the programmer, in contrast to languages like C and C++ which let the programmer have complete control over the memory. This improves the control significantly and lets memory be managed as and when needed, but also increases the development time. 

Overall the two languages have different philosophies. Since C++ code is compiled directly to machine code it is highly optimized. For CPU intensive tasks, C++ is thus much faster than Python. However, due to the single threaded nature imposed by GIL in Python it has the unique ability that C extensions written for it do not have to worry about thread safety related issues, and thus are readily integrable. Sections of code of such libraries which do not need any interaction with Python objects can release the GIL, and thus utilise multiple cores of the CPU. Libraries like NumPy are written in C and are compiled into machine code when the library is built. 


### Numba and JIT
Having established that for CPU intensive tasks, Python is limited by the GIL, and a solution to this problem is to use pre-compiled libraries like NumPy, what can we do if we want to speed up small parts of the program which do not communicate with Python objects? For programs in which there exist computationally intensive tasks which constitute a significant portion of the run-time, we can speed these up by using _Just-in-Time Compilation (JIT)_. 

While a standard Python interpreter reads your code line-by-line and executes it, a _Ahead-of-Time (AOT)_ compiler like C++ reads all the code, analyses it and converts it into machine code which can then directly be loaded into the machine and executed.
> You might question, why aren't all languages AOT compiled if it is so much faster than in time compilation. There are various reasons for it... 

A JIT approach is a hybrid approach, which in the initial call to the function compiles the code, translates the function into highly optimized machine code, which can be loaded directly into machine, skipping the slow interpretation part completely. Here's a [great video](https://www.youtube.com/watch?v=d7KHAVaX_Rs&) by Computerphile about how JIT compilers work.

Numba is one such JIT compiler, which translates a subset of Python code into machine code, and thus can be used to essentially "re-write" your performance critical functions as if they were C as you start running them. Here is a video [(YT) Make Python code 1000x faster with Numba](https://www.youtube.com/watch?v=x58W9A2lnQc) which introduces numba, and informs you about some good pratices while using numba, along with some simulation type examples so you understand exactly what numba is about. Here's an official [quick start guide](https://numba.pydata.org/numba-doc/dev/user/5minguide.html) to Numba. If you wish to explore further, the official documentation is a pretty good resource to do so. 

> As a sidenote, I would like to link one of my favorite projects till date which utilises the features of Numba, [(GitHub) String Art Generator](https://github.com/sjais1337/string-art/), and also is the project from which I learnt about Numba due to how necessary speeding it up was. It's a really cool project imo, and you might want to explore the project, and maybe generate some cool art with it.
### Case Study: JAX 

## Parallel Processing on the GPU
So far we have limited our discussion to just CPUs, and how we use a handful of cores to speed up our computation, and perform a few complex tasks simultaneously or divide a large task into smaller more manageable tasks. However, the GPU takes a radically different approach to parallel processing, built specifically for repetitive tasks of rendering graphics. As most of you probably already know, GPUs have proven to be extraordinarily effective for scientific computing tasks, which is due to the architecture of the chip.

- The architecture of a CPU gives it a few cores, each capable of performing complex, sequential operations. They handle the overall logic of the program, the user output and everything.
- The architecture of the GPU gives it thousands of cores, each one serving simple operational task. They all execute the same exact command in unison on a massive amount of data. This makes it ideal to process large amounts of data and perform simple operations on them.

> You might question, if GPUs have so many cores, why can't CPUs have them too? Well, tldr; there's a fundamental difference between the architecture of both the chips. Each core of a CPU has large, dedicated memory caches (L1, L2) and to store data and the CPU utilises a huge amount of the chip space for features like branch prediction and out of order execution. Thus the overall size of a CPU core is much bigger than the size of a GPU core, which requires a tiny cache and minimal control logic. Then, another question pops up, why not make bigger CPUs?

> We reach issues at the manufacturing level now. The CPUs themselves are printed at the nanometer scale on a silicon wafer, which usually has microscoping defects and due to even a single defect the entire chip can become useless. Thus, smaller chips have higher success rates than bigger chips. And to make smaller chips with the same compute, you need to have finer printing technologies, at which point we soon reach the limitation of physical laws, and not just in printing. 

> The bigger the chip is, the more time does it take for a singal to travel through it, which at the billions of operations per second rate becomes a huge bottleneck.  Here is an extremely cool video about [[YT] How are Microchips Made](https://www.youtube.com/watch?v=dX9CGRZwD-w), but at the end we will reach a dead end, due to [Amdahl's law](https://en.wikipedia.org/wiki/Amdahl's_law), which as Wikipedia states, is "a formula that shows how much faster a task can be completed when more resources are added to the system". 


The architecture of a GPU is called <b>SIMT (Single Instruction, Multiple Threads)</b>. A given group of threads on a GPU has to perform the same operation, in contrast to CPUs where each thread can have its own operation independent of the other threads. Large group of threads are called "Warps" or "Bundles" and are usually in groups of 32 and 64, and an entire bundle executes one instruction at a time. I highly recommend the following video to learn about the GPU architecture: [[YT] How do Graphics Cards Work? Exploring GPU Architecture](https://www.youtube.com/watch?v=h9Z4oGN89MU).

The way you program a GPU is significantly different from how you program a CPU. The workflow for GPGPU (General Purpose Computing on GPU) is:
1. Host Code (CPU): The main program still has to run on the CPU.
2. Data Transfer: The CPU copies the necessary data which is to be used to act from the system RAM to the VRAM. This step is the biggest performance bottleneck.
> If you have heard about ARM chips, then you probably know how the main reason Apple Silicon was so much better than the other competitors on the market is because Apple has complete control over what goes in their chips. Theoretically speaking if the CPU was built with unified memory, then this bottleneck would reduce dramatically. Infact, there exists the HSA foundation which is developing the HSA, which is a [Heterogenous System Architecture](http://en.wikipedia.org/wiki/Heterogeneous_System_Architecture). Here is an article by NVIDIA exploring the same in CUDA, [Unified Memory for CUDA Beginners](https://developer.nvidia.com/blog/unified-memory-cuda-beginners/).

3. Kernel Launch: The CPU launches a kernel, which is a function written by the programmer which will be executed by every single GPU thread you've launched.
4. Device Code (GPU): The threads all run in parallel.
5. Result Transfer: Once the kernel is finished the result is copied back from the VRAM to system RAM. 

The dominant frameworks as of now for GPGPU are NVIDIA's CUDA and OpenCL. If you want to get into this stuff, then official [CUDA C++ Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html) is the best resource out there.