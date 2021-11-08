// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in cross-platform C++ projects.

#pragma once
namespace winrt::Windows
{
#if __has_include(<winrt/Windows.UI.h>)
#include <winrt/Windows.UI.h>
#else
	namespace UI
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
#endif
#if __has_include(<winrt/Windows.Security.Cryptography.h>)
#include <winrt/Windows.Security.Cryptography.h>
#else
	namespace Security::Cryptography
	{
		enum class BinaryStringEncoding : int32_t
		{
			Utf8 = 0,
			Utf16LE = 1,
			Utf16BE = 2,
		};
	}
#endif
}