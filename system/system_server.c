#include <system_server.h>

#define TIMER_TEST 0

void sigalrm_handler(){
    printf("I'm 시계에요\n\n");
}

void set_inter_sec(struct itimerspec *ts, __time_t inter_sec){
    // it_value 가 0이면 timer가 안됨. 이유는?
    ts->it_value.tv_sec = 0;
    ts->it_value.tv_nsec = 1000;

    ts->it_interval.tv_sec = inter_sec;
    ts->it_interval.tv_nsec = 0;
}

void system_server(){
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec ts;
    timer_t timerID;

    printf("응애 나 아기 system_server!\n");

    // set timer signal handler
    // signal(SIGALRM, sigalrm_handler); - 예전 방식
    sa.sa_handler = sigalrm_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // create timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerID;
    if (timer_create(CLOCK_REALTIME, &sev, &timerID) == -1){
        printf("timer_create Error\n");
        exit(1);
    }

    // set timer
    set_inter_sec(&ts, 5);
    if (timer_settime(timerID, 0, &ts, NULL) == -1){
        printf("timer_settime Error\n");
        exit(1);
    }

    // system_server content
    while (1){
        sleep(1);
    }
    
    exit(0);
}

__pid_t create_system_server(){

    __pid_t system_server_pid;

    printf("여기서 system_server 생성\n");
#if TIMER_TEST == 0
    system_server_pid = fork();
    if (system_server_pid == -1)
        return -1;
    else if (system_server_pid == 0){
        if (prctl(PR_SET_NAME, (unsigned long) "system_server") == -1){
            perror("prctl");
        }
        system_server();
    }
#elif TIMER_TEST == 1
    system_server();
#endif
    return system_server_pid;
}