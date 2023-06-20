## 前置知识
### waitpid
```C
pid_t waitpid(pid_t pid, int *wstatus, int options);
```
#### 描述
1. waitpid函数用于等待改变状态的子进程，同时获得子进程改变的信息
2. 子进程状态改变有以下三种情况：子进程终止，子进程暂停和子进程接收到一个信号重新开始。
3. 执行此函数可以回收终止进程的资源避免僵尸进程
4. 返回对应改变状态的进程的PID，遇到错误返回-1
5. 若`WNOHANG`指定同时没有任何进程状态改变，将返回0
#### 使用
1. `pid`
    - pid<-1 等待当某个ID为-PID进程组中任何子进程
    - pid=-1 等待任何子进程
    - pid=0  等待同一个进程组中的任何子进程
    - pid>0  等待指定的子进程
2. `options`
    - `WNOHANG` 若子进程不存在直接立即返回
    - `WUNTRACED` 如果子进程暂停的的话也返回
    - `WCONTINUED` 如果子进程接受信号后重新开始，则返回
3. `wstatus`
    - `WIFEXITED(wstatus)` 若干子进程正常退出返回true（正常退出指调用exit函数或者从主函数退出）
    - `WEXITSTATUS(wstatus)` 返回子进程的退出状态，指exit(0),exit(1)中的状态
    - `WIFSIGNALED(wstatus)` 若子进程被中断，返回true
    - `WIFSTOPPED(wstatus)` 若子进程暂停，返回true
    - `WIFCONTINUED(wstatus)` 若子进程重新执行，返回true

### wait
```C
等价于waitpid(-1, &wstatus, 0);
```
#### 描述
1. 返回值同waitpid
### sigprocmask
```C
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```
#### 描述
1. 此函数是自定义用来改变信号阻塞mask
2. 如果`oidset!=NULL`，之前的mask将会存储至oldset中
3. 如果`set=NULL`，当前阻塞信号mask将不会改变，同时将修改oldset=mask。
4. 成功返回0，有错误返回-1，同时errno将会指明错误的原因。
5. 无法阻塞`SIGKILL`和`SIGSTOP`信号
6. 子进程可以继承父进程的信号阻塞mask
7. set和oldset置于NULL是允许的，但不会有任何作用
#### 使用
1. `how`
    - `SIG_BLOCK` 将当前信号结合**添加**至阻塞信号中
    - `SIG_UNBLOCK` 将当前信号集合从阻塞信号集合中**移除**
    - `SIG_SETMASK` 将阻塞信号集合**置为**当前的集合

### signal
```C
sighandler_t signal(int signum, sighandler_t handler);
```
#### 描述
1. 绑定信号处理函数，其中handler可以使`SIG_IGN`，`SIG_DFL`和自定义函数地址

### sigsuspend
```C++
int sigsuspend(const sigset_t *mask);
```
#### 描述
```C
// 等价于以下过程，前两步是原子操作
sigprocmask(SIG_BLOCK,&mask,&prev); // 解除对某个信号的阻塞
pause();
sigprocmask(SIG_SETMASK,&prev,NULL);// 还原至原来的阻塞状态
```
1. 该函数有效避免了死锁问题，限制了信号仅在pause调用之后才能进入，避免了死锁问题。

### sigemptyset 
### sigfillset
### sigaddset
### sigdelset