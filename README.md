# Simple Buffer Overflow
### **Warning:** Run this in a secure environment (VM)!

## Setup Instructions (Ubuntu 12.04)
- **Disable** address space layout randomization (**ASLR:** Technique that is used to increase the difficulty of performing a buffer overflow attack that requires the attacker to know the location of an executable in memory)
```
sudo sysctl -w kernel.randomize_va_space=0
```
```
sudo sysctl kernel.randomize_va_space=0 
```
### **shellcode.c (demo)**
Inside /src 
- Execute shellcode with user priviledges

```
gcc call_shellcode.c -o call_shellcode -z execstack
```
- -z execstack: can execute commands within stack
```
./call_shellcode
```
![image running call_shellcode (no root)](https://github.com/peterkary/buffer-overflow/blob/main/assets/images/pic1.png?raw=true)

- Execute shellcode with root priviledges
```
sudo chown root call_shellcode
```
```
sudo chmod 4755 call_shellcode
```
```
./call_shellcode
```
![image running call_shellcode (root)](https://github.com/peterkary/buffer-overflow/blob/main/assets/images/pic2.png?raw=true)

### **stack.c**
- Vulnerable file (compilation with root priviledges)
```
gcc stack.c -o stack -z execstack -fno-stack-protector
```
- -fno-stack-protector: use of stack guard for protecting stack overwrite
```
chmod 4755 stack
```

### **exploit.c**
- Creates 'badfile', fills it with NOP (0x90) instructions.
- 'badfile' should have shellcode and the address of shellcode


## Find the approximate shellcode address
- Run exploit to create the badfile
```
gcc exploit.c -o exploit
```
```
./exploit
```
- Compile stack.c with debuf flags activated
```
gcc stack.c -o stack_gdb -g -z execstack -fno-stack-protector
```
- Run stack_gdb in debug mode
- Set breakpoint to bof and run
![image running stack_gdb in debug mode](https://github.com/peterkary/buffer-overflow/blob/main/assets/images/pic6.png?raw=true)
- Print the addredss of buffer[] and the content of ebp register
![image finding addresses](https://github.com/peterkary/buffer-overflow/blob/main/assets/images/pic7.png?raw=true)

- The addresses distance is 0x20 bytes
- Since ASLR is off, the same addresses will be used everytime stack.c executes, so we change exploit.c accordingly
## Change return address of badfile
- Change the return address to 0xbffff1d0 (0x20 from the start of buffer[] + 0x04 bytes for Previous Frame Pointer + 0x04 bytes for Return Address) which means at least 0x28 bytes upper the buffer[] address
![image showing generated badfile](https://github.com/peterkary/buffer-overflow/blob/main/assets/images/pic8.png?raw=true)

- Finally, we execute stack, using badfile as input, so that we get root shell
![image running ./stack and gives root shell](https://github.com/peterkary/buffer-overflow/blob/main/assets/images/pic9.png?raw=true)
