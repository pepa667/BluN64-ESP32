#include "ble_notify_wrapper.h"
#include "host/ble_gatt.h"

/**
 * @brief Wrapper implementation for ble_gatts_notify_custom
 *
 * Maps to ble_gattc_notify_custom which is the notification API available
 * in the version of NimBLE bundled with ESP-IDF v5.0.
 */
int ble_gatts_notify_custom(uint16_t conn_handle, uint16_t attr_handle, struct os_mbuf *om)
{
    return ble_gattc_notify_custom(conn_handle, attr_handle, om);
}

/**
 * @brief Wrapper implementation for ble_gatts_indicate_custom
 *
 * Maps to ble_gattc_indicate_custom which is the indication API available
 * in the version of NimBLE bundled with ESP-IDF v5.0.
 */
int ble_gatts_indicate_custom(uint16_t conn_handle, uint16_t attr_handle, struct os_mbuf *om)
{
    return ble_gattc_indicate_custom(conn_handle, attr_handle, om);
}
