#include "pch.h"
#include "Reflection.h"
#include "Runtime.Reflection.g.cpp"
#include <rometadataresolution.h>

using namespace winrt;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Runtime::implementation
{
	hstring Reflection::GetMetadataFileLocation(TypeName typeId)
	{
		hstring path;
		check_hresult(RoGetMetaDataFile(static_cast<HSTRING>(::get_abi(typeId.Name)), nullptr, reinterpret_cast<HSTRING*>(put_abi(path)), nullptr, nullptr));
		return path;
	}
}