def bigChange(currValue: number, lastValue: number):
    if currValue - lastValue >= 20 or lastValue - currValue >= 20:
        return 1
    else:
        return 0

def on_bluetooth_connected():
    basic.show_icon(IconNames.YES)
bluetooth.on_bluetooth_connected(on_bluetooth_connected)

def on_bluetooth_disconnected():
    basic.show_icon(IconNames.NO)
bluetooth.on_bluetooth_disconnected(on_bluetooth_disconnected)

joystickbit.init_joystick_bit()
basic.show_icon(IconNames.NO)
bluetooth.start_uart_service()
basic.pause(1000)
lastX = 508
lastY = 508
currX = 508
currY = 508
serial.redirect_to_usb()
serial.set_baud_rate(BaudRate.BAUD_RATE115200)

def on_forever():
    global currX, currY, lastX, lastY
    currX = joystickbit.get_rocker_value(joystickbit.rockerType.X)
    currY = joystickbit.get_rocker_value(joystickbit.rockerType.Y)
    if joystickbit.get_button(joystickbit.JoystickBitPin.P12):
        basic.show_string("C")
        bluetooth.uart_write_string("crawl")
    if joystickbit.get_button(joystickbit.JoystickBitPin.P13):
        basic.show_string("D")
        bluetooth.uart_write_string("walk")
    if joystickbit.get_button(joystickbit.JoystickBitPin.P14):
        basic.show_string("E")
        bluetooth.uart_write_string("trot")
    if joystickbit.get_button(joystickbit.JoystickBitPin.P15):
        basic.show_string("F")
        bluetooth.uart_write_string("rest")
    if input.button_is_pressed(Button.A):
        basic.show_string("A")
        bluetooth.uart_write_string("hello")
    if input.button_is_pressed(Button.B):
        basic.show_string("B")
        bluetooth.uart_write_string("sit")
    if bigChange(currX, lastX) == 1 or bigChange(currY, lastY) == 1:
        if currX >= 500 and currX <= 550 and (currY >= 500 and currY <= 550):
            basic.pause(800)
            bluetooth.uart_write_string("balance")
            basic.clear_screen()
            basic.pause(200)
        else:
            if currX <= 200:
                bluetooth.uart_write_string("right")
                basic.show_leds("""
                    . . # . .
                    . . . # .
                    # # # # #
                    . . . # .
                    . . # . .
                    """)
            if currX >= 800:
                bluetooth.uart_write_string("left")
                basic.show_leds("""
                    . . # . .
                    . # . . .
                    # # # # #
                    . # . . .
                    . . # . .
                    """)
            if currY <= 200:
                serial.write_value("y", currY)
                bluetooth.uart_write_string("back")
                basic.show_leds("""
                    . . # . .
                    . . # . .
                    # . # . #
                    . # # # .
                    . . # . .
                    """)
            if currY >= 800:
                bluetooth.uart_write_string("forward")
                basic.show_leds("""
                    . . # . .
                    . # # # .
                    # . # . #
                    . . # . .
                    . . # . .
                    """)
            basic.pause(200)
        lastX = currX
        lastY = currY
basic.forever(on_forever)
