# 解决思路
## Part I 模拟
- 思路很简单，学过计网，再了解点http即可，把Tiny server读懂（200行很简单，主要学习socket和Rio等包的用法）
## Part II 预加载线程
- 采用csapp书中的预加载线程，类似生产者消费者，线程从主线程中不断获取socket描述符，进行操作，难度不大，在Part I的部分简单加几行即可
## Part III Cache
- 设置了一个Cache，其中设置了读锁和写锁，每次对Cache进行操作时，需首先访问锁的权限，查找Cache是否存在查找的域名时是读，对连接插入删除时是写。
- 当client ===> proxy，首先查找当前是否cache是否存在，若存在直接返回；若不存在，进行下面的步骤
- proxy ===> server和server ===> proxy,访问server，获取server的响应，持续读取。
    1. 若当前response的size超过MAX_OBJECT_SIZE，则结束后，将当前malloc的资源释放，此结点不插入至cache中。
    2. 若合法，再判断当前的cache的size，若没满，则进行lru算法删除结点，直至cache有足够的的容量满足新的结点后，再将新的结点插入cache中。

# 使用方法：
- 先运行tiny server在15555端口，进入tiny文件夹后，运行`./tiny 15555`
- 再运行proxy在12345端口，`./proxy 12345`
- 最后运行curl，通过proxy访问server。`curl -v --proxy http://localhost:12345 http://localhost:15555/csapp.h`

---
唯一一个lab基本没怎么参考别人的解法，靠着之前写过分布式缓存的思路头铁着写下去了，还好码力还行，码起来不是很费劲。
