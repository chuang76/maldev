## Backdoor putty.exe with shellcode

In this lab, we are going to backdoor putty.exe with our shellcode. 

First, open putty.exe (32-bit application) in x32dbg. Go to the entry point, there are two push instructions at the beginning. 

![](https://raw.githubusercontent.com/chuang76/maldev/main/figure/6-1.PNG)

There are different methods of implanting own code: (1) code cave (spare space in a PE file), e.g. at the end of .text segment; (2) new section; (3) extending section. 

So scroll down and we can find some space is not occupied. 

![](https://raw.githubusercontent.com/chuang76/maldev/main/figure/6-2.PNG)

Modify the instruction at 0x454AD0 to make it jump to the code cave, which address is 0x45C961. 

![](https://raw.githubusercontent.com/chuang76/maldev/main/figure/6-3.png)

Before running our shellcode, we need to store the registers onto the stack. So modify the instructions at 0x45C961 and 0x45C962 as `pushad` and `pushfd`, which means push general registers and eflag registers onto the stack. Then copy our shellcode into the code cave (right click > Binary > Edit). 

![](https://raw.githubusercontent.com/chuang76/maldev/main/figure/6-4.png)

However, that's not enough. If we run this patched file, we'll find the file exits immediately after running shellcode, because we forget to go back to putty.exe. 

How to get rid of that? First, determine which call will launch our shellcode and which call instruction will call exit. We can find out the last call (at 0x45CA19) is the call to the exit. Our idea is to jump out of this part and jump back to the original codes of putty.exe. So modify the instruction at 0x45CA16 as `jmp 0x45CA27`, which is another empty space. Then do the reverse operations for pushad and pushfd: `popfd` and `popad` to restore the registers at 0x45CA27 and 0x45CA278. 

![](https://raw.githubusercontent.com/chuang76/maldev/main/figure/6-5.png)

Also, restore the overwritten instructions at 0x454AD0 and 0x454AD2 (the beginning of putty.exe). Then jump back to 0x454AD7, which will call putty.456BE4. 

```assembly
push 60 
push putty.477AB0
```

So now we can save the patched file as a new executable (right click > Patches > Select All > Patch File). Run the new executable, we can find out we successfully backdoor putty.exe with our shellcode. 

![](https://raw.githubusercontent.com/chuang76/maldev/main/figure/6-6.PNG)
