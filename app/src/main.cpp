#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* Board-independent: app-led alias is defined in the board overlay */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

namespace {
    int test(){
        const struct device *dev0 = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        const struct device *dev1 = DEVICE_DT_GET(DT_NODELABEL(our_driver1));
        struct sensor_value val0, val1;

        if (!device_is_ready(dev0)) {
            LOG_ERR("our_driver0 not ready");
            return -ENODEV;
        }
        if (!device_is_ready(dev1)) {
            LOG_ERR("our_driver1 not ready");
            return -ENODEV;
        }

        /* --- Drive dev0 counter up to 3 via channel_get --- */
        sensor_channel_get(dev0, SENSOR_CHAN_AMBIENT_TEMP, &val0);  /* dev0 counter = 1 */
        sensor_channel_get(dev0, SENSOR_CHAN_AMBIENT_TEMP, &val0);  /* dev0 counter = 2 */
        sensor_channel_get(dev0, SENSOR_CHAN_AMBIENT_TEMP, &val0);  /* dev0 counter = 3 */
        LOG_INF("dev0 counter after 3x channel_get = %d (expected 3)", val0.val1);

        /* --- dev1 counter stays at 0 --- independent from dev0 --- */
        sensor_channel_get(dev1, SENSOR_CHAN_AMBIENT_TEMP, &val1);  /* dev1 counter = 1 */
        LOG_INF("dev1 counter after 1x channel_get = %d (expected 1)", val1.val1);

        /* --- Decrement dev0 twice via sample_fetch --- */
        sensor_sample_fetch_chan(dev0, SENSOR_CHAN_AMBIENT_TEMP);    /* dev0 counter = 2 */
        sensor_sample_fetch_chan(dev0, SENSOR_CHAN_AMBIENT_TEMP);    /* dev0 counter = 1 */
        sensor_channel_get(dev0, SENSOR_CHAN_AMBIENT_TEMP, &val0);  /* dev0 counter = 2 */
        LOG_INF("dev0 counter after 2x fetch + 1x get = %d (expected 2)", val0.val1);

        /* --- dev1 unaffected by dev0 ops --- */
        sensor_channel_get(dev1, SENSOR_CHAN_AMBIENT_TEMP, &val1);  /* dev1 counter = 2 */
        LOG_INF("dev1 counter after 2x channel_get = %d (expected 2)", val1.val1);

        /* --- Decrement dev1 once, dev0 unchanged --- */
        sensor_sample_fetch_chan(dev1, SENSOR_CHAN_AMBIENT_TEMP);    /* dev1 counter = 1 */
        sensor_channel_get(dev0, SENSOR_CHAN_AMBIENT_TEMP, &val0);  /* dev0 counter = 3 */
        sensor_channel_get(dev1, SENSOR_CHAN_AMBIENT_TEMP, &val1);  /* dev1 counter = 2 */
        LOG_INF("dev0 final = %d (expected 3), dev1 final = %d (expected 2)",
                val0.val1, val1.val1);

        return 0;
    }
}

int main(void)
{
    test();
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED device not ready");
        return 0;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
        LOG_ERR("Failed to configure LED pin");
        return 0;
    }

    LOG_INF("Heartbeat LED started, period: %d ms", CONFIG_APP_HEARTBEAT_PERIOD_MS);

    while (1) {
        gpio_pin_toggle_dt(&led);
        LOG_INF("LED toggled");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}
