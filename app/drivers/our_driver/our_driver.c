#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_data {
    int counter;
};

/* sample_fetch: decrement the per-instance counter */
static int sample_fetch_my_impl(const struct device *dev,
                                enum sensor_channel chan)
{
    struct our_driver_data *data = dev->data;

    data->counter--;
    LOG_INF("%s sample_fetch: counter = %d", dev->name, data->counter);
    return 0;
}

/* channel_get: increment the per-instance counter and return it */
static int channel_get_my_impl(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    struct our_driver_data *data = dev->data;

    data->counter++;
    val->val1 = data->counter;
    val->val2 = 0;
    LOG_INF("%s channel_get: counter = %d", dev->name, data->counter);
    return 0;
}

static int init(const struct device *dev)
{
    struct our_driver_data *data = dev->data;

    data->counter = 0;
    LOG_INF("%s initialized, counter = 0", dev->name);
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .sample_fetch = sample_fetch_my_impl,
    .channel_get  = channel_get_my_impl,
};

#define OUR_DRIVER_DATA(inst) \
    static struct our_driver_data our_driver_data_##inst = { \
        .counter = 0, \
    };

#define DEV_INST(inst) \
    OUR_DRIVER_DATA(inst) \
    DEVICE_DT_INST_DEFINE(inst, init, NULL, &our_driver_data_##inst, NULL, \
                          POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);