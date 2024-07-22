#ifndef _EVLK_74HC595_H_
#define _EVLK_74HC595_H_

#include "Arduino.h"
#include "evlk_nopins.h"

namespace _EVLK_74HC595_
{
    class hc595 : public _EVLK_NOPINS_::nopinRegister
    {
    private:
        using nopin_size_t = _EVLK_NOPINS_::nopin_size_t;
        uint8_t *const Buffer; // Arduino
        nopin_size_t DS;
        nopin_size_t ST;
        nopin_size_t SH;

        pin_size_t *initBuffer(uint8_t num);
        pin_size_t *getBuffer(nopin_size_t &pin);
        void bufferShift_1(bool);
        void bufferShift_8(uint8_t);
        void _shift(bool bit);
        void _latch();
        void _send(uint8_t data);

    public:
        const uint8_t Num;

        hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num);
        hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num, pin_size_t *maps);
        hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num, pin_size_t maphead);
        ~hc595();
        void Begin();            //init driver pins
        void shift(bool bit);    // 向寄存器中输入1bit
        void send(uint8_t data); // send = 8 * shift
        void clearBuffer();      //! If use real MR pin
        void updateBuffer();     // 更新Mask到寄存器中。

        using nopinRegister::digitalWrite;
        using nopinRegister::pinMode;
        void digitalWrite(nopin_size_t &pin, PinStatus val) override;
        PinStatus digitalRead(nopin_size_t &pin) override;
        void analogWrite(nopin_size_t &pin, int val) override; //! Same as digitalWrite
        int analogRead(nopin_size_t &pin) override;            //! Same as digitalRead

        void analogReference(uint8_t mode) override {};            //! NOT USE
        void pinMode(nopin_size_t &pin, PinMode mode) override {}; //! NOT USE
    };
};

#endif