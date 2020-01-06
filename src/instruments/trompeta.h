#ifndef TROMPETA
#define TROMPETA

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class Trompeta: public upc::Instrument {
    EnvelopeADSR adsr;
    float index;
    float F0;
    float salto;
	float A;
    std::vector<float> tbl;
  public:
    Trompeta(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
