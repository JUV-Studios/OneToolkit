#pragma once
#include "UI.Fonts.g.h"

namespace winrt::OneToolkit::UI
{
    namespace implementation
    {
        struct Fonts : FontsT<Fonts>
        {
            Fonts() = delete;
            static hstring DefaultTextFontFamily();
            static hstring DefaultIconFontFamily();
            static Windows::Foundation::Collections::IVectorView<hstring> InstalledFontFamilies();
        };
    }

    namespace factory_implementation
    {
        struct Fonts : FontsT<Fonts, implementation::Fonts>
        {
        };
    }
}