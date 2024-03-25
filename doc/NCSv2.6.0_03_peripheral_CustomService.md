<sup>SDK version: NCS v2.6.0  -  Link to Hands-on solution: https://github.com/ChrisKurz/Bluetooth/tree/main/Workspace/NCSv2.6.0/03_Peripheral_CustomService</sup>

# Bluetooth Low Energy: Peripheral with a user-defined Service (Custom Service) - _Write without Response_

## Introduction

The Bluetooth Standard mentions different data transfer operations. An overview is shown in this picture:

![](images/03_TransferOperations.jpg)

In this hands-on we use the "Write without Response" transfer operation. A smartphone takes over the role as the GATT Client, and a development kit (e.g. nRF52840DK) is used as the GATT Server. 


## Required Hardware/Software
- Development kit [nRF52840DK](https://www.nordicsemi.com/Products/Development-hardware/nRF52840-DK), [nRF52833DK](https://www.nordicsemi.com/Products/Development-hardware/nRF52833-DK), or [nRF52DK](https://www.nordicsemi.com/Products/Development-hardware/nrf52-dk)
- a smartphone ([Android](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=de&gl=US&pli=1) or [iOS](https://apps.apple.com/de/app/nrf-connect-for-mobile/id1054362403)), which runs the __nRF Connect__ app 
- install the _nRF Connect SDK_ v2.6.0 and _Visual Studio Code_. The installation process is described [here](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/lessons/lesson-1-nrf-connect-sdk-introduction/topic/exercise-1-1/).



## Hands-on step-by-step description

### Prepare the project

1) Make a copy of the project [Peripheral with Device Information Service](https://github.com/ChrisKurz/Bluetooth/tree/main/Workspace/NCSv2.6.0/02_Peripheral_DIS). We will add a custom service and characteristic to this project.

2) Add new folder "services" to the project. Create the files CustomService.c and CustomService.h in this new folder.

   So, the file/folder structure in your project folder should look like this:

   ![image](images/03_ProjectFolder.jpg)

3) Add CustomSerice.c file to your project by changing the CMakeLists.txt file. The whole file should then look like this:
	
	  _CMakeLists.txt_
	  
       # SPDX-License-Identifier: Apache-2.0

       cmake_minimum_required(VERSION 3.21.0)

       # Find external Zephyr project, and load its settings
       find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

       # Set project name
       project(MyPeripheralCusSer)

       # Add sources
       target_sources(app PRIVATE src/main.c
                                  services/CustomService.c)
			
			
### Adding Custom Service

4) We need a transmission buffer for transmitting data. Add following lines to CustomService.c:

	<sup>_services/CustomService.c_</sup>
	
       #include "CustomService.h"

       #define MAX_TRANSMIT_SIZE 240
       uint8_t data_buffer[MAX_TRANSMIT_SIZE];

       int CustomService_init(void)
       {   int err = 0;
   
           memset(&data_buffer, 0, MAX_TRANSMIT_SIZE);
           return err;
       }

5) Add the declaration of CustomService_init() function to header file __CustomService.h__:

	<sup>_services/CustomService.h_</sup>

       int CustomService_init(void); 

6) We need a UUID for the custom service and also for the custom feature. Create two UUIDs at https://www.uuidgenerator.net. And add them to CusomtService.c:
  
	<sup>_services/CustomService.c_</sup>

       /*Note that UUIDs have Least Significant Byte ordering */
       #define CUSTOM_SERVICE_UUID        0xd4, 0x86, 0x48, 0x24, 0x54, 0xB3, 0x43, 0xA1, 0xBC, 0x20, 0x97, 0x8F, 0xC3, 0x76, 0xC2, 0x75                       
       #define CUSTOM_CHARACTERISTIC_UUID 0xED, 0xAA, 0x20, 0x11, 0x92, 0xE7, 0x43, 0x5A, 0xAA, 0xE9, 0x94, 0x43, 0x35, 0x6A, 0xD4, 0xD3

   __Note:__ Sometimes a random UUID is generated for the Service only and the Characteristic only uses an incremented Service UUID (_Service UUID_ + 1). 

