#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
int main(int argc, char** argv) {
    
    pid_t pid = fork();
    if(pid > 0) {
        puts("I'm a parrent");
        int ext_stat;
        waitpid(pid, &ext_stat, 0);
    }
    else if(pid == 0) {
        puts("I'm a child");
    }
    else {
        puts("Error");
    } 

    return 0;
}
