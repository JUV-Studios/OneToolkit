#include "pch.h"
#include "LineEndingHelper.h"
#include "LineEndingHelper.g.cpp"
#include <Exports/StringHelper.h>

namespace winrt::OneToolkit::implementation
{
	hstring LineEndingHelper::GetNewLineString(LineEnding ending) noexcept
	{
		switch (ending)
		{
			case LineEnding::CR: return L"\r"; break;
			case LineEnding::LF: return L"\n"; break;
			case LineEnding::CRLF: return L"\r\n"; break;
			default: return L"\r"; break;
		}
	}

	hstring LineEndingHelper::FormatLines(array_view<hstring const> lines, LineEnding ending)
	{
		hstring finalString;
		auto newLineString = LineEndingHelper::GetNewLineString(ending);
		for (uint32_t index = 0; index < lines.size(); ++index)
		{
			hstring suffix = index == lines.size() - 1 ? L"" : newLineString;
			finalString = finalString + lines[index] + suffix;
		}
		
		return finalString;
	}

	hstring LineEndingHelper::NormalizeLineEnding(hstring const& text, LineEnding ending)
	{
		if (ending == LineEnding::CR)
		{
			auto crlf_cr = StringHelper::Replace(text, L"\r\n", L"\r");
			auto lfcr_cr = StringHelper::Replace(crlf_cr, L"\n\r", L"\r");
			return StringHelper::Replace(lfcr_cr, L"\n", L"\r");
		}
		if (ending == LineEnding::LF)
		{
			auto crlf_lf = StringHelper::Replace(text, L"\r\n", L"\n");
			auto lfcr_lf = StringHelper::Replace(crlf_lf, L"\n\r", L"\n");
			return StringHelper::Replace(lfcr_lf, L"\r", L"\n");
		}
		else
		{
			return StringHelper::Replace(NormalizeLineEnding(text, LineEnding::CR), L"\r", L"\r\n");
		}
	}
}
