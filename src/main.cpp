/*
  Copyright (c) 2020-2024 Alan Yorinks All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE
  Version 3 as published by the Free Software Foundation; either
  or (at your option) any later version.
  This library is distributed in the hope that it will be useful,f
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// This file is rather large, so it has been rearranged in logical sections.
// Here is the list of sections to help make it easier to locate items of
// interest, and aid when adding new features.

// 1. Feature Enabling Defines
// 2. Arduino ID
// 3. Client Command Related Defines and Support
// 4. Server Report Related Defines
// 5. i2c Related Defines
// 6. Pin Related Defines And Data Structures
// 7. Feature Related Defines, Data Structures and Storage Allocation
// 8. Command Functions
// 9. Scanning Inputs, Generating Reports And Running Steppers
// 10. Setup and Loop

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                    FEATURE ENABLING DEFINES                      */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// To disable a feature, comment out the desired enabling define or defines

// This will allow SPI support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
// #define SPI_ENABLED 1

// This will allow OneWire support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
// #define ONE_WIRE_ENABLED 1

// This will allow DHT support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
// #define DHT_ENABLED 1

// This will allow sonar support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
// #define SONAR_ENABLED 1

// This will allow servo support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
// #define SERVO_ENABLED 1

// This will allow stepper support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
// #define STEPPERS_ENABLED 1

// This will allow I2C support to be compiled into the sketch.
// Comment this out to save sketch space for the UNO
#define I2C_ENABLED 1

// clang-format off
#include <Arduino.h>
// clang-format on

#ifdef I2C_ENABLED
/**
 * @brief This library implements the i2c capabilities for Telemetrix
 *
 * For Wire version 1.0: the BUFFER_LENGTH macro is set to 32 bytes; however, we
 * go forked the ArduinoCore-avr framework and we can configure each of the
 * following buffers on a per-board basis:
 *     TWI_BUFFER_LENGTH
 *     ARDUINO_WIRE_TX_BUFFER_LENGTH
 *     ARDUINO_WIRE_RX_BUFFER_LENGTH
 *
 * We typically want to update these buffers in lockstep, additionally we may
 * want to update the these buffers to increase throughput:
 *     TELEMETRIX_I2C_REPORT_BUFFER_SIZE
 *     TELEMETRIX_MAX_COMMAND_LENGTH
 *
 */
// #ifdef __IMXRT1062__
// // Import the Teensy Version
// #include "i2c_driver_wire.h"
// #else
#include <Wire.h>
// #endif
/**
 * @brief The buffer size, in bytes, to hold i2c report data from the i2c device
 * - passing it back to the host application.
 *
 */
#ifndef TELEMETRIX_I2C_REPORT_BUFFER_SIZE
#define TELEMETRIX_I2C_REPORT_BUFFER_SIZE 64
#endif // end TELEMETRIX_I2C_REPORT_BUFFER_SIZE

/**
 * @brief We can configure the clock speed of i2c bus by setting this variable
 * during compilation.
 *
 */
#ifndef TELEMETRIX_I2C_CLOCK_SPEED
#define TELEMETRIX_I2C_CLOCK_SPEED 100000
#endif // end TELEMETRIX_I2C_CLOCK_SPEED

#endif // end I2C_ENABLED

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                    Arduino ID                                    */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// This value must be the same as specified when instantiating the
// telemetrix client. The client defaults to a value of 1.
// This value is used for the client to auto-discover and to
// connect to a specific board regardless of the current com port
// it is currently connected to.

#define ARDUINO_ID 1

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*         Client Command Related Defines and Support               */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// Commands Sent By The Client

// Add commands retaining the sequential numbering.
// The order of commands here must be maintained in the command_table.
#define SERIAL_LOOP_BACK 0
#define SET_PIN_MODE 1
#define DIGITAL_WRITE 2
#define ANALOG_WRITE 3
#define MODIFY_REPORTING                                                       \
  4 // mode(all, analog, or digital), pin, enable or disable
