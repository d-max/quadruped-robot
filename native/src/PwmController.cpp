#include "PwmController.h"
#include <Wire.h>

#define PCA9685_MODE1 0x0
#define PCA9685_PRE_SCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define MASK 0x7f   // 8th bit 0, all bits 1
#define SLEEP 0x10  // 5th bit 1, all bits 0

PwmController::PwmController(uint8_t address) {
    this->i2c_address = address;
}

void PwmController::init(void) {
    Wire.begin();

    uint8_t oldMode = readByte(PCA9685_MODE1);
    uint8_t newMode = (oldMode & MASK) | SLEEP;
    // go to sleep
    writeByte(PCA9685_MODE1, newMode);
    // set prescale
    // writeByte(PCA9685_PRESCALE, prescale);
    writeByte(PCA9685_MODE1, oldMode);
    delay(5);
    writeByte(PCA9685_MODE1, oldMode | 0xa0); // auto increment bit + restart
}

void PwmController::setFrequency(float frequency){

}

void PwmController::setPwm(uint8_t pin, uint16_t number) {
    Wire.beginTransmission(this->i2c_address);
    Wire.write(LED0_ON_L + 4 * pin);
    Wire.write(0);
    Wire.write(0);
    Wire.write(number);
    Wire.write(number >> 8);
    Wire.endTransmission();
}

uint8_t PwmController::readByte(uint8_t address) {
    Wire.beginTransmission(this->i2c_address);
    Wire.write(address);
    Wire.endTransmission();

    Wire.requestFrom((uint8_t) this->i2c_address, (uint8_t) 1);
    return Wire.read();
}

void PwmController::writeByte(uint8_t address, uint8_t value) {
    Wire.beginTransmission(this->i2c_address);
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission();
}
