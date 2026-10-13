#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "WiFi/config_WiFi.h"
#include "WiFi/WLAN_init.h"

int WLAN_init(void)
{
    printf("Starting WLAN initialization...\n");

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Pico not connected to Wi-Fi.\n");
        printf("Failed to connect within the timeout period.\n");
        printf("Program will continue without Wi-Fi.\n");
    } else {
        printf("Connected.\n");
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("IP address %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    return 0;
}