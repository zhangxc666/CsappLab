/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif
// 1
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y)
{
	return ~(~(x&~y)&(~(~x&y))); // 德摩根律推导
}
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void)
{
	return (1<<30)+(1<<30);  // 自然溢出
}
// 2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x)  // 考虑（x+1）==~x,再特判掉x=-1时的情况即可
{
    return !((~(x+1)^x)|!(x+1));
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x)
{
   int t=(170<<24)+(170<<16)+(170<<8)+170;  // 与上0xAAAAAAAA，再判断是否与0xAAAAAAAA相等，用异或可以判断==
	return !((x&t)^t);
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x)
{
	return (~x+1);
}
// 3
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x)// 把x与上0x30 和 0x0f，对两种情况分开考虑即可
{
   int t=(x|(~x))^(0xf);              
	return (!!(((x&(0x3f))+0x06)&(1<<4)))&(!(((x&t)>>4)^3));    // 前者右移4位判断当前值是否是3（用异或判断），后者用加上6判断是否产生进位即可
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z)
{
   // 以下是找0xffffffff的过程
   
   // int t=!(x^0);
   // int a=(t<<1)+t;
   // int b=(a<<2)+a;
   // int c=(b<<4)+b;
   // int d=(c<<8)+c;
   // int dd=(d<<16)+d;
   
   // 可用下面方法替代
   // 当x=0时,!x=1,故~(!x)+1=0xffffffff;
   // 当x!=0时,!x=0,故~(!x)+1=0x00000000;
   int dd=(~(!x))+1;
    return ((~dd)&(y))+(z&dd);
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y)  // 好难。。最后还是看网上答案了
{
   int signx = (x >> 31) & 1; //符号位，分两种情况讨论，当x异号和x同号
   int signy = (y >> 31) & 1; //当x，y同号时，此时进行减法不会溢出，直接比较y-x的符号位即可,-x <==> ~x+1
                              //当x，y异号时，此时进行减法会造成溢出。故直接进行符号位判断最好，当y为正时，x为负是此时结果一定为1，反之结果一定为0--->(signx&(!signy))
	return (signx&(!signy))|((!(signx^signy)&(!(((y+(~x+1))>>31)&1))));
}

