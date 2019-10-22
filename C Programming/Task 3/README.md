### Task 3

#### Short description:

Implement a program that recovers JPEGs from a forensic image (card.raw file storing byte after byte memory card data, formatted with FAT system whose "block size" is 512 bytes).

#### Specification:

•	Your program should accept exactly one command-line argument, the name of a forensic image from which to recover JPEGs. + If your program is not executed with exactly one command-line argument, it should remind the user of correct usage, and main should return 1.<br/>
•	If the forensic image cannot be opened for reading, your program should inform the user as much, and main should return 2.<br/>
•	Your program, if it uses malloc, must not leak any memory.<br/>

#### Example behavior:

$ ./task3<br/>
Usage: ./recover image<br/>
$ echo $?<br/>
1<br/>

$ ./task3 card.raw<br/>
$ echo $?<br/>
0<br/>
