#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static int channel_get_my_impl(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    LOG_INF("channel_get called for channel %d", chan);
    return 0;
}

static int sample_fetch_my_impl(const struct device *dev,
                                enum sensor_channel chan)
{
    LOG_INF("sample_fetch called for channel %d", chan);
    return 0;
}

//init function
static int init(const struct device *dev)
{
    LOG_INF("our driver is initialized!");
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get  = channel_get_my_impl,
    .sample_fetch = sample_fetch_my_impl,
};

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);