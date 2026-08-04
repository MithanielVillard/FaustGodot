#pragma once
#include <optional>
#include <godot_cpp/templates/list.hpp>
#include <godot_cpp/core/property_info.hpp>

class IPropertyHandler
{
public:
    virtual void AddProperty(godot::PropertyInfo const& property) = 0;
    virtual void SetProperty(godot::StringName const& name, godot::Variant const& value) = 0;
    virtual std::optional<godot::Variant> GetProperty(godot::StringName const& name) = 0;
    virtual godot::List<godot::PropertyInfo>& GetPropertyList() = 0;
    virtual void NotifyPropertyChanged() = 0;
};
