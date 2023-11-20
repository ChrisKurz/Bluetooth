#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>

/*  RSSI Parameter has to be calibarted. Add here RSSI level measured at 1m distance */
#define IBEACON_RSSI 0xc8

/*
 * Set iBeacon demo advertisement data. These values are for
 * demonstration only and must be changed for production environments!
 * UUID:  800a9882-3f68-409d-b103-cc9a9269e0fb
 * Major: 0x12 => 18
 * Minor: 0x34 => 52
 * RSSI:  not calibrated
 */
static const struct bt_data ad[] = {
BT_DATA_BYTES(BT_DATA_FLAGS,BT_LE_AD_NO_BREDR|BT_LE_AD_GENERAL),  // Flags[0..3]   
// Length - NOTE: HANDLED AUTOMATICALLY BY bt_data!
BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA,    // Type
              0x4C, 0x00,  // Company ID (Apple): 0x004C
              0x02, 0x15, // Beacon Type - Proximity Beacons 
                    // Proximity UUID was created with https://www.uuidgenerator.net
                    // created UUID:  800a9882-3f68-409d-b103-cc9a9269e0fb
              0x80, 0x0A, 0x98, 0x82,             // Proximity UUID - UUID[15..12] 
              0x3F, 0x68,                         // Proximity UUID - UUID[11..10] 
              0x40, 0x9D,                         // Proximity UUID - UUID[9..8] 
              0xB1, 0x03,                         // Proximity UUID - UUID[7..6] 
              0xCC, 0x9A, 0x92, 0x69, 0xE0, 0xFB, // Proximity UUID - UUID[5..0] 
              0x00, 0x02,   // Major id: 0x0002 = 2
              0x00, 0x03,   // Minor id: 0x0003 = 3
              IBEACON_RSSI) // Measured Power
};

static void bt_ready(int err){
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");

    /* Start advertising */
    err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Advertising failed to start (err %d)\n", err);
    return;
}

    printk("iBeacon started\n");
}

int main (void) {
    int err;

    printk("Starting MyBeacon application!\n");

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(bt_ready);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
    }

    return 0;
}