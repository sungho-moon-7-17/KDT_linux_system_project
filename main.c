#include <webserver.h>
#include <ui.h>
#include <input.h>
#include <system_server.h>

void sigchld_handler(int sig){
    printf("handler: Caught SIGCHLD : %d\nhandler: returning\n",sig);
}

int main(){

    __pid_t input_pid, ss_pid, ui_pid, ws_pid;

    signal(SIGCHLD, sigchld_handler);

    if ((input_pid = create_input()) <= 0){
        printf("create input error\n");
        return -1;
    }
    
    if ((ss_pid = create_system_server()) <= 0){
        printf("create input error\n");
        return -1;
    }

    if ((ui_pid = create_ui()) <= 0){
        printf("create input error\n");
        return -1;
    }

    if ((ws_pid = create_web_server()) <= 0){
        printf("create input error\n");
        return -1;
    }

    waitpid(input_pid, NULL, NULL);
    waitpid(ss_pid, NULL, NULL);
    waitpid(ui_pid, NULL, NULL);
    waitpid(ws_pid, NULL, NULL);

    return 0;
}
