function bigChange(currValue: number, lastValue: number): number {
    if (currValue - lastValue >= 20 || lastValue - currValue >= 20) {
        return 1
    } else {
        return 0
    }
    
}

bluetooth.onBluetoothConnected(function on_bluetooth_connected() {
    basic.showIcon(IconNames.Yes)
})
bluetooth.onBluetoothDisconnected(function on_bluetooth_disconnected() {
    basic.showIcon(IconNames.No)
})
joystickbit.initJoystickBit()
basic.showIcon(IconNames.No)
bluetooth.startUartService()
basic.pause(1000)
let lastX = 508
let lastY = 508
let currX = 508
let currY = 508
serial.redirectToUSB()
serial.setBaudRate(BaudRate.BaudRate115200)
basic.forever(function on_forever() {
    
    currX = joystickbit.getRockerValue(joystickbit.rockerType.X)
    currY = joystickbit.getRockerValue(joystickbit.rockerType.Y)
    if (joystickbit.getButton(joystickbit.JoystickBitPin.P12)) {
        basic.showString("C")
        bluetooth.uartWriteString("crawl")
    }
    
    if (joystickbit.getButton(joystickbit.JoystickBitPin.P13)) {
        basic.showString("D")
        bluetooth.uartWriteString("walk")
    }
    
    if (joystickbit.getButton(joystickbit.JoystickBitPin.P14)) {
        basic.showString("E")
        bluetooth.uartWriteString("trot")
    }
    
    if (joystickbit.getButton(joystickbit.JoystickBitPin.P15)) {
        basic.showString("F")
        bluetooth.uartWriteString("rest")
    }
    
    if (input.buttonIsPressed(Button.A)) {
        basic.showString("A")
        bluetooth.uartWriteString("hello")
    }
    
    if (input.buttonIsPressed(Button.B)) {
        basic.showString("B")
        bluetooth.uartWriteString("sit")
    }
    
    if (bigChange(currX, lastX) == 1 || bigChange(currY, lastY) == 1) {
        if (currX >= 500 && currX <= 550 && (currY >= 500 && currY <= 550)) {
            basic.pause(800)
            bluetooth.uartWriteString("balance")
            basic.clearScreen()
            basic.pause(200)
        } else {
            if (currX <= 200) {
                bluetooth.uartWriteString("right")
                basic.showLeds(`
                    . . # . .
                    . . . # .
                    # # # # #
                    . . . # .
                    . . # . .
                    `)
            }
            
            if (currX >= 800) {
                bluetooth.uartWriteString("left")
                basic.showLeds(`
                    . . # . .
                    . # . . .
                    # # # # #
                    . # . . .
                    . . # . .
                    `)
            }
            
            if (currY <= 200) {
                serial.writeValue("y", currY)
                bluetooth.uartWriteString("back")
                basic.showLeds(`
                    . . # . .
                    . . # . .
                    # . # . #
                    . # # # .
                    . . # . .
                    `)
            }
            
            if (currY >= 800) {
                bluetooth.uartWriteString("forward")
                basic.showLeds(`
                    . . # . .
                    . # # # .
                    # . # . #
                    . . # . .
                    . . # . .
                    `)
            }
            
            basic.pause(200)
        }
        
        lastX = currX
        lastY = currY
    }
    
})
