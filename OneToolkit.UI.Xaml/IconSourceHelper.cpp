#include "pch.h"
#include "IconSourceHelper.h"

using namespace OneToolkit::UI::Xaml::Controls;

namespace WUXC = Windows::UI::Xaml::Controls;
namespace MUXC = Microsoft::UI::Xaml::Controls;

WUXC::IconSource^ IconSourceHelper::ToPlatform(MUXC::IconSource^ iconSource)
{
	if (auto fontIconSource = dynamic_cast<MUXC::FontIconSource^>(iconSource))
	{
		auto result = ref new WUXC::FontIconSource;
		result->FontFamily = fontIconSource->FontFamily;
		result->FontSize = fontIconSource->FontSize;
		result->FontStyle = fontIconSource->FontStyle;
		result->FontWeight = fontIconSource->FontWeight;
		result->Foreground = fontIconSource->Foreground;
		result->Glyph = fontIconSource->Glyph;
		result->IsTextScaleFactorEnabled = fontIconSource->IsTextScaleFactorEnabled;
		result->MirroredWhenRightToLeft = fontIconSource->MirroredWhenRightToLeft;
		return result;
	}
	else if (auto bitmapIconSource = dynamic_cast<MUXC::BitmapIconSource^>(iconSource))
	{
		auto result = ref new WUXC::BitmapIconSource;
		result->Foreground = bitmapIconSource->Foreground;
		result->ShowAsMonochrome = bitmapIconSource->ShowAsMonochrome;
		result->UriSource = bitmapIconSource->UriSource;
		return result;
	}
	else if (auto pathIconSource = dynamic_cast<MUXC::PathIconSource^>(iconSource))
	{
		auto result = ref new WUXC::PathIconSource;
		result->Data = pathIconSource->Data;
		result->Foreground = pathIconSource->Foreground;
		return result;
	}
	else if (auto symbolIconSource = dynamic_cast<MUXC::SymbolIconSource^>(iconSource))
	{
		auto result = ref new WUXC::SymbolIconSource;
		result->Foreground = symbolIconSource->Foreground;
		result->Symbol = symbolIconSource->Symbol;
		return result;
	}
	else
	{
		return nullptr;
	}
}

MUXC::IconSource^ IconSourceHelper::ToLifted(WUXC::IconSource^ iconSource)
{
	if (auto fontIconSource = dynamic_cast<WUXC::FontIconSource^>(iconSource))
	{
		auto result = ref new MUXC::FontIconSource;
		result->FontFamily = fontIconSource->FontFamily;
		result->FontSize = fontIconSource->FontSize;
		result->FontStyle = fontIconSource->FontStyle;
		result->FontWeight = fontIconSource->FontWeight;
		result->Foreground = fontIconSource->Foreground;
		result->Glyph = fontIconSource->Glyph;
		result->IsTextScaleFactorEnabled = fontIconSource->IsTextScaleFactorEnabled;
		result->MirroredWhenRightToLeft = fontIconSource->MirroredWhenRightToLeft;
		return result;
	}
	else if (auto bitmapIconSource = dynamic_cast<WUXC::BitmapIconSource^>(iconSource))
	{
		auto result = ref new MUXC::BitmapIconSource;
		result->Foreground = bitmapIconSource->Foreground;
		result->ShowAsMonochrome = bitmapIconSource->ShowAsMonochrome;
		result->UriSource = bitmapIconSource->UriSource;
		return result;
	}
	else if (auto pathIconSource = dynamic_cast<WUXC::PathIconSource^>(iconSource))
	{
		auto result = ref new MUXC::PathIconSource;
		result->Data = pathIconSource->Data;
		result->Foreground = pathIconSource->Foreground;
		return result;
	}
	else if (auto symbolIconSource = dynamic_cast<WUXC::SymbolIconSource^>(iconSource))
	{
		auto result = ref new MUXC::SymbolIconSource;
		result->Foreground = symbolIconSource->Foreground;
		result->Symbol = symbolIconSource->Symbol;
		return result;
	}
	else
	{
		return nullptr;
	}
}

WUXC::IconSourceElement^ IconSourceHelper::CreateElement(WUXC::IconSource^ iconSource)
{
	auto srcElement = ref new WUXC::IconSourceElement;
	srcElement->IconSource = iconSource;
	return srcElement;
}