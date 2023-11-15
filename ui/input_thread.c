#include <input_thread.h>

static pthread_mutex_t g_sensor_mutex = PTHREAD_MUTEX_INITIALIZER;

int mssleep(unsigned int ms){
    struct timespec time_set;

    time_set.tv_sec = ms / 1000;
    time_set.tv_nsec = (ms % 1000) * 1000 * 1000;

    return nanosleep(&time_set,NULL);
}

/*
 *  sensor thread
 */
void *sensor_thread(void* arg)
{
    char saved_message[TOY_BUFFSIZE];
    char *s = arg;
    int i = 0;

    printf("%s", s);

    while (1) {
        i = 0;
        // 여기서 뮤텍스
        // 과제를 억지로 만들기 위해 한 글자씩 출력 후 슬립
        while (global_message[i] != NULL) {
            pthread_mutex_lock(&g_sensor_mutex);
            printf("%c", global_message[i]);
            fflush(stdout);
            mssleep(500);
            i++;
            pthread_mutex_unlock(&g_sensor_mutex);
        }
        mssleep(5000);
    }

    return 0;
}

/*
 *  command thread
 */


char *builtin_str[] = {
    "send",
    "mu",
    "sh",
    "exit"
};


int (*builtin_func[]) (char **) = {
    &toy_send,
    &toy_mutex,
    &toy_shell,
    &toy_exit
};

int toy_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

int toy_send(char **args)
{
    printf("send message: %s\n", args[1]);

    return 1;
}


int toy_mutex(char **args)
{
    if (args[1] == NULL) {
        return 1;
    }

    printf("save message: %s\n", args[1]);
    pthread_mutex_lock(&g_sensor_mutex);
    strcpy(global_message, args[1]);
    pthread_mutex_unlock(&g_sensor_mutex);
    return 1;
}

int toy_exit(char **args)
{
    return 0;
}

int toy_shell(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("toy");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("toy");
    } else{
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int toy_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < toy_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return 1;
}

char *toy_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0;

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror(": getline\n");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char **toy_split_line(char *line)
{
    int bufsize = TOY_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token, **tokens_backup;

    if (!tokens) {
        fprintf(stderr, "toy: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOY_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += TOY_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "toy: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOY_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

void toy_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf("TOY>");
        line = toy_read_line();
        args = toy_split_line(line);
        status = toy_execute(args);

        free(line);
        free(args);
    } while (status);
}

void *command_thread(void* arg)
{
    char *s = arg;

    printf("%s", s);

    toy_loop();

    return 0;
}
