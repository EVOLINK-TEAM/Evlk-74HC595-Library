#ifndef _EVLK_SHIFTREG_H_
#define _EVLK_SHIFTREG_H_

#include "Arduino.h"
#include "evlk_nopins.h"

namespace _EVLK_SHIFTREG_ {

class shiftreg : public _EVLK_NOPINS_::nopinRegister {
  private:
    using nopin_size_t = _EVLK_NOPINS_::nopin_size_t;
    const uint8_t bufferNum;

  protected:
    virtual void _shift(bool bit) = 0;
    virtual void _latch()         = 0;
    virtual void _shift8(uint8_t data);
    void bufferShift_1(bool);
    void bufferShift_8(uint8_t);
    uint8_t *initBuffer(uint8_t byteNum);

    shiftreg(uint8_t byteNum);
    ~shiftreg();

  public:
    uint8_t *const Buffer; // Buffers,save the state of the registers,小端

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
    void pinMode(pin_size_t pin, PinMode mode) override {}; // INFO: NOT WORK
    void analogReference(uint8_t mode) override {};         // INFO: NOT WORK
};

}; // namespace _EVLK_SHIFTREG_

#endif
