#include <stdio.h>
#include <stdlib.h>
#include "LongInt.h"

/* Digit in a Number in a certain position */
struct Digit {
    struct Digit *prevDigit;
    short data;
    struct Digit *nextDigit;
};

struct Number {
    struct Digit *first, *last, *curDigit;
    short sign;
};

int operationalSwitch; // Switching between addition and subtraction
int overflow;
int mulCarry = 0;
int subModified;
int wasAddition;
int wasSubtraction;
int wasSwitched;

int mulDigits(int, int);
int getSign(LongInt);
int matchSign(LongInt, LongInt);
void switchPlaces (LongInt, LongInt);
int addDigits(int, int);
int subDigits(int, int);
void putValue (LongInt, LongInt);
void removeDigitFront (LongInt);
void addZeros(LongInt,LongInt, int);
/*
    Creates a new empty number
*/
LongInt newLongInt() {
    LongInt newNumber = (struct Number *) malloc(sizeof(struct Number));
    (*newNumber).curDigit = NULL;
    (*newNumber).first = NULL;
    (*newNumber).last = NULL;
    (*newNumber).sign = 1;
    operationalSwitch = 0;
    overflow = 0;
    subModified = 0;
    wasAddition = 0;
    wasSubtraction = 0;
    wasSwitched = 0;
    return newNumber;
}

/*
    Adds a new digit to the end of a number
*/
void putDigit(LongInt number, int newDigit) {
    if(newDigit > 9 || newDigit < 0) printf("Invalid digit value");
    else {
        (*number).curDigit = (struct Digit *) malloc(sizeof(struct Digit));
        if ((*number).last == NULL) {
            (*number).first = (*number).curDigit;
            (*number).last = (*number).curDigit;
            (*number).curDigit -> prevDigit = NULL;
            (*number).curDigit -> nextDigit = NULL;
        } else {
            (*number).last -> nextDigit = (*number).curDigit;
            (*number).curDigit -> prevDigit = (*number).last;
            (*number).curDigit -> nextDigit = NULL;
            (*number).last = (*number).curDigit;
        }
        (*number).curDigit -> data = newDigit;
    }
}

/*
    Adds a new digit to the front of a number
*/
void putDigitFront(LongInt number, int newDigit) {
    if(newDigit > 9 || newDigit < 0) printf("Invalid digit value");
    else {
        if((*number).last == NULL) putDigit(number, newDigit);
        else{
                (*number).curDigit = (struct Digit *) malloc(sizeof(struct Digit));
                if ((*number).first == NULL) {
                    (*number).first = (*number).curDigit;
                    (*number).last = (*number).curDigit;
                    (*number).curDigit -> prevDigit = NULL;
                    (*number).curDigit -> nextDigit = NULL;
                } else {
                    (*number).first -> prevDigit = (*number).curDigit;
                    (*number).curDigit -> prevDigit = NULL;
                    (*number).curDigit -> nextDigit = (*number).first;
                    (*number).first -> prevDigit = (*number).curDigit;
                    (*number).first = (*number).curDigit;
                }
                (*number).curDigit -> data = newDigit;
            }
        }
}

/*
    Removes the last digit from the end of a number
*/
void removeDigit(LongInt number) {
    (*number).curDigit = (*number).last;
    if ((*number).first != (*number).last) {
        (*number).last = (*number).curDigit -> prevDigit;
        (*number).curDigit -> prevDigit -> nextDigit = NULL;
    } else {
        (*number).first = NULL;
        (*number).last = NULL;
    }
    free((*number).curDigit);
}

/*
    Removes the first digit from the number
*/
void removeDigitFront(LongInt number) {
    (*number).curDigit = (*number).first;
    if ((*number).first != (*number).last) {
        (*number).first = (*number).curDigit -> nextDigit;
        (*number).curDigit -> nextDigit = NULL;
        (*number).curDigit -> prevDigit = NULL;
    } else {
        (*number).first = NULL;
        (*number).last = NULL;
    }
    free((*number).curDigit);
}
/*
    Destroys the number, freeing the memory allocated to it
*/
void destroyNumber(LongInt number) {
    while ((*number).first != NULL)
    {
        removeDigit(number);
    }
}

/*
    Creates a new number and sets a value to it by a given integer
*/
void setValueInt(LongInt number, int value) {
    destroyNumber(number);
    if (value < 0) {
        (*number).sign = 0;
        value = -value;
    }
    else (*number).sign = 1;

    if (value == 0) putDigit(number, 0);
    else {
        while (value != 0)
        {
            putDigitFront(number, value % 10);
            value = value / 10;
        }
    }
}

