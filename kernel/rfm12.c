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
#include <asm/uaccess.h>

#define DEVICE_NAME "rfm12_ask"

#define DATA_MAX 512

//#define SPI 1
#define DEBUG 1

#ifdef DEBUG
//#  define DBG ( x ) printk (KERN_ALERT x)
#  define DBG_FMT(fmt, ...) printk(KERN_ALERT "%d <%s> : " fmt, __LINE__, __FUNCTION__, __VA_ARGS__)
#  define DBG(str) printk(KERN_ALERT "%d <%s> : " str, __LINE__, __FUNCTION__)
#  define MARK() printk(KERN_ALERT "%d <%s>\n", __LINE__, __FUNCTION__)
#else
#  define DBG_FMT(fmt, ...)
#  define DBG(str)
#  define MARK()
#endif

int ask_2272_1527_pulse_duty_factor[4]={13,5,7,11};

struct spi_device *spi;

u16 word;

struct packet {
    unsigned int duration;
    unsigned int count;
    u8 data[DATA_MAX]; // payload
};

//int len_body = 0; // length of payload - if no we expect a header is going to be received

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static void rfm12_tx_on(void);
static void rfm12_tx_off(void);

void
rfm12_ask_encode_tribit(int *code, int append, int byte, int cnt)
{
    int i;
    for (i=0;i<cnt;i++)
    {
        if (byte & (1<<(cnt-i-1))) {
            code[append+(i*2)]=ask_2272_1527_pulse_duty_factor[0];
            code[append+(i*2)+1]=ask_2272_1527_pulse_duty_factor[1];
        } else {
            code[append+(i*2)]=ask_2272_1527_pulse_duty_factor[2];
            code[append+(i*2)+1]=ask_2272_1527_pulse_duty_factor[3];
        }
    }
}

int rfm12_ask_modulate(struct packet *packet)
{
    unsigned int duration = packet->duration;
    unsigned int count = packet->count;
    u8 *data = packet->data;
    DBG_FMT("got packet:\n  duration: %i\n  count: %i\n  data: %s\n", duration, count, data);
    int i;
    for(;count>0;count--) {
        for(i=0;data[i]!='\0';i++) {
            unsigned int us = duration;
            switch(data[i]) {
                case '1':
                    DBG("switch on TX\n");
                    rfm12_tx_on();
                    for(;us>0;us--)
                        udelay(1);
                    break;
                case '0':
                    DBG("switch off TX\n");
                    rfm12_tx_off();
                    for(;us>0;us--)
                        udelay(1);
                    break;
               default:
                    DBG_FMT("MOEP! character <%c> is invalid!\n", data[i]);
                    return -EINVAL;
            }
        }
    }
    return 0;
}

void
rfm12_ask_trigger(int level, int us)
{
    if (level)
    {
        word = 0x8200|(1<<5)|(1<<4)|(1<<3);
        spi_write(spi, (const u8 *)&word, 2);
        for(;us>0;us--)
            udelay(1);
    }
    else
    {
        word = 0x8208;
        spi_write(spi, (const u8 *)&word, 2);
        for(;us>0;us--)
            udelay(1);
    }
}

// inline to avoid unneeded jumps
inline static void rfm12_tx_on(void)
{
#ifdef SPI
    word = 0x8200|(1<<5)|(1<<4)|(1<<3);
    spi_write(spi, (const u8 *)&word, 2);
#endif
}

// inline to avoid unneeded jumps
inline static void rfm12_tx_off(void)
{
#ifdef SPI
    word = 0x8208;
    spi_write(spi, (const u8 *)&word, 2);
#endif
}

static void _write_spi(u8 _word)
{
#ifdef SPI
    word = _word;
    spi_write(spi, (const u8 *)&word, 2);
#endif
}

void
rfm12_ask_2272_send(int *command, int delay, int cnt)
{
    int code[49];

    int i;
    printk(KERN_ALERT "encoding\n");
    for(i=0;i<3;i++)
    {
        rfm12_ask_encode_tribit(code, i*16, command[i], 8);
    }
    code[48]=7; //sync
    word = 0x8200|(1<<5)|(1<<4)|(1<<3);
    printk(KERN_ALERT "??\n");
    spi_write(spi, (const u8 *)&word, 2);     // 2. PwrMngt TX on
    int ii;
    for(ii=cnt;ii>0;ii--)                             // Sequenz cnt send
    {
        int rfm12_trigger_level=0;
        int i;
        for(i=0;i<49;i++)
        {
        printk(KERN_ALERT "running trigger\n");
            rfm12_ask_trigger(rfm12_trigger_level^=1,code[i]*delay);
        }
        printk(KERN_ALERT "running trigger (2)\n");
        rfm12_ask_trigger(0,24*delay);
    }
    word = 0x8208;
    printk(KERN_ALERT "set <0x8208>\n");
    spi_write(spi, (const u8 *)&word, 2);
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

    //word = 0x0000;
    //spi_write(spi, (const u8 *)&word, 2);

    word = 0x0000;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);
    spi_read(spi, (u8 *)&word, 2);
    MARK();

    word = 0xC0E0;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);        /* AVR CLK: 10MHz */
    word = RFM12BAND(433920);
    MARK();
    spi_write(spi, (const u8 *)&word, 2);    /* Select BAND, Enable FIFO */
    word = 0xC2AB;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);        /* Data Filter: internal */
    word = 0xCA81;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);        /* Set FIFO mode */
    word = 0xE000;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);        /* disable wakeuptimer */
    word = 0xC800;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);        /* disable low duty cycle */
    word = 0xC4F7;
    MARK();
    spi_write(spi, (const u8 *)&word, 2);        /* AFC settings: autotuning: -10kHz...+7,5kHz */

    int command[3];
    command[0] = 0;
    command[1] = 5;
    command[2] = 81;
    DBG("sending (0,5,81), 76, 10\n");
    rfm12_ask_2272_send(command, 76, 10);
    DBG("finished\n");

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
    int major_nr = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_nr < 0) {
        DBG_FMT("Registering char device failed with %d\n", major_nr);
        return major_nr;
    }
    DBG_FMT("device major number is: %i\n", major_nr);

#ifdef SPI
    DBG_FMT("register spi driver\n");
    return spi_register_driver(&rfm12_driver);
#endif
    return 0;
}

void cleanup_module(void)
{
#ifdef SPI
    spi_unregister_driver(&rfm12_driver);
#endif
}

MODULE_LICENSE("GPL");
