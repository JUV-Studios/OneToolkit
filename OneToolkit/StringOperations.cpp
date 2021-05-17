#include "pch.h"
#include "StringOperations.h"
#include "Data.Text.StringOperations.g.cpp"

using namespace winrt;
using namespace Windows::Foundation::Collections;

namespace winrt::OneToolkit::Data::Text::implementation
{
	hstring StringOperations::Trim(hstring const& text, hstring const& trimChars)
	{
		hstring output;
		check_hresult(WindowsTrimStringStart(static_cast<HSTRING>(winrt::get_abi(text)), static_cast<HSTRING>(winrt::get_abi(trimChars)), reinterpret_cast<HSTRING*>(winrt::put_abi(output))));
		check_hresult(WindowsTrimStringEnd(static_cast<HSTRING>(winrt::get_abi(output)), static_cast<HSTRING>(winrt::get_abi(trimChars)), reinterpret_cast<HSTRING*>(winrt::put_abi(output))));
		return output;
	}

	hstring StringOperations::TrimStart(hstring const& text, hstring const& trimChars)
	{
		hstring output;
		check_hresult(WindowsTrimStringStart(static_cast<HSTRING>(winrt::get_abi(text)), static_cast<HSTRING>(winrt::get_abi(trimChars)), reinterpret_cast<HSTRING*>(winrt::put_abi(output))));
		return output;
	}

	hstring StringOperations::TrimEnd(hstring const& text, hstring const& trimChars)
	{
		hstring output;
		check_hresult(WindowsTrimStringEnd(static_cast<HSTRING>(winrt::get_abi(text)), static_cast<HSTRING>(winrt::get_abi(trimChars)), reinterpret_cast<HSTRING*>(winrt::put_abi(output))));
		return output;
	}

	hstring StringOperations::Replace(hstring const& text, hstring const& replaced, hstring const& replacedWith)
	{
		hstring output;
		check_hresult(WindowsReplaceString(static_cast<HSTRING>(winrt::get_abi(text)), static_cast<HSTRING>(winrt::get_abi(replaced)), static_cast<HSTRING>(winrt::get_abi(replacedWith)), reinterpret_cast<HSTRING*>(winrt::put_abi(output))));
		return output;
	}

	IVectorView<hstring> StringOperations::Split(hstring const& text, hstring const& delimiter)
	{
		std::vector<hstring> values;
		std::wstring textCopy { text };
		wchar_t* state = nullptr;
		auto token = wcstok_s(textCopy.data(), delimiter.data(), &state);
		while (token != nullptr)
		{
			values.emplace_back(token);
			token = wcstok_s(nullptr, delimiter.data(), &state);
		}

		return single_threaded_vector(std::move(values)).GetView();
	}
}