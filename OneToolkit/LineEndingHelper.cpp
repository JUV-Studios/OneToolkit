#include "pch.h"
#include "LineEndingHelper.h"
#include "Data.Text.LineEndingHelper.g.cpp"

namespace winrt::OneToolkit::Data::Text::implementation
{
	hstring LineEndingHelper::GetNewLineString(LineEnding lineEnding) noexcept
	{
		if (lineEnding == LineEnding::CR)
		{
			return L"\r";
		}
		else if (lineEnding == LineEnding::LF)
		{
			return L"\n";
		}
		else if (lineEnding == LineEnding::CRLF)
		{
			return L"\r\n";
		}
		else
		{
			return L"";
		}
	}

	hstring LineEndingHelper::NormalizeLineEnding(hstring const& text, LineEnding lineEnding)
	{
		if (lineEnding == LineEnding::CR)
		{
			auto crlf_cr = StringOperations::Replace(text, L"\r\n", L"\r");
			auto lfcr_cr = StringOperations::Replace(crlf_cr, L"\n\r", L"\r");
			return StringOperations::Replace(lfcr_cr, L"\n", L"\r");
		}
		else if (lineEnding == LineEnding::LF)
		{
			auto crlf_lf = StringOperations::Replace(text, L"\r\n", L"\n");
			auto lfcr_lf = StringOperations::Replace(crlf_lf, L"\n\r", L"\n");
			return StringOperations::Replace(lfcr_lf, L"\r", L"\n");
		}
		else if (lineEnding == LineEnding::CRLF)
		{
			return StringOperations::Replace(NormalizeLineEnding(text, LineEnding::CR), L"\r", L"\r\n");
		}
		else
		{
			return text;
		}
	}
}