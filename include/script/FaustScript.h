#pragma once

#include <list>
#include <faust/dsp/interpreter-dsp.h>
#include <godot_cpp/classes/script_extension.hpp>
#include <godot_cpp/classes/script_language.hpp>

class IFaustHandler;

namespace godot
{

class AudioStreamFaust;

class FaustScript : public ScriptExtension
{
GDCLASS(FaustScript, ScriptExtension)

public:
    using ListIt = std::list<IFaustHandler*>::iterator;

    FaustScript() = default;
    ~FaustScript() override;

    String _get_source_code() const override;
    void _set_source_code(const String &p_code) override;
    bool _has_source_code() const override;

    bool _can_instantiate() const override;
    Error _reload(bool p_keep_state) override;
    Ref<Script> _get_base_script() const override;
    bool _is_tool() const override;
    bool _has_static_method(const StringName &p_method) const override;
    void _update_exports() override;
    StringName _get_instance_base_type() const override;

    TypedArray<Dictionary> _get_documentation() const override;
    ScriptLanguage* _get_language() const override;
    interpreter_dsp_factory* get_dsp_factory() const { return m_pFactory; }
    StringName _get_doc_class_name() const override;

    ListIt Attach(IFaustHandler& audioStream);
    void Detach(ListIt it);

protected:
    static void _bind_methods();

private:
    String m_sourceCode;
    interpreter_dsp_factory* m_pFactory {};
    std::list<IFaustHandler*> m_audioStreams {};
};

}

