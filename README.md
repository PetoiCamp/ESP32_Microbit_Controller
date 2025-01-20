# Petoi Micro: Bit Remote Controller

Bluetooth automatically connects the Micro: Bit controller to Petoi robots with BiBoard (ESP32). First-come-first-serve.

It supports three models of robots:
- Bittle with Arm
- Bittle
- Nybble
Upgrading the robot to the latest firmware (after Dec.2024) is required.

[![bitControllerGif](https://github.com/PetoiCamp/NonCodeFiles/blob/60d8b026e29721dee3285e874e65342800143d7a/gif/bitController.gif)](https://youtu.be/QLI-x_8qO98)

Click the GIF to open the YouTube demo.

[![Demo in action](https://github.com/PetoiCamp/NonCodeFiles/blob/7891a7aaaa65be7f1bc852528f36f37ceb62ce02/img/joystickGame.png)](https://youtu.be/xYmtNn0ri14)

Click the picture to open the YouTube demo.


## Installation

The codes in the sub-folders are intermediate test codes during R&D. You only need to use microbit-JoyStick.hex for the formal functionality. 

1. Import the microbit-JoyStick.hex on the Microsoft MakeCode (https://makecode.microbit.org) site, and download the program to the Micro: Bit board.
2. Upload the latest OpenCatEsp32 firmware to the (BiBoard).
3. Power on the micro: Bit board first.
4. Power on the Petoi robot.
5. You can keep the Bit powered on and click the reset button on the BiBoard to reconnect.

The Micro: Bit will show ![image](https://github.com/user-attachments/assets/e7946543-c4f3-45f8-a179-c942b8335558), it means the BLE is connected successfully between the Micro: Bit board and BiBoard.

It will play a connection sound and display N for Nybble, B for Bittle, and R for Bittle R (with the robot arm).

## Button layout
![](https://github.com/PetoiCamp/NonCodeFiles/blob/209e80b42aaad9011e9f6286b3bfb2ce2d14585b/img/Joystick_BittleR.png)

```
              Touch       D
                
Joystick    A       B   C   F
  
                          E
```
```
   Touch,    A,          B,           C,     D,     E,       F,       A+B,  C+E,  D+F,  E+F
  
ID:  0       1           2            3      4      5        6         7     8     9    10

Pin:                                  12     13     14      15
```
Touch is the logo in the middle. A and B are the Micro: Bit's built-in buttons.
    
## Instruction
### Joystick (left) alone: 
Controls the robot's walking direction: forward, backward, left, right, and diagonally.             
                      pushing the joystick full left/right turn rotation at the origin
                      Pushing forward twice quickly triggers fast forward.
### Single and double buttons: 
Their functions correspond to the array **keyCmd**. Users can modify the instructions for specific robot types and corresponding buttons. 
                          Currently, only A+B, C+E, D+F, and E+F are used for double-button combinations. 
                          Users can add other combinations like A+F, B+D, etc., but these combinations may be inconvenient to press.
### Button + joystick: 
Pressing and holding a button and then moving the joystick can specify its direction.
                   The default direction is random by a single click and release.

Different models will have slightly different key mapping. 

## Limitations:
The joystick needs 300 milliseconds to separate a double push event, causing the button + joystick forward action to respond slowly.
A single-button behavior could be triggered before the combination key.
When controlling the switch, there may be no response if the buttons are pressed too quickly.

## STL for Frosted LED Diffuser Cover for Micro: Bit
[![Frosted LED Diffuser Cover for Micro: Bit](https://github.com/PetoiCamp/NonCodeFiles/blob/afc51030f50540144f1c7ad6b9c12ecaffc0ed65/img/FrostedLedDiffuserMicroBit.png)](https://makerworld.com/en/models/756956#profileId-691309)

Rongzhong Li

Petoi LLC

Jan. 20, 2025