/*
    Creates a new number and sets a value to it by a given char sequence
*/
void setValue(LongInt number, char *value) {
    destroyNumber(number);
    if (*value != '\0') {
        if (*value == '-') {
            (*number).sign = 0;
            value++;
        } else if (*value == '+') {
            (*number).sign = 1;
            value++;
        }  else (*number).sign = 1;
        while (*value != '\0')
        {
            putDigit(number, *value - '0');
            value++;
        }
    }
}

/*
    Counts and returns the length of the number
*/
int length(LongInt number) {
    if((*number).last == NULL) return 0;
    else {
        int counter = 1;
        (*number).curDigit = (*number).first;
        while ((*number).curDigit != (*number).last)
        {
            (*number).curDigit = (*number).curDigit -> nextDigit;
            counter++;
        }
        return counter;
    }
}
/*
    Switched two numbers places in the allocated memory
*/
void switchPlaces (LongInt number1, LongInt number2)
{
    struct Number forSafeKeeping = *number1;
    *number1 = *number2;
    *number2 = forSafeKeeping;
}

/*
    Adds two numbers, saves result in third parameter
*/
void addition(LongInt number1, LongInt number2, LongInt number3) {
    /* Checks if signs of number of different - that it's subtraction */
    if(operationalSwitch == 0) {
        if(matchSign(number1, number2) == 0)
        {
            operationalSwitch = 1;
            (*number2).sign = -(*number2).sign;
            subtraction(number1, number2, number3);
            operationalSwitch = 0;
            wasSubtraction = 1;
        }
    }
        if (wasSubtraction != 1) {
        /* Adds by going through both numbers'same position from back to front */
        int i = 0;
        if((*number1).last != NULL && (*number2).last != NULL) {
            (*number3).sign = (*number1).sign;
            (*number1).curDigit = (*number1).last;
            (*number2).curDigit = (*number2).last;
            int length1 = length(number1);
            int length2 = length(number2);
            destroyNumber(number3);
            while(i < length1 || i < length2) // Till both numbers are walked through
            {
                /* Adds digits of both numbers in a position, saves result in first number's position */
                if((*number2).curDigit == NULL){ // if number2 has finished, but number1 still continues (is longer)
                    putDigitFront(number3, addDigits((*number1).curDigit -> data, 0));
                } else if ((*number1).curDigit == NULL) { // if number1 has finished, but number2 still continues (is longer)
                    putDigitFront(number3, addDigits(0, (*number2).curDigit -> data));
                } else {
                    putDigitFront(number3, addDigits((*number1).curDigit -> data, (*number2).curDigit -> data));
                }
                /* Addition in this position is done; moves to the next position */
                if((*number1).curDigit != NULL) { // if it's already the last position of a number, there is no use to move further
                    if((*number1).curDigit -> prevDigit == NULL) (*number1).curDigit = NULL;
                    else (*number1).curDigit = (*number1).curDigit -> prevDigit;
                }
                if((*number2).curDigit != NULL) { // same here
                    if((*number2).curDigit -> prevDigit == NULL) (*number2).curDigit = NULL;
                    else (*number2).curDigit = (*number2).curDigit -> prevDigit;
                }
                i++;
            }
            /* e.x. 99999 + 12 = 100011, thus the first "1" needs to be shows (it's an overflow from the last position) */
            if(overflow != 0) {
                putDigitFront(number3, 1);
                overflow = 0;
            }
        }
        }
        wasSubtraction = 0;
}

