using System;
using System.Collections;
using System.Collections.Generic;
using System.Dynamic;
using System.Reflection;
using Hexa.NET.ImGui;
using REFrameworkNET;
using REFrameworkNET.Callbacks;
using REFrameworkNET.Attributes;

public class MyPlugin {
    [Callback(typeof(ImGuiRender), CallbackType.Pre)]
    public static void RenderImGui() {
        // Read what Lua wrote
        var appType = SharedData.GetVariableType("app");
        var appTypeFullName = "None";
        if (appType == SharedDataType.REManagedObject) {
            var app = SharedData.GetVariableManagedObject("app");
            appTypeFullName = app.GetTypeDefinition().GetFullName();
            //API.LogInfo("Read via.Application from Lua via SharedData!");
        }

        var pi = SharedData.GetVariableNumber("pi");
        var count = SharedData.GetVariableNumber("count");
        var stringVal = SharedData.GetVariableString("stringVal");

        //API.LogInfo($"Lua sent pi={pi}, count={count}, stringVal={stringVal}");
        if (ImGui.Begin("TestInterop.cs")) {
            ImGui.Text($"Lua sent pi={pi}, count={count}, stringVal={stringVal}, appType={appTypeFullName}");
            ImGui.End();
        }

        // Write back for Lua to read
        SharedData.SetVariable("cs_result", pi * 2.0);
    }

    // public void OnFrame() {
    //     // You can also exchange data outside of draw
    //     if (SharedData.GetVariableType("app") != SharedDataType.None) {
    //         // Lua already set the data, do something
    //     }
    // }
}