#pragma once
#include "defines.h"

#include <faust/dsp/dsp.h>

class GodotDsp
{
public:
    GodotDsp();
    ~GodotDsp();

    int32 GetNumInputs()  const { return m_dspInstance->getNumInputs();  }
    int32 GetNumOutputs() const { return m_dspInstance->getNumOutputs(); }
    int32 GetSampleRate() const { return m_dspInstance->getSampleRate(); }

    void buildUserInterface(UI* uiInterface) const { m_dspInstance->buildUserInterface(uiInterface); }

    void Compute(int32 count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { m_dspInstance->compute(count, inputs, outputs); }
private:
    dsp* m_dspInstance;
};
