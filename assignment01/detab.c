#include<stdio.h>
#define TABSTOP 8

void detab()
{

    int ch_count = 0; /*counter for tabstop*/
    int ch = getchar();
    int position_of_line = 0; /*tracks position of each line*/

    while (ch != EOF) /*while not end of file*/
    {

        if (ch == '\t') /* if current char is tab, add spaces until tabstop*/
        {
            for (;ch_count < TABSTOP; ch_count++, position_of_line++)
            {
                putchar(' ');
            }
            ch_count = 0;

        }
        /*if current char is newline, reset count to 0*/
        else if (ch == '\n' || ch == '\r') 
        {
            ch_count = 0;
            putchar(ch);
            position_of_line = 0;
        }

        else if (ch == '\b') /*if current char is backspace, decrement count*/
        {
            if (position_of_line != 0)
            {
            position_of_line--;
            ch_count--;
            }
            
            putchar(ch);
            while (ch_count < 0)
                ch_count += TABSTOP; /*resets counter to valid position*/
        }
 
        else /*print all normal characters*/
        {
            position_of_line++;
            ch_count++;
            putchar(ch);
        }

        ch = getchar(); /*get next char in file*/

        if (ch_count == TABSTOP) /*if char counter equals tabstop reset it*/
            ch_count = 0;
    }
}

int main () 
{
    detab();

    return 0;
}
