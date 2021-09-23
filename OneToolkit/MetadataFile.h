#pragma once
#include "Runtime.MetadataFile.g.h"

namespace winrt::OneToolkit::Runtime::implementation
{
	struct MetadataFile : MetadataFileT<MetadataFile>
	{
		MetadataFile(Windows::UI::Xaml::Interop::TypeName typeId);
		DeclareAutoProperty(hstring, Path, {});
	};
}