
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Creating the struct of each digit
typedef struct node {
    int data;
    struct node* previous;
    struct node* next;
} digitNode;

//Prototypes for some functions used later
digitNode* subtractTwoNumbers(digitNode *n1, digitNode *n2);
digitNode* subtractTwoPositiveNumbers(digitNode *n1, digitNode *n2);
int compareTwoNumbers (digitNode* head1, digitNode* head2);

//Function to create a new node
digitNode *createNode(int data) {
    //Memory allocation operation
    digitNode *node = (digitNode*)malloc(sizeof(digitNode));

    node->data = data;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

//Function to delete lists and free them from the memory
void deleteList(digitNode *head) {
    digitNode *p1 = head;
    digitNode *p2;

    while (p1->next != NULL) {
        p2 = p1->next;
        free(p1);
        p1 = p2;
    }
}

//Function to create a new empty list & to clear a list content
digitNode *makeEmpty(digitNode *head) {
    //If the list is not empty, make it empty
    if (head != NULL)
        deleteList(head);

    //Memory allocation operation
    head = (digitNode*)malloc(sizeof(digitNode));

    //Making sure that the list is not out of memory
    if (head == NULL)
        printf("Out of memory!\n");

    head->next = NULL;
    head->previous = NULL;

    return head;
}

//Function to check weather a list is empty or not
int IsEmpty(digitNode *head) {
    return (head->next == NULL && head->previous == NULL);
}

//Function to insert a new node with specified data at the first of the list
void insertFirst(digitNode *head, int data) {
    digitNode *temp;
    temp = createNode(data);

    if (IsEmpty(head)) {
        temp->previous = head;
        head->next = temp;
    } else {
        temp->next = head->next;
        temp->next->previous = temp;
        temp->previous = head;
        head->next = temp;
    }
}

//Function to find the last node of a list
digitNode *findLastNode(digitNode *head) {
    digitNode *p = head;

    while (p->next != NULL) {
        p = p->next;
    }

    return p;
}

//Function to insert a new node with specified data at the end of the list
void insertLast(digitNode* head, int data) {
    digitNode* temp = createNode(data);

    if (IsEmpty(head)) {
        temp->previous = head;
        head->next = temp;
    } else {
        digitNode* lastNode = findLastNode(head);
        temp->previous = lastNode;
        lastNode->next = temp;
    }
}

//Function to remove the zeros on left of a number
digitNode* removeZeros(digitNode* head) {
    digitNode* p = findLastNode(head);

    while (p != NULL && p->data == 0) {
        digitNode* temp = p;
        p = p->previous;
        p->next = NULL;
        free(temp);
    }
    return head;
}

//Function to print a number
void displayNumber(digitNode *head) {
    //If the number to print is NULL, like the result from dividing by zero
    if (head == NULL)
        return;

    //Making sure that the list isn't empty
    if(head->next !=NULL){
        //The case where the number is zero
        if (head->next->data == 0 && head->next->next == NULL) {
            printf("0");
            return;
        }

        head = removeZeros(head);

        //Print a negative sign if the number is negative
        if (head->data == -1){
            printf ("-");
        }

        digitNode *p = findLastNode(head);

        do{
            printf("%d", p->data);
            p = p->previous;
        }while(p->previous != NULL);

    } else
        printf("The list is empty!!\n\n");
}

//Function to print a number on a file
void displayNumberOnFile(digitNode *head, FILE* ptr) {
    //If the number to print is NULL, like the result from dividing by zero
    if (head == NULL)
        return;

    //Making sure that the list isn't empty
    if(head->next !=NULL){
        //The case where the number is zero
        if (head->next->data == 0 && head->next->next == NULL) {
            fprintf(ptr, "0");
            return;
        }

        head = removeZeros(head);

        //Print a negative sign if the number is negative
        if (head->data == -1){
            fprintf (ptr, "-");
        }

        digitNode *p = findLastNode(head);

        do{
            fprintf(ptr, "%d", p->data);
            p = p->previous;
        }while(p->previous != NULL);

    } else
        fprintf(ptr, "The list is empty!!\n\n");
}

//Function to convert a string of numbers to linked list of it's digits
digitNode* stringToList (char number[]) {
    int numberLength = strlen(number);

    digitNode* head = NULL;
    head = makeEmpty(head);

    int j = 0;

    //Assigning a negative sign in head node if the number is negative
    if (number[0] == '-') {
        head->data = -1;
        j = 1;

        //Assigning a positive sign in head node if the number is positive
    } else if (number[0] == '+'){
        head->data = 1;
        j = 1;
    } else {
        head->data = 1;
    }

    for (int i = j; i < numberLength; i++) {
        //Converting from characters to integers using ASCII code
        int digit = number[i] - '0';
        insertFirst(head, digit);
    }

    return head;
}

//Function to add two positive numbers (linked lists)
digitNode* addTwoPositiveNumbers (digitNode* n1, digitNode* n2) {
    digitNode* result = NULL;
    result = makeEmpty(result);
    result->data = 1;

    digitNode* p1 = n1->next;
    digitNode* p2 = n2->next;

    int sum = 0;
    int carry = 0;

    //Tracing each digit of each number
    while (p1 != NULL || p2 != NULL) {
        int a = 0, b = 0;

        //If digits of first number isn't over
        if (p1 != NULL) {
            a = p1->data;
        } else {
            a = 0;
        }
        //If digits of second number isn't over
        if (p2 != NULL) {
            b = p2->data;
        } else {
            b = 0;
        }

        sum = a + b + carry;
        carry = sum / 10;
        sum %= 10;

        //Inserting one digit to the final answer
        insertLast(result, sum);

        if (p1 != NULL) {
            p1 = p1->next;
        }
        if (p2 != NULL) {
            p2 = p2->next;
        }
    }

    //If there is a carry, insert it to the final answer
    if (carry > 0) {
        insertLast(result, carry);
    }

    return result;
}

//Function to add two numbers(linked lists) taking their signs into consideration
digitNode* addTwoNumbers(digitNode *n1, digitNode *n2) {
    digitNode* result = NULL;
    result = makeEmpty(result);

    //First number is positive
    if (n1->data == 1) {
        //Both numbers are positive
        if (n2->data == 1) {
            result = addTwoPositiveNumbers(n1, n2);

            //First number is positive, second is negative
        } else if (n2->data == -1) {
            n2->data = 1; //Changing the sign to positive after knowing the real sign

            //Checking if the numbers are equal but have opposite signs
            if (compareTwoNumbers(n1, n2) == 0) {
                insertFirst(result, 0);

                //Returning the original sign
                n2->data = -1;

                return result;
            }
            //The operation becomes a subtraction operation
            result = subtractTwoPositiveNumbers(n1, n2);
        }

        //First number is negative
    } else if (n1->data == -1) {

        //First number is negative, second is positive
        if (n2->data == 1) {
            n1->data = 1;   //Changing the sign to positive after knowing the real sign

            //Checking if the numbers are equal but have opposite signs
            if (compareTwoNumbers (n1, n2) == 0) {
                insertFirst(result, 0);

                //Returning the original sign
                n1->data = -1;

                return result;
            }
            //The operation becomes a subtraction operation
            result = subtractTwoNumbers(n2, n1);

            //Both numbers are negative
        } else if (n2->data == -1) {
            result = addTwoPositiveNumbers(n1, n2);

            //Assigning a negative sign to the final result
            result->data = -1;
        }
    }

    return result;
}

//Function to count the digits of a number
//(counts the nodes in a list)
int countDigits(digitNode* head) {
    int count = 0;

    //Removing leading zeros on the left
    head = removeZeros(head);
    digitNode *p = head;

    while (p->next != NULL) {
        count++;
        p = p->next;
    }

    return count;
}

//Function to do a comparison between two numbers
int compareTwoNumbers (digitNode* head1, digitNode* head2) {
    head1 = removeZeros(head1);
    head2 = removeZeros(head2);

    int digits1 = countDigits(head1);
    int digits2 = countDigits(head2);

    //If one is positive and one is negative
    if (head1->data == 1 && head2->data == -1){
        return 1;
    } else if (head1->data == -1 && head2->data == 1){
        return -1;
    }

    //Cases in which the numbers vary in number of digits
    if (head1->data == 1 && head2->data == 1 && digits1 > digits2) {
        return 1;
    } else if (head1->data == 1 && head2->data == 1 && digits1 < digits2) {
        return -1;
    } else if (head1->data == -1 && head2->data == -1 && digits1 > digits2) {
        return -1;
    } else if (head1->data == -1 && head2->data == -1 && digits1 < digits2) {
        return 1;
    }

    digitNode *p1 = findLastNode(head1);
    digitNode *p2 = findLastNode(head2);

    //Comparing digit by digit in case both numbers have the same sign and the same number of digits
    if (head1->data == head2->data) {
        while (p1 != NULL && p2 != NULL) {
            if (p1->data > p2->data) {
                return 1;
            } else if (p1->data < p2->data) {
                return -1;
            }
            p1 = p1->previous;
            p2 = p2->previous;
        }

    }

    //If numbers are equal the function returns a zero
    return 0;
}

//Function to subtract two positive numbers(linked lists)
//Second number is smaller than first one
digitNode* subtractTwoPositiveNumbers(digitNode *n1, digitNode *n2) {
    digitNode* result = NULL;
    result = makeEmpty(result);

    digitNode *p1 = n1->next;
    digitNode *p2 = n2->next;

    int diff = 0;
    int borrow = 0;

    //Tracing each digit of each number
    while (p1 != NULL || p2 != NULL) {
        int a = 0, b = 0;

        //If digits of first number are't over
        if (p1 != NULL) {
            a = p1->data;
        } else {
            a = 0;
        }

        //If digits of second number are't over
        if (p2 != NULL) {
            b = p2->data;
        } else {
            b = 0;
        }

        diff = a - b - borrow;

        //The borrowing process in case the difference is negative
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        //Inserting one digit to the final answer
        insertLast(result, diff);

        if (p1 != NULL) {
            p1 = p1->next;
        }
        if (p2 != NULL) {
            p2 = p2->next;
        }
    }

    return result;
}

//Function to subtract two numbers(linked lists) taking their signs into consideration
digitNode* subtractTwoNumbers(digitNode* n1, digitNode* n2) {
    digitNode* result = NULL;
    result = makeEmpty(result);

    //Checking if the numbers are equal
    if (compareTwoNumbers(n1, n2) == 0) {
        insertFirst(result, 0);
        return result;
    }

    //Both numbers are positive
    if (n1->data == 1 && n2->data == 1) {
        //First number is larger than second
        if (compareTwoNumbers(n1, n2) == 1) {
            result = subtractTwoPositiveNumbers(n1, n2); //The operation becomes n1-n2
            result->data = 1;

            //First number is smaller than second
        } else if (compareTwoNumbers(n1, n2) == -1) {
            result = subtractTwoPositiveNumbers(n2, n1); //The operation becomes -(n2-n1)
            result->data = -1;    //Assigning a negative sign to the result

            //The numbers are equal
        } else if (compareTwoNumbers(n1, n2) == 0){
            result->data = 1;
            insertFirst(result, 0);
        }

        //First number is positive, second is negative
    } else if (n1->data == 1 && n2->data == -1) {
        result = addTwoPositiveNumbers(n1, n2); //The operation becomes n1+n2
        result->data = 1;

        //First number is negative, second is positive
    } else if (n1->data == -1 && n2->data == 1) {
        result = addTwoPositiveNumbers(n2, n1); //The operation becomes -(n1+n2)
        result->data = -1; //Assigning a negative sign to the result

        //Both numbers are negative
    } else if (n1->data == -1 && n2->data == -1) {
        //First number is larger than second
        if (compareTwoNumbers(n1, n2) == 1) {
            result = subtractTwoPositiveNumbers (n1, n2); //The operation becomes n1-n2
            result->data = -1;  //Assigning a negative sign to the result

            //First number is smaller than second
        } else if (compareTwoNumbers(n1, n2) == -1) {
            result = subtractTwoPositiveNumbers (n2, n1); //The operation becomes n2-n1
            result->data = 1;  //Assigning a positive sign to the result
        }
    }

    return result;
}

//Function to multiply two numbers
digitNode* multiplyTwoNumbers(digitNode *n1, digitNode *n2) {
    digitNode *result = NULL;
    result = makeEmpty(result);

    int mul = 0;
    int carry = 0;

    //The number of zeros to be inserted for the number to be summed to the final result
    int numberOfZeros = 0;

    digitNode* p1 = NULL;
    digitNode* p2 = n2->next;

    //If digits of second number aren't over
    while (p2 != NULL) {
        digitNode* multiplication = NULL;
        multiplication = makeEmpty(multiplication);

        p1 = n1->next;

        //If digits of first number are't over
        while (p1 != NULL) {
            int a = p1->data;
            int b = p2->data;

            mul = (a * b) + carry;
            carry = mul / 10;
            mul %= 10;

            //Insertion one digit to the final result
            insertLast(multiplication, mul);
            p1 = p1->next;
        }

        //Insertion carry, if any
        if (carry > 0) {
            insertLast(multiplication, carry);
            carry = 0;
        }

        //Insertion zeros to the number to be summed to the final result
        for (int i = 1; i <= numberOfZeros; i++) {
            insertFirst(multiplication, 0);
        }

        //Adding the multiplication of the first number by each digit of the second to the final result
        result = addTwoPositiveNumbers(result, multiplication);

        //Increment the number of zeros to be added in each loop
        numberOfZeros++;
        p2 = p2->next;
    }

    //Assigning the sign of the final result according to the similarity or difference of the signs of the multiplied numbers
    if (n1->data == n2->data){
        result->data = 1;
    } else {
        result->data = -1;
    }

    return result;
}

void insertlastToPointer(digitNode** head, int data) {
    digitNode * temp = createNode(data);

    if (IsEmpty(&head)) {
        temp->previous = *head;
        (*head)->next = temp;
    } else {
        digitNode * lastNode = findLastNode(&head);

        lastNode->next = temp;
        temp->previous = lastNode;
    }
}

//Function to copy a list content into another list
digitNode* copyList(digitNode* head) {
    digitNode* copiedList = NULL;
    copiedList = makeEmpty(copiedList);
    copiedList->data = head->data;

    digitNode* p = head->next;

    while (p != NULL) {
        insertLast(copiedList, p->data);
        p = p->next;
    }

    return copiedList;
}

//Function to divide two numbers(linked lists)
digitNode* divideTwoNumbers (digitNode* dividend, digitNode* divisor, digitNode* remainder) {

    //If the divisor is zero
    if (divisor->next->data == 0 && divisor->next->next == NULL) {
        printf("The value of divisor is ZERO, the quotient is infinite..\n\n");
        return NULL;
    }

    digitNode* quotient = NULL;
    quotient = makeEmpty(quotient);
    quotient->data = 1;

    remainder = NULL;
    remainder = makeEmpty(remainder);
    remainder->data = 1;

    //The case in which divisor is larger than dividend
    if (compareTwoNumbers(dividend, divisor) == -1) {
        digitNode *p = divisor->next;

        //Insertion to remainder
        while (p != NULL) {
            insertLast(remainder, p->data);
            p = p->next;
        }

        insertFirst(quotient, 0);
        return quotient;
    }

    int dividendDigits = countDigits(dividend);
    int divisorDigits = countDigits(divisor);

    digitNode* currentDividend = NULL;
    currentDividend = makeEmpty(currentDividend);
    currentDividend->data = 1;

    digitNode* numberToSubtract = NULL;
    numberToSubtract = makeEmpty(numberToSubtract);
    numberToSubtract->data = 1;

    digitNode* numberToMultiply = NULL;
    numberToMultiply = makeEmpty(numberToMultiply);
    numberToMultiply->data = 1;

    digitNode* numberToDoRepeatedSubtraction = NULL;
    numberToDoRepeatedSubtraction = makeEmpty(numberToDoRepeatedSubtraction);
    insertFirst(numberToMultiply, 1);

    int currentQuotient;

    digitNode* p = findLastNode(dividend);

    for (int i = 0; i < divisorDigits; i++) {
        insertFirst(currentDividend, p->data);
        p = p->previous;
    }


    while(p != NULL) {
        //printf("loooopp");
        if (compareTwoNumbers(currentDividend, divisor) == -1) {
            insertFirst(currentDividend, p->data);
        }

        currentQuotient = 0;


        numberToDoRepeatedSubtraction = copyList(currentDividend);
        while (compareTwoNumbers(numberToDoRepeatedSubtraction, divisor) == 1) {
            numberToDoRepeatedSubtraction = subtractTwoNumbers(numberToDoRepeatedSubtraction, divisor);
            currentQuotient++;
            //printf("current q = %d\n", currentQuotient);
            numberToDoRepeatedSubtraction->data = 1;
        }
        numberToDoRepeatedSubtraction = makeEmpty(numberToDoRepeatedSubtraction);

        insertFirst(quotient, currentQuotient);
        insertFirst(numberToMultiply, currentQuotient);
        numberToSubtract = multiplyTwoNumbers(numberToMultiply, divisor);
        numberToMultiply = makeEmpty(numberToMultiply);

        currentDividend = subtractTwoPositiveNumbers(currentDividend, numberToSubtract);
        currentDividend->data = 1;
        divisor->data = 1;
        /*printf("\ncirrdiv = ");
        displayNumber(currentDividend);
        printf("\ndivisot = ");
        displayNumber(divisor);*/

        p = p->previous;

        //displayNumber(currentDividend);
    }

     //Assigning the sign of the final result according to the similarity or difference of the signs of the multiplied numbers
    if (dividend->data == divisor->data){
        quotient->data = 1;
        remainder->data = 1;
    } else {
        quotient->data = -1;
        remainder->data = -1;
    }

    return quotient;
}

void printMenu() {
    printf ("\n\nPlease choose one of these options: \n");
    printf ("1- Display all numbers.\n");
    printf ("2- Do an operation between two numbers.\n");
    printf ("3- Exit program.\n\n");
}

void printOperations() {
    printf ("\nPlease choose an operation:\n");
    printf ("1- Addition.\n2- Subtraction.\n3- Multiplication.\n4- Division.\n\n");
}

int main()
{
    //Declaring input and output files
    FILE *f1_ptr, *f2_ptr;
    f1_ptr = fopen("numbers.txt", "r");
    f2_ptr = fopen("output.txt", "w");

    //2D array of strings to store numbers from the input file
    char numbers[500][1000];

    //Reading numbers from the input file
    int index = 0;
    int numberOfNumbers = 0;
    while (fscanf(f1_ptr, "%s", numbers[index++]) != EOF) {
        numberOfNumbers++; //The size of numbers array(array of lists)
    }

    //Converting each string to a linked list of it's digits and store them in array
    digitNode* numbersArray[numberOfNumbers];
    for (int i = 0; i < numberOfNumbers; i++) {
        numbersArray[i] = stringToList(numbers[i]);
    }

    printf("Welcome to the large integers calculator program...\n");

    int menuChoice = 0;

    do {
        printMenu();

        scanf ("%d", &menuChoice);

        switch(menuChoice){
            case 1:
                printf("\nNumbers in the file are:\n");
                for (int i = 0; i < numberOfNumbers; i++) {
                    printf("%d) %s\n", i+1, numbers[i]);
                }
                break;

            case 2:
                printf("\nEnter the order of two numbers from the list:\n");
                //Printing numbers to user to choose two from them
                for (int i = 0; i < numberOfNumbers; i++) {
                    printf("%d) %s\n", i+1, numbers[i]);
                }

                int numbersChoice1 = 0;
                int numbersChoice2 = 0;

                scanf("%d%d", &numbersChoice1, &numbersChoice2);

                if (numbersChoice1 < 1 || numbersChoice2 < 1 || numbersChoice1 > numberOfNumbers || numbersChoice1 > numberOfNumbers) {
                    printf ("\nOne or more of the entered options do not exist!\n");
                    break;
                }

                printOperations();

                int operationChoice = 0;
                scanf("%d", &operationChoice);

                //Declaring the result list
                digitNode* result = NULL;
                result = makeEmpty(result);

                //Declaring the remainder (in division) list
                digitNode* remainder = NULL;
                remainder = makeEmpty(remainder);

                switch(operationChoice) {
                    case 1:
                        result = addTwoNumbers(numbersArray[numbersChoice1-1], numbersArray[numbersChoice2-1]);

                        //Printing the operation and the result in the output file
                        displayNumberOnFile(numbersArray[numbersChoice1-1], f2_ptr);
                        fprintf(f2_ptr, " + ");
                        displayNumberOnFile(numbersArray[numbersChoice2-1], f2_ptr);
                        fprintf(f2_ptr, " = ");
                        displayNumberOnFile(result, f2_ptr);
                        fprintf(f2_ptr, "\n");

                        //Printing the operation and the result in the output screen
                        displayNumber(numbersArray[numbersChoice1-1]);
                        printf(" + ");
                        displayNumber(numbersArray[numbersChoice2-1]);
                        printf(" = ");
                        displayNumber(result);

                        break;

                    case 2:
                        result = subtractTwoNumbers(numbersArray[numbersChoice1-1], numbersArray[numbersChoice2-1]);

                        //Printing the operation and the result in the output file
                        displayNumberOnFile(numbersArray[numbersChoice1-1], f2_ptr);
                        fprintf(f2_ptr, " - ");
                        displayNumberOnFile(numbersArray[numbersChoice2-1], f2_ptr);
                        fprintf(f2_ptr, " = ");
                        displayNumberOnFile(result, f2_ptr);
                        fprintf(f2_ptr, "\n");

                        //Printing the operation and the result in the output screen
                        displayNumber(numbersArray[numbersChoice1-1]);
                        printf(" - ");
                        displayNumber(numbersArray[numbersChoice2-1]);
                        printf(" = ");
                        displayNumber(result);

                        break;

                    case 3:
                        result = multiplyTwoNumbers(numbersArray[numbersChoice1-1], numbersArray[numbersChoice2-1]);

                        //Printing the operation and the result in the output file
                        displayNumberOnFile(numbersArray[numbersChoice1-1], f2_ptr);
                        fprintf(f2_ptr, " * ");
                        displayNumberOnFile(numbersArray[numbersChoice2-1], f2_ptr);
                        fprintf(f2_ptr, " = ");
                        displayNumberOnFile(result, f2_ptr);
                        fprintf(f2_ptr, "\n");

                        //Printing the operation and the result in the output screen
                        displayNumber(numbersArray[numbersChoice1-1]);
                        printf(" * ");
                        displayNumber(numbersArray[numbersChoice2-1]);
                        printf(" = ");
                        displayNumber(result);

                        break;

                    case 4:
                        result = divideTwoNumbers(numbersArray[numbersChoice1-1], numbersArray[numbersChoice2-1], &remainder);

                        //Printing the operation and the result in the output file
                        displayNumberOnFile(numbersArray[numbersChoice1-1], f2_ptr);
                        fprintf(f2_ptr, " / ");
                        displayNumberOnFile(numbersArray[numbersChoice2-1], f2_ptr);
                        fprintf(f2_ptr, " = ");
                        displayNumberOnFile(result, f2_ptr);
                        fprintf (f2_ptr, "\nReminder = ");
                        displayNumberOnFile(remainder, f2_ptr);
                        fprintf(f2_ptr, "\n");

                        //Printing the operation in the output file
                        displayNumber(numbersArray[numbersChoice1-1]);
                        printf(" / ");
                        displayNumber(numbersArray[numbersChoice2-1]);
                        printf(" = ");
                        displayNumber(result);
                        printf ("\nReminder = ");
                        displayNumber(remainder);

                        break;

                    default:
                        printf("\nInvalid option!\n");
                        break;
                }
                break;

            case 3:
                printf ("\n\nThank you for using this program...\n\n");
                break;
        }

    } while (menuChoice >= 1 && menuChoice < 3);

    //Deleting all stored lists from memory
    for (int i = 0; i < numberOfNumbers; i++){
        deleteList(numbersArray[i]);
    }

    return 0;
}