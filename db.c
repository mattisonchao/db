#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Connection_t {
    int file_descriptor;
};
typedef struct Connection_t Connection;

Connection *open_connection(char *filename) {
    int fd = open(filename, O_RDWR | O_CREAT | S_IWUSR | S_IRUSR);
    if (fd == -1) {
        printf("Unable to open file '%s' \n", filename);
        exit(EXIT_FAILURE);
    }

    Connection *connection = malloc(sizeof(Connection));
    connection->file_descriptor = fd;
    return connection;
}


char *get_db_filename(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Must supply a filename for the database.\n");
        exit(EXIT_FAILURE);
    }
    return argv[1];
}

struct InputBuffer_t {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
};
typedef struct InputBuffer_t InputBuffer;

InputBuffer *new_input_buffer() {
    InputBuffer *inputBuffer = malloc(sizeof(InputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
    return inputBuffer;
}

void print_prompt() {
    printf("db >");
}

void read_input(InputBuffer *input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

int main(int argc, char *argv[]) {
    char *db_filename = get_db_filename(argc, argv);
    Connection *connection = open_connection(db_filename);

    InputBuffer *input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);
        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s' .\n", input_buffer->buffer);
        }
    };
}


