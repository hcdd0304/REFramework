-- Set some variables that C# can read
local my_obj = sdk.get_managed_singleton("app.PlayerManager")
log.info("my_obj type: " .. my_obj:get_type_definition():get_full_name())
sdk.shared_data.set_variable("app", my_obj)
sdk.shared_data.set_variable("pi", 3.14159)
sdk.shared_data.set_variable("count", 42)
sdk.shared_data.set_variable("stringVal", "Xin chào thế giới!")

re.on_draw_ui(function()
    -- Read what C# wrote
    local t = sdk.shared_data.get_variable_type("cs_result")

    if t == sdk.SharedDataType.String then
        imgui.text("C# wrote: " .. sdk.shared_data.get_variable_string("cs_result"))
    elseif t == sdk.SharedDataType.Number then
        imgui.text("C# wrote: " .. sdk.shared_data.get_variable_number("cs_result"))
    elseif t == sdk.SharedDataType.REManagedObject then
        local obj = sdk.shared_data.get_variable_managed_object("cs_result")
        imgui.text("C# wrote object: " .. obj:get_type_definition():get_full_name())
    else
        imgui.text("No data from C# yet")
    end
end)