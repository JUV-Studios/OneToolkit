#pragma once
#include "ObservableBase.g.h"
#include "ObservableBaseHelper.g.h"

namespace winrt::OneToolkit::Mvvm
{
    namespace implementation
    {
        struct ObservableBase : ObservableBaseT<ObservableBase>
        {
        public:
            ObservableBase() = default;
            bool SuppressEvents() const noexcept;
            void SuppressEvents(bool value);
            virtual bool Decide(hstring const& propertyName);
            virtual void WhenPropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventArgs const& args);
            void Raise(hstring const& propertyName);
            event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
            void PropertyChanged(event_token token) noexcept;
        private:
            bool m_SuppressEvents;
            event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
        };

        struct ObservableBaseHelper : ObservableBaseHelperT<ObservableBaseHelper>
        {
            ObservableBaseHelper() = delete;
            static void Raise(OneToolkit::Mvvm::ObservableBase const& instance, hstring const& propertyName);
        };
    }

    namespace factory_implementation
    {
        struct ObservableBase : ObservableBaseT<ObservableBase, implementation::ObservableBase>
        {
        };

        struct ObservableBaseHelper : ObservableBaseHelperT<ObservableBaseHelper, implementation::ObservableBaseHelper>
        {
        };
    }
}