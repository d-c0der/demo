### BMPSCALER

#### Short description:
"bmpscaler.c" creates a rezized copy of 24-bit BMP image. 

#### Specification:
•	Program accepts exactly three command-line arguments, where:<br/>
•	the first must be a floating-point value in the range (0.0, 100.0] (ie., a positive value less than or equal to 100.0),<br/>
•	the second must be the name of a BMP to be resized, and<br/>
•	the third must be the name of the resized version to be written.<br/>
•	If program is not executed with such, it reminds the user of correct usage, and main returns 1.<br/>

#### Example behavior:
$ ./bmpscaler
Wrong number of arguments
$ echo $?
1<br/>
<br/>
$ ./bmpscaler .5 large.bmp smaller.bmp
$ echo $?
0<br/>
<br/>
$ ./bmpscaler small.bmp larger.bmp
$ echo $?
0<br/>

#### Warning:
"Bmpscaler.c" was created and intended to run on Linux system.<br/>

#### Extras:
Additional header file "bmp.h" containing BMP related structure types is included.