7) The custom UUIDs have to be declared. Add following lines in CustomService.c:

	<sup>_services/CustomService.c_</sup>

       #define BT_UUID_CUSTOM_SERIVCE         BT_UUID_DECLARE_128(CUSTOM_SERVICE_UUID)
       #define BT_UUID_CUSTOM_CHARACTERISTIC  BT_UUID_DECLARE_128(CUSTOM_CHARACTERISTIC_UUID)

   This also requires to add the bluetooth uuid.h header file to the CustomService.c file:

	<sup>_services/CustomService.c_</sup>
	
       #include <zephyr/bluetooth/uuid.h>

8) We need to define and register our service and its characteristics. By using the following helper macro we statically register our Service in our BLE host stack.

   Add the following lines __after the lines we added in step 7__ in CustomService.c:

<sup>_services/CustomService.c_</sup>

    /* Custom Service Declaration and Registration */
    BT_GATT_SERVICE_DEFINE(CustomService,
                    BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERIVCE),
                    BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHARACTERISTIC,
                                           BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                                           BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, 
                                           NULL, on_receive, NULL),
    );

9) And this also requires the gatt.h header file. Include it in the CustomServices.c file:
   
   <sup>_services/CustomService.c_</sup>
   
       #include <zephyr/bluetooth/gatt.h>

### Add data transfer (Write without Response) to the project

10) Now add the function that takes care about getting a trigger if data is received. Add the following to CustomService.c just before the __BT_GATT_SERVICE_DEFINE()__ macro is called. This is needed, because the macro defines to call __on_receive()__ function!

	<sup>_services/CustomService.c_</sup>

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

### Handling of Custom Service in main() function

11) Include CustomService.h in your main.c file. Add following line in main.c:

	<sup>_src/main.c_</sup>

        #include "../services/CustomService.h"

   __Note:__ The relative path of the header file is used here. To make it more readable, the include path can also be defined in the CMakeLists.txt file by inserting the following line: __include_directories( services/ )__ This allows to use __#include "CustomService.h"__. Further info about this CMake command can be found [here](https://cmake.org/cmake/help/latest/command/include_directories.html).
   

12) We initialize our service by adding the following after enabling the Bluetooth stack in main.c:

	<sup>_src/main.c_ => main() function</sup>

             //Initalize services
             err = CustomService_init();
             if (err) {
                 printk("Custom Service initialization failed!\n");
                 return 0;
             }

13) Update the Bluetooth Device name by changing the CONFIG_BT_DEVICE_NAME setting in the prj.conf file. 

	<sup>_prj.conf_</sup>

        CONFIG_BT_DEVICE_NAME="Custom Service Peripheral"


### Testing

14) Finally, build the project ("Pristine Build"!!!). 
 
15) Use the _Serial Terminal_ to check the debug output. First connect Terminal, then perfrom a reset by pressing the reset button on the development kit. Following output should be seen on the terminal:
    
    ![](images/03_Testing_startAdvertising_NCSv2.6.0.jpg)
    
16) Use the _nRF Connect_ Smartphone app and start scanning. The app should find our device (device name: "Custom Service Peripheral")
    
    ![](images/03_Testing_Scanning.jpg)
    
17) Click in the smartphone app the "Connect" button. Now a connection between the smartphone and the developement kit is established. In the Terminal you should see that the device went into "Connected" mode. 
    
    ![](images/03_Testing_connected_NCSv2.6.0.jpg)
    
18) And the smartphone should list the GATT database content in the "Client" tab:
    
    ![](images/03_Testing_GATT.jpg)
    
    In the GATT database you find an "Unkown Service" and an "Unknown Characteristic". Check its UUIDs and compare it to the UUIDs we defined in step 6.

19) Open the "Unknown Characteristic" (click on the button with the arrow beside this chacteristic) and enter a hex value. For example: CAFE
    
    ![](images/03_Testing_testString.jpg)
    
    Click on the "Write" button. 
    
20) In the Terminal program you should see that the hex values were received:
    
    ![](images/03_Testing_received_NCSv2.6.0.jpg)
