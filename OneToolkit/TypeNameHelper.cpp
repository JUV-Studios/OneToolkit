#include "pch.h"
#include "TypeNameHelper.h"
#include "Runtime.TypeNameHelper.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Runtime::implementation
{
	bool TypeNameHelper::IsTypePrimitive(TypeName typeId)
	{
		return IsTypePrimitive(typeId.Name);
	}

	hstring TypeNameHelper::ExtractNamespace(TypeName typeId)
	{
		return ExtractNamespace(typeId.Name);
	}

	hstring TypeNameHelper::ToString(TypeName typeId)
	{
		return typeId.Name;
	}

	TypeName TypeNameHelper::FromString(hstring const& typeName)
	{
		TypeName result;
		result.Name = typeName;
		result.Kind = IsTypePrimitive(typeName) ? TypeKind::Primitive : TypeKind::Metadata;
		return result;
	}

	bool TypeNameHelper::IsTypePrimitive(hstring const& typeName)
	{
		return std::find(typeName.begin(), typeName.end(), L'.') == typeName.end();
	}

	hstring TypeNameHelper::ExtractNamespace(hstring const& typeName)
	{
		std::wstring_view typeNameView = typeName;
		return hstring(typeNameView.substr(0, typeNameView.find_last_of(L'.') - 1));
	}
}