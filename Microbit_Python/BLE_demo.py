def on_uart_data_received():
    global tempMsg
    tempMsg = bluetooth.uart_read_until(serial.delimiters(Delimiters.NEW_LINE))
    if tempMsg == "heart":
        basic.show_icon(IconNames.HEART)
    elif tempMsg == "clear":
        basic.clear_screen()
bluetooth.on_uart_data_received(serial.delimiters(Delimiters.NEW_LINE),
    on_uart_data_received)

def on_bluetooth_connected():
    basic.show_icon(IconNames.YES)
bluetooth.on_bluetooth_connected(on_bluetooth_connected)

def on_bluetooth_disconnected():
    basic.show_icon(IconNames.NO)
bluetooth.on_bluetooth_disconnected(on_bluetooth_disconnected)

def on_button_pressed_a():
    bluetooth.uart_write_string("previous")
    basic.show_string("A")
input.on_button_pressed(Button.A, on_button_pressed_a)

def on_button_pressed_b():
    bluetooth.uart_write_string("next")
    basic.show_string("B")
input.on_button_pressed(Button.B, on_button_pressed_b)

tempMsg = ""
basic.show_icon(IconNames.NO)
tempMsg = ""
bluetooth.set_transmit_power(7)
bluetooth.start_uart_service()
