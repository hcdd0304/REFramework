#pragma once

#include <msclr/marshal_cppstd.h>

#include "ManagedObject.hpp"

#pragma managed

namespace REFrameworkNET {
public enum class SharedDataType : int32_t {
    REManagedObject = 0,
    Number = 1,
    String = 2,
    None = 3,
};

public ref class SharedData sealed {
public:
    static void SetVariable(System::String^ key, ManagedObject^ value);
    static void SetVariable(System::String^ key, double value);
    static void SetVariable(System::String^ key, System::String^ value);

    static SharedDataType GetVariableType(System::String^ key);

    static void ClearVariable(System::String^ key);

    static ManagedObject^ GetVariableManagedObject(System::String^ key);
    static double GetVariableNumber(System::String^ key);
    static System::String^ GetVariableString(System::String^ key);
};
}
