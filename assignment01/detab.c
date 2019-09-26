#include<stdio.h>
#define TABSTOP 8
#define TRUE 1
#define FALSE 0

void detab()
{

    int ch_count = 0;
    int ch = getchar();
    int beginning_of_line = TRUE;

    while (ch != EOF) /*while not end of file*/
    {
        ch_count++; /*increment the char count*/

        if (ch == '\t') /* if current char is tab, add spaces until tabstop*/
        {
            beginning_of_line = FALSE;
            for (;ch_count <= TABSTOP; ch_count++)
            {
                putchar(' ');
            }
            ch_count = 0;

        }

        else if (ch == '\n' || ch == '\r') /*if current char is newline, reset count to 0*/
        {
            ch_count = 0;
            putchar(ch);
            beginning_of_line = TRUE;
        }

        else if (ch == '\b') /*if current char is backspace, decrement count*/
        {
            if (beginning_of_line == FALSE){

            ch_count--;
            putchar(ch);

            }
        }
 
        else /*print all normal characters*/
        {
            beginning_of_line = FALSE;
            putchar(ch);
        }

        ch = getchar(); /*get next char in file*/

        if (ch_count == TABSTOP)
            ch_count = 0;
    }
}

int main () 
{
    detab();

    return 0;
}
