> #### IoT Assignment #02 - *Smart Drone Hangar*
> 
> Computer Science and Engineering Course @ University of Bologna, Cesena Campus

# DRU GUI — Drone Remote Unit

## How to run with default Serial Port 
In order to start and work properly, the application needs **Arduino to be connected to serial port**. Default port is set to `/dev/cu.usbmodem1201`.

To run the application, simply type in your terminal:
- on UNIX OS:
```sh
./gradlew run
```
- on Windows OS:
```powershell
./gradlew.bat run
```

## Setting the Serial Port
To change the default serial port value, specify the new port as argument to gradlew with the command:
- on UNIX OS:
```sh
./gradlew run --args="my/serial/port"
```
- on Windows OS:
```powershell
./gradlew.bat run --args="my/serial/port"
```
