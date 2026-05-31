#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_data {
    struct gpio_dt_spec led;
};

/* sample_fetch: turn the LED ON */
static int sample_fetch_my_impl(const struct device *dev,
                                enum sensor_channel chan)
{
    struct our_driver_data *data = dev->data;

    LOG_INF("sample_fetch: LED on (channel %d)", chan);
    gpio_pin_set_dt(&data->led, 0);
    k_msleep(1000);
    return 0;
}

/* channel_get: turn the LED OFF */
static int channel_get_my_impl(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    struct our_driver_data *data = dev->data;

    LOG_INF("channel_get: LED off (channel %d)", chan);
    gpio_pin_set_dt(&data->led, 1);
    k_msleep(1000);
    return 0;
}

static int init(const struct device *dev)
{
    struct our_driver_data *data = dev->data;

    if (!gpio_is_ready_dt(&data->led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }
    gpio_pin_configure_dt(&data->led, GPIO_OUTPUT_INACTIVE);

    LOG_INF("our driver initialized");
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .sample_fetch = sample_fetch_my_impl,
    .channel_get  = channel_get_my_impl,
};

#define OUR_DRIVER_DATA(inst) \
    static struct our_driver_data our_driver_data_##inst = { \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios), \
    };

#define DEV_INST(inst) \
    OUR_DRIVER_DATA(inst) \
    DEVICE_DT_INST_DEFINE(inst, init, NULL, &our_driver_data_##inst, NULL, \
                          POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);