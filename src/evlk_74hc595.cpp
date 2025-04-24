#include "evlk_74hc595.h"

#define pMode(pin, val) _EVLK_NOPINS_::pinMode((pin), (PinMode)(val))
#define dWrite(pin, val) _EVLK_NOPINS_::digitalWrite((pin), (PinStatus)(val))

#define SER_U dWrite(DS, HIGH)
#define SER_D dWrite(DS, LOW)
#define SCK_U dWrite(SH, HIGH)
#define SCK_D dWrite(SH, LOW)
#define RCK_U dWrite(ST, HIGH)
#define RCK_D dWrite(ST, LOW)

namespace _EVLK_SHIFTREG_ {
hc595::hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num)
    : DS(DS), ST(ST), SH(SH), shiftreg(num) {};
hc595::~hc595() {
    pMode(DS, INPUT);
    pMode(ST, INPUT);
    pMode(SH, INPUT);
}
void hc595::_shift(bool bit) {
    bit ? SER_U : SER_D;
    SCK_U;
    delayMicroseconds(1);
    SCK_D;
};
void hc595::_latch() {
    RCK_U;
    delayMicroseconds(1);
    RCK_D;
};

void hc595::Begin() {
    pMode(DS, OUTPUT);
    pMode(ST, OUTPUT);
    pMode(SH, OUTPUT);
}
}; // namespace _EVLK_SHIFTREG_