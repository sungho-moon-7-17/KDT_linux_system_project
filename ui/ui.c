#include <ui.h>

__pid_t create_ui(){

    __pid_t ui_pid;

    printf("여기서 ui_process 생성\n");

    ui_pid = fork();
    if (ui_pid == -1)
        return -1;
    else if (ui_pid == 0){
        execl("/usr/bin/google-chrome-stable", "google_chrome-stable", "http://localhost:8282", NULL);
    }
    
    return ui_pid;

}