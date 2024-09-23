# ESP32_Microbit_BLE
Establish the Bluetooth connection between ESP32 and micro:bit.
Preparation:
1. Import the microbit-BluetoothTest02.hex(in the /Microbit_Python) in the Microsoft MakeCode (https://makecode.microbit.org), and download the program to the micro:bit board.
2. Modify the variable *serverBtDeviceName* in the source code according to your own micro:bit board. You can check it in the mobile app (e.g. BLE Scanner):
serverBtDeviceName="BBC micro:bit [pogiv]";    // It should be modified according to your own board.
3. Upload the sketch to the ESP32 board(BiBoard) and connect the double-touch sensor to Pins 34 and 35 on the extension hat.
4. Power on the micro:bit board first
5. Power on the BiBoard by battery and connect it to the computer with a USB type-C data cable, then open the serial monitor.

After the ico on micro:bit board change to ![image](https://github.com/user-attachments/assets/e7946543-c4f3-45f8-a179-c942b8335558), it means the BLE is connected successfully between the  micro:bit board and BiBoard.

When you touch one side of the touch sensor, the icon Heart appears on the micro:bit board; touching the other side of the touch sensor clears the icon Heart.

If you press the B button on the micro:bit board. In the serial monitor, the *photoIndex* value will be printed, and it'll be increased from the current value(default value is 1) to 9;

If you press the A button on the micro:bit board. In the serial monitor will print the *photoIndex* value, which will be decreased from its current value to 1.


