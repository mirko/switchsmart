//
// rfm12 ASK driver - this driver module adds rfm12 ASK support to the
//                    linux kernel
//
// Copyright (C) 2010 Mirko Vogt <dev@nanl.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "rfm12"
#define DATA_MAX 512

#define GPIO_TX 104 // do not set if no GPIO should be toggled while transmitting data

#define SPI 1
//#define DEBUG 1 // WARNING: enabling debug may screw up timing

#ifdef DEBUG
#  define DBG_FMT(fmt, ...) printk(KERN_ALERT "%d <%s> : " fmt, __LINE__, __FUNCTION__, __VA_ARGS__)
#  define DBG(str) printk(KERN_ALERT "%d <%s> : " str, __LINE__, __FUNCTION__)
#  define MARK() printk(KERN_ALERT "%d <%s>\n", __LINE__, __FUNCTION__)
#  define BUG printk(KERN_ALERT "BUG! BUG! BUG! - %d <%s>\n", __LINE__, __FUNCTION__);
#else
#  define DBG_FMT(fmt, ...)
#  define DBG(str)
#  define MARK()
#endif

struct spi_device *spi;
unsigned long flags;
u8 word[2];
unsigned int major_nr;

struct packet {
    unsigned int duration;
    unsigned int count;
    u8 data[DATA_MAX]; // payload
};

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static void _write_spi(u16 _word)
{
#ifdef SPI
    word[0] = _word &0xff;
    word[1] = _word >> 8;
    spi_write(spi, (const u8 *)&word, 2);
#endif
}

// inline to avoid unneeded jumps
static void rfm12_tx_on(void)
{
#ifdef SPI
    _write_spi(0x8238);
#endif
#ifdef GPIO_TX
    gpio_set_value(GPIO_TX, 0);
#endif
}

// inline to avoid unneeded jumps
static void rfm12_tx_off(void)
{
#ifdef SPI
    //_write_spi(0x8208);
    _write_spi(0x8208);
#endif
#ifdef GPIO_TX
    gpio_set_value(GPIO_TX, 1);
#endif
}

int rfm12_ask_modulate(struct packet *packet)
{
    unsigned int duration = packet->duration;
    unsigned int count = packet->count;
    u8 *data = packet->data;
    DBG_FMT("got packet:\n  duration: %i\n  count: %i\n  data: %s\n", duration, count, data);
    int i;
    u8 on;
//    // disable interrupts
//    local_irq_save(flags);
//    // disable preemption
//    preempt_disable();
//    // START this code is executed as atomic operation
    for(;count>0;count--) {
        //make sure TX is powered OFF when start new round (should not happen (see below))
        DBG("switch off TX (top)\n");
        rfm12_tx_off();
        #ifdef DEBUG
        if(on)
            BUG;
        #endif
        on = 0x00;
        for(i=0;data[i]!='\0';i++) {
            unsigned int us = duration;
            DBG_FMT("module on? %x\n", on);
            switch(data[i]) {
                case '1':
                    DBG("1\n");
                    //only switch on TX, if it's not yet on,
                    //avoid unneeded transfer of data via SPI
                    //and who knows what's the module doing
                    //in this case anyway...
                    //if(!on) {
                    //if(on == 0x00) {
                    //    DBG("switch on TX\n");
                        rfm12_tx_on();
                        on = 0x01;
                    //}
                    for(;us>0;us--)
                        udelay(1);
                    break;
                case '0':
                    DBG("0\n");
                    //only switch off TX, if it's not yet off,
                    //avoid unneeded transfer of data via SPI
                    //and who knows what's the module doing
                    //in this case anyway...
                    //if(on) {
                    //if(on == 0x01) {
                    //    DBG("switch off TX\n");
                        rfm12_tx_off();
                        on = 0x00;
                    //}
                    for(;us>0;us--)
                        udelay(1);
                    break;
               default:
                    DBG_FMT("MOEP! character <%c> is invalid!\n", data[i]);
                    return -EINVAL;
            }
        }
//        // END this code is executed as atomic operation
//        // re-enable interrupts
//        local_irq_restore(flags);
//        // re-enable preemption
//        preempt_enable();
        //wait some time, before sending again
        //wait only if data is going to be sent more than once and there will be a further round
        //wait 5*duration microseconds before sending again (arbitrary chosen)
        if ((packet->count > 1) && (count != 0))
            udelay(duration * 5);
        //make sure TX is powered OFF when start new round (should not happen)
        DBG("switch off TX (bot)\n");
        rfm12_tx_off();
        #ifdef DEBUG
        //if(on)
        if(on = 0x01)
            BUG; // TX shouldn't be left on - this should be fixed within the data array written to the character device if so
        #endif
        on = 0x00;
    }
    return 0;
}

