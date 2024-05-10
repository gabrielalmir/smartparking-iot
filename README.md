# Smart Parking System

This project is a smart parking system that utilizes infrared sensors to detect the presence of vehicles in parking spots. It communicates with a server to update the status of each parking spot and displays real-time information on an LCD screen. The system also connects to Wi-Fi to ensure reliable communication with the server and to display the current Wi-Fi signal strength.

## Features

- **Real-Time Parking Status**: Detects whether a parking spot is occupied or vacant using infrared sensors.
- **Wi-Fi Connectivity**: Connects to Wi-Fi to communicate with a server and display the current Wi-Fi signal strength.
- **Local Time Display**: Displays the current local time on the LCD screen.
- **Server Communication**: Updates the server with the status of each parking spot.

## Hardware Requirements

- Arduino board (e.g., Arduino Uno) or Esp32 Board
- LiquidCrystal_I2C LCD display
- Infrared sensors
- Wi-Fi module (e.g., ESP8266)

## Software Requirements

- Arduino IDE
- HTTPClient library
- LiquidCrystal_I2C library

## Setup

1. **Connect the Hardware**: Connect the infrared sensors to the Arduino board according to the wiring diagram.
2. **Configure the Wi-Fi Credentials**: Set the `WIFI_SSID` and `WIFI_PASS` constants in the code to match your Wi-Fi network credentials.
3. **Upload the Code**: Upload the provided code to the Arduino board using the Arduino IDE.
4. **Power On**: Power on the Arduino board and the LCD display. The system should automatically connect to Wi-Fi and start displaying the parking status and local time.

## Usage

- **Monitor Parking Status**: The LCD screen will display the current status of each parking spot in real-time.
- **Check Wi-Fi Signal**: The LCD screen will also show the current Wi-Fi signal strength.
- **Local Time**: The system will display the current local time on the LCD screen.

## Troubleshooting

- **No Connection**: Ensure that the Wi-Fi credentials are correctly set and that the Wi-Fi module is properly connected.
- **Sensor Issues**: Check the wiring of the infrared sensors and ensure they are correctly placed to detect vehicles.
- **LCD Display**: Verify that the LCD display is correctly connected and initialized in the code.

## Contributing

Contributions to improve the functionality, reliability, or user experience of the smart parking system are welcome. Please submit pull requests or open issues on the project repository.

