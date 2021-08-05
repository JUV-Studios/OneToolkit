#include "pch.h"
#include "FileSizeHelper.h"
#include "Storage.FileSizeHelper.g.cpp"

using namespace juv;

namespace winrt::OneToolkit::Storage::implementation
{
	constexpr std::array BinaryUnits{ L"B", L"KiB", L"MiB", L"GiB", L"TiB", L"PiB", L"EiB", L"ZiB", L"YiB" };
	constexpr std::array DecimalUnits{ L"B", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB", L"ZB", L"YB" };

	hstring FileSizeHelper::ToFormattedString(uint64 size, bool useBinaryPrefix)
	{
		uint8 index = 0;
		auto preciseSize = static_cast<double>(size);
		const auto exponent = useBinaryPrefix ? 1024 : 1000;
		while (preciseSize >= exponent)
		{
			preciseSize /= exponent;
			++index;
		}

		if (index > 8)
		{
			index = 0;
			preciseSize = static_cast<double>(size);
		}

		return hstring(std::format(L"{0} {1}", preciseSize, useBinaryPrefix ? BinaryUnits[index] : DecimalUnits[index]));
	}

	uint64 FileSizeHelper::FromFormattedString(hstring const& formattedstring, bool useBinaryPrefix)
	{
		throw hresult_not_implemented();
	}
}