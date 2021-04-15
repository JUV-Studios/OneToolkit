#pragma once
#include "LineEndingHelper.g.h"

namespace winrt::OneToolkit
{
    namespace implementation
    {
        struct LineEndingHelper : LineEndingHelperT<LineEndingHelper>
        {
            LineEndingHelper() = delete;
            static hstring GetNewLineString(LineEnding ending) noexcept;
            static hstring FormatLines(array_view<hstring const> lines, LineEnding ending);
            static hstring NormalizeLineEnding(hstring const& text, LineEnding ending);
        };
    }

    namespace factory_implementation
    {
        struct LineEndingHelper : LineEndingHelperT<LineEndingHelper, implementation::LineEndingHelper>
        {
        };
    }
}