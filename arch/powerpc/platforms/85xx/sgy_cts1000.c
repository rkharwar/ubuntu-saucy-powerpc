/*
 * Servergy CTS-1000 Setup
 *
 * Maintained by Ben Collins <ben.c@servergy.com>
 *
 * Copyright 2012 by Servergy, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of_gpio.h>

#include <asm/machdep.h>

static int halt_gpion = -EINVAL;
static int halt_trigger;

static struct of_device_id child_match[] = {
	{
		.compatible = "sgy,gpio-halt",
	},
	{},
};

static void gpio_halt_cb(void)
{
	if (!gpio_is_valid(halt_gpion))
		return;

	printk(KERN_INFO "gpio-halt: triggering GPIO.\n");

	gpio_set_value(halt_gpion, halt_trigger);
}

static int __devinit gpio_halt_probe(struct platform_device *pdev)
{
	enum of_gpio_flags flags;
	struct device_node *child, *node = pdev->dev.of_node;
	int gpio, err;
	int trigger;

	if (!node)
		return -ENODEV;

	child = of_find_matching_node(node, child_match);
	if (!child)
		return -ENODEV;

	/* Technically we could just read the first one, but punish
	 * DT writers for invalid form. */
	if (of_gpio_count(child) != 1)
		return -EINVAL;

	/* Get the gpio number relative to the dynamic base. */
	gpio = of_get_gpio_flags(child, 0, &flags);
	if (!gpio_is_valid(gpio))
		return -EINVAL;

	err = gpio_request(gpio, "gpio-halt");
	if (err) {
		printk(KERN_ERR "gpio-halt: error requesting GPIO %d.\n",
		       gpio);
		return err;
	}

	trigger = (flags == OF_GPIO_ACTIVE_LOW);

	gpio_direction_output(gpio, !trigger);

	/* Save it for later */
	halt_gpion = gpio;
	halt_trigger = trigger;

	/* Register our halt function */
	ppc_md.halt = gpio_halt_cb;
	ppc_md.power_off = gpio_halt_cb;

	printk(KERN_INFO "gpio-halt: registered GPIO %d (%d trigger).\n",
	       gpio, trigger);

	return 0;
}

static int __devexit gpio_halt_remove(struct platform_device *pdev)
{
	if (gpio_is_valid(halt_gpion)) {
		ppc_md.halt = NULL;
		ppc_md.power_off = NULL;

		gpio_free(halt_gpion);

		halt_gpion = -EINVAL;
	}

	return 0;
}

static struct of_device_id gpio_halt_match[] = {
	/* We match on the gpio bus itself and scan the children since they
	 * wont be matched against us. We know the bus wont match until it
	 * has been registered too. */
	{
		.compatible = "fsl,qoriq-gpio",
	},
	{},
};
MODULE_DEVICE_TABLE(of, gpio_halt_match);

static struct platform_driver gpio_halt_driver = {
	.driver = {
		.name		= "gpio-halt",
		.owner		= THIS_MODULE,
		.of_match_table = gpio_halt_match,
	},
	.probe		= gpio_halt_probe,
	.remove		= __devexit_p(gpio_halt_remove),
};

module_platform_driver(gpio_halt_driver);

MODULE_DESCRIPTION("Driver to support GPIO triggered system halt for Servergy CTS-1000 Systems.");
MODULE_VERSION("1.0");
MODULE_AUTHOR("Ben Collins <ben.c@servergy.com>");
MODULE_LICENSE("GPL");
