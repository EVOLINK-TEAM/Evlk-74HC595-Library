#ifndef _EVLK_74HC595_H_
#define _EVLK_74HC595_H_
#include "evlk_shiftreg.h"

namespace _EVLK_SHIFTREG_ {

class hc595 : public _EVLK_SHIFTREG_::shiftreg {
  private:
    using nopin_size_t = _EVLK_NOPINS_::nopin_size_t;
    void _shift(bool bit) override;
    virtual void _latch() override;

    nopin_size_t DS;
    nopin_size_t ST;
    nopin_size_t SH;

  public:
    hc595(nopin_size_t DS, nopin_size_t ST, nopin_size_t SH, uint8_t num = 1);
    ~hc595();
    void Begin();
};

}; // namespace _EVLK_SHIFTREG_
#endif