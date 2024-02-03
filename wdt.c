#include <wdt.h>
#include <xlog.h>

void wdt_feeder(wdt_t *wdt)
{
    X_WAIT_FOR(wdt->device);

    while (true)
    {
        wdt_feed(wdt->device, wdt->channel);
        k_msleep(wdt->timeout_cfg->window.max / 2);
    }
}

void wdt_install(wdt_t *wdt)
{
    wdt->channel = wdt_install_timeout(wdt->device, wdt->timeout_cfg);
    __ASSERT(wdt->channel >= 0, "unable to setup watchdog, err: %d", wdt->channel);
    SURE(wdt_setup, wdt->device, 0);
}