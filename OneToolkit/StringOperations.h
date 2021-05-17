#pragma once
#include "Data.Text.StringOperations.g.h"

namespace winrt::OneToolkit::Data::Text
{
    namespace implementation
    {
        struct StringOperations : StringOperationsT<StringOperations>
        {
            StringOperations() = delete;
            static hstring Trim(hstring const& text, hstring const& trimChars);
            static hstring TrimStart(hstring const& text, hstring const& trimChars);
            static hstring TrimEnd(hstring const& text, hstring const& trimChars);
            static hstring Replace(hstring const& text, hstring const& replaced, hstring const& replacedWith);
            static Windows::Foundation::Collections::IVectorView<hstring> Split(hstring const& text, hstring const& delimiter);
        };
    }

    namespace factory_implementation
    {
        struct StringOperations : StringOperationsT<StringOperations, implementation::StringOperations>
        {
        };
    }
}