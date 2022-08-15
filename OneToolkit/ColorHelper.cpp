// Code adapted from https://github.com/CommunityToolkit/WindowsCommunityToolkit/blob/main/Microsoft.Toolkit.Uwp/Helpers/ColorHelper.cs.

#include "UI.ColorHelper.g.h"
#include "PredefinedColors.h"
#include <format>
#include <string>
#include <ranges>
#include <CorError.h>
#include <wil/safecast.h>
#include <winrt/Windows.Foundation.Collections.h>
#undef max
#undef min

import juv;
import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::UI;
using namespace Windows::Foundation::Collections;
using namespace OneToolkit::System;

namespace winrt::OneToolkit::UI
{
	namespace implementation
	{
		struct ColorHelper : ColorHelperT<ColorHelper>
		{
			ColorHelper() = delete;

			static IMapView<hstring, Color> PredefinedColors()
			{
				return single_threaded_map(GetPredefinedColors()).GetView();
			}

			static Color ToColor(hstring const& colorString)
			{
				auto const ThrowArgumentException = []()
				{
					throw hresult_error(COR_E_ARGUMENT, L"The parameter \"colorString\" must not be null or empty.");
				};

				auto const ThrowFormatException = []() -> Color
				{
					throw hresult_error(COR_E_FORMAT, L"The parameter \"colorString\" is not a recognized Color format.");
				};

				if (colorString.empty())
				{
					ThrowArgumentException();
				}

				if (colorString[0] == L'#')
				{
					switch (colorString.size())
					{
					case 9:
					{
						auto const cuint = wil::safe_cast<uint32>(std::stoul(std::wstring(StringOperations::Substring(colorString, 1)), nullptr, 16));
						auto a = static_cast<uint8>(cuint >> 24);
						auto r = static_cast<uint8>((cuint >> 16) & 0xff);
						auto g = static_cast<uint8>((cuint >> 8) & 0xff);
						auto b = static_cast<uint8>(cuint & 0xff);

						return { a, r, g, b };
					}

					case 7:
					{
						auto const cuint = wil::safe_cast<uint32>(std::stoul(std::wstring(StringOperations::Substring(colorString, 1)), nullptr, 16));
						auto r = static_cast<uint8>((cuint >> 16) & 0xff);
						auto g = static_cast<uint8>((cuint >> 8) & 0xff);
						auto b = static_cast<uint8>(cuint & 0xff);

						return { 255, r, g, b };
					}

					case 5:
					{
						auto const cuint = wil::safe_cast<uint16>(std::stoul(std::wstring(StringOperations::Substring(colorString, 1)), nullptr, 16));
						auto a = static_cast<uint8>(cuint >> 12);
						auto r = static_cast<uint8>((cuint >> 8) & 0xf);
						auto g = static_cast<uint8>((cuint >> 4) & 0xf);
						auto b = static_cast<uint8>(cuint & 0xf);
						a = static_cast<uint8>(a << 4 | a);
						r = static_cast<uint8>(r << 4 | r);
						g = static_cast<uint8>(g << 4 | g);
						b = static_cast<uint8>(b << 4 | b);

						return { a, r, g, b };
					}

					case 4:
					{
						auto const cuint = wil::safe_cast<uint16>(std::stoul(std::wstring(StringOperations::Substring(colorString, 1)), nullptr, 16));
						auto r = static_cast<uint8>((cuint >> 8) & 0xf);
						auto g = static_cast<uint8>((cuint >> 4) & 0xf);
						auto b = static_cast<uint8>(cuint & 0xf);
						r = static_cast<uint8>(r << 4 | r);
						g = static_cast<uint8>(g << 4 | g);
						b = static_cast<uint8>(b << 4 | b);

						return { 255, r, g, b };
					}

					default: return ThrowFormatException();
					}
				}


				if (colorString.size() > 3 && colorString[0] == L's' && colorString[1] == L'c' && colorString[2] == L'#')
				{
					auto const values = StringOperations::Split(colorString, L",");;
					if (values.size() == 4)
					{

						// TODO implement InvariantCulture conversion.
						auto const scA = std::stod(std::wstring(StringOperations::Substring(values[0], 3)));
						auto const scR = std::stod(std::wstring(values[1]));
						auto const scG = std::stod(std::wstring(values[2]));
						auto const scB = std::stod(std::wstring(values[3]));

						return { static_cast<uint8>(scA * 255), static_cast<uint8>(scR * 255), static_cast<uint8>(scG * 255), static_cast<uint8>(scB * 255) };
					}

					if (values.size() == 3)
					{
						auto const scR = std::stod(std::wstring(StringOperations::Substring(values[0], 3)));
						auto const scG = std::stod(std::wstring(values[1]));
						auto const scB = std::stod(std::wstring(values[2]));

						return { 255, static_cast<uint8>(scR * 255), static_cast<uint8>(scG * 255), static_cast<uint8>(scB * 255) };
					}

					return ThrowFormatException();
				}

				if (auto const prop = PredefinedColors().TryLookup(colorString))
				{
					return *prop;
				}

				return ThrowFormatException();
			}

