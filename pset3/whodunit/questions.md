# Questions

## What's `stdint.h`?

"stdint.h" is header file which contains the definition of additional integer types. With them, you can build
projects with enhanced compatibility with other programs and improved data handling. 

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

They address the need to store integers in a fixed length data types, taking predefined amount of bits, particularly in our case matching bmp file structure.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1 / 4 / 4 / 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM / 66 77 / 0x42 0x4d

## What's the difference between `bfSize` and `biSize`?

They are parts of two different BMP header structures, `bfSize` representing image size without header structures and biSize representing the number of bytes required by BITMAPINFOHEADER struct. 

## What does it mean if `biHeight` is negative?

If biHeight is negative, the bitmap is a top-down color table and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

E.g if specified file name was not correct

## Why is the third argument to `fread` always `1` in our code?

We only need to read given block of bytes once

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

The fseek() function  sets the file position indicator for the file pointed to by first argument. The new  position, measured in bytes, is obtained by adding offset bytes determined by second argument to the position specified by third argument.

## What is `SEEK_CUR`?

It is one of pre-determined third argument variants of fseek() function. It sets the offset of fseek() to be relative to the the current position indicator.
