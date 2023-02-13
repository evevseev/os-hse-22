// Разработать программу, в которой родительский процесс вычисляет число 
// Фибоначчи, а процесс-ребенок вычисляет значение факториала.
// В качестве исходного значения используется аргумент из командной строки.

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


uint64_t fib(int num) {
    if (num < 2) {
        return num;
    }

    uint64_t a0 = 0;
    uint64_t a1 = 1;
    uint64_t a2;

    for (int i = 2; i <= num; ++i) {
        a2 = a1 + a0;
        a0 = a1;
        a1 = a2;
    }

    return a2;
}

uint64_t fact(int num) {
    uint64_t result = 1;
    for (int i = 2; i <= num; ++i) {
        result *= i;
    }

    return result;
}

int main(int argc, char *argv[]) {
    // Get and validate input
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }

    int number = 0;
    try {
        number = std::stoi(argv[1]);
    } catch (std::invalid_argument &e) {
        std::cout << argv[1] << "is not a number" << std::endl;
        return 1;
    }

    if (number < 0) {
        std::cout << "The number can not be negative" << std::endl;
        return 1;
    }


    // From manual: 
    // On  success, the PID of the child process is returned in the parent, and 0 is returned in the child.  
    // On failure, -1 is returned in the parent, no child process is created, and errno is set appropriately.
    pid_t cpid = fork();
    
    if (cpid == -1) {
        std::cout << "Was not able to create a child process" << std::endl;
        return 1;
    } else if (cpid == 0) {
        std::cout << "[pid #" << getpid() << "]: Factorial: " << fact(number) << std::endl;
        
        // Wait child process to finish
        wait(nullptr);
    } else {
        std::cout << "[pid #" << getpid() << "]: Fibonacci: " << fib(number) << std::endl;
    }

    return 0;
}

