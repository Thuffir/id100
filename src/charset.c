/***********************************************************************************************************************
 *
 * ID100 Utility
 * Character Set
 *
 * (C) 2017 Gergely Budai
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 **********************************************************************************************************************/
#include "charset.h"

/***********************************************************************************************************************
 * Get a requested character from the character set
 **********************************************************************************************************************/
CharSetType CharSetGetChar(uint8_t ascii)
{
// This holds the whole character set as bitmap, 16 bit per character
static const CharSetType charSet[] = {
// Space
0b\
000\
000\
000\
000\
000,
// !
0b\
010\
010\
010\
000\
010,
// "
0b\
101\
101\
000\
000\
000,
// #
0b\
101\
111\
101\
111\
101,
// $
0b\
011\
110\
010\
011\
110,
// %
0b\
101\
001\
010\
100\
101,
// &
0b\
010\
101\
010\
101\
011,
// '
0b\
010\
010\
000\
000\
000,
// (
0b\
001\
010\
010\
010\
001,
// )
0b\
100\
010\
010\
010\
100,
// *
0b\
000\
101\
010\
101\
000,
// +
0b\
000\
010\
111\
010\
000,
// ,
0b\
000\
000\
000\
010\
100,
// -
0b\
000\
000\
111\
000\
000,
// .
0b\
000\
000\
000\
000\
010,
// /
0b\
001\
001\
010\
100\
100,
// 0
0b\
111\
101\
101\
101\
111,
// 1
0b\
001\
001\
001\
001\
001,
// 2
0b\
111\
001\
111\
100\
111,
// 3
0b\
111\
001\
111\
001\
111,
// 4
0b\
100\
101\
111\
001\
001,
// 5
0b\
111\
100\
111\
001\
111,
// 6
0b\
111\
100\
111\
101\
111,
// 7
0b\
111\
001\
001\
001\
001,
// 8
0b\
111\
101\
111\
101\
111,
// 9
0b\
111\
101\
111\
001\
111,
// :
0b\
000\
010\
000\
010\
000,
// ;
0b\
000\
010\
000\
010\
100,
// <
0b\
001\
010\
100\
010\
001,
// =
0b\
000\
111\
000\
111\
000,
// >
0b\
100\
010\
001\
010\
100,
// ?
0b\
010\
101\
001\
010\
010,
// @
0b\
010\
101\
111\
100\
011,
// A
0b\
010\
101\
111\
101\
101,
// B
0b\
110\
101\
110\
101\
110,
// C
0b\
010\
101\
100\
101\
010,
// D
0b\
110\
101\
101\
101\
110,
// E
0b\
111\
100\
110\
100\
111,
// F
0b\
111\
100\
110\
100\
100,
// G
0b\
011\
100\
101\
101\
010,
// H
0b\
101\
101\
111\
101\
101,
// I
0b\
111\
010\
010\
010\
111,
// J
0b\
111\
001\
001\
101\
010,
// K
0b\
101\
101\
110\
101\
101,
// L
0b\
100\
100\
100\
100\
111,
// M
0b\
101\
111\
101\
101\
101,
// N
0b\
101\
111\
111\
111\
101,
// O
0b\
010\
101\
101\
101\
010,
// P
0b\
110\
101\
110\
100\
100,
// Q
0b\
010\
101\
101\
010\
001,
// R
0b\
110\
101\
110\
101\
101,
// S
0b\
011\
100\
010\
001\
110,
// T
0b\
111\
010\
010\
010\
010,
// U
0b\
101\
101\
101\
101\
010,
// V
0b\
101\
101\
101\
010\
010,
// W
0b\
101\
101\
111\
111\
010,
// X
0b\
101\
101\
010\
101\
101,
// Y
0b\
101\
101\
010\
010\
010,
// Z
0b\
111\
001\
010\
100\
111,
// [
0b\
011\
010\
010\
010\
011,
// Backslash
0b\
100\
100\
010\
001\
001,
// ]
0b\
110\
010\
010\
010\
110,
// ^
0b\
010\
101\
000\
000\
000,
// _
0b\
000\
000\
000\
000\
111,
// `
0b\
010\
001\
000\
000\
000,
// a
0b\
000\
010\
101\
101\
011,
// b
0b\
100\
110\
101\
101\
110,
// c
0b\
000\
011\
100\
100\
011,
// d
0b\
001\
011\
101\
101\
011,
// e
0b\
000\
010\
101\
110\
011,
// f
0b\
011\
010\
111\
010\
010,
// g
0b\
000\
010\
101\
011\
110,
// h
0b\
100\
110\
101\
101\
101,
// i
0b\
010\
000\
010\
010\
010,
// j
0b\
010\
000\
010\
010\
100,
// k
0b\
100\
100\
101\
110\
101,
// l
0b\
010\
010\
010\
010\
001,
// m
0b\
000\
111\
111\
101\
101,
// n
0b\
000\
010\
101\
101\
101,
// o
0b\
000\
010\
101\
101\
010,
// p
0b\
000\
110\
101\
110\
100,
// q
0b\
000\
011\
101\
011\
001,
// r
0b\
000\
100\
110\
100\
100,
// s
0b\
000\
010\
100\
010\
100,
// t
0b\
010\
111\
010\
010\
001,
// u
0b\
000\
101\
101\
101\
010,
// v
0b\
000\
101\
101\
010\
010,
// w
0b\
000\
101\
101\
111\
010,
// x
0b\
000\
101\
010\
010\
101,
// y
0b\
000\
101\
101\
011\
110,
// z
0b\
000\
111\
011\
110\
111,
// {
0b\
011\
010\
110\
010\
011,
// |
0b\
010\
010\
000\
010\
010,
// }
0b\
110\
010\
011\
010\
110,
// ~
0b\
000\
001\
111\
100\
000
};

  // Limit ascii char
  if((ascii < ' ') || (ascii > '~')) {
    ascii = ' ';
  }

  // Return requested (or limited) character
  return charSet[ascii - ' '];
}
