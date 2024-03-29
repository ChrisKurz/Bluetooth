#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h> 
#include <zephyr/bluetooth/uuid.h>
#include "../services/CustomService.h"

static const struct bt_data ad[] = {
         BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
         BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_DIS_VAL)),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed (err 0x%02x)\n", err);
    } else {
        printk("Connected\n");

        if (bt_conn_set_security(conn, BT_SECURITY_L4)) {
            printk("Failed to set security\n");
        }
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason 0x%02x)\n", reason);
}

static void identity_resolved(struct bt_conn *conn, const bt_addr_le_t *rpa, const bt_addr_le_t *identity)
{
    char addr_identity[BT_ADDR_LE_STR_LEN];
    char addr_rpa[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(identity, addr_identity, sizeof(addr_identity));
    bt_addr_le_to_str(rpa, addr_rpa, sizeof(addr_rpa));

    printk("Identity resolved %s -> %s\n", addr_rpa, addr_identity);
}

static void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err)
{
    if (!err) {
        printk("Security changed: level %u\n", level);
    } else {
        printk("Security failed: level %u err %d\n", level, err);
    }
}

static struct bt_conn_cb conn_callbacks = {
           .connected = connected,
           .disconnected = disconnected,
           .identity_resolved = identity_resolved,
           .security_changed = security_changed,      
};

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
     printk("Passkey: %06u\n", passkey);
}

static void pairing_failed(struct bt_conn *conn, enum bt_security_err reason)
{
    printk("Pairing Failed (%d). Disconnecting.\n", reason);
    bt_conn_disconnect(conn, BT_HCI_ERR_AUTH_FAIL);
}

static void pairing_complete(struct bt_conn *conn, bool bonded)
{
    printk("Pairing Complete\n");
}

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
    printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
        .passkey_display = auth_passkey_display,
        .passkey_entry = NULL,
        .cancel = auth_cancel,       
};

static struct bt_conn_auth_info_cb auth_cb_info = {
        .pairing_failed = pairing_failed,
        .pairing_complete = pairing_complete,
};

void main (void) {
    int err;

    printk("Starting MyPeripheral application!");

    bt_conn_cb_register(&conn_callbacks);

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }
    printk("Bluetooth initialized\n");

    //Initalize services
    err = CustomService_init();
    if (err) {  
        printk("Custom Service initialization failed!");
        return;
    }

    bt_conn_auth_cb_register(&auth_cb_display);
    bt_conn_auth_info_cb_register(&auth_cb_info);

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }

    printk("Advertising successfully started\n");

}
