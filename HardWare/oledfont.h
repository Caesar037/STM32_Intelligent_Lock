#ifndef __OLEDFONT_H
#define __OLEDFONT_H

//常用ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
/************************************6*8的点阵************************************/
const unsigned char F6x8[][6] =		
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// sp
0x00, 0x00, 0x00, 0x2f, 0x00, 0x00,// !
0x00, 0x00, 0x07, 0x00, 0x07, 0x00,// "
0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14,// #
0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12,// $
0x00, 0x62, 0x64, 0x08, 0x13, 0x23,// %
0x00, 0x36, 0x49, 0x55, 0x22, 0x50,// &
0x00, 0x00, 0x05, 0x03, 0x00, 0x00,// '
0x00, 0x00, 0x1c, 0x22, 0x41, 0x00,// (
0x00, 0x00, 0x41, 0x22, 0x1c, 0x00,// )
0x00, 0x14, 0x08, 0x3E, 0x08, 0x14,// *
0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,// +
0x00, 0x00, 0x00, 0xA0, 0x60, 0x00,// ,
0x00, 0x08, 0x08, 0x08, 0x08, 0x08,// -
0x00, 0x00, 0x60, 0x60, 0x00, 0x00,// .
0x00, 0x20, 0x10, 0x08, 0x04, 0x02,// /
0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,// 1
0x00, 0x42, 0x61, 0x51, 0x49, 0x46,// 2
0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,// 3
0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,// 4
0x00, 0x27, 0x45, 0x45, 0x45, 0x39,// 5
0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
0x00, 0x01, 0x71, 0x09, 0x05, 0x03,// 7
0x00, 0x36, 0x49, 0x49, 0x49, 0x36,// 8
0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,// 9
0x00, 0x00, 0x36, 0x36, 0x00, 0x00,// :
0x00, 0x00, 0x56, 0x36, 0x00, 0x00,// ;
0x00, 0x08, 0x14, 0x22, 0x41, 0x00,// <
0x00, 0x14, 0x14, 0x14, 0x14, 0x14,// =
0x00, 0x00, 0x41, 0x22, 0x14, 0x08,// >
0x00, 0x02, 0x01, 0x51, 0x09, 0x06,// ?
0x00, 0x32, 0x49, 0x59, 0x51, 0x3E,// @
0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C,// A
0x00, 0x7F, 0x49, 0x49, 0x49, 0x36,// B
0x00, 0x3E, 0x41, 0x41, 0x41, 0x22,// C
0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C,// D
0x00, 0x7F, 0x49, 0x49, 0x49, 0x41,// E
0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,// F
0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A,// G
0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F,// H
0x00, 0x00, 0x41, 0x7F, 0x41, 0x00,// I
0x00, 0x20, 0x40, 0x41, 0x3F, 0x01,// J
0x00, 0x7F, 0x08, 0x14, 0x22, 0x41,// K
0x00, 0x7F, 0x40, 0x40, 0x40, 0x40,// L
0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F,// M
0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F,// N
0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E,// O
0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,// P
0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
0x00, 0x7F, 0x09, 0x19, 0x29, 0x46,// R
0x00, 0x46, 0x49, 0x49, 0x49, 0x31,// S
0x00, 0x01, 0x01, 0x7F, 0x01, 0x01,// T
0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F,// U
0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F,// V
0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F,// W
0x00, 0x63, 0x14, 0x08, 0x14, 0x63,// X
0x00, 0x07, 0x08, 0x70, 0x08, 0x07,// Y
0x00, 0x61, 0x51, 0x49, 0x45, 0x43,// Z
0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,// [
0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55,// 55
0x00, 0x00, 0x41, 0x41, 0x7F, 0x00,// ]
0x00, 0x04, 0x02, 0x01, 0x02, 0x04,// ^
0x00, 0x40, 0x40, 0x40, 0x40, 0x40,// _
0x00, 0x00, 0x01, 0x02, 0x04, 0x00,// '
0x00, 0x20, 0x54, 0x54, 0x54, 0x78,// a
0x00, 0x7F, 0x48, 0x44, 0x44, 0x38,// b
0x00, 0x38, 0x44, 0x44, 0x44, 0x20,// c
0x00, 0x38, 0x44, 0x44, 0x48, 0x7F,// d
0x00, 0x38, 0x54, 0x54, 0x54, 0x18,// e
0x00, 0x08, 0x7E, 0x09, 0x01, 0x02,// f
0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,// g
0x00, 0x7F, 0x08, 0x04, 0x04, 0x78,// h
0x00, 0x00, 0x44, 0x7D, 0x40, 0x00,// i
0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,// j
0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,// k
0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,// l
0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,// m
0x00, 0x7C, 0x08, 0x04, 0x04, 0x78,// n
0x00, 0x38, 0x44, 0x44, 0x44, 0x38,// o
0x00, 0xFC, 0x24, 0x24, 0x24, 0x18,// p
0x00, 0x18, 0x24, 0x24, 0x18, 0xFC,// q
0x00, 0x7C, 0x08, 0x04, 0x04, 0x08,// r
0x00, 0x48, 0x54, 0x54, 0x54, 0x20,// s
0x00, 0x04, 0x3F, 0x44, 0x40, 0x20,// t
0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C,// u
0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,// v
0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C,// w
0x00, 0x44, 0x28, 0x10, 0x28, 0x44,// x
0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,// y
0x00, 0x44, 0x64, 0x54, 0x4C, 0x44,// z
0x14, 0x14, 0x14, 0x14, 0x14, 0x14,// horiz lines
};
/****************************************8*16的点阵************************************/
const unsigned char F8X16[]=	  
{
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// 0
  0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,//! 1
  0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//" 2
  0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,//# 3
  0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,//$ 4
  0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,//% 5
  0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,//& 6
  0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//' 7
  0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,//( 8
  0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,//) 9
  0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,//* 10
  0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,//+ 11
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,//, 12
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//- 13
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,//. 14
  0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,/// 15
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,//0 16
  0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//1 17
  0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,//2 18
  0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,//3 19
  0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,//4 20
  0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,//5 21
  0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,//6 22
  0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,//7 23
  0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,//8 24
  0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,//9 25
  0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,//: 26
  0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,//; 27
  0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,//< 28
  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,//= 29
  0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,//> 30
  0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,//? 31
  0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,//@ 32
  0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,//A 33
  0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,//B 34
  0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,//C 35
  0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,//D 36
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,//E 37
  0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,//F 38
  0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,//G 39
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,//H 40
  0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//I 41
  0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,//J 42
  0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,//K 43
  0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,//L 44
  0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,//M 45
  0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,//N 46
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,//O 47
  0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,//P 48
  0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,//Q 49
  0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,//R 50
  0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,//S 51
  0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//T 52
  0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//U 53
  0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,//V 54
  0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,//W 55
  0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,//X 56
  0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,//Y 57
  0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,//Z 58
  0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,//[ 59
  0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,//\ 60
  0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,//] 61
  0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//^ 62
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//_ 63
  0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//` 64
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,//a 65
  0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,//b 66
  0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,//c 67
  0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,//d 68
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,//e 69
  0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//f 70
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,//g 71
  0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//h 72
  0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//i 73
  0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,//j 74
  0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,//k 75
  0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,//l 76
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,//m 77
  0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,//n 78
  0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,//o 79
  0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,//p 80
  0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,//q 81
  0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,//r 82
  0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,//s 83
  0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,//t 84
  0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,//u 85
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,//v 86
  0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,//w 87
  0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,//x 88
  0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,//y 89
  0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,//z 90
  0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,//{ 91
  0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,//| 92
  0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,//} 93
  0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//~ 94
};

