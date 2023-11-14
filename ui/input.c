#include <input.h>

void input(){
    printf("enter input\n");

    while(1){

    }

    exit(0);
}

__pid_t create_input(){

    __pid_t input_pid;

    printf("여기서 input_process 생성\n");

    input_pid = fork();
    if (input_pid == -1)
        return -1;
    else if (input_pid == 0)
        input();
    
    return input_pid;
}
