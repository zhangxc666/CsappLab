### Attack Lab
#### level4
- getval_280找到58序列，将cookie存入rax之中，返回地址为0x004019cc
- setval_426找到48 89 c7序列，将cookie再送至rdi之中，返回地址0x004019c5
- 再设置返回地址，返回到touch2即可 ，返回地址为0x004017ec