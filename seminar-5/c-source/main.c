// Разработать программы, реализующие двусторонний обмен данными по именованным каналам.
// То есть, организовать их взаимодействие как независимых процессов.

// Имена каналов для обмена сообщениями передавать через аргументы командной строки.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 25 // buffer size for reading from pipe

// Function for sending message via fd and logging the action
ssize_t send_message(int fd, const char *message, size_t length);

// Function for receiving message (limited by buf_size) via fd and logging the action
ssize_t receive_message(int fd, char *buffer, size_t buf_size);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    int fd_forward[2], fd_reversed[2]; // descriptors
    char buffer[BUFFER_SIZE]; // buffer

    // Remove link, in order to be able to create a new pipe
    unlink(argv[1]);
    unlink(argv[2]);

    // Create named pipes using mknod
    if (mknod(argv[1], S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        return 1;
    }
    if (mknod(argv[2], S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        return 1;
    }

    // Open pipes
    if ((fd_forward[0] = open(argv[1], O_RDONLY | O_NONBLOCK)) == -1) {
        perror("open 1");
        return 1;
    }
    if ((fd_forward[1] = open(argv[1], O_WRONLY)) == -1) {
        perror("open 1");
        return 1;
    }
    if ((fd_reversed[0] = open(argv[2], O_RDONLY | O_NONBLOCK)) == -1) {
        perror("open 2");
        return 1;
    }
    if ((fd_reversed[1] = open(argv[2], O_WRONLY)) == -1) {
        perror("open 2");
        return 1;
    }

    // Instead of creating two separate programs (with almost the identical code) I decided to use fork.
    // Anyway, task says that there are could be only two processes working at the same time, 
    // so we could not have e.g. 2 readers and 1 writer
    pid_t cpid = fork(); 
    if (cpid == -1) {
        // Was not able to create child process
        perror("fork");
        return 1;
    } else if (cpid == 0) {
        // Child process
        close(fd_forward[0]); // close input of pipe
        close(fd_reversed[1]); // close output of second pipe

        // Make random action: Sent message or Get Message
        int sent_messages = 0;
        while(1) {
            if(rand() % 2 == 0 && sent_messages < 10){
                if (send_message(fd_forward[1], "Hello from child!", 18) == -1) {
                    // Was not able to send message
                    perror("send_message child");
                    return 1;
                }
                ++sent_messages;
            } else {
                if (receive_message(fd_reversed[0], buffer, BUFFER_SIZE) == -1 && errno != EAGAIN) {
                    // Was not able to get message
                    perror("receive_message child");
                    return 1;
                }
            }
            sleep(rand() % 2);
        }
    } else {
        // Parent process
        close(fd_forward[1]); // close output of pipe
        close(fd_reversed[0]); // close input of second pipe

        // Make random action: Sent message or Get Message
        int sent_messages = 0;
        while(1) {
            if (rand() % 2 == 0 && sent_messages < 8) {
                if (send_message(fd_reversed[1], "Hello from parent!", 19) == -1) {
                    // Was not able to send message
                    perror("send_message parent");
                    return 1;
                }
                ++sent_messages;
            } else {
                if (receive_message(fd_forward[0], buffer, BUFFER_SIZE) == -1 && errno != EAGAIN) {
                    // Was not able to get message
                    perror("receive_message parent");
                    return 1;
                } 
            }
            sleep(rand() % 2);
        }
    }
    return 0;
}

// Function for sending message via fd and logging the action
ssize_t send_message(int fd, const char *message, size_t length) {
    ssize_t bytes = write(fd, message, length);

    if (bytes > 0) {
        printf("[# %d] (==> [%d]) Sent: %s\n", getpid(), fd, message);
    }
    return bytes;
}

// Function for receiving message (limited by buf_size) via fd and logging the action
ssize_t receive_message(int fd, char *buffer, size_t buf_size) {
    ssize_t bytes = read(fd, buffer, buf_size); 

    if (bytes > 0) {
        printf("[# %d] (<== [%d]) Received %ld bytes: ", getpid(), fd, bytes);

        for (size_t i = 0; i < bytes; ++i) {
            putchar(buffer[i]);
        }
        putchar('\n');
    }
    return bytes;
}