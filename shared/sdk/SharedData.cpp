#include <memory>
#include <string>
#include <unordered_map>

#include "SharedData.hpp"

namespace sdk::shared_data {
    struct Variable {
        VariableType type{VariableType::None};
        ::REManagedObject* obj_value{nullptr};
        double number_value{0.0};
        std::string string_value{};
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
        var.type = VariableType::Number;
        var.number_value = value;
    }

    void set_variable(std::string_view key, std::string_view value) {
        auto& var = get_variable_map()[std::string(key)];
        var.type = VariableType::String;
        var.string_value = value;
    }

    VariableType get_variable_type(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end()) {
            return VariableType::None;
        }

        return it->second.type;
    }

    void clear_variable(std::string_view key) {
        auto& map = get_variable_map();
        map.erase(std::string(key));
    }

    ::REManagedObject* get_variable_re_managed_object(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end() || it->second.type != VariableType::REManagedObject) {
            return nullptr;
        }

        return it->second.obj_value;
    }

    double get_variable_number(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end() || it->second.type != VariableType::Number) {
            return 0.0;
        }

        return it->second.number_value;
    }

    std::string get_variable_string(std::string_view key) {
        auto& map = get_variable_map();
        auto it = map.find(std::string(key));

        if (it == map.end() || it->second.type != VariableType::String) {
            return {};
        }

        return it->second.string_value;
    }
}
