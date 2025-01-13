# AT93C46 EEPROM Bit-Banging Interface

This project provides a comprehensive interface for the AT93C46 EEPROM using bit-banging techniques with AVR microcontrollers. It includes functions for initialization, SPI communication, and EEPROM-specific operations such as enabling/disabling write operations, erasing, and writing data.

## Features

- **Bit-banging SPI Interface**: Custom implementation for SPI communication.
- **EEPROM Operations**:
  - Write Enable/Disable (`EWEN`, `EWDS`)
  - Erase All (`ERAL`)
  - Write All (`WRAL`)
  - Byte-wise Write and Read
  - Erase specific addresses
- **Test APIs**: Includes a test menu for interacting with the EEPROM via serial communication.

## Files

### 1. `at93c46.h`
Contains function declarations, pin definitions, and macros for controlling the EEPROM. Key features:
- Macros for bit-banging SPI and pin control.
- Function declarations for EEPROM instructions.

### 2. `at93c46.cpp`
Implements the functions declared in the header file. Key highlights:
- Initialization of EEPROM pins.
- SPI data transmission and reception.
- EEPROM-specific commands like `at_ewen`, `at_eral`, and `at_write`.

### 3. `main.cpp`
The entry point for the application:
- Initializes the EEPROM.
- Displays the test menu on a serial monitor.
- Waits for user input to execute specific EEPROM commands.

### 4. `test_api.cpp`
Defines the test menu and its functionalities:
- Provides a menu to execute different EEPROM operations via serial communication.
- Includes helper functions to write patterns and sequential data to the EEPROM.
- Displays the NVRAM content in a readable format.

## Usage

### Hardware Setup
1. Connect the AT93C46 EEPROM to the microcontroller with the following pin configuration:
   - **NSS_PIN**: Slave Select
   - **DI_PIN**: Data In
   - **DO_PIN**: Data Out
   - **CLK_PIN**: Clock

2. Pin Connection:

   
![image](https://github.com/user-attachments/assets/20f4d105-e07d-4f0c-9967-51918bca6bcb)

4. Ensure the pins are correctly mapped in `at93c46.h`.

## Test Menu
![image](https://github.com/user-attachments/assets/92cd6bb2-e471-43f5-bb6c-beb56bc6cf6e)

### Test Case 1: 
  - We enable EWEN - CMD '2'
  - Writing ASCII characters - CMD '9'
  - Dumping NVram - CMD '1'

![image](https://github.com/user-attachments/assets/dcf1ffa8-4998-4245-83af-ad5881a2feb0)

### Test Case 2: 
  - ERAL (Erase content at all locations) - CMD '5'
  - Dumping NVram - CMD '1'

![image](https://github.com/user-attachments/assets/a2a6ecc1-d71c-436e-a826-46d2b2208344)

### Test Case 3:
  - WRAL 0xA5 (Writing 0xA5 at all memory locaations) - CMD '4'

![image](https://github.com/user-attachments/assets/dccaf3e0-2cf2-48cc-8069-04a17926184b)

### Test Case 4:
  - ERAL (Erase content at all locations) - CMD '5'
  - CMD '6' - Writing from 0 - 8th location

![image](https://github.com/user-attachments/assets/9325bfa3-f196-4e5e-92cb-c52e3901e226)

### Test Case 5:
  - ERAL (Erase content at all locations) - CMD '5'
  - CMD '7' - Writing from 0 - 8th location

![image](https://github.com/user-attachments/assets/492f1756-d955-4fb3-86b4-2cc229f8743d)


## Reference -> https://www.kn34pc.com/spr/arduino_93c46.html





