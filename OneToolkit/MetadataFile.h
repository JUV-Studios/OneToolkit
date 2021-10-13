#pragma once
#include "Runtime.MetadataFile.g.h"
#include <rometadata.h>
#include <rometadataresolution.h>

namespace winrt::OneToolkit::Runtime::implementation
{
	struct MetadataFile : MetadataFileT<MetadataFile>
	{
	public:
		MetadataFile(Windows::UI::Xaml::Interop::TypeName typeId);
		DeclareAutoProperty(hstring, Path, {});
		hstring RuntimeVersion() const;
	private:
		mdTypeDef m_Token;
		com_ptr<IMetaDataDispenserEx> m_Dispenser;
		com_ptr<IMetaDataImport2> m_MetadataImport;
	};
}