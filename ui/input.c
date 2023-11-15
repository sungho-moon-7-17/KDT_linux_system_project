#include <input.h>

typedef struct _sig_ucontext {
    unsigned long uc_flags;
    struct ucontext *uc_link;
    stack_t uc_stack;
    struct sigcontext uc_mcontext;
    sigset_t uc_sigmask;
} sig_ucontext_t;

void segfault_handler(int sig_num, siginfo_t * info, void * ucontext) {
  void * array[50];
  void * caller_address;
  char ** messages;
  int size, i;
  sig_ucontext_t * uc;

  uc = (sig_ucontext_t *) ucontext;


  /* Get the address at the time the signal was raised */
  caller_address = (void *) uc->uc_mcontext.rip;  // RIP: x86_64 specific     arm_pc: ARM

  fprintf(stderr, "\n");

  if (sig_num == SIGSEGV)
    printf("signal %d (%s), address is %p from %p\n", sig_num, strsignal(sig_num), info->si_addr,
           (void *) caller_address);
  else
    printf("signal %d (%s)\n", sig_num, strsignal(sig_num));

  size = backtrace(array, 50);
  /* overwrite sigaction with caller's address */
  array[1] = caller_address;
  messages = backtrace_symbols(array, size);

  /* skip first stack frame (points here) */
  for (i = 1; i < size && messages != NULL; ++i) {
    printf("[bt]: (%d) %s\n", i, messages[i]);
  }

  free(messages);

  exit(EXIT_FAILURE);
}

void input(){
  pthread_t t_cli, t_ssor;
  pthread_attr_t attr;
  int t_error = 0;

  printf("enter input\n");

  signal(SIGSEGV, segfault_handler);

  // create thread - command line , sensor
  t_error += pthread_attr_init(&attr);
  t_error += pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  t_error += pthread_create(&t_cli, &attr, command_thread, (void *) "HI command_thread start\n");
  t_error += pthread_create(&t_ssor, &attr, sensor_thread, (void *) "HI sensor_thread start\n");
  if (t_error != 0){
    perror("create thread error\n");
    exit(1);
  }

  while(1){
    sleep(1);
  }

  exit(0);
}

__pid_t create_input(){

  __pid_t input_pid;

  printf("여기서 input_process 생성\n");

  input_pid = fork();
  if (input_pid == -1)
      return -1;
  else if (input_pid == 0){
      if (prctl(PR_SET_NAME, (unsigned long) "input") == -1){
          perror("prctl");
      }
      input();
  }
  
  return input_pid;
}
