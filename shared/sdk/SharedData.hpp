#pragma once

#include <cstdint>
#include <string_view>

struct REManagedObject;

namespace sdk::shared_data {
    enum class VariableType : uint8_t {
        REManagedObject,
        Double,
        Int64,
        None
    };

    void set_variable(std::string_view key, ::REManagedObject* value);
    void set_variable(std::string_view key, double value);
    void set_variable(std::string_view key, int64_t value);

    VariableType get_variable_type(std::string_view key);

    ::REManagedObject* get_variable_re_managed_object(std::string_view key);
    double get_variable_double(std::string_view key);
    int64_t get_variable_int64(std::string_view key);
}
