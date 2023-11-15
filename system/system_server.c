#include <system_server.h>

#define TIMER_TEST 0

int g_timer_count = 0;
static pthread_mutex_t g_timer_mutex = PTHREAD_MUTEX_INITIALIZER;

#pragma region thread
void * watchdog_thread(void * arg){
    printf("%s\n", (char *)arg);

    while(1){
        sleep (10);
    }
}
void * monitor_thread(void * arg){
    printf("%s\n", (char *)arg);

    while(1){
        sleep (10);
    }
}
void * disk_service_thread(void * arg){
    printf("%s\n", (char *)arg);

    while(1){
        sleep (10);
    }
}
void * camera_service_thread(void * arg){
    printf("%s\n", (char *)arg);

    while(1){
        sleep (10);
    }
}

#pragma endregion

void sigalrm_handler(){
    pthread_mutex_lock(&g_timer_mutex);
    g_timer_count++;
    pthread_mutex_unlock(&g_timer_mutex);
}

void set_timer_sec(struct itimerspec *ts, __time_t inter_sec){
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

    int t_error;
    pthread_t t_wd, t_m, t_ds, t_cs;
    pthread_attr_t attr;

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
    set_timer_sec(&ts, 5);
    if (timer_settime(timerID, 0, &ts, NULL) == -1){
        printf("timer_settime Error\n");
        exit(1);
    }

    // create thread - command line , sensor
    t_error += pthread_attr_init(&attr);
    t_error += pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    t_error += pthread_create(&t_wd, &attr, watchdog_thread, (void *) "HI watchdog_thread start\n");
    t_error += pthread_create(&t_m, &attr, monitor_thread, (void *) "HI monitor_thread start\n");
    t_error += pthread_create(&t_ds, &attr, disk_service_thread, (void *) "HI disk_service_thread start\n");
    t_error += pthread_create(&t_cs, &attr, camera_service_thread, (void *) "HI camera_service_thread start\n");
    if (t_error != 0){
        perror("create thread error\n");
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