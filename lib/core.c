#include <stdlib.h>
#include <stdio.h>

#define DEVICE_NAME "/dev/rfm12_ask"

#define DATA_MAX 512

struct packet {
    unsigned int duration;
    unsigned int count;
    char data[DATA_MAX]; // payload
};

enum type {
    SOCKET_TYPE_2272,
    SOCKET_TYPE_FNORD,
};

struct device {
    int type;
    char *id;
    char *name;
    char *category;
    struct packet (*on)(char system_code, char unit_code);
    struct packet (*off)(char system_code, char unit_code);
    union {
        struct {
            char system_code;
            char unit_code;
        } device_2272;
        struct {
            char ipaddr;
            char port;
        } device_fnord;
    };
};

int write(struct packet *_packet) {
    printf("data: %s\n", _packet->data);
    FILE *fd = fopen(DEVICE_NAME, "w");
    if (fd == NULL) {
        printf("can not open device: %s\n", DEVICE_NAME);
        return EXIT_FAILURE;
    }
    if (fwrite(_packet, 1, sizeof(_packet), fd) != sizeof(_packet))
        return EXIT_FAILURE;
    fclose(fd);
    return 0;
}

//void switch_2272_on(struct device *dev) {
struct packet switch_2272_on(char system_code, char unit_code) {
    printf("switch on\n");
    struct packet pkg = {
        .duration = 100,
        .count = 1,
        .data = "101",
    };
    return pkg;
}

struct packet switch_2272_off(char system_code, char unit_code) {
    printf("switch off\n");
    struct packet pkg;
    return pkg;
}

int main(void)
{
    //struct packet foo = {
    //    .duration = 5,
    //    .count = 10,
    //    .data = "01000000101010101010000001010101010107100001",
    //};

    //struct device *ptr;
    //ptr = malloc(sizeof(device) * 2);
    //if(!ptr)
    //    return NULL;
    struct device dev_arr[2];
    struct device tmp;

    tmp.type = SOCKET_TYPE_2272;
    tmp.id = "ROOM_1__LAMP_1";
    tmp.category = "living room";
    tmp.name = "ceiling light";
    tmp.on = &switch_2272_on,
    tmp.off = &switch_2272_off,
    dev_arr[0] = tmp;

    //tmp.type = SOCKET_TYPE_2272;
    //tmp.id = "ROOM_1__LAMP_2";
    //tmp.category = "living room";
    //tmp.name = "desktop";
    ////tmp.on_function_t = *2272_on,
    ////tmp.off_function_t = *2272_off,
    //dev_arr[1] = tmp;

    printf("test: %p\n", dev_arr[0].on);
    //printf("test: %s\n", dev_arr[1].name);

    struct packet pkg = (dev_arr[0].on)(0x00, 0x01);

    return write(&pkg);
}
