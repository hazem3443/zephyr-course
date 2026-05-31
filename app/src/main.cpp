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
        const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val;

        if (!device_is_ready(dev)) {
            LOG_ERR("our_driver0 not found or not ready");
            return -ENODEV;
        }
        auto ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("sensor_channel_get returned %d", ret);

        const struct device *dev1 = DEVICE_DT_GET(DT_NODELABEL(our_driver1));
        struct sensor_value val1;
        if (!device_is_ready(dev1)) {
            LOG_ERR("our_driver1 not found or not ready");
            return -ENODEV;
        }
        sensor_sample_fetch_chan(dev1, SENSOR_CHAN_AMBIENT_TEMP);
        auto ret1 = sensor_channel_get(dev1, SENSOR_CHAN_AMBIENT_TEMP, &val1);
        LOG_INF("sensor_channel_get returned %d", ret1);

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
