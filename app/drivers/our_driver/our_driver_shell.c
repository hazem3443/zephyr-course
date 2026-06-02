#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <stdlib.h>
#include "our_driver.h"

/* sensor fetch <device_name> */
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);

    if (ret) {
        shell_error(sh, "sensor_sample_fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "%s: sample fetched", dev->name);
    return 0;
}

/* sensor read <device_name> */
static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

    if (ret) {
        shell_error(sh, "sensor_channel_get failed: %d", ret);
        return ret;
    }

    shell_print(sh, "%s: counter = %d", dev->name, val.val1);
    return 0;
}

/* sensor info <device_name> */
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    shell_print(sh, "name:  %s", dev->name);
    shell_print(sh, "ready: %s", device_is_ready(dev) ? "yes" : "no");
    return 0;
}

/* sensor set <device_name> <value>
 *   Calls our_driver_set_counter() extension API.
 *   value must be a decimal integer in [-1000, 1000].
 */
static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Device not found: %s", argv[1]);
        return -ENODEV;
    }

    char *end;
    long val = strtol(argv[2], &end, 10);

    if (*end != '\0') {
        shell_error(sh, "Invalid number: '%s'", argv[2]);
        return -EINVAL;
    }
    if (val < -1000 || val > 1000) {
        shell_error(sh, "Value %ld out of range [-1000, 1000]", val);
        return -EINVAL;
    }

    int ret = our_driver_set_counter(dev, (int)val);

    if (ret) {
        shell_error(sh, "set_counter failed: %d", ret);
        return ret;
    }

    shell_print(sh, "%s: counter set to %d", dev->name, (int)val);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD_ARG(fetch, NULL, "<dev>              Call sensor_sample_fetch()", cmd_sensor_fetch, 2, 0),
    SHELL_CMD_ARG(read,  NULL, "<dev>              Call sensor_channel_get() and print counter", cmd_sensor_read, 2, 0),
    SHELL_CMD_ARG(info,  NULL, "<dev>              Print device name and ready state", cmd_sensor_info, 2, 0),
    SHELL_CMD_ARG(set,   NULL, "<dev> <value>      Set counter via extension API [-1000, 1000]", cmd_sensor_set, 3, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor driver commands", NULL);