// Разработать программу использующую для работы с текстовыми файлами системные вызовы. 
// Программа на языке C должна прочитать, используя буфер, размер которого превышает читаемые файлы 
// и записать прочитанный файл в файл с другим именем. Имена файлов для чтения и записи задавать с использованием аргументов командной строки.

// Использовать для работы с файлами буфер ограниченного размера, требующий циклического использования.

// Читать и переписывать не только текстовые, но и исполняемые файлы, включая скрипты, которые сохраняют режим доступа, 
// обеспечивающий их запуск. При этом обычные текстовые файлы запускаться не должны.

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024 // buffer size for cycle reading

int file_copy(const char *input_file, const char *output_file);

// Main function
int main(int argc, char *argv[]) {
    // Check if there are 2 arguments provided
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    // Call function to copy file
    return file_copy(argv[1], argv[2]);
}

// input_file - file to copy
// output_file - file to copy to
int file_copy(const char *input_file, const char *output_file) {
    int input_fd, output_fd; // file descriptors
    char buffer[BUFFER_SIZE]; // buffer
    ssize_t read_bytes, wrote_bytes; // bytes read and written (ssize_t is like size_t, but can be -1. we need it for error checking)
    struct stat file_stat; // file status
    
    // Open input_file for reading
    if ((input_fd = open(input_file, O_RDONLY)) == -1) { // O_RDONLY - open for reading only
        perror("open");
        return 1;
    }

    // Open output_file for writing
    if ((output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) { // O_WRONLY - open for writing only, O_CREAT - create file if it doesn't exist, O_TRUNC - truncate file to 0 length if it exists
        perror("open");
        return 1;
    }


    // Read chunks of input_file
    while ((read_bytes = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        // And write them to output_file
        if ((wrote_bytes = write(output_fd, buffer, read_bytes)) != read_bytes) {
            // If less bytes were written than read, then there was an error
            perror("write");
            return 1;
        }
    }

    // Check if there was an error while reading last chunk
    if (read_bytes == -1) {
        perror("read");
        return 1;
    }

    // Get input_file info
    if (fstat(input_fd, &file_stat) == -1) { 
        perror("fstat");
        return 1;
    } else {
        // Set output_file permissions to the same as input_file
        if (chmod(output_file, file_stat.st_mode) == -1) {
            perror("chmod");
            return 1;
        }
    }

    // Close file descriptors
    if (close(input_fd) == -1) {
        perror("close");
        close(output_fd);
        return 1;
    }

    if (close(output_fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}