			/// <summary>
			/// Converts a <see cref="Color"/> to a hexadecimal string representation.
			/// </summary>
			/// <param name="color">The color to convert.</param>
			/// <returns>The hexadecimal string representation of the color.</returns>
			static hstring ToHex(Color color)
			{
				// TODO
				return L"";
				return std::format(L"#{:X2}{:X2}{:X2}{:X2}", color.A, color.R, color.G, color.B).data();
				// return $"#{color.A:X2}{color.R:X2}{color.G:X2}{color.B:X2}";
			}

			/// <summary>
			/// Converts a <see cref="Color"/> to a premultiplied Int32 - 4 byte ARGB structure.
			/// </summary>
			/// <param name="color">The color to convert.</param>
			/// <returns>The int representation of the color.</returns>
			static int ToInt(Color color)
			{
				auto const a = color.A + 1;
				auto const col = (color.A << 24) | (static_cast<uint8>((color.R * a) >> 8) << 16) | (static_cast<uint8>((color.G * a) >> 8) << 8) | static_cast<uint8>((color.B * a) >> 8);
				return col;
			}

			static HslColor ToHsl(Color color)
			{
				double constexpr toDouble = 1.0 / 255;
				auto const r = toDouble * color.R;
				auto const g = toDouble * color.G;
				auto const b = toDouble * color.B;
				auto const max = std::max(std::max(r, g), b);
				auto const min = std::min(std::min(r, g), b);
				auto const chroma = max - min;
				double h1;

				if (chroma == 0)
				{
					h1 = 0;
				}
				else if (max == r)
				{
					// The % operator doesn't do proper modulo on negative
					// numbers, so we'll add 6 before using it
					h1 = std::fmod(((g - b) / chroma) + 6, 6);
				}
				else if (max == g)
				{
					h1 = 2 + ((b - r) / chroma);
				}
				else
				{
					h1 = 4 + ((r - g) / chroma);
				}

				double const lightness = 0.5 * (max + min);
				double const saturation = chroma == 0 ? 0 : chroma / (1 - std::abs((2 * lightness) - 1));
				HslColor ret;
				ret.H = 60 * h1;
				ret.S = saturation;
				ret.L = lightness;
				ret.A = toDouble * color.A;
				return ret;
			}

			static HsvColor ToHsv(Color color)
			{
				double constexpr toDouble = 1.0 / 255;
				auto const r = toDouble * color.R;
				auto const g = toDouble * color.G;
				auto const b = toDouble * color.B;
				auto const max = std::max(std::max(r, g), b);
				auto const min = std::min(std::min(r, g), b);
				auto const chroma = max - min;
				double h1;

				if (chroma == 0)
				{
					h1 = 0;
				}
				else if (max == r)
				{
					// The % operator doesn't do proper modulo on negative
					// numbers, so we'll add 6 before using it
					h1 = std::fmod(((g - b) / chroma) + 6, 6);
				}
				else if (max == g)
				{
					h1 = 2 + ((b - r) / chroma);
				}
				else
				{
					h1 = 4 + ((r - g) / chroma);
				}

				double const saturation = chroma == 0 ? 0 : chroma / max;
				HsvColor ret;
				ret.H = 60 * h1;
				ret.S = saturation;
				ret.V = max;
				ret.A = toDouble * color.A;
				return ret;
			}

