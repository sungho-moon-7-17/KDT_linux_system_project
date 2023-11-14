#include <webserver.h>

__pid_t create_web_server(){

    __pid_t web_server_pid;

    printf("여기서 web_server_process 생성\n");

    web_server_pid = fork();
    if (web_server_pid == -1)
        return -1;
    else if (web_server_pid == 0){
        execl("/usr/local/bin/filebrowser", "filebrowser", "-p", "8282", NULL);
    }
    
    return web_server_pid;
}