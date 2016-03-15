# Project 1

Written by Evan Allan and Michael Berezanich

### Assignment

You will implement `wordc-mp`, a multi-process C program, whose inputs and outputs should be identical to those for wordc except for one difference: `wordc-mp` will accept an additional (4th) command-line argument denoting its “degree of concurrency.” By this we mean the number of processes a particular execution of `wordc-mp` would employ. 

For a degree of concurrency n > 1, your program must create n − 1 additional processes using the fork() system call (for a total of n processes including the original one that starts the execution of the program). Let us denote these processes as P0, P1, ..., Pn−1, where P0 is the parent of all the remaining processes. Each of these n processes will carry out word counting over (roughly) 1 / n of the input text file. E.g., you may choose for P0 to process the first 1 / n words or bytes (while respecting word boundaries), P1 the second 1 / n words or bytes, etc. 

P0 must create n − 1 pipes (using the pipe() system call), one for reading information from each of its children. Each child process is to carry out a word counting for its subset of words (which we will call its “mini word count”) and then pass its output to the parent process using the appropriate pipe. The parent must then merge the outcome of its own mini word count with those of all of its children to create the final output that it would write to a file in the format expected in the last project. Important: The following is different from the first version of this project: Each child process is to indicate to the parent that it has finished its mini word count by closing its “write end” of the pipe which will cause the parent to read the end-of-file (EOF) character from its “read end”. Notice that this makes the project significantly easier than if you had to implement this messaging via signals! Here are some specifications we would like you to adhere to: 

* We expect that your executable would be run as follows:
`$ ./wordc-mp input-textfile output-countfile output-runtime n`
* Your implementations must handle erroneous inputs properly and help the user understand what went wrong in case they made a mistake in usage. 
* Your treatment of words regarding case-sensitivity, special characters, etc., should be exactly like in the first project. If you would like to make a change for some reason, you must indicate that in your README file and also mention this to the TAs during your demo. 
* Do not forget to indicate the names of BOTH members of your team in your submission to Dropbox!