#define GET_FIRMWARE_VERSION 5
#define ARE_U_THERE 6
#define SERVO_ATTACH 7
#define SERVO_WRITE 8
#define SERVO_DETACH 9
#define I2C_BEGIN 10
#define I2C_READ 11
#define I2C_WRITE 12
#define SONAR_NEW 13
#define DHT_NEW 14
#define STOP_ALL_REPORTS 15
#define SET_ANALOG_SCANNING_INTERVAL 16
#define ENABLE_ALL_REPORTS 17
#define RESET 18
#define SPI_INIT 19
#define SPI_WRITE_BLOCKING 20
#define SPI_READ_BLOCKING 21
#define SPI_SET_FORMAT 22
#define SPI_CS_CONTROL 23
#define ONE_WIRE_INIT 24
#define ONE_WIRE_RESET 25
#define ONE_WIRE_SELECT 26
#define ONE_WIRE_SKIP 27
#define ONE_WIRE_WRITE 28
#define ONE_WIRE_READ 29
#define ONE_WIRE_RESET_SEARCH 30
#define ONE_WIRE_SEARCH 31
#define ONE_WIRE_CRC8 32
#define SET_PIN_MODE_STEPPER 33
#define STEPPER_MOVE_TO 34
#define STEPPER_MOVE 35
#define STEPPER_RUN 36
#define STEPPER_RUN_SPEED 37
#define STEPPER_SET_MAX_SPEED 38
#define STEPPER_SET_ACCELERATION 39
#define STEPPER_SET_SPEED 40
#define STEPPER_SET_CURRENT_POSITION 41
#define STEPPER_RUN_SPEED_TO_POSITION 42
#define STEPPER_STOP 43
#define STEPPER_DISABLE_OUTPUTS 44
#define STEPPER_ENABLE_OUTPUTS 45
#define STEPPER_SET_MINIMUM_PULSE_WIDTH 46
#define STEPPER_SET_ENABLE_PIN 47
#define STEPPER_SET_3_PINS_INVERTED 48
#define STEPPER_SET_4_PINS_INVERTED 49
#define STEPPER_IS_RUNNING 50
#define STEPPER_GET_CURRENT_POSITION 51
#define STEPPER_GET_DISTANCE_TO_GO 52
#define STEPPER_GET_TARGET_POSITION 53
#define GET_FEATURES 54
#define SONAR_SCAN_OFF 55
#define SONAR_SCAN_ON 56

#define BOARD_HARD_RESET 57

/* Command Forward References*/

// If you add a new command, you must add the command handler
// here as well.

extern void serial_loopback();

extern void set_pin_mode();

extern void digital_write();

extern void analog_write();

extern void modify_reporting();

extern void get_firmware_version();

extern void are_you_there();

extern void i2c_begin();

extern void i2c_read();

extern void i2c_write();

extern void stop_all_reports();

extern void set_analog_scanning_interval();

extern void enable_all_reports();

extern void reset_data();

extern void init_pin_structures();

extern void get_features();

extern void board_hard_reset();

// When adding a new command update the command_table.
// The command length is the number of bytes that follow
// the command byte itself, and does not include the command
// byte in its length.

// The command_func is a pointer the command's function.
struct command_descriptor {
  // a pointer to the command processing function
  void (*command_func)(void);
};

void do_nothing() {
  // This function doesn't do anything
  return;
}

// An array of pointers to the command functions.
// The list must be in the same order as the command defines.

const command_descriptor command_table[] = {
    {&serial_loopback},
    {&set_pin_mode},
    {&digital_write},
    {&analog_write},
    {&modify_reporting},
    {&get_firmware_version},
    {&are_you_there},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&i2c_begin},
    {&i2c_read},
    {&i2c_write},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
    {&do_nothing},
};

// maximum length of a command in bytes
#ifndef TELEMETRIX_MAX_COMMAND_LENGTH
#define TELEMETRIX_MAX_COMMAND_LENGTH 30
#endif

// buffer to hold incoming command data
byte command_buffer[TELEMETRIX_MAX_COMMAND_LENGTH];

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                 Reporting Defines and Support                    */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// Reports sent to the client

