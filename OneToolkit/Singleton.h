#pragma once
#include "Singleton.g.h"

namespace winrt::OneToolkit
{
    namespace implementation
    {
        struct Singleton : SingletonT<Singleton>
        {
        public:
            Singleton() = delete;
            static IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName const& typeId);
            static IInspectable GetInstance(Windows::UI::Xaml::Interop::TypeName const& typeId, SingletonInstanceProvider const& provider);
            static void SetInstance(Windows::UI::Xaml::Interop::TypeName const& typeId, IInspectable const& value);
        private:
            static hstring GetStoreTypeName(hstring const& typeName);
            static void AddInstance(hstring const& storeTypeName, IInspectable const& value);
        };
    }

    namespace factory_implementation
    {
        struct Singleton : SingletonT<Singleton, implementation::Singleton>
        {
        };
    }
}