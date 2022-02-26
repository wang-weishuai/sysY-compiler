# sysY-compiler
This is a [sysY](https://pku-minic.github.io/online-doc/#/misc-app-ref/sysy-spec) to Risc-V compiler (based on C-language) which I implemented it myself. To get the online-document, you can click [here](https://pku-minic.github.io/online-doc/#/).

**Note:This document has been updated this year！But the basic implementation remains the same. You can use this complier to compile the sysY-language to RicsV-language.**

It is difficult to implement it in one step, so I split the process into three stages:

    sysY   -- eeyore
    eeyore -- tigger
    tigger -- riscV
![image](https://github.com/wang-weishuai/sysY-compiler/blob/main/img/img2.png)

This seems to complicate the task, but actually it greatly simplifies the logic of the code and we can code and debug easier than before.
![image](https://github.com/wang-weishuai/sysY-compiler/blob/main/img/compilerimg3.png)