#define DIGITAL_REPORT DIGITAL_WRITE
#define ANALOG_REPORT ANALOG_WRITE
#define FIRMWARE_REPORT 5
#define I_AM_HERE 6
#define SERVO_UNAVAILABLE 7
#define I2C_TOO_FEW_BYTES_RCVD 8
#define I2C_TOO_MANY_BYTES_RCVD 9
#define I2C_READ_REPORT 10
#define SONAR_DISTANCE 11
#define DHT_REPORT 12
#define SPI_REPORT 13
#define ONE_WIRE_REPORT 14
#define STEPPER_DISTANCE_TO_GO 15
#define STEPPER_TARGET_POSITION 16
#define STEPPER_CURRENT_POSITION 17
#define STEPPER_RUNNING_REPORT 18
#define STEPPER_RUN_COMPLETE_REPORT 19
#define FEATURES 20

#define DEBUG_PRINT 99

#ifdef I2C_ENABLED
// A buffer to hold i2c report data
byte i2c_report_message[TELEMETRIX_I2C_REPORT_BUFFER_SIZE];
#endif

bool stop_reports = false; // a flag to stop sending all report messages

// Input pin reporting control sub commands (modify_reporting)
#define REPORTING_DISABLE_ALL 0
#define REPORTING_ANALOG_ENABLE 1
#define REPORTING_DIGITAL_ENABLE 2
#define REPORTING_ANALOG_DISABLE 3
#define REPORTING_DIGITAL_DISABLE 4

// DHT Report sub-types
#define DHT_DATA 0
#define DHT_READ_ERROR 1

// firmware version - update this when bumping the version
#define FIRMWARE_MAJOR 5
#define FIRMWARE_MINOR 4
#define FIRMWARE_PATCH 4

// Feature Masks And Storage

#define ONEWIRE_FEATURE 0x01
#define DHT_FEATURE 0x02
#define STEPPERS_FEATURE 0x04
#define SPI_FEATURE 0x08
#define SERVO_FEATURE 0x10
#define SONAR_FEATURE 0x20
#define I2C_FEATURE 0x40

// a byte to hold the enabled features
// the masks are OR'ed into the features byte
uint8_t features = 0;

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                     i2c Related Defines*/
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/**********************************/
/* i2c defines */

#ifdef I2C_ENABLED
// uncomment out the next line to create a 2nd i2c port
// #define SECOND_I2C_PORT

#ifdef SECOND_I2C_PORT
// Change the pins to match SDA and SCL for your board
#define SECOND_I2C_PORT_SDA PB3
#define SECOND_I2C_PORT_SCL PB10

TwoWire Wire2(SECOND_I2C_PORT_SDA, SECOND_I2C_PORT_SCL);
#endif

// a pointer to an active TwoWire object
TwoWire *current_i2c_port;
#endif

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*           Pin Related Defines And Data Structures                */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// Pin mode definitions

// INPUT defined in Arduino.h = 0
// OUTPUT defined in Arduino.h = 1
// INPUT_PULLUP defined in Arduino.h = 2
// The following are defined for arduino_telemetrix (AT)
#define AT_ANALOG 3
#define AT_MODE_NOT_SET 255

// maximum number of pins supported
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
#define MAX_DIGITAL_PINS_SUPPORTED 22
#define MAX_ANALOG_PINS_SUPPORTED 8
#else
#define MAX_DIGITAL_PINS_SUPPORTED 100
#define MAX_ANALOG_PINS_SUPPORTED 16
#endif

// Analog input pin numbers are defined from
// A0 - A7. Since we do not know if the board
// in use also supports higher analog pin numbers
// we need to define those pin numbers to allow
// the program to compile, even though the
// pins may not exist for the board in use.

#ifndef A8
#define A8 2047
#endif

#ifndef A9
#define A9 2047
#endif

#ifndef A10
#define A10 2047
#endif

#ifndef PIN_A11
#define A11 2047
#endif

#ifndef PIN_A12
#define A12 2047
#endif

#ifndef PIN_A13
#define A13 2047
#endif

#ifndef PIN_A14
#define A14 2047
#endif

#ifndef PIN_A15
#define A15 2047
#endif

