#pragma once
#include "Storage.SettingsService.g.h"

namespace winrt::OneToolkit::Storage::implementation
{
    struct SettingsService : SettingsServiceT<SettingsService>
    {
        SettingsService(Windows::Storage::ApplicationDataContainer const& dataContainer);
        DeclareAutoProperty(Windows::Storage::ApplicationDataContainer, DataContainer, nullptr);
        IInspectable GetValue(hstring const& key, IInspectable const& defaultValue = nullptr) const;
        void SetValue(hstring const& key, IInspectable const& newValue) const;
    };
}

namespace winrt::OneToolkit::Storage::factory_implementation
{
    struct SettingsService : SettingsServiceT<SettingsService, implementation::SettingsService>
    {
    };
}