/*
    Subtracts two numbers, saves result in the third parameter
*/
void subtraction(LongInt number1, LongInt number2, LongInt number3) {
    if(operationalSwitch == 0) {
        if(matchSign(number1, number2) == 0)
        {
            operationalSwitch = 1;
            (*number2).sign = -(*number2).sign;
            addition(number1, number2, number3);
            operationalSwitch = 0;
            wasAddition = 1;
        }
    }
    if (wasAddition != 1) {
    /* Subs by going through both numbers'same position from back to front */
        int i = 0;
        int nullCounter = 0; // For all-null digits in number3 situations
        int whatDigit = 0;
        if((*number1).last != NULL && (*number2).last != NULL) {
            destroyNumber(number3);
            int length1 = length(number1);
            int length2 = length(number2);
            (*number3).sign = (*number1).sign;
            if(length2 > length1) {
                    switchPlaces(number1, number2);
                    wasSwitched = 1;
                    (*number3).sign = -(*number3).sign;
            } else if(length2 == length1) { // For situation like: 15 - 35
                (*number1).curDigit = (*number1).first;
                (*number2).curDigit = (*number2).first;
                while((*number2).curDigit -> data  >= (*number1).curDigit -> data && i == 0)
                {
                    if ((*number2).curDigit -> data  > (*number1).curDigit -> data)
                    {
                       switchPlaces(number1, number2);
                        wasSwitched = 1;
                        (*number3).sign = -(*number3).sign;
                        i++;
                    }
                    // For situation like : 1115 - 1118
                    if (i != 0) break;
                    if ((*number1).curDigit -> nextDigit == NULL) break;
                    (*number1).curDigit = (*number1).curDigit -> nextDigit;
                    (*number2).curDigit = (*number2).curDigit -> nextDigit;
                }
            }
            (*number1).curDigit = (*number1).last;
            (*number2).curDigit = (*number2).last;
            i = 0;
        while(i < length1 || i < length2) // Till both numbers are walked through
            {
                /* Subs digits of both numbers in a position, saves result in first number's position */
                if((*number2).curDigit == NULL){ // if number2 has finished, but number1 still continues (is longer)
                    whatDigit = subDigits((*number1).curDigit -> data, 0);
                    if (whatDigit == 0) nullCounter++;
                    putDigitFront(number3, whatDigit);

                } else {
                    whatDigit = subDigits((*number1).curDigit -> data, (*number2).curDigit -> data);
                    if (whatDigit == 0) nullCounter++;
                    putDigitFront(number3, whatDigit);
                }
                /* Subtraction in this position is done; moves to the next position */
                if((*number1).curDigit != NULL) { // if it's already the last position of a number, there is no use to move further
                    if((*number1).curDigit -> prevDigit == NULL) (*number1).curDigit = NULL;
                    else (*number1).curDigit = (*number1).curDigit -> prevDigit;
                }
                if((*number2).curDigit != NULL) { // same here
                    if((*number2).curDigit -> prevDigit == NULL) (*number2).curDigit = NULL;
                    else (*number2).curDigit = (*number2).curDigit -> prevDigit;
                }
                i++;
            }
        }
        /* For all-null digits in number3 situations */
        if (nullCounter == i)
        {
            destroyNumber(number3);
            (*number3).sign = 1;
            putDigit(number3, 0);
        }
        // For situation like: 99 - 90 = 9 (not 09)
        int length3 = length(number3);
        (*number3).curDigit = (*number3).first;
        if(length3 > 1)
        {
            while((*number3).curDigit -> data == 0)
            {
                (*number3).first = (*number3).curDigit -> nextDigit;
                (*number3).curDigit -> nextDigit -> prevDigit = NULL;
                (*number3).curDigit = (*number3).curDigit -> nextDigit;
                free((*number3).curDigit -> prevDigit);
            }
        }
        if (wasSwitched == 1)
        {
            switchPlaces(number1, number2);
            wasSwitched = 0;
        }
    }
    wasAddition = 0;
}

/*
    Return the sign of the number
*/
int getSign(LongInt number) {
    if ((*number).sign == 0) return 0;
    else if ((*number).sign == 1) return 1;
    else return -1; // In case of an error?
}

/*
    Return 1 - if signs match, 0 - if not
*/
int matchSign(LongInt number1, LongInt number2) {
    if(getSign(number1) == getSign(number2)) return 1;
    else return 0;
}

/*
    Adds two digits
*/
int addDigits(int firstDigit, int secondDigit) {
    int sum = firstDigit + secondDigit; // Adds two digits
    if (overflow != 0) // If there was an overflow in the sum from the last addition
    {
        sum += 1;
        overflow = 0;
    }
    if(sum > 9) // If there is an overflow in the current addition
    {
        overflow = 1;
        sum -= 10;
    }
    if(sum < 0) printf("Addition result error. ");
    return sum;
}

/*
    Subtracts two digits
*/
int subDigits(int firstDigit, int secondDigit) {
    if (subModified != 0)
    {
        firstDigit -= 1;
        subModified = 0;
    }
    if (firstDigit < secondDigit)
    {
        firstDigit += 10;
        subModified = 1;
    }
    int sub = firstDigit - secondDigit;
    return sub;
}

void printNumber(LongInt number) {
    if((*number).last != NULL) {
        (*number).curDigit = (*number).first;
        if((*number).sign == 0 || (*number).sign == -1) printf("-");
        while((*number).curDigit != NULL)
        {
            printf("%d", (*number).curDigit -> data);
            (*number).curDigit = (*number).curDigit -> nextDigit;
        }
    }
    printf("\n");
}