// To translate a pin number from an integer value to its analog pin number
// equivalent, this array is used to look up the value to use for the pin.
#ifdef ARDUINO_SAMD_MKRWIFI1010
const int analog_read_pins[20] = {A0, A1, A2, A3, A4, A5, A6};
#elif ARDUINO_FSP
const int analog_read_pins[20] = {A0, A1, A2, A3, A4, A5};
#else
const int analog_read_pins[20] = {A0, A1, A2,  A3,  A4,  A5,  A6,  A7,
                                  A8, A9, A10, A11, A12, A13, A14, A15};
#endif

// a descriptor for digital pins
struct pin_descriptor {
  byte pin_number;
  byte pin_mode;
  bool reporting_enabled; // If true, then send reports if an input pin
  int last_value;         // Last value read for input mode
};

// an array of digital_pin_descriptors
pin_descriptor the_digital_pins[MAX_DIGITAL_PINS_SUPPORTED];

// a descriptor for digital pins
struct analog_pin_descriptor {
  byte pin_number;
  byte pin_mode;
  bool reporting_enabled; // If true, then send reports if an input pin
  int last_value;         // Last value read for input mode
  int differential;       // difference between current and last value needed
                          // to generate a report
};

// an array of analog_pin_descriptors
analog_pin_descriptor the_analog_pins[MAX_ANALOG_PINS_SUPPORTED];

unsigned long current_millis;  // for analog input loop
unsigned long previous_millis; // for analog input loop
uint8_t analog_sampling_interval = 19;

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*  Feature Related Defines, Data Structures and Storage Allocation */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                       Command Functions                          */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// A method to send debug data across the serial link
void send_debug_info(byte id, int value) {
  byte debug_buffer[5] = {(byte)4, (byte)DEBUG_PRINT, 0, 0, 0};
  debug_buffer[2] = id;
  debug_buffer[3] = highByte(value);
  debug_buffer[4] = lowByte(value);
  Serial.write(debug_buffer, 5);
}

// a function to loop back data over the serial port
void serial_loopback() {
  byte loop_back_buffer[3] = {2, (byte)SERIAL_LOOP_BACK, command_buffer[0]};
  Serial.write(loop_back_buffer, 3);
}

void set_pin_mode()
/*
    Set a pin to digital input, digital input_pullup, digital output,
    and analog input. PWM is considered digital output, and i2c, spi, dht,
    sonar, servo, and onewire have their own init methods.
*/
{
  byte pin;
  byte mode;
  pin = command_buffer[0];
  mode = command_buffer[1];

  switch (mode) {
  case INPUT:
    the_digital_pins[pin].pin_mode = mode;
    the_digital_pins[pin].reporting_enabled = command_buffer[2];
    pinMode(pin, INPUT);
    break;
  case INPUT_PULLUP:
    the_digital_pins[pin].pin_mode = mode;
    the_digital_pins[pin].reporting_enabled = command_buffer[2];
    pinMode(pin, INPUT_PULLUP);
    break;
  case OUTPUT:
    the_digital_pins[pin].pin_mode = mode;
    pinMode(pin, OUTPUT);
    break;
  case AT_ANALOG:
    the_analog_pins[pin].pin_mode = mode;
    the_analog_pins[pin].differential =
        (command_buffer[2] << 8) + command_buffer[3];
    the_analog_pins[pin].reporting_enabled = command_buffer[4];
    break;
  default:
    break;
  }
}

// set the analog scanning interval
void set_analog_scanning_interval() {
  analog_sampling_interval = command_buffer[0];
}

// set the state of digital output pin
void digital_write() {
  byte pin;
  byte value;
  pin = command_buffer[0];
  value = command_buffer[1];
  digitalWrite(pin, value);
}

// set the pwm value for a digital output pin
// The term analog is confusing here, but it is what
// Arduino uses.
void analog_write() {
  // command_buffer[0] = PIN, command_buffer[1] = value_msb,
  // command_buffer[2] = value_lsb
  byte pin; // command_buffer[0]
  unsigned int value;

  pin = command_buffer[0];

  value = (command_buffer[1] << 8) + command_buffer[2];
  analogWrite(pin, value);
}

