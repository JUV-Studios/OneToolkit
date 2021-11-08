#pragma once
#include "Storage.SettingsService.g.h"

namespace winrt::OneToolkit::Storage::implementation
{
    struct SettingsService : SettingsServiceT<SettingsService>
    {
        SettingsService(Windows::Storage::ApplicationDataContainer const& dataContainer);
        juv::auto_property<Windows::Storage::ApplicationDataContainer> const DataContainer;
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
