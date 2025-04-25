#ifndef _EVLK_HV5523_H_
#define _EVLK_HV5523_H_
#include "evlk_shiftreg.h"

namespace _EVLK_SHIFTREG_ {

class hv5523 : public _EVLK_SHIFTREG_::shiftreg {
  private:
    using nopin_size_t = _EVLK_NOPINS_::nopin_size_t;
    void _shift(bool bit) override;
    virtual void _latch() override;

    nopin_size_t DIN;
    nopin_size_t LE;
    nopin_size_t CLK;

  public:
    hv5523(nopin_size_t DIN, nopin_size_t LE, nopin_size_t CLK, uint8_t num = 1);
    ~hv5523();
    void Begin();
};

}; // namespace _EVLK_SHIFTREG_
#endif