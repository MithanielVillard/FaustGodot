#include "GodotDsp.h"

#include <faust/dsp/llvm-dsp.h>
#include <godot_cpp/classes/audio_server.hpp>

using namespace godot;

GodotDsp::GodotDsp()
{
    std::string theCode = "import(\"stdfaust.lib\"); process = no.noise;";
    std::string error_string;

    const char* argv = "";
    llvm_dsp_factory* pFactory = createDSPFactoryFromString("faust", theCode, 0, &argv, "", error_string);
    m_dspInstance = pFactory->createDSPInstance();

    m_dspInstance->init(static_cast<int>(AudioServer::get_singleton()->get_mix_rate()));
}

GodotDsp::~GodotDsp()
{
    delete m_dspInstance;
}
