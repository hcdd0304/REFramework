#include <msclr/marshal_cppstd.h>

#include "sdk/SharedData.hpp"

#include "API.hpp"
#include "ManagedObject.hpp"

#include "SharedData.hpp"

namespace REFrameworkNET {
void SharedData::SetVariable(System::String^ key, ManagedObject^ value) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    ::sdk::shared_data::set_variable(native_key, (::REManagedObject*)value->Ptr());
}

void SharedData::SetVariable(System::String^ key, double value) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    ::sdk::shared_data::set_variable(native_key, value);
}

void SharedData::SetVariable(System::String^ key, System::Int64 value) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    ::sdk::shared_data::set_variable(native_key, (int64_t)value);
}

SharedDataType SharedData::GetVariableType(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    return (SharedDataType)(int)::sdk::shared_data::get_variable_type(native_key);
}

ManagedObject^ SharedData::GetVariableManagedObject(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    auto type = ::sdk::shared_data::get_variable_type(native_key);

    if (type == ::sdk::shared_data::VariableType::None) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableManagedObject: variable '" + key + "' does not exist");
    }

    if (type != ::sdk::shared_data::VariableType::REManagedObject) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableManagedObject: variable '" + key + "' is not a REManagedObject");
    }

    auto val = ::sdk::shared_data::get_variable_re_managed_object(native_key);

    if (val == nullptr) {
        return nullptr;
    }

    return ManagedObject::Get<ManagedObject>(val);
}

double SharedData::GetVariableDouble(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    auto type = ::sdk::shared_data::get_variable_type(native_key);

    if (type == ::sdk::shared_data::VariableType::None) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableDouble: variable '" + key + "' does not exist");
    }

    if (type != ::sdk::shared_data::VariableType::Double) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableDouble: variable '" + key + "' is not a Double");
    }

    return ::sdk::shared_data::get_variable_double(native_key);
}

System::Int64 SharedData::GetVariableInt64(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = msclr::interop::marshal_as<std::string>(key);
    auto type = ::sdk::shared_data::get_variable_type(native_key);

    if (type == ::sdk::shared_data::VariableType::None) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableInt64: variable '" + key + "' does not exist");
    }

    if (type != ::sdk::shared_data::VariableType::Int64) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableInt64: variable '" + key + "' is not an Int64");
    }

    return ::sdk::shared_data::get_variable_int64(native_key);
}
}
