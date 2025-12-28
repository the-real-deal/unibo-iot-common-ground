> #### IoT Assignment 02 - *Smart Drone Hangar*
> 
> Computer Science and Engineering Course @ University of Bologna, Cesena Campus

# DRU GUI — Drone Remote Unit

To run the application, simply type in your terminal:
```sh
./gradlew run
```

## Setting the Serial Port
In order to start and work properly, the application needs **Arduino to be connected to serial port**. Default port is set to `/dev/cu.usbmodem1201`; to change that, specify the new port as argument to gradlew with the command:

```sh
./gradlew run --args="my/serial/port"
```

