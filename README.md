# ESP32-RC522-RFID-reader-writer

## Purpose
The **ESP32-RC522-RFID-reader-writer** is a project designed to interface an **ESP32 microcontroller** with the **RC522 RFID module**. It enables the detection, reading, and writing of NFC tags that comply with the ISO/IEC 14443A standard. This module is ideal for applications that require NFC or RFID capabilities such as access control, asset management, inventory systems, and more.

## Features
- **Configurability**: The project is designed with configurable pins and can be configured to keep performing a certain task.
- **UART input handling**: The project takes input through UART at 115200 baudrate, and accepts the entered string after the 0x0D (\n) character then extracts data through String handling.
- **Detection**: The module can detect RFID tags within range and retrieve information like UID (Unique Identifier).
- **Reading**: The ability to read data from RFID tags, displaying the tag's contents.
- **Writing**: It can write data to compatible RFID tags, such as MIFARE Classic cards.

## Applications
This module can be used in various applications, including but not limited to:
- **Access Control Systems**: Grant or deny access based on RFID tag data.
- **Asset Management**: Keep track of items using RFID tags.
- **Inventory Management**: Use RFID tags to automate the process of tracking stock and inventory.
- **Payment Systems**: Integrate with payment systems that use RFID tags or cards.

## Requirements
To use the **ESP32-RC522-RFID-reader-writer**, you will need the following:
- **Hardware**:
  - **ESP32** microcontroller board
  - **RC522 RFID module**
  - **RFID tags/cards** (e.g., MIFARE Classic, ISO/IEC 14443A standard)
  - Jumper wires and a breadboard for connections

- **Software**:
  - Arduino IDE with ESP32 support
  - RC522 library for Arduino (available from the library manager)

## Installation
1. Install the **ESP32 board** in the Arduino IDE.
2. Install the **RC522 library** via the Arduino Library Manager.
3. Connect the **ESP32** to the **RC522 module** following the pinout.
4. Upload the provided code to the ESP32.

## Usage
1. Power on the system with the **ESP32** and **RC522 module** connected.
2. Open the **Serial Monitor** in Arduino IDE.
3. You can choose different modes (e.g., detect, read, write) from the interactive menu in the Serial Monitor.

## Warnings and Safety Precautions
- **Avoid Short Circuits**: Ensure all connections are correctly made to avoid damaging the components.
- **Use Proper Voltage**: The ESP32 and RC522 module typically operate at 3.3V. Do not apply higher voltages to avoid damage.
- **RFID Tag Compatibility**: The system is designed for MIFARE Classic tags or tags that comply with the ISO/IEC 14443A standard, other RFID tags may not be supported.
- **Avoid Physical Damage**: Handle the ESP32 and RC522 module with care, especially the RFID module’s antenna.

## Disclaimer
This module is intended for educational and non-commercial use. Any modification or use of this module in critical applications (e.g., security systems) should be done with caution and proper validation.

## License
This project is open-source and released under the MIT License. Feel free to modify and use it in your own projects.

## Contributions
Contributions are welcome! Please feel free to open issues or submit pull requests to improve the functionality or add new features to this module.

---

**Happy coding!**
