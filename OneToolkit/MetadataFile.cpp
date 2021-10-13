#include "pch.h"
#include "MetadataFile.h"

using namespace juv;
using namespace winrt;
using namespace Windows::UI::Xaml::Interop;
using namespace OneToolkit::System;

namespace winrt::OneToolkit::Runtime::implementation
{
	MetadataFile::MetadataFile(TypeName typeId)
	{
		check_hresult(MetaDataGetDispenser(CLSID_CorMetaDataDispenser, IID_IMetaDataDispenserEx, m_Dispenser.put_void()));
		check_hresult(RoGetMetaDataFile(static_cast<HSTRING>(::get_abi(typeId.Name)), m_Dispenser.get(), reinterpret_cast<HSTRING*>(put_abi(m_Path)), m_MetadataImport.put(), &m_Token));
	}

	hstring MetadataFile::RuntimeVersion() const
	{
		std::array<wchar_t, MAX_PACKAGE_NAME> buffer;
		*buffer.end() = L'\0';
		uint32 returnedSize;
		check_hresult(m_MetadataImport->GetVersionString(buffer.data(), static_cast<DWORD>(buffer.size()), reinterpret_cast<DWORD*>(&returnedSize)));
		return buffer.data();
	}
}