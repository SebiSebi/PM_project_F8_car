# Fast and Furios Car

- [Intro](#intro)
- [Using the car](#using-the-car)
- [BT communication](#bt-communication)
- [Feature Summary](#feature-summary)
- [Build and run usage](#build-and-run-usage)

Intro
-----

This are the source code and other important files for the design of my remote controlled car. The structure of the **src**
folder is the following:
  *  **/src/remote** - the code that runs on the remote uC;
  *  **/src/car** - the code that runs on the car uC;
  *  **/src/shared** - code that is shared by both the car and the remote control;

Using the car
-------------

To play with the car you first have to plug into the battery into the car and then into the remote control. This order
**must** be followed accordingly. If the connection between the car and the remote has been established the green
LED on the car should start to turn on and off a couple of times per second. By default, the car is controlled using
the joystick button. You can switch between joystick and accelerometer using the push button on the remote.

BT communication
----------------

The master Bluetooth module is set to bind to the address **98:D3:31:FD:44:70** which is the MAC address of the slave
bluetooth device. The pairing between the devices takes a couple of seconds (depending on the physical distance
between the two devices). After the devices had connected I have implemented a handshake protocol to make sure
the communication is runnuing smoothly. The handshake protocol is described in the bluetooth.c source file.

Feature Summary
---------------

1. 2 modes of operation: joystick and rotating the RC;
2. safe minimum distance and collision control;
3. horn
4. 4x4 car

Build and run usage
-------------------

In order to program the uC (car or RC) you have to follow this steps:
1. Run **make** in the directory car or remote;
2. Run **burn_car.sh** or **burn_remote.sh** from the root directory.


**Enjoy!**


