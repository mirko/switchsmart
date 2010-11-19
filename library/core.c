#include <stdlib.h>
#include <stdio.h>

#define DEVICE_NAME "/dev/rfm12_ask"

#define DATA_MAX 512

struct packet {
    unsigned int duration;
    unsigned int count;
    char data[DATA_MAX]; // payload
};

struct packet foo = {
    .duration = 2,
    .count = 2,
    .data = "010",
};

int main() {
    printf("data: %s\n", foo.data);
    FILE *fd = fopen(DEVICE_NAME, "w");
//    foo.data = "01";
    if (fd == NULL) {
        printf("can not open device: %s\n", DEVICE_NAME);
        return EXIT_FAILURE;
    }
    fwrite(&foo, 1, sizeof(foo), fd);
    fclose(fd);
}
