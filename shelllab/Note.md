## ǰ��֪ʶ
### waitpid
```C
pid_t waitpid(pid_t pid, int *wstatus, int options);
```
#### ����
1. waitpid�������ڵȴ��ı�״̬���ӽ��̣�ͬʱ����ӽ��̸ı����Ϣ
2. �ӽ���״̬�ı�����������������ӽ�����ֹ���ӽ�����ͣ���ӽ��̽��յ�һ���ź����¿�ʼ��
3. ִ�д˺������Ի�����ֹ���̵���Դ���⽩ʬ����
4. ���ض�Ӧ�ı�״̬�Ľ��̵�PID���������󷵻�-1
5. ��`WNOHANG`ָ��ͬʱû���κν���״̬�ı䣬������0
#### ʹ��
1. `pid`
    - pid<-1 �ȴ���ĳ��IDΪ-PID���������κ��ӽ���
    - pid=-1 �ȴ��κ��ӽ���
    - pid=0  �ȴ�ͬһ���������е��κ��ӽ���
    - pid>0  �ȴ�ָ�����ӽ���
2. `options`
    - `WNOHANG` ���ӽ��̲�����ֱ����������
    - `WUNTRACED` ����ӽ�����ͣ�ĵĻ�Ҳ����
    - `WCONTINUED` ����ӽ��̽����źź����¿�ʼ���򷵻�
3. `wstatus`
    - `WIFEXITED(wstatus)` �����ӽ��������˳�����true�������˳�ָ����exit�������ߴ��������˳���
    - `WEXITSTATUS(wstatus)` �����ӽ��̵��˳�״̬��ָexit(0),exit(1)�е�״̬
    - `WIFSIGNALED(wstatus)` ���ӽ��̱��жϣ�����true
    - `WIFSTOPPED(wstatus)` ���ӽ�����ͣ������true
    - `WIFCONTINUED(wstatus)` ���ӽ�������ִ�У�����true

### wait
```C
�ȼ���waitpid(-1, &wstatus, 0);
```
#### ����
1. ����ֵͬwaitpid
### sigprocmask
```C
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```
#### ����
1. �˺������Զ��������ı��ź�����mask
2. ���`oidset!=NULL`��֮ǰ��mask����洢��oldset��
3. ���`set=NULL`����ǰ�����ź�mask������ı䣬ͬʱ���޸�oldset=mask��
4. �ɹ�����0���д��󷵻�-1��ͬʱerrno����ָ�������ԭ��
5. �޷�����`SIGKILL`��`SIGSTOP`�ź�
6. �ӽ��̿��Լ̳и����̵��ź�����mask
7. set��oldset����NULL������ģ����������κ�����
#### ʹ��
1. `how`
    - `SIG_BLOCK` ����ǰ�źŽ��**���**�������ź���
    - `SIG_UNBLOCK` ����ǰ�źż��ϴ������źż�����**�Ƴ�**
    - `SIG_SETMASK` �������źż���**��Ϊ**��ǰ�ļ���

### signal
```C
sighandler_t signal(int signum, sighandler_t handler);
```
#### ����
1. ���źŴ�����������handler����ʹ`SIG_IGN`��`SIG_DFL`���Զ��庯����ַ

### sigsuspend
```C++
int sigsuspend(const sigset_t *mask);
```
#### ����
```C
// �ȼ������¹��̣�ǰ������ԭ�Ӳ���
sigprocmask(SIG_BLOCK,&mask,&prev); // �����ĳ���źŵ�����
pause();
sigprocmask(SIG_SETMASK,&prev,NULL);// ��ԭ��ԭ��������״̬
```
1. �ú�����Ч�������������⣬�������źŽ���pause����֮����ܽ��룬�������������⡣

### sigemptyset 
### sigfillset
### sigaddset
### sigdelset