void multiplication(LongInt number1, LongInt number2, LongInt answer) {
    if (number1 != NULL && number2 != NULL) {
        destroyNumber(answer);
        if (((*number1).last->data == 0 && (*number1).first->data == 0)||((*number2).last->data == 0 && (*number2).first->data == 0)) {
            answer->sign = 1;
            putDigit(answer, 0);
            return;
        }

        if ( matchSign(number1, number2) == 1) answer->sign = 1;
        else answer->sign = 0;

        if (((*number1).last->data == 1 && (*number1).first->data == 1)) {
            putValue(answer, number2);
            return;
        }

          if (((*number2).last->data == 1 && (*number2).first->data == 1)) {
            putValue(answer, number1);
            return;
        }

        int length1 = length(number1);
        int length2 = length(number2);
        int i, j, k, mul; //i, j and k will be used for loops and mul for saving digit needed to put in the structure

        if (length1 > length2) { //This one to make that multiplier would always be bigger than multiplicand
            switchPlaces(number1, number2);  // in order to save some additions (addition number equal to digits of multiplicand)
            int temporary = length1;
            length1 = length2;
            length2 = temporary;
        }

        (*number1).curDigit = (*number1).last;
        (*number2).curDigit = (*number2).last;

        for (j = 0; j < length2; j++) {
            mul = mulDigits((*number1).curDigit->data, (*number2).curDigit->data);
            putDigitFront(answer, mul);
            (*number2).curDigit = (*number2).curDigit->prevDigit;
        }

        if (overflow == 1) {
            putDigitFront(answer, mulCarry);
            overflow = 0;
        }

        (*number1).curDigit = (*number1).curDigit->prevDigit;
        (*number2).curDigit = (*number2).last;
        length1--; //Because we multiplied one digit already and have answer in answer :)

        LongInt toSumUp = newLongInt(); //This will be useful when adding
        LongInt toSumUp2 = newLongInt();
        int leftShift = 1; // shifting counter

        for (i = 0; i < length1; i++) {
            for (j = 0; j < length2; j++) {
                mul = mulDigits((*number1).curDigit->data, (*number2).curDigit->data);
                putDigitFront(toSumUp, mul);
                (*number2).curDigit = (*number2).curDigit->prevDigit;
            }

                if (overflow == 1) {
                    putDigitFront(toSumUp, mulCarry);
                    overflow = 0;
                }
                    k = leftShift;
                    while (k > 0) {
                        putDigit(toSumUp, 0);
                        k--;
                    }
                    putValue(toSumUp2, answer);
                addition(toSumUp, toSumUp2, answer);
                destroyNumber(toSumUp);
                leftShift++;
                (*number1).curDigit = (*number1).curDigit->prevDigit;
                (*number2).curDigit = (*number2).last;
        }
        if ( matchSign(number1, number2) == 1) answer->sign = 1;
        else answer->sign = 0;
        destroyNumber(toSumUp);
        destroyNumber(toSumUp2);
    }
}

int mulDigits (int dig1, int dig2) {
    int multi;
    multi = dig1 * dig2;
    if (overflow == 1) multi+=mulCarry;
    if (multi > 9) {
        mulCarry = multi / 10;
        overflow = 1;
        return (multi % 10);
    }
    else {
        overflow = 0;
        return multi;
    }
}

void putValue (LongInt number1, LongInt number2) {
    destroyNumber(number1);
    (*number2).curDigit = (*number2).last;

    while ((*number2).curDigit != NULL) {
        putDigitFront(number1, (*number2).curDigit->data);
        (*number2).curDigit = (*number2).curDigit->prevDigit;
    }
}