char Hzk[][32]={

{0x00,0x00,0xF8,0x08,0x08,0x08,0x08,0x09,0x0E,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x80,0x60,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"广",0*/
/* (16 X 16 , 宋体 )*/

{0x00,0xE0,0x00,0xFF,0x00,0x20,0xC0,0x00,0xFE,0x00,0x20,0xC0,0x00,0xFF,0x00,0x00},
{0x81,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0xFF,0x00,0x00},/*"州",1*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0xFE,0x02,0x96,0x5A,0x33,0xFE,0x32,0x5A,0x96,0x02,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x05,0x1D,0x15,0x15,0x15,0x15,0x15,0x55,0x95,0x55,0x35,0x04,0x04,0x00},/*"粤",2*/
/* (16 X 16 , 宋体 )*/

{0x80,0x80,0xEE,0x88,0x88,0x88,0xE8,0x8F,0x08,0x88,0x78,0x48,0x4E,0x40,0xC0,0x00},
{0x00,0x00,0x7F,0x24,0x24,0x24,0x7F,0x00,0x81,0x40,0x30,0x0F,0x30,0x41,0x80,0x00},/*"嵌",3*/
/* (16 X 16 , 宋体 )*/

{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},/*"温",4*/
/* (16 X 16 , 宋体 )*/

{0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00},/*"湿",5*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",6*/
/* (16 X 16 , 宋体 )*/

{0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00},
{0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00},/*"光",7*/
/* (16 X 16 , 宋体 )*/

{0x00,0xFE,0x42,0x42,0x42,0xFE,0x00,0x42,0xA2,0x9E,0x82,0xA2,0xC2,0xBE,0x00,0x00},
{0x80,0x6F,0x08,0x08,0x28,0xCF,0x00,0x00,0x2F,0xC8,0x08,0x08,0x28,0xCF,0x00,0x00},/*"照",8*/
/* (16 X 16 , 宋体 )*/

{0x00,0x80,0x60,0xF8,0x07,0x04,0xE4,0xA4,0xA4,0xBF,0xA4,0xA4,0xE4,0x04,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x40,0x40,0x7F,0x4A,0x4A,0x4A,0x4A,0x4A,0x7F,0x40,0x40,0x00},/*"值",9*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"：",10*/
/* (16 X 16 , 宋体 )*/

{0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00},
{0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00},/*"℃",11*/
/* (16 X 16 , 宋体 )*/

/**************************************天气信息***********************************/
{0x00,0xFC,0x84,0x84,0xFC,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x00},
{0x00,0x3F,0x10,0x10,0x3F,0x00,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00},/*"晴",12*/
/* (16 X 16 , 宋体 )*/

{0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12,0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00},
{0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11,0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00},/*"间",13*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0x10,0x10,0x98,0xA4,0x47,0x44,0xA4,0x54,0x0C,0x04,0x00,0x00,0x00,0x00},
{0x00,0x81,0x89,0x89,0x44,0x44,0x4A,0x31,0x21,0x11,0x09,0x05,0x03,0x00,0x00,0x00},/*"多",14*/
/* (16 X 16 , 宋体 )*/

{0x40,0x40,0x42,0x42,0x42,0x42,0xC2,0x42,0x42,0x42,0x42,0x42,0x42,0x40,0x40,0x00},
{0x00,0x20,0x70,0x28,0x24,0x23,0x20,0x20,0x20,0x24,0x28,0x30,0xE0,0x00,0x00,0x00},/*"云",15*/
/* (16 X 16 , 宋体 )*/

{0x40,0x44,0x54,0x65,0x46,0x44,0x64,0x54,0x44,0x40,0xFE,0x02,0x22,0xDA,0x06,0x00},
{0x00,0x00,0x7E,0x22,0x22,0x22,0x22,0x7E,0x00,0x00,0xFF,0x08,0x10,0x08,0x07,0x00},/*"部",16*/
/* (16 X 16 , 宋体 )*/

{0x00,0xFE,0x02,0x22,0xDA,0x06,0x00,0xFE,0x22,0x22,0x22,0x22,0x22,0xFE,0x00,0x00},
{0x00,0xFF,0x08,0x10,0x88,0x47,0x30,0x0F,0x02,0x02,0x02,0x42,0x82,0x7F,0x00,0x00},/*"阴",17*/
/* (16 X 16 , 宋体 )*/

{0x20,0x18,0x0A,0xAA,0xAA,0xAA,0x0A,0xFE,0x0A,0xAA,0xAA,0xAA,0x0A,0x28,0x18,0x00},
{0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},/*"雷",18*/
/* (16 X 16 , 宋体 )*/

{0x00,0x80,0x60,0xF8,0x07,0x20,0x22,0x2C,0x20,0xFF,0x20,0x28,0x24,0x22,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x02,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x02,0x02,0x02,0x00},/*"伴",19*/
/* (16 X 16 , 宋体 )*/

{0x04,0x04,0x04,0x84,0xE4,0x3C,0x27,0x24,0x24,0x24,0x24,0xE4,0x04,0x04,0x04,0x00},
{0x04,0x02,0x01,0x00,0xFF,0x09,0x09,0x09,0x09,0x49,0x89,0x7F,0x00,0x00,0x00,0x00},/*"有",20*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0x00,0xE0,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x20,0x40,0x80,0x00,0x00},
{0x08,0x04,0x03,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,0x01,0x0E,0x00},/*"小",21*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
{0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},/*"中",22*/
/* (16 X 16 , 宋体 )*/

{0x00,0xFE,0x02,0x22,0xDA,0x06,0x08,0xC8,0xB8,0x8F,0xE8,0x88,0x88,0x88,0x08,0x00},
{0x00,0xFF,0x08,0x10,0x08,0x07,0x08,0x08,0x08,0x08,0xFF,0x08,0x08,0x08,0x08,0x00},/*"阵",23*/
/* (16 X 16 , 宋体 )*/

{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00},
{0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00},/*"大",24*/
/* (16 X 16 , 宋体 )*/

{0x00,0x40,0x40,0x5F,0xF5,0x55,0x55,0x55,0x55,0x55,0xF5,0x5F,0x40,0x40,0x00,0x00},
{0x09,0x09,0x85,0x45,0x2B,0x51,0x81,0x7D,0x01,0x11,0x2B,0x45,0x85,0x09,0x09,0x00},/*"暴",25*/
/* (16 X 16 , 宋体 )*/

{0x40,0x3C,0x10,0xFF,0x10,0x10,0x40,0x48,0x48,0x48,0x7F,0x48,0xC8,0x48,0x40,0x00},
{0x02,0x06,0x02,0xFF,0x01,0x01,0x00,0x02,0x0A,0x12,0x42,0x82,0x7F,0x02,0x02,0x00},/*"特",26*/
/* (16 X 16 , 宋体 )*/

{0x00,0x02,0x0C,0x80,0x60,0x08,0x88,0x68,0x18,0x0F,0xE8,0x08,0x08,0x08,0x08,0x00},
{0x02,0x02,0x7E,0x01,0x00,0x20,0x11,0x0D,0x41,0x81,0x7F,0x01,0x05,0x09,0x30,0x00},/*"冻",27*/
/* (16 X 16 , 宋体 )*/

{0x02,0xE2,0x22,0x22,0x22,0x22,0x22,0xFE,0x22,0x22,0x22,0x22,0x22,0xE2,0x02,0x00},
{0x00,0xFF,0x00,0x00,0x09,0x12,0x00,0x7F,0x00,0x09,0x12,0x40,0x80,0x7F,0x00,0x00},/*"雨",28*/
/* (16 X 16 , 宋体 )*/

{0x00,0x08,0x08,0x28,0x48,0x08,0x08,0xFF,0x08,0x08,0x48,0x28,0x08,0x08,0x00,0x00},
{0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00},/*"夹",29*/
/* (16 X 16 , 宋体 )*/

{0x10,0x0C,0x05,0x55,0x55,0x55,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00},
{0x00,0x00,0x41,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00},/*"雪",30*/
/* (16 X 16 , 宋体 )*/

{0x00,0x02,0x0C,0xC0,0x00,0x20,0x20,0xE0,0x00,0xFF,0x70,0xA0,0x10,0x08,0x00,0x00},
{0x02,0x02,0x7F,0x00,0x10,0x08,0x06,0x41,0x80,0x7F,0x00,0x01,0x06,0x08,0x10,0x00},/*"冰",31*/
/* (16 X 16 , 宋体 )*/

{0x10,0x0C,0x05,0x55,0xD5,0x55,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00},
{0x00,0x04,0x02,0x7D,0x95,0x95,0x95,0x95,0x9D,0x91,0xA1,0x9F,0x80,0xC0,0x00,0x00},/*"雹",32*/
/* (16 X 16 , 宋体 )*/

{0x10,0x60,0x02,0x0C,0xC0,0x00,0x94,0xA4,0x84,0x94,0xA2,0x82,0xA3,0x1A,0x00,0x00},
{0x04,0x04,0x7C,0x03,0x00,0x04,0x04,0x04,0x44,0x84,0x7E,0x05,0x04,0x04,0x04,0x00},/*"浮",33*/
/* (16 X 16 , 宋体 )*/

{0x00,0x40,0x20,0x10,0x0C,0x00,0x00,0x7F,0x00,0x00,0x04,0x08,0x10,0x60,0x00,0x00},
{0x40,0x40,0x44,0x44,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00},/*"尘",34*/
/* (16 X 16 , 宋体 )*/

{0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0x42,0xE2,0x52,0x4A,0xC6,0x42,0x40,0xC0,0x00},
{0x04,0x44,0x82,0x7F,0x01,0x20,0x10,0x8C,0x43,0x20,0x18,0x47,0x80,0x40,0x3F,0x00},/*"扬",35*/
/* (16 X 16 , 宋体 )*/

{0x10,0x60,0x02,0x8C,0x00,0x80,0x60,0x18,0x00,0xFF,0x00,0x00,0x88,0x10,0x60,0x00},
{0x04,0x04,0x7E,0x01,0x00,0x80,0x80,0x40,0x40,0x23,0x10,0x0C,0x03,0x00,0x00,0x00},/*"沙",36*/
/* (16 X 16 , 宋体 )*/

{0x02,0xE2,0x22,0x22,0x3E,0x00,0x80,0x9E,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00},
{0x00,0x43,0x82,0x42,0x3E,0x40,0x47,0x44,0x44,0x44,0x7F,0x44,0x44,0x54,0xE7,0x00},/*"强",37*/
/* (16 X 16 , 宋体 )*/

{0x10,0x0C,0x05,0x55,0x55,0xD5,0x05,0x7F,0x05,0x55,0x55,0x55,0x05,0x14,0x0C,0x00},
{0x10,0x10,0x10,0x8A,0xA9,0x6B,0x35,0x25,0x25,0xAB,0xE9,0x08,0x10,0x10,0x10,0x00},/*"雾",38*/
/* (16 X 16 , 宋体 )*/

{0x50,0x4C,0xA5,0x35,0xD5,0x95,0x45,0xDF,0x45,0x55,0xD5,0x55,0x45,0xD4,0x0C,0x00},
{0x42,0x4A,0x29,0xA5,0x92,0x7C,0x00,0x57,0x55,0x55,0x7F,0x55,0x55,0x57,0x40,0x00},/*"霾",39*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0xFE,0x02,0x12,0x22,0xC2,0x02,0xC2,0x32,0x02,0xFE,0x00,0x00,0x00,0x00},
{0x80,0x60,0x1F,0x00,0x20,0x10,0x0C,0x03,0x0C,0x30,0x00,0x0F,0x30,0x40,0xF8,0x00},/*"风",40*/
/* (16 X 16 , 宋体 )*/

{0x00,0xFE,0x42,0x82,0x62,0xFE,0x00,0x00,0xFE,0xAA,0xAA,0xAA,0xFE,0x00,0x00,0x00},
{0x80,0x7F,0x04,0x03,0x04,0x1F,0x20,0x42,0x53,0x4A,0x42,0x4A,0x53,0x42,0x60,0x00},/*"飓",41*/
/* (16 X 16 , 宋体 )*/

{0x08,0x08,0x88,0xFF,0x48,0x48,0x00,0x08,0x48,0xFF,0x08,0x08,0xF8,0x00,0x00,0x00},
{0x81,0x65,0x08,0x07,0x20,0xC0,0x08,0x04,0x23,0xC0,0x03,0x00,0x23,0xC4,0x0F,0x00},/*"热",42*/
/* (16 X 16 , 宋体 )*/

{0x00,0xC4,0x44,0x44,0x5F,0x44,0x44,0xDF,0x44,0x44,0x5F,0x44,0x44,0x44,0xC0,0x00},
{0x01,0x00,0x00,0x3E,0x02,0x02,0x02,0xFF,0x02,0x12,0x22,0x1E,0x00,0x01,0x00,0x00},/*"带",43*/
/* (16 X 16 , 宋体 )*/

{0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0xF0,0x10,0x11,0x16,0xD0,0x10,0x10,0x00},
{0x80,0x40,0x20,0x18,0x06,0x41,0x20,0x10,0x3F,0x44,0x42,0x41,0x40,0x40,0x78,0x00},/*"龙",44*/
/* (16 X 16 , 宋体 )*/

{0x00,0x40,0x48,0x4A,0xCC,0x48,0x78,0x4F,0x48,0x48,0xCC,0x4A,0x48,0x40,0x00,0x00},
{0x04,0x04,0x02,0x01,0x7E,0x82,0x82,0x82,0x92,0xA2,0x9E,0x81,0xF2,0x04,0x04,0x00},/*"卷",45*/
/* (16 X 16 , 宋体 )*/

{0x00,0x02,0x0C,0xC0,0x00,0x40,0x20,0x10,0x0C,0x23,0xCC,0x10,0x20,0x40,0x40,0x00},
{0x02,0x02,0x7F,0x00,0x00,0x00,0x01,0x09,0x11,0x21,0xD1,0x0D,0x03,0x00,0x00,0x00},/*"冷",46*/
/* (16 X 16 , 宋体 )*/

{0x08,0x08,0x88,0xFF,0x48,0x48,0x00,0x08,0x48,0xFF,0x08,0x08,0xF8,0x00,0x00,0x00},
{0x81,0x65,0x08,0x07,0x20,0xC0,0x08,0x04,0x23,0xC0,0x03,0x00,0x23,0xC4,0x0F,0x00},/*"热",47*/
/* (16 X 16 , 宋体 )*/

{0x80,0x80,0x88,0x88,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0x88,0x88,0x80,0x80,0x00},
{0x20,0x20,0x10,0x08,0x04,0x02,0x01,0xFF,0x01,0x02,0x04,0x08,0x10,0x20,0x20,0x00},/*"未",48*/
/* (16 X 16 , 宋体 )*/

{0x20,0x18,0x0F,0x08,0xF8,0x08,0x08,0x00,0x00,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00},
{0x81,0x41,0x31,0x0D,0x03,0x0D,0x71,0x01,0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00},/*"知",49*/
/* (16 X 16 , 宋体 )*/

{0x40,0x40,0x42,0x42,0x42,0x42,0x42,0xFE,0x42,0x42,0x42,0x42,0x42,0x40,0x40,0x00},
{0x80,0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x03,0x0C,0x10,0x20,0x40,0x80,0x80,0x00},/*"天",50*/
/* (16 X 16 , 宋体 )*/

{0x20,0x10,0x4C,0x47,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0xD4,0x04,0x04,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x30,0x40,0xF0,0x00},/*"气",51*/
/* (16 X 16 , 宋体 )*/

{0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00},
{0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00},/*"光",52*/
/* (16 X 16 , 宋体 )*/

{0x08,0xF7,0x14,0x54,0x94,0x14,0xF4,0x04,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00},
{0x1D,0x13,0x11,0x55,0x99,0x51,0x3F,0x91,0x41,0x20,0x13,0x0C,0x33,0x40,0x80,0x00},/*"敏",53*/
/* (16 X 16 , 宋体 )*/

{0x08,0x08,0x08,0x88,0x78,0x0F,0x88,0x08,0xE8,0x08,0x08,0x08,0x08,0xC8,0x08,0x00},
{0x10,0x88,0x86,0x41,0x44,0x22,0x11,0x0C,0x03,0x0C,0x10,0x22,0x41,0x80,0x80,0x00},/*"灰",54*/
/* (16 X 16 , 宋体 )*/

{0x00,0x40,0x20,0x10,0x0C,0x00,0x00,0x7F,0x00,0x00,0x04,0x08,0x10,0x60,0x00,0x00},
{0x40,0x40,0x44,0x44,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00},/*"尘",55*/
/* (16 X 16 , 宋体 )*/

{0x00,0x80,0x60,0xF8,0x07,0x04,0xE4,0xA4,0xA4,0xBF,0xA4,0xA4,0xE4,0x04,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x40,0x40,0x7F,0x4A,0x4A,0x4A,0x4A,0x4A,0x7F,0x40,0x40,0x00},/*"值",56*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x7F,0x04,0x08,0x08,0x04,0x07,0x08,0x04,0x00,0x00,0x00,0x00},/*"μ",57*/
/* (16 X 16 , 宋体 )*/

{0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x80,0x00},
{0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00},/*"开",58*/
/* (16 X 16 , 宋体 )*/

{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00},
{0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},/*"机",59*/
/* (16 X 16 , 宋体 )*/

{0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
{0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},/*"中",60*/
/* (16 X 16 , 宋体 )*/

/*********************************************************************************/
};


#endif
