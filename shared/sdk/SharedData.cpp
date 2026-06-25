#include <memory>
#include <string>
#include <unordered_map>

#include "SharedData.hpp"

namespace sdk::shared_data {
    struct Variable {
        VariableType type{VariableType::None};
        ::REManagedObject* obj_value{nullptr};
        double double_value{0.0};
        int64_t int64_value{0};
    };

    using VariableMap = std::unordered_map<std::string, Variable>;

    static VariableMap& get_variable_map() {
        static std::unique_ptr<VariableMap> map = std::make_unique<VariableMap>();
        return *map;
    }

    void set_variable(std::string_view key, ::REManagedObject* value) {
        auto& var = get_variable_map()[std::string(key)];
        var.type = VariableType::REManagedObject;
        var.obj_value = value;
    }

    void set_variable(std::string_view key, double value) {
        auto& var = get_variable_map()[std::string(key)];
        var.type = VariableType::Double;
        var.double_value = value;
    }

    void set_variable(std::string_view key, int64_t value) {
        auto& var = get_variable_map()[std::string(key)];
        var.type = VariableType::Int64;
        var.int64_value = value;
    }

    VariableType get_variable_type(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end()) {
            return VariableType::None;
        }

        return it->second.type;
    }

    ::REManagedObject* get_variable_re_managed_object(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end() || it->second.type != VariableType::REManagedObject) {
            return nullptr;
        }

        return it->second.obj_value;
    }

    double get_variable_double(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end() || it->second.type != VariableType::Double) {
            return 0.0;
        }

        return it->second.double_value;
    }

    int64_t get_variable_int64(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end() || it->second.type != VariableType::Int64) {
            return 0;
        }

        return it->second.int64_value;
    }
}
