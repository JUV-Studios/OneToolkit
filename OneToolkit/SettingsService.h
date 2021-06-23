#pragma once
#include "Storage.SettingsService.g.h"

namespace winrt::OneToolkit::Storage::implementation
{
    struct SettingsService : SettingsServiceT<SettingsService>
    {
    public:
        SettingsService(Windows::Storage::ApplicationDataContainer const& dataContainer);
        Windows::Storage::ApplicationDataContainer DataContainer() const noexcept;
        IInspectable GetValue(hstring const& key, IInspectable const& defaultValue = nullptr);
        void SetValue(hstring const& key, IInspectable const& newValue);
    private:
        Windows::Storage::ApplicationDataContainer m_DataContainer;
    };
}

namespace winrt::OneToolkit::Storage::factory_implementation
{
    struct SettingsService : SettingsServiceT<SettingsService, implementation::SettingsService>
    {
    };
}