// This method allows you modify what reports are generated.
// You can disable all reports, including dhts, and sonar.
// You can disable only digital and analog reports on a
// pin basis, or enable those on a pin basis.
void modify_reporting() {
  int pin = command_buffer[1];

  switch (command_buffer[0]) {
  case REPORTING_DISABLE_ALL:
    for (int i = 0; i < MAX_DIGITAL_PINS_SUPPORTED; i++) {
      the_digital_pins[i].reporting_enabled = false;
    }
    for (int i = 0; i < MAX_ANALOG_PINS_SUPPORTED; i++) {
      the_analog_pins[i].reporting_enabled = false;
    }
    break;
  case REPORTING_ANALOG_ENABLE:
    if (the_analog_pins[pin].pin_mode != AT_MODE_NOT_SET) {
      the_analog_pins[pin].reporting_enabled = true;
    }
    break;
  case REPORTING_ANALOG_DISABLE:
    if (the_analog_pins[pin].pin_mode != AT_MODE_NOT_SET) {
      the_analog_pins[pin].reporting_enabled = false;
    }
    break;
  case REPORTING_DIGITAL_ENABLE:
    if (the_digital_pins[pin].pin_mode != AT_MODE_NOT_SET) {
      the_digital_pins[pin].reporting_enabled = true;
    }
    break;
  case REPORTING_DIGITAL_DISABLE:
    if (the_digital_pins[pin].pin_mode != AT_MODE_NOT_SET) {
      the_digital_pins[pin].reporting_enabled = false;
    }
    break;
  default:
    break;
  }
}

// retrieve the features byte
void get_features() {
  byte report_message[3] = {2, FEATURES, features};
  Serial.write(report_message, 3);
}

// Return the firmware version number
void get_firmware_version() {
  byte report_message[5] = {4, FIRMWARE_REPORT, FIRMWARE_MAJOR, FIRMWARE_MINOR,
                            FIRMWARE_PATCH};
  Serial.write(report_message, 5);
}

// Query the firmware for the Arduino ID in use
void are_you_there() {
  byte report_message[3] = {2, I_AM_HERE, ARDUINO_ID};
  Serial.write(report_message, 3);
}

/***********************************
   i2c functions
 **********************************/

// initialize i2c data transfers
void i2c_begin() {
#ifdef I2C_ENABLED
  byte i2c_port = command_buffer[0];
  if (not i2c_port) {
    Wire.begin();
    Wire.setClock(TELEMETRIX_I2C_CLOCK_SPEED);
  }

#ifdef SECOND_I2C_PORT
  else {
    Wire2.begin();
    Wire2.setClock(TELEMETRIX_I2C_CLOCK_SPEED);
  }
#endif
#endif
}

