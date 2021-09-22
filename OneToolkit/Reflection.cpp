#include "pch.h"
#include "Reflection.h"
#include "Runtime.Reflection.g.cpp"
#include <rometadataresolution.h>

using namespace juv;
using namespace winrt;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Runtime::implementation
{
	TypeName Reflection::GetTypeId(hstring const& typeName)
	{
		TypeName result;
		result.Name = typeName;
		result.Kind = std::find(typeName.begin(), typeName.end(), L'.') == typeName.end() ? TypeKind::Primitive : TypeKind::Metadata;
		return result;
	}

	uint64 Reflection::GetSizeOf(TypeName typeId)
	{
		if (typeId == xaml_typename<bool>()) return sizeof(bool);
		else if (typeId == xaml_typename<char16>()) return sizeof(char16);
		else if (typeId == xaml_typename<uint8>()) return sizeof(uint8);
		else if (typeId == xaml_typename<int8>()) return sizeof(int8);
		else if (typeId == xaml_typename<uint16>()) return sizeof(uint16);
		else if (typeId == xaml_typename<int16>()) return sizeof(int16);
		else if (typeId == xaml_typename<uint32>()) return sizeof(uint32);
		else if (typeId == xaml_typename<int32>()) return sizeof(int32);
		else if (typeId == xaml_typename<uint64>()) return sizeof(uint64);
		else if (typeId == xaml_typename<int64>()) return sizeof(int64);
		else if (typeId == xaml_typename<float>()) return sizeof(float);
		else if (typeId == xaml_typename<double>()) return sizeof(double);
		else if (typeId == xaml_typename<hstring>()) return sizeof(hstring);
		else if (typeId == xaml_typename<guid>()) return sizeof(guid);
		else if (typeId == xaml_typename<Windows::Foundation::Point>()) return sizeof(Windows::Foundation::Point);
		else if (typeId == xaml_typename<Windows::Foundation::Size>()) return sizeof(Windows::Foundation::Size);
		else if (typeId == xaml_typename<Windows::Foundation::Rect>()) return sizeof(Windows::Foundation::Rect);
		else if (typeId == xaml_typename<Windows::Foundation::DateTime>()) return sizeof(Windows::Foundation::DateTime);
		else if (typeId == xaml_typename<Windows::Foundation::TimeSpan>()) return sizeof(Windows::Foundation::TimeSpan);
		else return sizeof(size_t); // TODO Calculate type's size from metadata if it's a value type.
	}

	Windows::Foundation::IInspectable Reflection::ActivateType(TypeName typeId)
	{
		throw hresult_not_implemented();
	}

	hstring Reflection::ExtractNamespace(Windows::UI::Xaml::Interop::TypeName typeId)
	{
		std::wstring_view typeNameView = typeId.Name;
		return hstring(typeNameView.substr(0, typeNameView.find_last_of(L'.') - 1));
	}

	hstring Reflection::GetMetadataFileLocation(TypeName typeId)
	{
		hstring path;
		check_hresult(RoGetMetaDataFile(static_cast<HSTRING>(::get_abi(typeId.Name)), nullptr, reinterpret_cast<HSTRING*>(put_abi(path)), nullptr, nullptr));
		return path;
	}
}