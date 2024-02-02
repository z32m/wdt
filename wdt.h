#include <zephyr/kernel.h>
#include <zephyr/drivers/watchdog.h>
#include <xglob.h>
#include <xdts.h>

#ifndef WDT_TIMEOUT
#define WDT_TIMEOUT 3000
#endif

typedef struct wdt_timeout_cfg wdt_timeout_cfg_t;

typedef struct
{
    device_t *device;
    int channel;
    wdt_timeout_cfg_t *timeout_cfg;
} wdt_t;

#define DEFINE_WATCHDOG(_wdt_dev)               \
    DEFINE_DEVICE(_wdt_dev);                    \
    wdt_timeout_cfg_t _##_wdt_dev##wdt_cfg =    \
        {                                       \
            .callback = NULL,                   \
            .flags = WDT_FLAG_RESET_SOC,        \
            .window = {                         \
                .min = 0,                       \
                .max = WDT_TIMEOUT}};           \
    wdt_t _##_wdt_dev##_wdt = {.device = NULL}; \
    K_THREAD_DEFINE(wdt_feeder_id, 512, wdt_feeder, &_##_wdt_dev##_wdt, NULL, NULL, 5, 0, 0)

void wdt_feeder(wdt_t *wdt);
void wdt_install(wdt_t *wdt);

#define WDT_INSTALL(_wdt_dev)                              \
    _##_wdt_dev##_wdt.device = _wdt_dev;                   \
    _##_wdt_dev##_wdt.timeout_cfg = &_##_wdt_dev##wdt_cfg; \
    \        
    wdt_install(&_##_wdt_dev##_wdt)