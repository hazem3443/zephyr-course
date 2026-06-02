#ifndef APP_DRIVERS_OUR_DRIVER_OUR_DRIVER_H_
#define APP_DRIVERS_OUR_DRIVER_OUR_DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/**
 * @brief our_driver extension API
 *
 * struct sensor_driver_api MUST be the first member so that the device->api
 * pointer is compatible with the standard sensor subsystem (which casts it
 * to struct sensor_driver_api *) while also allowing callers of the
 * extension to cast to struct our_driver_api *.
 */
struct our_driver_api {
    /** Standard sensor API — must remain the first member */
    struct sensor_driver_api sensor;

    /**
     * Extension: directly set the driver's internal counter.
     *
     * @param dev   our_driver device instance
     * @param value New counter value; valid range [-1000, 1000]
     * @return 0 on success, negative errno on failure
     */
    int (*set_counter)(const struct device *dev, int value);
};

/**
 * @brief Inline helper — call the set_counter extension on @p dev.
 */
static inline int our_driver_set_counter(const struct device *dev, int value)
{
    const struct our_driver_api *api =
        (const struct our_driver_api *)dev->api;

    if (api == NULL || api->set_counter == NULL) {
        return -ENOTSUP;
    }
    return api->set_counter(dev, value);
}

#endif /* APP_DRIVERS_OUR_DRIVER_OUR_DRIVER_H_ */
