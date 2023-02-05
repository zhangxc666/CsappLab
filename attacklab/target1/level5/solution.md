- 把%rsp里的栈指针地址放到%rdi
- 拿到bias的值放到%rsi
- 利用add xy，把栈指针地址和bias加起来放到%rax，再传到%rdi
- 调用touch3

##### 参考文档
- [CSAPP-Attacklab](https://zhuanlan.zhihu.com/p/60724948)