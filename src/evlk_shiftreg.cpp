#include "evlk_shiftreg.h"

#define P2B(pin, Num) (Num - pin / 8 - 1)

namespace _EVLK_SHIFTREG_ {

uint8_t *shiftreg::initBuffer(uint8_t byteNum) {
    if (!byteNum)
        return NULL;
    uint8_t *arr = new uint8_t[byteNum]();
    return arr;
};
shiftreg::shiftreg(uint8_t byteNum) : Buffer(initBuffer(byteNum)), bufferNum(byteNum) {};
shiftreg::~shiftreg() {
    if (Buffer)
        delete[] Buffer;
}

void shiftreg::_shift8(uint8_t data) {
    for (size_t i = 0; i < 8; i++) {
        bool b = data >> 7;
        data   = data << 1;
        // bool b = data & 1;
        // data   = data >> 1;
        _shift(b);
    }
};
void shiftreg::bufferShift_1(bool bit) {
    for (size_t i = 0; i < bufferNum - 1; i++) {
        Buffer[i] = (Buffer[i] << 1) | (Buffer[i + 1] >> 7);
    }
    Buffer[bufferNum - 1] = (Buffer[bufferNum - 1] << 1) | bit;
};
void shiftreg::bufferShift_8(uint8_t data) {
    memmove(Buffer, Buffer + 1, bufferNum - 1);
    Buffer[bufferNum - 1] = data;
}

void shiftreg::shift(bool bit) {
    if (Buffer)
        bufferShift_1(bit);
    _shift(bit);
};
void shiftreg::shift8(uint8_t data) {
    if (Buffer)
        bufferShift_8(data);
    _shift8(data);
};
void shiftreg::latch() { _latch(); };

void shiftreg::uploadBuffer() {
    if (Buffer)
        for (size_t i = 0; i < bufferNum; i++)
            _shift8(Buffer[i]);
};

void shiftreg::digitalWrite(pin_size_t pin, PinStatus val) {
    if (!Buffer)
        return;
    val ? Buffer[P2B(pin, bufferNum)] |= (1 << pin % 8)
        : Buffer[P2B(pin, bufferNum)] &= ~(1 << pin % 8);
    uploadBuffer();
    latch();
};
void shiftreg::digitalWrite(uint8_t mask[], int n, int status) {
    if (!Buffer)
        return;
    for (int i = 0; i < n; i++)
        status ? Buffer[bufferNum - i - 1] |= mask[n - i - 1]
               : Buffer[bufferNum - i - 1] &= ~mask[n - i - 1];
    uploadBuffer();
    latch();
};
PinStatus shiftreg::digitalRead(pin_size_t pin) {
    return Buffer ? (PinStatus)((Buffer[P2B(pin, bufferNum)] >> pin % 8) & 1) : (PinStatus)0;
};
void shiftreg::analogWrite(pin_size_t pin, int val) {
    return digitalWrite(pin, (PinStatus)(bool)val);
};
int shiftreg::analogRead(pin_size_t pin) {
    return digitalRead(pin) == HIGH ? ((1 << analogResolution) - 1) : 0;
};

} // namespace _EVLK_SHIFTREG_