void division(LongInt number1, LongInt number2, LongInt answer) {
    if ( number1 != NULL && number2 != NULL) {
            destroyNumber(answer);

        if (matchSign(number1, number2) == 1) answer->sign = 1;
        else answer->sign = 0; //identifying sign of the answer

        if ((*number1).last->data == 0 && (*number1).first->data == 0)  {
            // If dividend == 0 then answer = 0
            putDigit(answer, 0);
        return;
        }

         if ((*number2).last->data == 0 && (*number2).first->data == 0) {
            printf("ERROR: Division by zero!\n"); // if divisor == 0 then ERROR
        return;
        }

            if (isSmaller(number1, number2) == 1) {
            putDigit(answer, 0);   // if dividend < divisor then answer = 0
            return;
            }

            if (isSmaller(number1, number2) == 2) {
                putDigit(answer, 1); // if numbers are equal then answer = 1
                return;
            }

            int nullCounter = 0;

    // Removing zero's from both numbers endings to make division simplified
    (*number1).curDigit = (*number1).last;
    (*number2).curDigit = (*number2).last;
    while ( ( (*number1).curDigit -> data == 0 ) && ( (*number2).curDigit -> data == 0 ) ) {
        removeDigit(number1);
        removeDigit(number2);
        nullCounter++;
        (*number1).curDigit = (*number1).last;
        (*number2).curDigit = (*number2).last;
    }

            //Length of divisor
        int length2 = length(number2);

         if (((*number2).last->data == 1 && (*number2).first->data == 1) && (length2 == 1)) {
            putValue(answer, number1); // if division is by 1, answer is the same as first number
            addZeros(number1, number2, nullCounter);
        return;
        }

    (*number1).curDigit = (*number1).first; //setting current pointer to the first digit

    // New variables, which will be needed to compute division
    LongInt numerator = newLongInt();
    LongInt sumVariable = newLongInt();
    putDigit(sumVariable, 0);
    LongInt subVariable = newLongInt();
    LongInt difference = newLongInt();
    LongInt constAddend = newLongInt();
    LongInt Addend = newLongInt();
    putValue(constAddend, number2);
    putValue(Addend, number2);
    int i, k, zeroIndicator = 0;


    // DIVISION CALCULATING LOOP
while ((*number1).curDigit != NULL) {

    //  getting equal portion of digits from the dividend
    for (i = length(numerator); i < length2; i++) {
        putDigit(numerator, (*number1).curDigit->data);
        (*number1).curDigit = (*number1).curDigit -> nextDigit;
    }

    if (zeroIndicator == 0) {
    //if it's divisor portion is still bigger, increment digits of dividend portion by 1
    if (isSmaller(numerator, number2) == 1) {
        putDigit(numerator, (*number1).curDigit->data);
        (*number1).curDigit = (*number1).curDigit -> nextDigit;
    }
}
     k = 0;
    // multiply divisor by 1-9 through the loop

    while ((isSmaller(sumVariable, numerator) != 0)&&(isSmaller(sumVariable, numerator) != 2)) {
        addition(Addend, constAddend, sumVariable);
        putValue(Addend, sumVariable);
        k++;
    }

    if (isSmaller(sumVariable,numerator) == 2) {
        destroyNumber(numerator);
         destroyNumber(sumVariable);
         putValue(Addend, number2);
        putDigit(sumVariable, 0);
        if (k > 0) k++;
        zeroIndicator = 1;
        putDigit(answer, k);
    }
     else {
    putValue(subVariable, sumVariable);
    subtraction(subVariable, constAddend, sumVariable);
    if (isSmaller(numerator, sumVariable) == 1) {
        putDigit(answer, 0);
        destroyNumber(sumVariable);
        putValue(Addend, number2);
        putDigit(sumVariable, 0);
    zeroIndicator = 0;
    } else {
    subtraction(numerator, sumVariable, difference);
    putValue(numerator, difference);
    putDigit(answer, k);
    destroyNumber(sumVariable);
    putValue(Addend, number2);
    putDigit(sumVariable, 0);
    zeroIndicator = 0;
        }
     }
  }
    destroyNumber(sumVariable);
    destroyNumber(subVariable);
    destroyNumber(Addend);
    destroyNumber(constAddend);
    destroyNumber(numerator);
    destroyNumber(difference);
    addZeros(number1, number2, nullCounter);
}
}

int isSmaller(LongInt number1, LongInt number2) {
      // PRIMARY COMPARE
        int len1 = length(number1);
        int len2 = length(number2);
    if (len1 < len2) return 1;
    else if (len1 > len2) return 0;
      // SECONDARY COMPARE
        (*number1).curDigit = (*number1).first;
        (*number2).curDigit = (*number2).first;
        int i;

        for( i = 0; i < len1; i++) {
            if ((*number1).curDigit->data > (*number2).curDigit->data) return 0;
            else if ((*number1).curDigit->data < (*number2).curDigit->data) return 1;
            else {
                (*number1).curDigit = (*number1).curDigit -> nextDigit;
                (*number2).curDigit = (*number2).curDigit -> nextDigit;
            }
        }
    return 2; // it means numbers are equal
}

void addZeros(LongInt number1 ,LongInt number2, int nullCounter) {
    int i;
    for (i = 0; i < nullCounter; i++) {
        putDigit(number1, 0);
        putDigit(number2, 0);
    }
}
