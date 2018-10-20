#include <stdio.h>
#include "LongInt.h"
#include "LongInt.c"

int main(void)
{
    /*
     *  Creating new empty number
     */
    LongInt number1 = newLongInt();

    /*
     *  Printing the number (boundary case, because the number is empty)
     */
    printNumber(number1);

    /*
     *  Putting new digit to the front (boundary case, because the number is empty)
     */
    putDigitFront(number1, 3);
    printNumber(number1);

    /*
     *  Putting new digit to the end (boundary case, because the number is empty)
     */
    putDigit(number1, 4);
    printNumber(number1);

    /*
     *  Putting another digit to the end
     */
    putDigit(number1, 5);
    printNumber(number1);

    /*
     *  Putting yet another digit to the end
     */
    putDigitFront(number1, 6);
    printNumber(number1);

    /*
     *  Removes the last digit
     */
    removeDigit(number1);
    printNumber(number1);

    /*
     *  Removes the last digit
     */
    removeDigit(number1);
    printNumber(number1);

    /*
     *  Removes the last digit
     */
    removeDigit(number1);
    printNumber(number1);

    /*
     *  Removes the last digit
     */
    removeDigit(number1);
    printNumber(number1);

    /*
     *  Putting invalid digit value (boundary case)
     */
    putDigitFront(number1, -1);
    printNumber(number1);

    /*
     *  Putting invalid digit value (boundary case)
     */
    putDigit(number1, 1);
    printNumber(number1);

    /*
     *  Deletes the whole number and frees the memory allocated to it
     */
    destroyNumber(number1);
    printNumber(number1);

    /*
     *  Sets a given integer value to a number
     */
    setValueInt(number1, -145);
    printNumber(number1);

    /*
     *  Creating new empty number
     */
    LongInt number2 = newLongInt();

    /*
     *  Sets a given string value to a number
     */
    setValue(number2, "-1587468196");
    printNumber(number2);

    /*
     *  Sets a given string value to a number
     */
    setValue(number2, "+158746158746819615874681961587468196158746819615874681961587468196158746819615874681961587468196158746819615874681968196");
    printNumber(number2);

    /*
     *  Sets a given string value to a number
     */
    setValue(number2, "0");
    printNumber(number2);

    /*
     *  Prints the amount of digits in a given number
     */
    printf("number1 length: %d\n", length(number1));
    printf("number2 length: %d\n", length(number2));

    LongInt number3 = newLongInt();
    setValue(number1, "49789");
    setValueInt(number2, 324);
    setValueInt(number3, 0);

    /*
     *  Adds two numbers, saves result into the third one
     */
    addition(number1, number2, number3);
    printNumber(number3);

    setValueInt(number1, 10);
    setValueInt(number2, 100);
    setValueInt(number3, 0);
    /*
     *  Adds two numbers, saves result into the third one
     */
	 
    subtraction(number1, number2, number3);
    printNumber(number3);

    setValue(number1, "158");
    setValue(number2, "159");
    /*
     *  Adds two numbers, saves result into the third one (boundary case)
     */
     printf("SUBTRACTION\n");
     printNumber(number1);
     printNumber(number2);
     printNumber(number3);
     subtraction(number1, number2, number3);
     printNumber(number1);
     printNumber(number2);
     printNumber(number3);

    printf("MULTIPLICATION TEST\n");

    LongInt num1 = newLongInt();
    setValue(num1, "-100");

    LongInt num2 = newLongInt();
    setValue(num2, "-10");


    LongInt mulResult = newLongInt();

    multiplication(num1, num2, mulResult);
    printf("Multiplication ANSWER value:\n");
    printNumber(mulResult);

    printf("DIVISION TEST\n");

    LongInt numb1 = newLongInt();
    setValue(numb1, "-99");
    LongInt numb2 = newLongInt();
    setValue(numb2, "100");
    LongInt divResult = newLongInt();

    printf("Division ANSWER value:\n");
    division(numb1, numb2, divResult);
    printNumber(divResult);

    int x;
    x = isSmaller(numb1, numb2);
    printf("Compare answer: %d\n", x);
	
	
    return 0;
}
