# Bluetooth LE

In this repository you will find several step-by-step hands-on descriptions showing how to use the Nordic Semiconductor Bluetooth Stack included in the _nRF Connect SDK_. 

Here is a list of available hands-on exercises:

## Beacon

- [iBeacon](doc/NCSv2.5.0_01_Beacon.md) - initialize bluetooth stack and realize a broadcaster (creating project from scratch)

## Bluetooth Services

### Bluetooth Services offered in Zephyr Repository (NCS folder: zephyr/subsys/bluetooth/services)

- [BLE Peripheral with Device Information Service](doc/NCSv2.5.0_02_peripheral_Service_DeviceInformation.md) - allow to establish a connection. Example uses GATT Service, which is available within NCS (Device Information Service).

- [BLE Peripheral with Immediate Alert Service](doc/NCSv2.5.0_02a_peripheral_Service_ImmediateAlert.md) - Example uses another GATT Service, which is available within NCS (Immediate Alert Service).

- [BLE Peripheral with Battery Service](doc/NCSv2.5.0_02b_peripheral_Service_Battery.md) - Here a Battery Service (BAS) is added to a project. Battery level measurement is emulated here.

### Bluetooth Services offered in NRF Repository (NCS folder: nrf/subsys/bluetooth/services)

### Custom Bluetooth Services

- [Adding a Custom Service (_Write without Response_)](doc/NCSv2.5.0_03_peripheral_CustomService.md) - here we add a custom service and characteristic to the previous project. This example uses the Bluetooth LE Data Transfer Operation "Write Without Response".

## Bluetooth Stack Feature Settings

- [Using 2M-PHY](doc/NCSv2.3.0_06_peripheral_2MPHY.md) - requesting 2M-PHY usage. 

## Bluetooth Security

- [Enable Securtiy on a Bluetooth Peripheral](doc/NCSv2.3.0_10_peripheral_EnableSecurity.md)
