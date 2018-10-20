#ifndef LONGINT_H
#define LONGINT_H

typedef struct Number *LongInt;

/*
 *   create a new empty number
 */
extern LongInt createNew();

/*
 *   put a new digit to the end of the number
 */
extern void putDigit(LongInt, int);

/*
 *   remove the last digit in a number
 */
extern void removeDigit(LongInt);

/*
 *   clear out the number and free the memory allocated to it
 */
extern void destroyNumber(LongInt);

/*
 *   set a value for a number by a given integer
 */
extern void setValueInt(LongInt, int);

/*
 *   set a value for a number by a given char string
 *   first position should be either "+" or "-" to indicate wheter it's positive or negative value
 *   zero is considered as positive value
 */
extern void setValue(LongInt, char *);

/*
 *  provides the length of a number
 *  RETURN VALUE: the number of digits in a number
 */
extern int length(LongInt);

/*
 * Adds first two numbers and puts the result into the third one
 */
extern void addition(LongInt, LongInt, LongInt);

/*
 * Subtracts first two numbers and puts the result into the third one
 */
extern void subtraction(LongInt, LongInt, LongInt);

/*
 * Multiplies first two numbers and puts the result into the third one
 */
extern void multiplication(LongInt, LongInt, LongInt);

/*
 * Divides first two numbers and puts the result into the third one
 */
extern void division(LongInt, LongInt, LongInt);

/*
 * Compares two numbers
 * RETURN VALUE: 1 - if first number is smaller, 0 - if first number is bigger, 2 - if numbers are equal
 */
extern int isSmaller(LongInt, LongInt);

/*
 * Prints the whole number to the screen
 */
extern void printNumber(LongInt);

#endif
