﻿// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in cross-platform C++ projects.

#pragma once
#include <cstdint>

namespace winrt::Windows::UI
{
    struct Color
    {
        uint8_t A;
        uint8_t R;
        uint8_t G;
        uint8_t B;
    };

    inline bool operator==(Color const& left, Color const& right) noexcept
    {
        return left.A == right.A && left.R == right.R && left.G == right.G && left.B == right.B;
    }

    inline bool operator!=(Color const& left, Color const& right) noexcept
    {
        return !(left == right);
    }
}