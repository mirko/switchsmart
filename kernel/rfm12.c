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
// //


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>

int ask_2272_1527_pulse_duty_factor[4]={13,5,7,11};

struct spi_device *spi;

u16 word;

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

void
rfm12_ask_trigger(int level, int us)
{
	if (level)
	{
		word = 0x8200|(1<<5)|(1<<4)|(1<<3);
		spi_write(spi, (const u8 *)&word, 2); // 2. PwrMngt TX on
		for(;us>0;us--)
			udelay(1);
	}
	else
	{
		word = 0x8208;
		spi_write(spi, (const u8 *)&word, 2);											// 2. PwrMngt TX off
		for(;us>0;us--)
			udelay(1);
	}
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
	spi_write(spi, (const u8 *)&word, 2);	 // 2. PwrMngt TX on
	int ii;
	for(ii=cnt;ii>0;ii--)							 // Sequenz cnt send
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
	spi_write(spi, (const u8 *)&word, 2);												// 2. PwrMngt TX off
}

#define RFM12BAND(freq)	(freq<800000?0x80D7:0x80E7)

static int __devinit rfm12_probe(struct spi_device *spi_)
{
	printk(KERN_ALERT "probing\n");
	spi = spi_;
	printk(KERN_ALERT "setting mode\n");
	spi->mode = SPI_MODE_0;
	printk(KERN_ALERT "setting word size\n");
	spi->bits_per_word = 16;
	printk(KERN_ALERT "setup spi device\n");
	spi_setup(spi);

	//word = 0x0000;
	//spi_write(spi, (const u8 *)&word, 2);

	word = 0x0000;
	printk(KERN_ALERT "get status\n");
	spi_write(spi, (const u8 *)&word, 2);
	spi_read(spi, (u8 *)&word, 2);
	printk(KERN_ALERT "got %x\n", word);

	word = 0xC0E0;
	printk(KERN_ALERT "set <1>\n");
	spi_write(spi, (const u8 *)&word, 2);		/* AVR CLK: 10MHz */
	word = RFM12BAND(433920);
	printk(KERN_ALERT "set <2>\n");
	spi_write(spi, (const u8 *)&word, 2);	/* Select BAND, Enable FIFO */
	word = 0xC2AB;
	printk(KERN_ALERT "set <3>\n");
	spi_write(spi, (const u8 *)&word, 2);		/* Data Filter: internal */
	word = 0xCA81;
	printk(KERN_ALERT "set <4>\n");
	spi_write(spi, (const u8 *)&word, 2);		/* Set FIFO mode */
	word = 0xE000;
	printk(KERN_ALERT "set <5>\n");
	spi_write(spi, (const u8 *)&word, 2);		/* disable wakeuptimer */
	word = 0xC800;
	printk(KERN_ALERT "set <6>\n");
	spi_write(spi, (const u8 *)&word, 2);		/* disable low duty cycle */
	word = 0xC4F7;
	printk(KERN_ALERT "set <7>\n");
	spi_write(spi, (const u8 *)&word, 2);		/* AFC settings: autotuning: -10kHz...+7,5kHz */

	int command[3];
	command[0] = 0;
	command[1] = 5;
	command[2] = 81;
	printk(KERN_ALERT "sending (0,5,81), 76, 10\n");
	rfm12_ask_2272_send(command, 76, 10);
	printk(KERN_ALERT "finished\n");

	return 0;
}

static struct spi_driver rfm12_driver = {
	.driver = {
		.name = "rfm12",
		.owner = THIS_MODULE,
	},
	.probe = rfm12_probe,
};

int init_module(void)
{
	printk(KERN_ALERT "init module\n");
	printk(KERN_ALERT "register spi driver\n");
	int res_spi_register_driver = spi_register_driver(&rfm12_driver);
	//int res_setup_sysfs = setup_sysfs();
	//return res_spi_register_driver | res_setup_sysfs;
	return res_spi_register_driver;
}

void cleanup_module(void)
{
	spi_unregister_driver(&rfm12_driver);
}

MODULE_LICENSE("GPL");
