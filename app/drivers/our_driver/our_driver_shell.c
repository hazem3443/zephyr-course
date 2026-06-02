#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static int cmd_channel_get_handler(const struct shell *shell, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);
    if (!dev) {
        shell_error(shell, "Device not found: %s", argv[1]);
        return -EFAULT;
    }
    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &val);

    if (ret) {
        shell_error(shell, "Failed to get channel: %d", ret);
        return -EFAULT;
    }
    shell_info(shell, "Channel of my driver is: %d", val.val1);
    // shell_warn(shell, "Channel of my driver is: %d", 5);
    // shell_error(shell, "Channel of my driver is: %d", 5);
    // shell_fprintf(shell, /* purple */ SHELL_OPTION ,"Channel of my driver is: %d\n", 5);
    // shell_fprintf(shell, /* purple */ SHELL_VT100_COLOR_BLUE ,"Channel of my driver is: %d\n", 5);
    // shell_fprintf(shell, /* purple */ SHELL_VT100_COLOR_MAGENTA ,"Channel of my driver is: %d\n", 5);
    // shell_fprintf(shell, /* purple */ SHELL_VT100_COLOR_CYAN ,"Channel of my driver is: %d\n", 5);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
    SHELL_CMD_ARG(channel_get, NULL, "Get channel of my driver", cmd_channel_get_handler, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(our_driver, &our_driver_subcmd, "Commands for our driver shell", NULL);