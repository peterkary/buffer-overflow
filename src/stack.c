// File vulnerable to buffer overflow attack 
// It reads data from a file into a buffer without proper bounds checking (main)
// then passes that data to a function (bof) that uses strcpy to copy it to a smaller buffer, potentially overflowing it

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// bof function takes string str as an argument
int bof(char *str)
{
	char buffer[24];
	// using strcpy (no bounds checking)
	strcpy(buffer,str);
	return 1;
}

int main(int argc, char **argv)
{
	char str[517];
	FILE *badfile;
	badfile=fopen("badfile","r");
	// reads 517 bytes from badfile to local buffer str
	fread(str,sizeof(char),517,badfile); 
	// passing the potentially oversized input to bof function
	bof(str);
	printf("returned Properly\n");
	return 1;
}
