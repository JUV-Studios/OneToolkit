#include "pch.h"
#include "MetadataFile.h"
#include <rometadataresolution.h>

using namespace winrt;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Runtime::implementation
{
	MetadataFile::MetadataFile(TypeName typeId)
	{
		check_hresult(RoGetMetaDataFile(static_cast<HSTRING>(::get_abi(typeId.Name)), nullptr, reinterpret_cast<HSTRING*>(put_abi(m_Path)), nullptr, nullptr));
	}
}