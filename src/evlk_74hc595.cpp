#include "evlk_74hc595.h"

#define pMode(pin, val) _EVLK_NOPINS_::pinMode((pin), (PinMode)(val))
#define dWrite(pin, val) _EVLK_NOPINS_::digitalWrite((pin), (PinStatus)(val))

#define SER_U dWrite(DS, HIGH)
#define SER_D dWrite(DS, LOW)
#define SCK_U dWrite(SH, HIGH)
#define SCK_D dWrite(SH, LOW)
#define RCK_U dWrite(ST, HIGH)
#define RCK_D dWrite(ST, LOW)

#define P2B(pin, Num) (Num - pin / 8 - 1)

namespace _EVLK_74HC595_ {

uint8_t *hc595::initBuffer(uint8_t num) {
    if (!num)
        return NULL;
    pin_size_t *arr = new pin_size_t[num]();
    return arr;
};
hc595::hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num)
    : Num(num), Buffer(initBuffer(num)), DS(DS), ST(ST), SH(SH) {};
hc595::~hc595() {
    if (Buffer)
        delete[] Buffer;
    pMode(DS, INPUT);
    pMode(ST, INPUT);
    pMode(SH, INPUT);
}
void hc595::Begin() {
    pMode(DS, OUTPUT);
    pMode(ST, OUTPUT);
    pMode(SH, OUTPUT);
}
void hc595::bufferShift_1(bool bit) {
    for (size_t i = 0; i < Num - 1; i++) {
        Buffer[i] = (Buffer[i] << 1) | (Buffer[i + 1] >> 7);
    }
    Buffer[Num - 1] = (Buffer[Num - 1] << 1) | bit;
};
void hc595::bufferShift_8(uint8_t data) {
    memmove(Buffer, Buffer + 1, Num - 1);
    Buffer[Num - 1] = data;
}
void hc595::_shift(bool bit) {
    if (bit)
        SER_U;
    else
        SER_D;
    SCK_U;
    delayMicroseconds(1);
    SCK_D;
};
void hc595::_shift8(uint8_t data) {
    for (size_t i = 0; i < 8; i++) {
        bool b = data >> 7;
        data   = data << 1;
        // bool b = data & 1;
        // data   = data >> 1;
        _shift(b);
    }
};

void hc595::shift(bool bit) {
    bufferShift_1(bit);
    _shift(bit);
};
void hc595::shift8(uint8_t data) {
    bufferShift_8(data);
    _shift8(data);
};
void hc595::latch() {
    RCK_U;
    delayMicroseconds(1);
    RCK_D;
};

void hc595::uploadBuffer() {
    for (size_t i = 0; i < Num; i++)
        _shift8(Buffer[i]);
};

void hc595::digitalWrite(pin_size_t pin, PinStatus val) {
    val ? Buffer[P2B(pin, Num)] |= (1 << pin % 8) : Buffer[P2B(pin, Num)] &= ~(1 << pin % 8);
    uploadBuffer();
    latch();
};
void hc595::digitalWrite(uint8_t mask[], int n, int status) {
    for (int i = 0; i < n; i++)
        status ? Buffer[Num - i - 1] |= mask[n - i - 1] : Buffer[Num - i - 1] &= ~mask[n - i - 1];
    uploadBuffer();
    latch();
};
PinStatus hc595::digitalRead(pin_size_t pin) {
    return (PinStatus)((Buffer[P2B(pin, Num)] >> pin % 8) & 1);
};
void hc595::analogWrite(pin_size_t pin, int val) {
    return digitalWrite(pin, (PinStatus)(bool)val);
};
int hc595::analogRead(pin_size_t pin) {
    return digitalRead(pin) == HIGH ? ((1 << analogResolution) - 1) : 0;
};

} // namespace _EVLK_74HC595_
