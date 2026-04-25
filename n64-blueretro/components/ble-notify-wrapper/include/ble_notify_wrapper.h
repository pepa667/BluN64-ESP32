#ifndef BLE_NOTIFY_WRAPPER_H
#define BLE_NOTIFY_WRAPPER_H

#include <stdint.h>
#include "nimble/ble.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Wrapper for ble_gatts_notify_custom
 * Maps to the appropriate NimBLE API for sending GATT notifications
 */
int ble_gatts_notify_custom(uint16_t conn_handle, uint16_t attr_handle, struct os_mbuf *om);

/**
 * @brief Wrapper for ble_gatts_indicate_custom
 * Maps to the appropriate NimBLE API for sending GATT indications
 */
int ble_gatts_indicate_custom(uint16_t conn_handle, uint16_t attr_handle, struct os_mbuf *om);

#ifdef __cplusplus
}
#endif

#endif // BLE_NOTIFY_WRAPPER_H
