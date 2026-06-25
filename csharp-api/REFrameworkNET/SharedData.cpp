#include <msclr/marshal_cppstd.h>

#include "API.hpp"
#include "ManagedObject.hpp"

#include "SharedData.hpp"

namespace REFrameworkNET {
namespace detail {
static const REFrameworkSDKFunctions* get_functions() {
    return REFrameworkNET::API::GetNativeImplementation()->sdk()->functions;
}

// Match sdk::shared_data::VariableType values
constexpr int SHARED_DATA_TYPE_NONE = 3;
constexpr int SHARED_DATA_TYPE_REMANAGEDOBJECT = 0;
constexpr int SHARED_DATA_TYPE_NUMBER = 1;
constexpr int SHARED_DATA_TYPE_STRING = 2;

// Converts a System::String^ (UTF-16) to a UTF-8 std::string
static std::string to_utf8(System::String^ str) {
    if (str == nullptr) {
        return {};
    }

    auto bytes = System::Text::Encoding::UTF8->GetBytes(str);
    if (bytes->Length == 0) {
        return {};
    }

    pin_ptr<System::Byte> p = &bytes[0];
    return std::string(reinterpret_cast<const char*>(p), bytes->Length);
}

// Converts a UTF-8 const char* to a System::String^ (UTF-16)
static System::String^ from_utf8(const char* str) {
    if (str == nullptr) {
        return nullptr;
    }

    auto len = (int)std::strlen(str);
    if (len == 0) {
        return gcnew System::String("");
    }

    auto bytes = gcnew cli::array<System::Byte>(len);
    pin_ptr<System::Byte> p = &bytes[0];
    memcpy(p, str, len);

    return System::Text::Encoding::UTF8->GetString(bytes);
}
}

void SharedData::SetVariable(System::String^ key, ManagedObject^ value) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    auto handle = (REFrameworkManagedObjectHandle)(value != nullptr ? value->Ptr() : nullptr);
    detail::get_functions()->shared_data_set_variable_managed_object(native_key.c_str(), handle);
}

void SharedData::SetVariable(System::String^ key, double value) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    detail::get_functions()->shared_data_set_variable_number(native_key.c_str(), value);
}

void SharedData::SetVariable(System::String^ key, System::String^ value) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    auto native_value = detail::to_utf8(value);
    detail::get_functions()->shared_data_set_variable_string(native_key.c_str(), native_value.c_str());
}

SharedDataType SharedData::GetVariableType(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    return (SharedDataType)detail::get_functions()->shared_data_get_variable_type(native_key.c_str());
}

void SharedData::ClearVariable(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    detail::get_functions()->shared_data_clear_variable(native_key.c_str());
}

ManagedObject^ SharedData::GetVariableManagedObject(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    auto functions = detail::get_functions();
    auto type = functions->shared_data_get_variable_type(native_key.c_str());

    if (type == detail::SHARED_DATA_TYPE_NONE) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableManagedObject: variable '" + key + "' does not exist");
    }

    if (type != detail::SHARED_DATA_TYPE_REMANAGEDOBJECT) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableManagedObject: variable '" + key + "' is not a REManagedObject");
    }

    auto val = functions->shared_data_get_variable_managed_object(native_key.c_str());

    if (val == nullptr) {
        return nullptr;
    }

    return ManagedObject::Get<ManagedObject>(val);
}

double SharedData::GetVariableNumber(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    auto functions = detail::get_functions();
    auto type = functions->shared_data_get_variable_type(native_key.c_str());

    if (type == detail::SHARED_DATA_TYPE_NONE) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableNumber: variable '" + key + "' does not exist");
    }

    if (type != detail::SHARED_DATA_TYPE_NUMBER) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableNumber: variable '" + key + "' is not a Number");
    }

    return functions->shared_data_get_variable_number(native_key.c_str());
}

System::String^ SharedData::GetVariableString(System::String^ key) {
    if (key == nullptr) {
        throw gcnew System::ArgumentNullException("key");
    }

    auto native_key = detail::to_utf8(key);
    auto functions = detail::get_functions();
    auto type = functions->shared_data_get_variable_type(native_key.c_str());

    if (type == detail::SHARED_DATA_TYPE_NONE) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableString: variable '" + key + "' does not exist");
    }

    if (type != detail::SHARED_DATA_TYPE_STRING) {
        throw gcnew System::InvalidOperationException(
            "SharedData.GetVariableString: variable '" + key + "' is not a String");
    }

    auto val = functions->shared_data_get_variable_string(native_key.c_str());
    return detail::from_utf8(val);
}
}
