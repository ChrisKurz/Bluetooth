#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/services/dis.h>
#include <zephyr/bluetooth/conn.h> 
#include <zephyr/bluetooth/conn.h> 
#include <zephyr/bluetooth/uuid.h>

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_DIS_VAL)),
};

void start_advertising(void)
{
    int err;

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
    }
    else {
        printk("Advertising successfully started\n");
    }
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
         printk("Connection failed (err 0x%02x)\n", err);
    } else {
         printk("Connected\n");
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason 0x%02x)\n", reason);

    start_advertising();
}

/* Register a callback structure for connection events. */
BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
};

int main (void) 
{
    printk("Starting MyPeripheral application!\n");

    int err;

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return 0;
    }
    printk("Bluetooth initialized\n");

    start_advertising();    

    return 0;
}  