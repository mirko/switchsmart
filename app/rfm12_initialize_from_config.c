#include "../lib/core.h"

int main()
{
    struct device dev_arr[32]; //FIXME

    create_objs_by_cfg(&dev_arr);
    dev_arr[0].on(dev_arr[0].code);
}
