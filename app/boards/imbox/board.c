#include <zephyr/init.h>
#include <zephyr/sys/printk.h>
#include <soc/gpio_reg.h>
#include <esp_rom_sys.h>

/*
 * Raw ESP32 GPIO helpers — used before the Zephyr GPIO driver is up.
 * All three LEDs are GPIO_ACTIVE_LOW (pin LOW = LED on).
 *
 * LED assignments (from imbox.dts):
 *   Red   = GPIO 23
 *   Green = GPIO 22
 *   Blue  = GPIO 21
 *
 * sys_write32 is not available in early board library context on Xtensa;
 * use direct volatile 32-bit register writes instead.
 */
#define RAW_WRITE32(addr, val)  (*((volatile uint32_t *)(addr)) = (val))

#define LED_MASK  (BIT(23) | BIT(22) | BIT(21))

/* Route pins to simple GPIO output (SIG_GPIO_OUT_IDX = 256) */
#define GPIO_FUNC_OUT_GPIO  256U

static inline void leds_raw_init(void)
{
	/* Select simple GPIO output function for GPIO 21, 22 and 23 */
	RAW_WRITE32(GPIO_FUNC21_OUT_SEL_CFG_REG, GPIO_FUNC_OUT_GPIO);
	RAW_WRITE32(GPIO_FUNC22_OUT_SEL_CFG_REG, GPIO_FUNC_OUT_GPIO);
	RAW_WRITE32(GPIO_FUNC23_OUT_SEL_CFG_REG, GPIO_FUNC_OUT_GPIO);

	/* Set output direction */
	RAW_WRITE32(GPIO_ENABLE_W1TS_REG, LED_MASK);

	/* Start with LEDs off (pins HIGH, active-low) */
	RAW_WRITE32(GPIO_OUT_W1TS_REG, LED_MASK);
}

static inline void leds_on(void)
{
	/* Drive pins LOW → LEDs on */
	RAW_WRITE32(GPIO_OUT_W1TS_REG, LED_MASK);
}

static inline void leds_off(void)
{
    /* Drive pins HIGH → LEDs off */
    RAW_WRITE32(GPIO_OUT_W1TC_REG, LED_MASK);
}

void board_early_init_hook(void)
{
	/* Blink red + green (≈ white) three times to signal power-on */
	leds_raw_init();

	for (int i = 0; i < 3; i++) {
		leds_on();
		esp_rom_delay_us(200000); /* 200 ms on */
		leds_off();
		esp_rom_delay_us(200000); /* 200 ms off */
	}

	printk("Board Initialized\n");
}