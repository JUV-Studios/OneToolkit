#pragma once
#include "ObservableBase.g.h"

namespace winrt::OneToolkit::Mvvm
{
    namespace implementation
    {
        struct ObservableBase : ObservableBaseT<ObservableBase>
        {
        public:
            virtual bool Decide(hstring const& propertyName);
            virtual void WhenPropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventArgs const& args);
            void Raise(hstring const& propertyName);
            event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
            void PropertyChanged(event_token token) noexcept;
        private:
            event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;
        };
    }
    
    namespace factory_implementation
    {
        struct ObservableBase : ObservableBaseT<ObservableBase, implementation::ObservableBase>
        {
        };
    }
}