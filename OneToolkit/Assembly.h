#pragma once
#include "Runtime.Assembly.g.h"
#include <rometadata.h>
#include <rometadataresolution.h>

namespace winrt::OneToolkit::Runtime::implementation
{
	struct Assembly : AssemblyT<Assembly>
	{
	public:
		Assembly(Windows::UI::Xaml::Interop::TypeName typeName);
		hstring Path() const noexcept;
		hstring RuntimeVersion() const;
	private:
		hstring m_Path;
		mdTypeDef m_Token;
		com_ptr<IMetaDataDispenserEx> m_Dispenser;
		com_ptr<IMetaDataImport2> m_MetadataImport;
	};
}