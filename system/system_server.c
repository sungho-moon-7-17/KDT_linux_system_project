#include <system_server.h>

void system_server(){
    printf("응애 나 아기 system_server!\n");

    while (1){
        sleep(1);
    }
    
    exit(0);
}

__pid_t create_system_server(){

    __pid_t system_server_pid;

    printf("여기서 system_server 생성\n");

    system_server_pid = fork();
    if (system_server_pid == -1)
        return -1;
    else if (system_server_pid == 0){
        if (prctl(PR_SET_NAME, (unsigned long) "system_server") == -1){
            perror("prctl");
        }
        system_server();
    }
    
    return system_server_pid;
}