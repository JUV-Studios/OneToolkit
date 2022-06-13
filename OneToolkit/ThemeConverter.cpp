﻿#include "UI.ThemeConverter.g.h"

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		struct ThemeConverter : ThemeConverterT<ThemeConverter>
		{
			ThemeConverter() = delete;

			static AbsoluteTheme ToAbsoluteTheme(RelativeTheme value, AbsoluteTheme defaultTheme) noexcept
			{
				return value == RelativeTheme::Default ? defaultTheme : AbsoluteTheme(static_cast<int>(value) - 1);
			}
		};
	}

	namespace factory_implementation
	{
		struct ThemeConverter : ThemeConverterT<ThemeConverter, implementation::ThemeConverter>
		{
		};
	}
}

#include "UI.ThemeConverter.g.cpp"