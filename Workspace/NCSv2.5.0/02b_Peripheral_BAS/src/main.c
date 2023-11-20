#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h> 
#include <zephyr/bluetooth/uuid.h>

#include <zephyr/bluetooth/services/dis.h>
#include <zephyr/bluetooth/services/bas.h>

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

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

int main (void) 
{
    uint8_t battLevel=100;

    printk("Starting MyPeripheral application!\n");

    bt_conn_cb_register(&conn_callbacks);

    int err;

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return 0;
    }
    printk("Bluetooth initialized\n");

    start_advertising();

    while(1){
        battLevel=battLevel-1;
        if (battLevel==0) {
            battLevel=100;
        }
        k_sleep(K_SECONDS(1));

        err=bt_bas_set_battery_level(battLevel);
        if(err!=0){
            printk("Error: could not set battery level! (Error Code= %i)\n",err);
        }
        printk("Battery Level = %i\n",battLevel);
    }

    return 0;
}