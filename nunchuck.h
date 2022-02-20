

#pragma once
#include <Wire.h>

// Calibration accelerometer values, depends on your Nunchuk
#define NUNCHUK_ACCEL_X_ZERO 512
#define NUNCHUK_ACCEL_Y_ZERO 512
#define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
#define NUNCHUK_JOYSTICK_X_ZERO 127
#define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
#define NUNCHUK_DISABLE_ENCRYPTION

// The Nunchuk I2C address
#define NUNCHUK_ADDRESS 0x52

#if ARDUINO >= 100
#define I2C_READ() Wire.read()
#define I2C_WRITE(x) Wire.write(x)
#else
#define I2C_READ() Wire.receive()
#define I2C_WRITE(x) Wire.send(x)
#endif

#define I2C_START(x) Wire.beginTransmission(x)
#define I2C_STOP() Wire.endTransmission(true)

uint8_t nunchuk_data[6];
uint8_t nunchuk_cali[16];


/**
 * Initializes the Nunchuk communication by sending a sequence of bytes
 */
static void nunchuk_init() {

#ifdef NUNCHUK_DISABLE_ENCRYPTION
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xF0);
    I2C_WRITE(0x55);
    I2C_STOP();
    // delay(1);
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xFB);
    I2C_WRITE(0x00);
    I2C_STOP();
    // delay(1);
#else
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0x40);
    I2C_WRITE(0x00);
    I2C_STOP();
    // delay(1);
#endif

}

/**
 * Decodes a byte if encryption is used
 *
 * @param x The byte to be decoded
 */
static inline uint8_t nunchuk_decode_byte(uint8_t x) {
#ifdef NUNCHUK_DISABLE_ENCRYPTION
    return x;
#else
    return (x ^ 0x17) + 0x17;
#endif
}

/**
 * Central function to read a full chunk of data from Nunchuk
 *
 * @return A boolean if the data transfer was successful
 */
static uint8_t nunchuk_read() {
    uint8_t i;
    Wire.requestFrom(NUNCHUK_ADDRESS, 6);
#ifdef SAM
    delayMicroseconds(10);
#endif
    for (i = 0; i < 6 && Wire.available(); i++) {
        nunchuk_data[i] = nunchuk_decode_byte(I2C_READ());
    }
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0x00);
#ifdef SAM
    delayMicroseconds(100);
#endif
    I2C_STOP();
    return i == 6;
}

/**
 * Checks the current state of button Z
 */
static uint8_t nunchuk_buttonZ() {
    return (~nunchuk_data[5] >> 0) & 1;
}

/**
 * Checks the current state of button C
 */
static uint8_t nunchuk_buttonC() {
    return (~nunchuk_data[5] >> 1) & 1;
}

/**
 * Retrieves the raw X-value of the joystick
 */
static uint8_t nunchuk_joystickX_raw() {
    return nunchuk_data[0];
}

/**
 * Retrieves the raw Y-value of the joystick
 */
static uint8_t nunchuk_joystickY_raw() {
    return nunchuk_data[1];
}

/**
 * Retrieves the calibrated X-value of the joystick
 */
static int16_t nunchuk_joystickX() {
    return (int16_t) nunchuk_joystickX_raw() - (int16_t) NUNCHUK_JOYSTICK_X_ZERO;
}

/**
 * Retrieves the calibrated Y-value of the joystick
 */
static int16_t nunchuk_joystickY() {
    return (int16_t) nunchuk_joystickY_raw() - (int16_t) NUNCHUK_JOYSTICK_Y_ZERO;
}

/**
 * Calculates the angle of the joystick
 */
static float nunchuk_joystick_angle() {
    return atan2((float) nunchuk_joystickY(), (float) nunchuk_joystickX());
}

/**
 * Retrieves the raw X-value of the accelerometer
 */
static uint16_t nunchuk_accelX_raw() {
    return ((uint16_t) nunchuk_data[2] << 2) | ((nunchuk_data[5] >> 2) & 3);
}

/**
 * Retrieves the raw Y-value of the accelerometer
 */
static uint16_t nunchuk_accelY_raw() {
    return ((uint16_t) nunchuk_data[3] << 2) | ((nunchuk_data[5] >> 4) & 3);
}

/**
 * Retrieves the raw Z-value of the accelerometer
 */
static uint16_t nunchuk_accelZ_raw() {
    return ((uint16_t) nunchuk_data[4] << 2) | ((nunchuk_data[5] >> 6) & 3);
}

/**
 * Retrieves the calibrated X-value of the accelerometer
 */
static int16_t nunchuk_accelX() {
    return (int16_t) nunchuk_accelX_raw() - (int16_t) NUNCHUK_ACCEL_X_ZERO;
}

/**
 * Retrieves the calibrated Y-value of the accelerometer
 */
static int16_t nunchuk_accelY() {
    return (int16_t) nunchuk_accelY_raw() - (int16_t) NUNCHUK_ACCEL_Y_ZERO;
}

/**
 * Retrieves the calibrated Z-value of the accelerometer
 */
static int16_t nunchuk_accelZ() {
    return (int16_t) nunchuk_accelZ_raw() - (int16_t) NUNCHUK_ACCEL_Z_ZERO;
}

/**
 * Calculates the pitch angle THETA around y-axis of the Nunchuk in radians
 */
static float nunchuk_pitch() { // tilt-y
    return atan2((float) nunchuk_accelY(), (float) nunchuk_accelZ());
}

/**
 * Calculates the roll angle PHI around x-axis of the Nunchuk in radians
 */
static float nunchuk_roll() { // tilt-x
    return atan2((float) nunchuk_accelX(), (float) nunchuk_accelZ());
}
