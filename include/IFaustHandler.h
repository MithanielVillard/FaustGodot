#pragma once
#include "script/FaustScript.h"

class dsp;

//This must be an interface because of Godot Class Static
class IFaustHandler
{
public:
    virtual void UpdateDsp() = 0;

    virtual void set_faust_dsp(godot::Ref<godot::FaustScript> const& script) = 0;
    virtual godot::Ref<godot::FaustScript> get_faust_dsp() const = 0;

protected:
    godot::FaustScript::ListIt m_faustScriptIt;
    godot::Ref<godot::FaustScript> m_faustScript;
};