#define RFM12BAND(freq)    (freq<800000?0x80D7:0x80E7)

static int __devinit rfm12_probe(struct spi_device *spi_)
{
    DBG("probing\n");
    spi = spi_;
    DBG("setting mode\n");
    spi->mode = SPI_MODE_0;
    DBG("setting word size\n");
    spi->bits_per_word = 16;
    DBG("setup spi device\n");
    spi_setup(spi);

//    _write_spi(0x0000);
//    MARK();
//    spi_write(spi, (const u8 *)&word, 2);
//    spi_read(spi, (u8 *)&word, 2);
//    MARK();

    _write_spi(0xC0E0);
    MARK();
    _write_spi(RFM12BAND(433920)); // 433 MHz
    MARK();
    _write_spi(0xC2AB);
    MARK();
    _write_spi(0xCA81);
    MARK();
    _write_spi(0xE000);
    MARK();
    _write_spi(0xC800);
    MARK();
    _write_spi(0xC4F7);
    MARK();

    DBG("probe finished\n");

    return 0;
}

static struct spi_driver rfm12_driver = {
    .driver = {
        .name = "rfm12",
        .owner = THIS_MODULE,
    },
    .probe = rfm12_probe,
};


static int device_open(struct inode *inode, struct file *file)
{
    DBG("device opened\n");
    return 0;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
    DBG("device read attempt\n");
    return -EINVAL;
}

static ssize_t device_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
    DBG("device write attempt\n");
    DBG_FMT("length: %i\n", length);
    struct packet foo;
    DBG_FMT("size should be greater than %i but less than %i - it is: %i\n", sizeof(struct packet) - DATA_MAX, sizeof(struct packet), length);
    if ((length > sizeof(struct packet)) || (length <= (sizeof(struct packet) - DATA_MAX))) // too much or no payload
        return -EINVAL;
    DBG_FMT("allocated memory variable is assigned to: %i", sizeof(foo));
    if (copy_from_user(&foo, buffer, length))
        return -EFAULT;
    DBG_FMT("  header :: duration: %i, count: %i\n", foo.duration, foo.count);
    DBG_FMT("  payload :: as string: < %s >\n", foo.data);

    if (rfm12_ask_modulate(&foo) < 0)
        return -EINVAL; // does not quite work...

    return length;
}

static int device_release(struct inode *inode, struct file *file)
{
    DBG("device released\n");
    return 0;
}

struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

int init_module(void)
{
    DBG("init module\n");
    major_nr = register_chrdev(0, DEVICE_NAME, &fops);

#ifdef GPIO_TX
    //int gpio_valid = gpio_is_valid(GPIO_TX);
    int gpio_req = gpio_request(GPIO_TX, NULL);
    if (gpio_req < 0)
        return -EINVAL;
    gpio_direction_output(GPIO_TX, 1);
    // SYNC
    gpio_set_value(GPIO_TX, 0);
    gpio_set_value(GPIO_TX, 1);
    gpio_set_value(GPIO_TX, 0);
    gpio_set_value(GPIO_TX, 1);
#endif

    if (major_nr < 0) {
        //DBG_FMT("Registering char device failed: %i\n", major_nr);
        printk(KERN_ALERT "Registering char device failed: %i\n", major_nr);
        return major_nr;
    }
    //DBG_FMT("device major number is: %i\n", major_nr);
    printk(KERN_INFO "device major number is: %i\n", major_nr);

#ifdef SPI
    DBG("register spi driver\n");
    return spi_register_driver(&rfm12_driver);
#endif

    return 0;
}

void cleanup_module(void)
{
    unregister_chrdev(major_nr, DEVICE_NAME);
#ifdef SPI
    spi_unregister_driver(&rfm12_driver);
#endif
#ifdef GPIO_TX
    gpio_free(GPIO_TX);
#endif
}

MODULE_LICENSE("GPL");
