#pragma once

#include <deque>
#include <unordered_set>
#include <spdlog/spdlog.h>

#include <utility/FunctionHook.hpp>
#include <safetyhook.hpp>
#include <sdk/TDBVer.hpp>

#include "../Mod.hpp"

#define SUPPORT_PATH_CUSTOM_PLATFORM_PREFIX (TDB_VER >= 81)

class LooseFileLoader : public Mod {
public:
    static std::shared_ptr<LooseFileLoader>& get();

public:
    LooseFileLoader();
    std::string_view get_name() const override { return "LooseFileLoader"; }

    std::optional<std::string> on_initialize() override;
    void on_config_load(const utility::Config& cfg) override;
    void on_config_save(utility::Config& cfg) override;
    
    void on_frame() override;
    void on_draw_ui() override;

    void hook();

    bool is_enabled() const {
        return m_enabled->value();
    }

private:
    bool handle_path(const wchar_t* path, size_t hash);

#if TDB_VER > 67
    static uint64_t path_to_hash_hook(const wchar_t* path);
#else
    static uint64_t path_to_hash_hook(void* This, const wchar_t* path);
#endif

    bool m_hook_success{false};
    bool m_attempted_hook{false};
    uint32_t m_files_encountered{};
    uint32_t m_uncached_hits{};
    uint32_t m_cache_hits{};
    uint32_t m_loose_files_loaded{};

    std::shared_mutex m_mutex{};
    std::deque<std::wstring> m_recent_accessed_files{}; // max 100
    std::deque<std::wstring> m_recent_loose_files{}; // max 100
    std::unordered_set<std::wstring> m_all_accessed_files{};
    std::unordered_set<std::wstring> m_all_loose_files{};

    std::unordered_set<size_t> m_files_on_disk{};
    std::unordered_set<size_t> m_seen_files{};
    std::shared_mutex m_files_on_disk_mutex{};

    std::unique_ptr<FunctionHook> m_path_to_hash_hook{nullptr};

    ModToggle::Ptr m_enabled{ ModToggle::create(generate_name("Enabled")) };
    ModToggle::Ptr m_log_accessed_files{ ModToggle::create(generate_name("LogAccessedFiles")) };
    ModToggle::Ptr m_log_loose_files{ ModToggle::create(generate_name("LogLooseFiles")) };
    bool m_show_recent_files{false}; // Not persistent because its for dev purposes
    bool m_enable_file_cache{true};

    std::shared_ptr<spdlog::logger> m_logger{nullptr};
    std::shared_ptr<spdlog::logger> m_loose_file_logger{nullptr};

#pragma region Loose File Support for other platforms (EGS, ...)
#if SUPPORT_PATH_CUSTOM_PLATFORM_PREFIX
    ModToggle::Ptr m_enable_custom_platform_prefix{ ModToggle::create(generate_name("EnableCustomPlatformPrefix"), false) };
    ModString::Ptr m_target_platform_prefix{ ModString::create(generate_name("TargetPlatformPrefix"), "STM") };

    std::vector<safetyhook::MidHook> m_loose_file_path_sprintf_hooks;
    std::wstring cached_platform_string{L""};
    std::shared_mutex m_platform_prefix_mutex{};

    static void loose_file_path_sprintf_hook_wrapper(safetyhook::Context& context);
    void loose_file_path_sprintf_hook(safetyhook::Context& context);
    void find_and_hook_sprintf_for_loose_file_paths();
#endif
#pragma endregion

    ValueList m_options{
        *m_enabled,
        *m_log_accessed_files,
        *m_log_loose_files,
#if SUPPORT_PATH_CUSTOM_PLATFORM_PREFIX
        *m_enable_custom_platform_prefix,
        *m_target_platform_prefix
#endif
    };
};