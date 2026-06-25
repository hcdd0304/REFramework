#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class REManagedObject;

namespace sdk::shared_data {
    enum class VariableType : uint8_t {
        REManagedObject,
        Number,
        String,
        None
    };

    void set_variable(std::string_view key, ::REManagedObject* value);
    void set_variable(std::string_view key, double value);
    void set_variable(std::string_view key, std::string_view value);

    VariableType get_variable_type(std::string_view key);

    void clear_variable(std::string_view key);

    ::REManagedObject* get_variable_re_managed_object(std::string_view key);
    double get_variable_number(std::string_view key);
    std::string get_variable_string(std::string_view key);
}
