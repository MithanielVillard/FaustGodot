#include "script/FaustScript.h"
#include "script/FaustScriptLanguage.h"
#include "AudioStreamFaust.h"
#include "IFaustHandler.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

FaustScript::~FaustScript()
{
    deleteInterpreterDSPFactory(m_pFactory);
}

String FaustScript::_get_source_code() const
{
    return m_sourceCode;
}

void FaustScript::_set_source_code(const String &p_code)
{
    m_sourceCode = p_code;
    _reload(true);
}

bool FaustScript::_has_source_code() const
{
    return !m_sourceCode.is_empty();
}

bool FaustScript::_can_instantiate() const
{
    return false;
}

Error FaustScript::_reload(bool p_keep_state)
{
    const char* argv[] =  { "--import-dir bin/libraries" };
    std::string error_msg;
    
    m_pFactory = createInterpreterDSPFactoryFromString("godot", m_sourceCode.ascii().get_data(), 1, argv, error_msg);
    if (!m_pFactory)
    {
        UtilityFunctions::printerr(error_msg.data());
        return FAILED;
    }

    for (IFaustHandler* handler : m_audioStreams)
        handler->UpdateDsp();

    return OK;
}

Ref<Script> FaustScript::_get_base_script() const
{
    return ScriptExtension::_get_base_script();
}

bool FaustScript::_is_tool() const
{
    return false;
}

bool FaustScript::_has_static_method(const StringName &p_method) const
{
    return false;
}

void FaustScript::_update_exports()
{
    ScriptExtension::_update_exports();
}

TypedArray<Dictionary> FaustScript::_get_documentation() const
{
    Dictionary dic;
    dic["name"] = "Faust";
    return TypedArray<Dictionary> { dic };
}

ScriptLanguage* FaustScript::_get_language() const
{
    return FaustScriptLanguage::get_instance();
}

StringName FaustScript::_get_doc_class_name() const
{
    return "FaustScript";
}

FaustScript::ListIt FaustScript::Attach(IFaustHandler& audioStream)
{
    m_audioStreams.emplace_back(&audioStream);
    auto end = m_audioStreams.end();
    std::advance(end, -1);
    return end;
}

void FaustScript::Detach(ListIt const it)
{
    if (!m_audioStreams.empty())
        m_audioStreams.erase(it);
}

void FaustScript::_bind_methods(){}
