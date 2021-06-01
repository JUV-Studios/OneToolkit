#pragma once
#include "UI.FontHelper.g.h"

namespace winrt::OneToolkit::UI
{
    namespace implementation
    {
        struct FontHelper : FontHelperT<FontHelper>
        {
            FontHelper() = delete;
            static Windows::Foundation::Collections::IVectorView<hstring> InstalledFontFamilies();
        };
    }

    namespace factory_implementation
    {
        struct FontHelper : FontHelperT<FontHelper, implementation::FontHelper>
        {
        };
    }
}