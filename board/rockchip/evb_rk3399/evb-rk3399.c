// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2016 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <init.h>
#include <log.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/arch-rockchip/periph.h>
#include <asm/arch-rockchip/clock.h>
#include <asm/arch-rockchip/grf_rk3399.h>
#include <asm/arch-rockchip/hardware.h>
#include <asm/arch-rockchip/misc.h>
#include <power/regulator.h>
#include <led.h>
#include "debug.h"
#include <eeprom.h>
#include <i2c_eeprom.h>
#include <net.h>

#define GRF_IO_VSEL_BT565_SHIFT 0
#define PMUGRF_CON0_VSEL_SHIFT 8

#ifndef CONFIG_SPL_BUILD
int board_early_init_f(void)
{
	struct udevice *regulator;
	int ret;

	ret = regulator_get_by_platname("vcc5v0_host", &regulator);
	if (ret) {
		debug("%s vcc5v0_host init fail! ret %d\n", __func__, ret);
		goto out;
	}

	ret = regulator_set_enable(regulator, true);
	if (ret)
		debug("%s vcc5v0-host-en set fail! ret %d\n", __func__, ret);

	if (IS_ENABLED(CONFIG_LED))
		led_default_state();

out:
	return 0;
}
#endif

#ifdef CONFIG_MISC_INIT_R
static void setup_iodomain(void)
{
	struct rk3399_grf_regs *grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	struct rk3399_pmugrf_regs *pmugrf = syscon_get_first_range(ROCKCHIP_SYSCON_PMUGRF);

	/* BT565 is in 1.8v domain */
	rk_setreg(&grf->io_vsel, 1 << GRF_IO_VSEL_BT565_SHIFT);

	/* Set GPIO1 1.8v/3.0v source select to PMU1830_VOL */
	rk_setreg(&pmugrf->soc_con0, 1 << PMUGRF_CON0_VSEL_SHIFT);
}

static int get_ethaddr_from_eeprom(u8 *addr, u8 offset)
{
	int ret;
	struct udevice *dev;

	ret = uclass_first_device_err(UCLASS_I2C_EEPROM, &dev);
	if (ret)
		return ret;

	return i2c_eeprom_read(dev, offset, addr, 6);
}

static int set_mac_address_from_eeprom(void)
{
	u8 ethaddr[6];

	debug("EEPROM read, address: 0x%x offset: 0x%x\n", CONFIG_SYS_DEF_EEPROM_ADDR, CONFIG_SYS_DEF_EEPROM_MAC_OFFSET);
	get_ethaddr_from_eeprom(ethaddr, CONFIG_SYS_DEF_EEPROM_MAC_OFFSET);	// TODO check return value

	if (is_valid_ethaddr(ethaddr))
	{
		debug("Setting MAC address read from EEPROM: %x:%x:%x:%x:%x:%x\n",
				ethaddr[0], ethaddr[1], ethaddr[2], ethaddr[3], ethaddr[4], ethaddr[5]);
		eth_env_set_enetaddr("ethaddr", ethaddr);
		return 0;
	}
	else
	{
		debug("ethaddr from EEPROM is not valid\n");
		return -1;
	}
}

static int set_mac_address_from_efuse(void)
{
	const u32 cpuid_offset = 0x7;
	const u32 cpuid_length = 0x10;
	u8 cpuid[cpuid_length];
	int ret;

	setup_iodomain();

	ret = rockchip_cpuid_from_efuse(cpuid_offset, cpuid_length, cpuid);
	if (ret)
		return ret;

	ret = rockchip_cpuid_set(cpuid, cpuid_length);
	if (ret)
		return ret;

	ret = rockchip_setup_macaddr();

	return ret;
}

static void set_mac_address(void)
{
	/* Only set a MAC address, if none is set in the environment */
	if (env_get("ethaddr"))
	{
		debug("MAC address already set in environment\n");
		return;
	}

	set_mac_address_from_eeprom();

	// wont't do anything if env variable ethaddr is already set by set_mac_address_from_eeprom()
	set_mac_address_from_efuse();

	return;
}

int misc_init_r(void)
{
	set_mac_address();
	return 0;
}
#endif
