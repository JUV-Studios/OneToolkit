#pragma once
#include "Data.Text.LineEndingHelper.g.h"

namespace winrt::OneToolkit::Data::Text
{
    namespace implementation
    {
        struct LineEndingHelper : LineEndingHelperT<LineEndingHelper>
        {
            LineEndingHelper() = delete;
            static hstring GetNewLineString(LineEnding lineEnding) noexcept;
            static hstring NormalizeLineEnding(hstring const& text, LineEnding lineEnding);
        };
    }

    namespace factory_implementation
    {
        struct LineEndingHelper : LineEndingHelperT<LineEndingHelper, implementation::LineEndingHelper>
        {
        };
    }
}