// read a number of bytes from a specific i2c register
void i2c_read() {
#ifdef I2C_ENABLED
  // data in the incoming message:
  // i2c_port [0]
  // address, [1]
  // register, [2]
  // number of bytes, [3]
  // stop transmitting flag [4]
  // i2c port [5]
  // write the register [6]

  int message_size = 0;
  byte address = command_buffer[0];
  byte the_register = command_buffer[1];

  // set the current i2c port if this is for the primary i2c
  if (command_buffer[4] == 0) {
    current_i2c_port = &Wire;
  }

#ifdef SECOND_I2C_PORT
  // this is for port 2
  if (command_buffer[4] == 1) {
    current_i2c_port = &Wire2;
  }
#endif

  // write byte is true, then write the register
  if (command_buffer[5]) {
    current_i2c_port->beginTransmission(address);
    current_i2c_port->write((byte)the_register);
    current_i2c_port->endTransmission(command_buffer[3]); // default = true
  }
#ifdef ARDUINO_AVR_NANO_EVERY
  current_i2c_port->requestFrom(
      address,
      (size_t)command_buffer[2]); // all bytes are returned in requestFrom
#else
  current_i2c_port->requestFrom(
      address,
      command_buffer[2]); // all bytes are returned in requestFrom
#endif

  // check to be sure correct number of bytes were returned by slave
  auto number_of_byes_given_in_command_buffer = command_buffer[2];
  /**
   * @brief We need to make sure that we're not creating a bug where
      current_i2c_port->available() is greater than 255.
   *
   */
  auto current_i2c_bytes_available = current_i2c_port->available();

  if (number_of_byes_given_in_command_buffer < current_i2c_bytes_available) {
    byte report_message[6] = {5,
                              I2C_TOO_FEW_BYTES_RCVD,
                              1,
                              address,
                              number_of_byes_given_in_command_buffer,
                              static_cast<byte>(current_i2c_bytes_available)};
    Serial.write(report_message, 6);
    return;
  } else if (number_of_byes_given_in_command_buffer >
             current_i2c_bytes_available) {
    byte report_message[6] = {5,
                              I2C_TOO_MANY_BYTES_RCVD,
                              1,
                              address,
                              number_of_byes_given_in_command_buffer,
                              static_cast<byte>(current_i2c_bytes_available)};
    Serial.write(report_message, 6);
    return;
  }

  // packet length
  i2c_report_message[0] = command_buffer[2] + 5;

  // report type
  i2c_report_message[1] = I2C_READ_REPORT;

  // i2c_port
  i2c_report_message[2] = command_buffer[4];

  // number of bytes read
  i2c_report_message[3] = command_buffer[2]; // number of bytes

  // device address
  i2c_report_message[4] = address;

  // device register
  i2c_report_message[5] = the_register;

  // append the data that was read
  for (message_size = 0;
       message_size < command_buffer[2] && current_i2c_port->available();
       message_size++) {
    i2c_report_message[6 + message_size] = current_i2c_port->read();
  }
  // send slave address, register and received bytes

  for (int i = 0; i < message_size + 6; i++) {
    Serial.write(i2c_report_message[i]);
  }
#endif
}

// write a specified number of bytes to an i2c device
void i2c_write() {
#ifdef I2C_ENABLED
  // command_buffer[0] is the number of bytes to send
  // command_buffer[1] is the device address
  // command_buffer[2] is the i2c port
  // additional bytes to write= command_buffer[3..];

  // set the current i2c port if this is for the primary i2c
  if (command_buffer[2] == 0) {
    current_i2c_port = &Wire;
  }

#ifdef SECOND_I2C_PORT
  // this is for port 2
  if (command_buffer[2] == 1) {
    current_i2c_port = &Wire2;
  }
#endif

  current_i2c_port->beginTransmission(command_buffer[1]);

  // write the data to the device
  for (int i = 0; i < command_buffer[0]; i++) {
    current_i2c_port->write(command_buffer[i + 3]);
  }
  current_i2c_port->endTransmission();
  delayMicroseconds(70);
#endif
}

// stop all reports from being generated

void stop_all_reports() {
  stop_reports = true;
  delay(20);
  Serial.flush();
}

// enable all reports to be generated
void enable_all_reports() {
  Serial.flush();
  stop_reports = false;
  delay(20);
}

// retrieve the next command from the serial link
void get_next_command() {
  byte command;
  byte packet_length;
  command_descriptor command_entry;

  // clear the command buffer
  memset(command_buffer, 0, sizeof(command_buffer));

  // if there is no command waiting, then return
  if (not Serial.available()) {
    return;
  }
  // get the packet length
  packet_length = (byte)Serial.read();

  while (not Serial.available()) {
    delay(1);
  }

  // get the command byte
  command = (byte)Serial.read();

  // uncomment the next line to see the packet length and command
  // send_debug_info(packet_length, command);
  command_entry = command_table[command];

  if (packet_length > 1) {
    // get the data for that command
    for (int i = 0; i < packet_length - 1; i++) {
      // need this delay or data read is not correct
      while (not Serial.available()) {
        delay(1);
      }
      command_buffer[i] = (byte)Serial.read();
      // uncomment out to see each of the bytes following the command
      // send_debug_info(i, command_buffer[i]);
    }
  }
  command_entry.command_func();
}

