#include<stdio.h>
#define TABSTOP 8

void detab()
{

    int ch_count = 0;
    int ch = getchar();
    int mod = 16%8;
    printf("The mod of 8 and 8 is %d \n", mod);

    while (ch != EOF) /*while not end of file*/
    {
        ch_count++; /*increment the char count*/

        if (ch == '\t') /* if current char is tab, add spaces until tabstop*/
        {
            while ((ch_count % TABSTOP) != 0){
                putchar(' ');
                ch_count++;
            }
        }

        else if (ch == '\n' || ch == '\r') /*if current char is newline, reset count to 0*/
        {
            ch_count = 0;
            putchar(ch);
        }

        else if (ch == '\b') /*if current char is backspace, decrement count*/
        {
            if (ch_count > 0){

            ch_count--;
            putchar(ch);

            }
        }
 
        else /*print all normal characters*/
        {
            putchar(ch);
        }

        ch = getchar(); /*get next char in file*/
    }
}

int main () 
{
    detab();

    return 0;
}