// 4
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x)
{ 
   // 当x=0时，~x+1仍然为0,此时两者或结果的符号位为0，再进行加一即可
   // 讨论其他情况的为什么x|(~x+1)符号位为1
   // 容易发现，~x每次会改变符号位，故当+1不影响符号位时，此时结果始终为0|1=1
   // 考虑当加一影响符号位时，此时~x一定为0x (7F)/(FF) FF FF FF,反推对应x为 0x (10)/(00) 00 00 00
   // 当x为0x10000000时 ~x+1为0x10 00 00 00，此时或的结果始终为1
   // 当x为0x00000000时 ~x+1为0x00 00 00 00，此时x=0或上~x+1的符号位为0
   // 故通过讨论当且仅当x=0时，x|(~x+1)符号位为0,其他值为1
   // 原先是打算对或的结果加1，看其是否进位，如~(((((((~x+1)|x)>>31)&1)+1))>>1)&1;
   // 还有更简单的方法，因为异或值符号位仅有0,1，当其右移时是算数右移，故0>>31=0x00000000,加1后为1,1>>31=0xffffffff,加1后为0
	// return ~(((((((~x+1)|x)>>31)&1)+1))>>1)&1;
   return ((((~x+1)|x)>>31)+1);
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
/*
   大致题意如下：
   对一个positive number，你需要找到其最高的一位1（1——31，不包括符号位），记为n，加上符号位一共是n+1
   对一个negative number，你需要找到其最高的一位0（1——31，不包过符号位），记为n，加上符号位一共是n+1

   本题思路如下：
   先找出最高的0 or 1，这就要求我们进行适当的转化。当为负数时，对其取反，和正数一样找到最高的1即可
   此步操作可以总结为：
      int flag=x>>31; //获取符号位
      x= ((~flag) & x) | (flag & (~x)); // 当flag=0xff ff ff ff时，此时是x是负数，对其取反；当flag=0x 00 00 00 00时，此时x是整数，不修改它
   接着利用二分思想处理，通过查找查高16位是否有1，若有则继续对高16位进行二分，若无则对低16位进行二分，以此类推。。
      int bit_16=(!!(x>>16))<<4;// 当有高16位(x>>16)>0时，表示高16位有1，此时!!(x>>16)=1，故最终答案必然在高16位，即加上16=1<<4。
      x=x>>bit_16               // 当bit_16=16时，取高16位；当bit_16时，取低16位
      int bit_8=(!!(x>>8))<<3;  // 与bit_8同理
      x=x>>bit_8;
      int bit_4=(!!(x>>4))<<2;  
      x=x>>bit_4;
      int bit_2=(!!(x>>2))<<1;
      x=x>>bit_2;
      int bit_1=(!!(x>>1));
      x=x>>bit_1;
      int bit_0=x;
   参考https://www.zhihu.com/column/c_1480603406519238656整个过程：     
   设 x = 1101 1000 1100 0000 | 0000 0100 1000 0000负数取反
      y = 0010 0111 0011 1111 | 1111 1011 0111 1111
   高16位右移：y = 0000 0000 0000 0000 | 0010 0111 0011 1111
   规格化：y = 0000 0000 0000 0000 | 0000 0000 0000 0001
   bit_16 处理：bit_16 = y << 4 = 16  //说明 x 至少需要 16 位 
   x 右移：x = x >> bit_16 = 0000 0000 0000 0000 | 0010 0111 0011 1111 > 
   低 16 位中的高 8 位右移：y = 0000 0000 0000 0000 | 0000 0000 | 0010 0111 
   规格化：y = 0000 0000 0000 0000 | 0000 0000 | 0000 0001 
   bit_8 处理：bit_8 = y << 3 = 8 //说明 x 又需要 8 位 
   x 右移：x = x >> bit_8 = 0000 0000 0000 0000 | 0000 0000 | 0010 0111 > 
   低 8 位的高 4 位右移：y = 0000 0000 0000 0000 | 0000 0000 | 0000 | 0010 
   规格化：y = 0000 0000 0000 0000 | 0000 0000 | 0000 | 0001 
   bit_4 处理：bit_4 = y << 2 = 4 //说明 x 又需要 4 位 
   x 右移：x = x >> bit_4 = 0000 0000 0000 0000 | 0000 0000 | 0000 | 0010 > 
   低 4 位的高 2 位右移：y = 0000 0000 0000 0000 | 0000 0000 | 0000 | 00 | 00 
   规格化：y = 0000 0000 0000 0000 | 0000 0000 | 0000 | 00 | 00 
   bit_2 处理：bit_2 = y << 1 = 0 //说明 x 这 4 位中的 1 在低 2 位中  
   x 右移：x = x >> bit_2 = 0000 0000 0000 0000 | 0000 0000 | 0000 | 00 | 10 //不移动 > 
   低 2 位的高 1 位右移：y = 0000 0000 0000 0000 | 0000 0000 | 0000 | 00 | 0 | 1 
   规格化：y = 0000 0000 0000 0000 | 0000 0000 | 0000 | 00 | 0 | 1 
   bit_1 处理：bit_1 = y << 0 = 1 //说明 x 又需要1位  
   x 右移：x = x >> bit_1 = 0000 0000 0000 0000 | 0000 0000 | 0000 | 00 | 0 | 1 //不移动 > 
   bit_0 处理：bit_0 = x = 1 
   bit_8 + bit_4 + bit_2 + bit_1 + bit_0 + 1= 31 与正确答案符合！
*/
int howManyBits(int x) 
{ 
   int flag,bit16,bit8,bit4,bit2,bit1,bit0;
   flag=x>>31;
   x= ((~flag) & x) | (flag & (~x)); 
   bit16=(!!(x>>16))<<4;
   x=x>>bit16;               
   bit8=(!!(x>>8))<<3;  
   x=x>>bit8;
   bit4=(!!(x>>4))<<2;  
   x=x>>bit4;
   bit2=(!!(x>>2))<<1;
   x=x>>bit2;
   bit1=(!!(x>>1));
   x=x>>bit1;
   bit0=x;
	return bit0+bit1+bit2+bit4+bit8+bit16+1;
}
// float
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf)
{
	return 2;
}
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf)
{
	return 2;
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x)
{
	return 2;
}
