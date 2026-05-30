#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(blueledonboard)
#define LED_RED DT_ALIAS(redledexternal)
#define LED_GREEN DT_ALIAS(greenledexternal)
#define LED_BLUE DT_ALIAS(blueledexternal)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(LED_RED, gpios);
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(LED_GREEN, gpios);
static const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(LED_BLUE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;
    if (!gpio_is_ready_dt(&led_red)) return 0;
    if (!gpio_is_ready_dt(&led_green)) return 0;
    if (!gpio_is_ready_dt(&led_blue)) return 0;
    
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    if (gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    if (gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    if (gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;
        if (gpio_pin_toggle_dt(&led_red) < 0) return 0;
        if (gpio_pin_toggle_dt(&led_green) < 0) return 0;
        if (gpio_pin_toggle_dt(&led_blue) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
    }
    return 0;
}
