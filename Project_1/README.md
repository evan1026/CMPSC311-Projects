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

  `$. /wordc.sh input-textfile output-countfile output-runtime`

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
