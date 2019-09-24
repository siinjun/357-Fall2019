#include<stdio.h>
#define TABSTOP 8

int main () 
{
    int ch_count = 0;
    int ch = getchar();

    while (ch != EOF) /*while not end of file*/
    {
        ch_count++; /*increment the char count*/
        if (ch == '\t') /* if current char is tab, add spaces until tabstop*/
        {
            for (ch_count; ch_count <= TABSTOP; ch_count++)/*adds spaces for however much is left until a tabstop*/
            {
                putchar(' ');
            }
            ch_count = 0;
        }

        else if (ch == '\n' || ch == '\r') /*if current char is newline, reset count to 0*/
        {
            ch_count = 0;
            putchar(ch);
        }

        else if (ch == '\b') /*if current char is backspace, decrement count*/
        {
            ch_count--;
            putchar(ch);
        }
 
        else
        {
            putchar(ch);
        }
        ch = getchar(); /*get next char in file*/

        if (ch_count == TABSTOP)
        {
            ch_count = 0;
        }
    }
    
    return 0;
}
