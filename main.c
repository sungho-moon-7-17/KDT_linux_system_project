#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <assert.h>

#include <system_server.h>
#include <gui.h>
#include <input.h>
#include <web_server.h>
#include <toy_message.h>

#define NUM_MESSAGES 10

static mqd_t watchdog_queue;
static mqd_t monitor_queue;
static mqd_t disk_queue;
static mqd_t camera_queue;

static void
sigchldHandler(int sig)
{
    int status, savedErrno;
    pid_t childPid;

    savedErrno = errno;

    printf("handler: Caught SIGCHLD : %d\n", sig);

    while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("handler: Reaped child %ld - ", (long) childPid);
        (NULL, status);
    }

    if (childPid == -1 && errno != ECHILD)
        printf("waitpid");

    printf("handler: returning\n");

    errno = savedErrno;
}

int main()
{
    pid_t spid, gpid, ipid, wpid;
    int status, savedErrno;
    int sigCnt;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;
    int retcode;
    struct mq_attr attr;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        printf("sigaction");
        return 0;
    }

    printf("메인 함수입니다.\n");

    /* 메시지 큐 속성을 초기화합니다. */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;   
    attr.mq_msgsize = sizeof(toy_msg_t);
    attr.mq_curmsgs = 0;

    /* 메시지 큐를 생성한다. */
    mq_unlink("/watchdog_queue");
    mq_unlink("/monitor_queue");
    mq_unlink("/disk_queue");
    mq_unlink("/camera_queue");

    watchdog_queue = mq_open("/watchdog_queue", O_CREAT | O_RDWR, 0777, &attr);
    monitor_queue = mq_open("/monitor_queue", O_CREAT | O_RDWR, 0777, &attr);
    disk_queue = mq_open("/disk_queue", O_CREAT | O_RDWR, 0777, &attr);
    camera_queue = mq_open("/camera_queue", O_CREAT | O_RDWR, 0777, &attr);


    printf("시스템 서버를 생성합니다.\n");
    spid = create_system_server();
    printf("웹 서버를 생성합니다.\n");
    wpid = create_web_server();
    printf("입력 프로세스를 생성합니다.\n");
    ipid = create_input();
    printf("GUI를 생성합니다.\n");
    gpid = create_gui();

    waitpid(spid, &status, 0);
    waitpid(gpid, &status, 0);
    waitpid(ipid, &status, 0);
    waitpid(wpid, &status, 0);

    return 0;
}
