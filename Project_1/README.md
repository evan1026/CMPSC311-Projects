[//]: # (Note: If you can see this, you are reading the raw file. This file is best viewed when rendered using markdown.)
[//]: # (Note: A rendering of it can be found at https://github.com/evan1026/CMPSC311-Projects)

# Project 1

### Assignment
You will create two implementations of a `wordcount` application which
does the following: it takes as input a large text file, finds out the number
of occurrences of each uniquely-occurring word, and then prints these as
_word, count_ pairs into an output file (one pair per line of the output file)
wherein the pairs are arranged in a dictionary order. It also computes its
execution time and prints that into a second output file. E.g., if the input
file were

    aardvark zebra cat zebra zebra

the first output file would be

    aardvark, 1
    cat, 1
    zebra, 3

Your first implementation will be in C and the second using the shell
utilities covered in class (cat, grep, awk, sort, sed, cut, pipes, IO re-direction
facilities, and perhaps others). The reason for asking you to implement
the same application in two different ways is three-fold: (i) to help you
appreciate very concretely the relative pros and cons of C and shell programming,
(ii) to make you think of how you might want to combine
the "best of both" (for you to think on your own, no submission required
for such a "hybrid" solution), (iii) to understand how the performance of
these two implementations compares for some selected inputs. Through
this excercise, hopefully, you will begin to develop an appreciation for the
ease-of-implementation vs. performance efficiency trade-offs that systems
programmers must often navigate.

Next we present some specifics:
 * The C executable, named `wordc`, should be used as follows:

  `$ ./wordc input-textfile output-countfile output-runtime`

  The shell implementation must be in the form of a shell script named
`wordc.sh`, that should be invoked as follows:

  `$ ./wordc.sh input-textfile output-countfile output-runtime`

  Here the last argument refers to a file wherein you will record the
execution time of your program (at a fine enough granularity so that
the execution times for the two versions of `wordcount` may be compared).

 * Your implementations must handle erroneous inputs properly and
help the user understand what went wrong in case they made a mistake in usage.

 * Your treatment of words should not be case-sensitive (e.g., `Cat` and
`cat` are equivalent). You are to ignore all characters other than alphabets
and numerals. In particular, you are to ignore punctuation
marks and not consider them parts of words (which will simplify
your job quite a lot). So `Cat's` and `Cat!` should be treated
as `cats` and `cat`, respectively, whereas `"Chairman Meow"` becomes
`Chairman` and `Meow`.

 * Your C program must create a sorted linked list of structs, one for
each uniquely-occurring word it finds in the input file. Only when
the entire list is ready should you write it into the output file.

### Code Files
More detailed info can be found within the files themselves, but here is a basic rundown:

 * `src/main.c` - The file containing the `main()` function. This file handles I/O as well as measuring the time.
 * `src/list.*` - The header and c files associated with the linked list. The list itself does not handle counting, only keeping track of which words have been found. This is done in conjunction with a hashtable to speed up the program.
 * `src/hashtable.*` - The header and c files associated with the hashtable. This is used to speed up the program (which was originally horrifically slow). It allows for constant time access to see if a word has been found and get/set its associated count.
 * `wordc.sh` - The shell script for this project. Most of the heavy lifting is done by awk, whose code can be found toward the end of the file.

### Experience / Problems
When we first wrote the code, both the script and the C program were running on the scale of seconds. The C program was faster, at around 2 seconds, with the shell program taking 30 seconds or more. Then, we switched the shell file from associative arrays to an awk script. This sped up the script so much it was going faster than the C program. As a result, we had to add the hashtable to the C program, because it changed the program from an `O(n^2)` algorithm to an `O(n log n)` one (the `n log n` came entirely from the sort; adding a word was `O(1)`, where it had previously been `O(n)`). This is why the hashtable is there; to give the C program a fighting chance. It still follows the specifications in that it still makes a sorted linked list of words in which each word appears once, but it no longer tries to keep it sorted at runtime. As a result of all of this, you should find that, on a modern machine, both programs will probably run in under a second, and the C program will be a bit faster than the shell script (although you won't be able to see the difference except on large files).

Overall, the experience was what it was meant to be: the C programming took a lot more work, but, when done right, it produced quicker code. Shell scripting, on the other hand, took very little work, but produced slower programs.

As far as what might go wrong, the program handles ASCII text just fine, but has issues with UTF-8. The `arabian nights` file has some UTF-8 characters in it, and, as a result, the count file had a bunch of numbers at the top which did not seem to correspond to numbers in the actual file. We believe this is because of the UTF-8 characters, as we are just handling each byte as a `char`, whereas UTF-8 doesn't adhere to that.

### Performance
#### TODO
