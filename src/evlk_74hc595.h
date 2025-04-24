#ifndef _EVLK_74HC595_H_
#define _EVLK_74HC595_H_

#include "Arduino.h"
#include "evlk_nopins.h"

namespace _EVLK_74HC595_ {

class hc595 : public _EVLK_NOPINS_::nopinRegister {
  private:
    using nopin_size_t = _EVLK_NOPINS_::nopin_size_t;
    nopin_size_t DS;
    nopin_size_t ST;
    nopin_size_t SH;

    pin_size_t *initBuffer(uint8_t num);
    void bufferShift_1(bool);
    void bufferShift_8(uint8_t);
    void _shift(bool bit);
    void _shift8(uint8_t data);

  public:
    uint8_t *const Buffer; // Buffers,save the state of the registers,小端
    const uint8_t Num;     // How many of 74HC595

    hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num = 1);
    ~hc595();
    void Begin(); // init driver pins

    void shift(bool bit);      // 向寄存器中输入1bit
    void shift8(uint8_t data); // 8 * shift ，高位先移
    void latch();              // 应用寄存器状态
    void uploadBuffer();       // 更新Bufffer到寄存器中。

    using nopinRegister::digitalWrite;
    using nopinRegister::pinMode;
    void digitalWrite(pin_size_t pin, PinStatus val) override;
    void digitalWrite(uint8_t mask[], int n, int status) override;
    PinStatus digitalRead(pin_size_t pin) override;         // NOTE: Read statue form Buffers
    void analogWrite(pin_size_t pin, int val) override;     // INFO: Same as digitalWrite
    int analogRead(pin_size_t pin) override;                // INFO: Same as digitalRead
    void pinMode(pin_size_t pin, PinMode mode) override {}; // NOTE: NOT USE
    void analogReference(uint8_t mode) override {};         // NOTE: NOT USE
};

}; // namespace _EVLK_74HC595_

#endif
