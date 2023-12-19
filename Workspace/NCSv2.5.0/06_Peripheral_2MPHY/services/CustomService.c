#include "CustomService.h"
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>   


#define MAX_TRANSMIT_SIZE 240
uint8_t data_buffer[MAX_TRANSMIT_SIZE];

/*Note that UUIDs have Least Significant Byte ordering */
#define CUSTOM_SERVICE_UUID        0xd4, 0x86, 0x48, 0x24, 0x54, 0xB3, 0x43, 0xA1, 0xBC, 0x20, 0x97, 0x8F, 0xC3, 0x76, 0xC2, 0x75                       
#define CUSTOM_CHARACTERISTIC_UUID 0xED, 0xAA, 0x20, 0x11, 0x92, 0xE7, 0x43, 0x5A, 0xAA, 0xE9, 0x94, 0x43, 0x35, 0x6A, 0xD4, 0xD3

#define BT_UUID_CUSTOM_SERIVCE         BT_UUID_DECLARE_128(CUSTOM_SERVICE_UUID)
#define BT_UUID_CUSTOM_CHARACTERISTIC  BT_UUID_DECLARE_128(CUSTOM_CHARACTERISTIC_UUID)


int CustomService_init(void)
{   int err = 0;

    memset(&data_buffer, 0, MAX_TRANSMIT_SIZE);
    return err;
}

/* This function is called whenever the RX Characteristic has been written to by a Client */
ssize_t on_receive(struct bt_conn *conn,
                          const struct bt_gatt_attr *attr,
                          const void *buf,
                          uint16_t len,
                          uint16_t offset,
                          uint8_t flags)
{
   const uint8_t * buffer = buf;

   printk("Received data: handle=%d, conn=%p, data=0x", attr->handle, conn);
   for(uint8_t i = 0; i < len; i++){
        printk("%02X", buffer[i]);
   }
   printk("\n");
   return len;
}

/* Custom Service Declaration and Registration */
BT_GATT_SERVICE_DEFINE(CustomService,
                BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERIVCE),
                BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHARACTERISTIC,
                                       BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                                       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, 
                                       NULL, on_receive, NULL),
);