#pragma once
#include "UI.FontHelper.g.h"

namespace winrt::OneToolkit::UI
{
    namespace implementation
    {
        struct FontHelper : FontHelperT<FontHelper>
        {
            FontHelper() = delete;
            static hstring DefaultIconFontFamily();
            static Windows::Foundation::Collections::IVectorView<hstring> GetInstalledFontFamilies();
        };
    }

    namespace factory_implementation
    {
        struct FontHelper : FontHelperT<FontHelper, implementation::FontHelper>
        {
        };
    }
}