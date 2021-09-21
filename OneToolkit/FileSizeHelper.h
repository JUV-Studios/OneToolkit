#pragma once
#include "Storage.FileSizeHelper.g.h"

namespace winrt::OneToolkit::Storage
{
	namespace implementation
	{
		struct FileSizeHelper : FileSizeHelperT<FileSizeHelper>
		{
			FileSizeHelper() = delete;
			static hstring ToFormattedString(juv::uint64 size, bool useBinaryPrefix);
			static juv::uint64 FromFormattedString(hstring const& formattedString, bool useBinaryPrefix);
		};
	}
	
	namespace factory_implementation
	{
		struct FileSizeHelper : FileSizeHelperT<FileSizeHelper, implementation::FileSizeHelper>
		{
		};
	}
}