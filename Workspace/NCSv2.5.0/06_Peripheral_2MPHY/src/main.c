#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h> 
#include <zephyr/bluetooth/uuid.h>

#include <zephyr/bluetooth/services/dis.h>
#include "../services/CustomService.h"

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

static void request_phy_update(struct bt_conn *conn)
{
    int err;

    err = bt_conn_le_phy_update(conn, BT_CONN_LE_PHY_PARAM_2M);
    if (err) {
        printk("PHY update request failed: %d\n",  err);
    }
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed (err 0x%02x)\n", err);
    } else {
        printk("Connected\n");
        printk("Request PHY Update (2M PHY)\n");
        request_phy_update(conn);         
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason 0x%02x)\n", reason);

    start_advertising();
}

static const char *phy2str(uint8_t phy)
{
    switch (phy) {
        case 0: return "No packets";
        case BT_GAP_LE_PHY_1M: return "LE 1M PHY";
        case BT_GAP_LE_PHY_2M: return "LE 2M PHY";
        case BT_GAP_LE_PHY_CODED: return "LE Coded PHY";
        default: return "Unknown";
    }
}

static void le_phy_updated(struct bt_conn *conn,
        struct bt_conn_le_phy_info *param)
{
    printk("LE PHY updated: TX PHY %s, RX PHY %s\n",
             phy2str(param->tx_phy), phy2str(param->rx_phy));
}

/* Register a callback structure for connection events. */
BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
    .le_phy_updated = le_phy_updated,
};

/* --- THIS IS ANOTHER WAY TO DEFINE THE CALLBACK STRUCTURE FOR CONNECTION EVENTS. */
/* --- USE EITHER THE BT_CONN_CB_DEFINE() MACRO OR THE FOLLOWING STRUCTURE DEFINITION */
/* >>> In this project we use the macro. Therefore the following lines are commented out. */
//static struct bt_conn_cb conn_callbacks = {
//    .connected = connected,
//    .disconnected = disconnected,
//};


int main (void) 
{
    int err;

    printk("2M-PHY example\n");

/* --- THIS IS ANOTHER WAY TO DEFINE THE CALLBACK STRUCTURE FOR CONNECTION EVENTS. */
/* --- USE EITHER THE BT_CONN_CB_DEFINE() MACRO OR THE FOLLOWING STRUCTURE DEFINITION */
/* >>> In this project we use the macro. Therefore the following lines are commented out. */
//    /* Register a callback structure for connection events. */
//    bt_conn_cb_register(&conn_callbacks);

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return 0;
    }
    printk("Bluetooth initialized\n");

    //Initalize services
    err = CustomService_init();
    if (err) {
        printk("Custom Service initialization failed!\n");
        return 0;
    }

    start_advertising();    

    return 0;
}