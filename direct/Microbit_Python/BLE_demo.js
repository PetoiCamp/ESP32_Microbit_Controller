bluetooth.onUartDataReceived(serial.delimiters(Delimiters.NewLine), function on_uart_data_received() {
    
    tempMsg = bluetooth.uartReadUntil(serial.delimiters(Delimiters.NewLine))
    if (tempMsg == "heart") {
        basic.showIcon(IconNames.Heart)
    } else if (tempMsg == "clear") {
        basic.clearScreen()
    }
    
})
bluetooth.onBluetoothConnected(function on_bluetooth_connected() {
    basic.showIcon(IconNames.Yes)
})
bluetooth.onBluetoothDisconnected(function on_bluetooth_disconnected() {
    basic.showIcon(IconNames.No)
})
input.onButtonPressed(Button.A, function on_button_pressed_a() {
    bluetooth.uartWriteString("previous")
    basic.showString("A")
})
input.onButtonPressed(Button.B, function on_button_pressed_b() {
    bluetooth.uartWriteString("next")
    basic.showString("B")
})
let tempMsg = ""
basic.showIcon(IconNames.No)
tempMsg = ""
bluetooth.startUartService()
