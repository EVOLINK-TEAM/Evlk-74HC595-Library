#include "evlk_74hc595.h"

#define pMode(pin, val) (pin).mode((PinMode)(val))
#define dWrite(pin, val) (pin).dwrite((PinStatus)(val))

#define SER_U dWrite(DS,HIGH)
#define SER_D dWrite(DS,LOW)
#define SCK_U dWrite(SH,HIGH)
#define SCK_D dWrite(SH,LOW)
#define RCK_U dWrite(ST,HIGH)
#define RCK_D dWrite(ST,LOW)

namespace _EVLK_74HC595_
{
    uint8_t *hc595::initBuffer(uint8_t num)
    {
        if (!num)
            return NULL;
        pin_size_t *arr = new pin_size_t[num]();
        return arr;
    };
    uint8_t *hc595::getBuffer(nopin_size_t &pin) { return isIn(pin) ? &Buffer[pin.Idx / 8] : NULL; }
    hc595::hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num)
        : nopinRegister(num, 8), Num(num), Buffer(initBuffer(num)),
          DS(DS), ST(ST), SH(SH)
    {
        pMode(DS, OUTPUT);
        pMode(ST, OUTPUT);
        pMode(SH, OUTPUT);
    };
    hc595::hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num, pin_size_t *maps)
        : nopinRegister(num, 8, maps), Num(num), Buffer(initBuffer(num)),
          DS(DS), ST(ST), SH(SH)
    {
        pMode(DS, OUTPUT);
        pMode(ST, OUTPUT);
        pMode(SH, OUTPUT);
    };
    hc595::hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num, pin_size_t maphead)
        : nopinRegister(num, 8, maphead), Num(num), Buffer(initBuffer(num)),
          DS(DS), ST(ST), SH(SH)
    {
        pMode(DS, OUTPUT);
        pMode(ST, OUTPUT);
        pMode(SH, OUTPUT);
    };
    hc595::~hc595()
    {
        if (Buffer)
            delete[] Buffer;
    }

    void hc595::bufferShift_1(bool bit)
    {
        if (Num == 0)
            return;
        bool preLastBit = bit;
        for (size_t i = 0; i < Num; ++i)
        {
            bool lastBit = Buffer[i] & 1;
            Buffer[i] = (Buffer[i] >> 1) | (preLastBit << 7);
            preLastBit = lastBit;
        }
    };
    void hc595::bufferShift_8(uint8_t data)
    {
        if (Num == 0)
            return;
        memmove(Buffer + 1, Buffer, Num - 1);
        Buffer[0] = data;
    }
    void hc595::_shift(bool bit)
    {
        if (bit)
            SER_U;
        else
            SER_D;
        SCK_U;
        delayMicroseconds(1);
        SCK_D;
    };
    void hc595::_latch()
    {
        RCK_U;
        delayMicroseconds(1);
        RCK_D;
    };
    void hc595::_send(uint8_t data)
    {
        for (size_t i = 0; i < 8; i++)
        {
            // bool b = data >> 7;
            // data = data << 1;
            bool b = data & 1;
            data = data >> 1;
            _shift(b);
        }
    };

    void hc595::shift(bool bit)
    {
        bufferShift_1(bit);
        _shift(bit);
        _latch();
    };
    void hc595::send(uint8_t data)
    {
        bufferShift_8(data);
        _send(data);
        _latch();
    };
    void hc595::clearBuffer() { memset(Buffer, 0, Num); };
    void hc595::updateBuffer()
    {
        for (size_t i = 0; i < Num; i++)
            _send(Buffer[Num - i - 1]);
        _latch();
    };

    void hc595::digitalWrite(nopin_size_t &pin, PinStatus val)
    {
        uint8_t *buffer = getBuffer(pin);
        if (!buffer)
            return;
        uint8_t mask = 1 << (UnitNum - 1) >> unitNum(pin);
        if (val)
            *buffer |= mask;
        else
            *buffer &= ~mask;
        updateBuffer();
    };
    PinStatus hc595::digitalRead(nopin_size_t &pin)
    {
        uint8_t *buffer = getBuffer(pin);
        if (!buffer)
            return (PinStatus)0;
        uint8_t mask = 1 << UnitNum >> unitNum(pin);
        return (PinStatus)(*buffer & mask);
    };
    void hc595::analogWrite(nopin_size_t &pin, int val) { return digitalWrite(pin, (PinStatus)(bool)val); };
    int hc595::analogRead(nopin_size_t &pin) { return digitalRead(pin) == HIGH ? ((1 << analogResolution) - 1) : 0; };
}