// reset the internal data structures to a known state
void reset_data() {
  // reset the data structures

  // fist stop all reporting
  stop_all_reports();

  current_millis = 0;  // for analog input loop
  previous_millis = 0; // for analog input loop
  analog_sampling_interval = 19;

  enable_all_reports();
}

// initialize the pin data structures
void init_pin_structures() {
  for (byte i = 0; i < MAX_DIGITAL_PINS_SUPPORTED; i++) {
    the_digital_pins[i].pin_number = i;
    the_digital_pins[i].pin_mode = AT_MODE_NOT_SET;
    the_digital_pins[i].reporting_enabled = false;
    the_digital_pins[i].last_value = 0;
  }

  // establish the analog pin array
  for (byte i = 0; i < MAX_ANALOG_PINS_SUPPORTED; i++) {
    the_analog_pins[i].pin_number = i;
    the_analog_pins[i].pin_mode = AT_MODE_NOT_SET;
    the_analog_pins[i].reporting_enabled = false;
    the_analog_pins[i].last_value = 0;
    the_analog_pins[i].differential = 0;
  }
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*    Scanning Inputs, Generating Reports And Running Steppers      */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// scan the digital input pins for changes
void scan_digital_inputs() {
  byte value;

  // report message

  // byte 0 = packet length
  // byte 1 = report type
  // byte 2 = pin number
  // byte 3 = value
  byte report_message[4] = {3, DIGITAL_REPORT, 0, 0};

  for (int i = 0; i < MAX_DIGITAL_PINS_SUPPORTED; i++) {
    if (the_digital_pins[i].pin_mode == INPUT ||
        the_digital_pins[i].pin_mode == INPUT_PULLUP) {
      if (the_digital_pins[i].reporting_enabled) {
        // if the value changed since last read
        value = (byte)digitalRead(the_digital_pins[i].pin_number);
        if (value != the_digital_pins[i].last_value) {
          the_digital_pins[i].last_value = value;
          report_message[2] = (byte)i;
          report_message[3] = value;
          Serial.write(report_message, 4);
        }
      }
    }
  }
}

// scan the analog input pins for changes
void scan_analog_inputs() {
  int value;

  // report message

  // byte 0 = packet length
  // byte 1 = report type
  // byte 2 = pin number
  // byte 3 = high order byte of value
  // byte 4 = low order byte of value

  byte report_message[5] = {4, ANALOG_REPORT, 0, 0, 0};

  uint8_t adjusted_pin_number;
  int differential;

  current_millis = millis();
  if (current_millis - previous_millis > analog_sampling_interval) {
    previous_millis = current_millis;

    for (int i = 0; i < MAX_ANALOG_PINS_SUPPORTED; i++) {
      if (the_analog_pins[i].pin_mode == AT_ANALOG) {
        if (the_analog_pins[i].reporting_enabled) {
          // if the value changed since last read
          // adjust pin number for the actual read
          adjusted_pin_number = (uint8_t)(analog_read_pins[i]);
          value = analogRead(adjusted_pin_number);
          differential = abs(value - the_analog_pins[i].last_value);
          if (differential >= the_analog_pins[i].differential) {
            // trigger value achieved, send out the report
            the_analog_pins[i].last_value = value;
            // input_message[1] = the_analog_pins[i].pin_number;
            report_message[2] = (byte)i;
            report_message[3] = highByte(value); // get high order byte
            report_message[4] = lowByte(value);
            Serial.write(report_message, 5);
            delay(1);
          }
        }
      }
    }
  }
}

void board_hard_reset() {
#if defined(ARDUINO_FSP)
  NVIC_SystemReset();
  delay(2000);
#endif
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                    Setup And Loop                                */
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void setup() {
  // set up features for enabled features

#ifdef I2C_ENABLED
  features |= I2C_FEATURE;
#endif

  init_pin_structures();

  Serial.begin(115200);
  pinMode(13, OUTPUT);
  for (int i = 0; i < 4; i++) {
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250);
  }
}

void loop() {
  // keep processing incoming commands
  get_next_command();

  if (!stop_reports) { // stop reporting
    scan_digital_inputs();
    scan_analog_inputs();
  }
}
