#include "evlk_hv5523.h"

#define pMode(pin, val) _EVLK_NOPINS_::pinMode((pin), (PinMode)(val))
#define dWrite(pin, val) _EVLK_NOPINS_::digitalWrite((pin), (PinStatus)(val))

#define SER_U dWrite(DIN, HIGH)
#define SER_D dWrite(DIN, LOW)
#define SCK_U dWrite(CLK, HIGH)
#define SCK_D dWrite(CLK, LOW)
#define RCK_U dWrite(LE, HIGH)
#define RCK_D dWrite(LE, LOW)

namespace _EVLK_SHIFTREG_ {
hv5523::hv5523(nopin_size_t DIN, nopin_size_t LE, nopin_size_t CLK, uint8_t num)
    : shiftreg(num * 4), DIN(DIN), LE(LE), CLK(CLK) {};
hv5523::~hv5523() {
    pMode(DIN, INPUT);
    pMode(LE, INPUT);
    pMode(CLK, INPUT);
}
void hv5523::_shift(bool bit) {
    bit ? SER_U : SER_D;
    SCK_U;
    delayMicroseconds(1);
    SCK_D;
    delayMicroseconds(1);
};
void hv5523::_latch() {
    RCK_U;
    delayMicroseconds(1);
    RCK_D;
};

void hv5523::Begin() {
    pMode(DIN, OUTPUT);
    pMode(LE, OUTPUT);
    pMode(CLK, OUTPUT);
    _EVLK_NOPINS_::digitalWrite(DIN, LOW);
    _EVLK_NOPINS_::digitalWrite(LE, LOW);
    _EVLK_NOPINS_::digitalWrite(CLK, LOW);
}
}; // namespace _EVLK_SHIFTREG_