			/// <summary>
			/// Creates a <see cref="Color"/> from the specified hue, saturation, lightness, and alpha values.
			/// </summary>
			/// <param name="hue">0..360 range hue</param>
			/// <param name="saturation">0..1 range saturation</param>
			/// <param name="lightness">0..1 range lightness</param>
			/// <param name="alpha">0..1 alpha</param>
			/// <returns>The created <see cref="Color"/>.</returns>
			static Color FromHsl(double hue, double saturation, double lightness, double alpha = 1.0)
			{
				if (hue < 0 || hue > 360)
				{
					throw hresult_error(COR_E_ARGUMENTOUTOFRANGE, L"hue");
				}

				double chroma = (1 - std::abs((2 * lightness) - 1)) * saturation;
				double h1 = hue / 60;
				double x = chroma * (1 - std::abs(std::fmod(h1, 2) - 1));
				double m = lightness - (0.5 * chroma);
				double r1, g1, b1;

				if (h1 < 1)
				{
					r1 = chroma;
					g1 = x;
					b1 = 0;
				}
				else if (h1 < 2)
				{
					r1 = x;
					g1 = chroma;
					b1 = 0;
				}
				else if (h1 < 3)
				{
					r1 = 0;
					g1 = chroma;
					b1 = x;
				}
				else if (h1 < 4)
				{
					r1 = 0;
					g1 = x;
					b1 = chroma;
				}
				else if (h1 < 5)
				{
					r1 = x;
					g1 = 0;
					b1 = chroma;
				}
				else
				{
					r1 = chroma;
					g1 = 0;
					b1 = x;
				}

				auto const r = static_cast<uint8>(255 * (r1 + m));
				auto const g = static_cast<uint8>(255 * (g1 + m));
				auto const b = static_cast<uint8>(255 * (b1 + m));
				auto const a = static_cast<uint8>(255 * alpha);

				return { a, r, g, b };
			}

			/// <summary>
			/// Creates a <see cref="Color"/> from the specified hue, saturation, value, and alpha values.
			/// </summary>
			/// <param name="hue">0..360 range hue</param>
			/// <param name="saturation">0..1 range saturation</param>
			/// <param name="value">0..1 range value</param>
			/// <param name="alpha">0..1 alpha</param>
			/// <returns>The created <see cref="Color"/>.</returns>
			static Color FromHsv(double hue, double saturation, double value, double alpha = 1.0)
			{
				if (hue < 0 || hue > 360)
				{
					throw hresult_error(COR_E_ARGUMENTOUTOFRANGE, L"hue");
				}

				double chroma = value * saturation;
				double h1 = hue / 60;
				double x = chroma * (1 - std::abs(std::fmod(h1, 2) - 1));
				double m = value - chroma;
				double r1, g1, b1;

				if (h1 < 1)
				{
					r1 = chroma;
					g1 = x;
					b1 = 0;
				}
				else if (h1 < 2)
				{
					r1 = x;
					g1 = chroma;
					b1 = 0;
				}
				else if (h1 < 3)
				{
					r1 = 0;
					g1 = chroma;
					b1 = x;
				}
				else if (h1 < 4)
				{
					r1 = 0;
					g1 = x;
					b1 = chroma;
				}
				else if (h1 < 5)
				{
					r1 = x;
					g1 = 0;
					b1 = chroma;
				}
				else
				{
					r1 = chroma;
					g1 = 0;
					b1 = x;
				}

				auto const r = static_cast<uint8>(255 * (r1 + m));
				auto const g = static_cast<uint8>(255 * (g1 + m));
				auto const b = static_cast<uint8>(255 * (b1 + m));
				auto const a = static_cast<uint8>(255 * alpha);

				return { a, r, g, b };
			}
		};
	}

	namespace factory_implementation
	{
		struct ColorHelper : ColorHelperT<ColorHelper, implementation::ColorHelper>
		{
		};
	}
}

#include "UI.ColorHelper.g.cpp"