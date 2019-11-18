#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

void extract(int readfile, char *name);
void createnewfile(char *namebuf, int readfile, int amount);
int octtodec(int n);
