#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Sensor shell ready.");
    LOG_INF("Try: sensor fetch <dev>  sensor read <dev>  sensor info <dev>");
    return 0;
}
