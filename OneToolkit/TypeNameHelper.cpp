#include "pch.h"
#include "Assembly.h"
#include "Runtime.TypeNameHelper.g.h"
#include <winstring.h>
#include <rometadataresolution.h>

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Runtime::factory_implementation
{
	struct TypeNameHelper : TypeNameHelperT<TypeNameHelper, TypeNameHelper>
	{
		static hstring GetNamespace(TypeName const& typeName)
		{
			std::wstring_view nameView = typeName.Name;
			return hstring(nameView.substr(0, nameView.find_last_of(L'.') - 1));
		}

		static hstring GetDisplayName(TypeName const& typeName)
		{
			std::wstring_view nameView = typeName.Name;
			return hstring(nameView.substr(nameView.find_last_of(L'.') + 1));
		}

		static uint64 GetBaseSize(TypeName const& typeName)
		{
			if (typeName == xaml_typename<bool>()) return sizeof(bool);
			else if (typeName == xaml_typename<char16>()) return sizeof(char16);
			else if (typeName == xaml_typename<uint8>()) return sizeof(uint8);
			else if (typeName == xaml_typename<int8>()) return sizeof(int8);
			else if (typeName == xaml_typename<uint16>()) return sizeof(uint16);
			else if (typeName == xaml_typename<int16>()) return sizeof(int16);
			else if (typeName == xaml_typename<uint32>()) return sizeof(uint32);
			else if (typeName == xaml_typename<int32>()) return sizeof(int32);
			else if (typeName == xaml_typename<uint64>()) return sizeof(uint64);
			else if (typeName == xaml_typename<int64>()) return sizeof(int64);
			else if (typeName == xaml_typename<float>()) return sizeof(float);
			else if (typeName == xaml_typename<double>()) return sizeof(double);
			else if (typeName == xaml_typename<hstring>()) return sizeof(hstring);
			else if (typeName == xaml_typename<guid>()) return sizeof(guid);
			else if (typeName == xaml_typename<Point>()) return sizeof(Point);
			else if (typeName == xaml_typename<Size>()) return sizeof(Size);
			else if (typeName == xaml_typename<Rect>()) return sizeof(Rect);
			else if (typeName == xaml_typename<DateTime>()) return sizeof(DateTime);
			else if (typeName == xaml_typename<TimeSpan>()) return sizeof(TimeSpan);
			else throw hresult_illegal_method_call(L"Can't take the size of a reference type"); // TODO Calculate type's size from metadata if it's a value type.
		}

		static OneToolkit::Runtime::Assembly GetContainingAssembly(TypeName const& typeName)
		{
			return make<implementation::Assembly>(typeName);
		}

		static bool IsGenericType(TypeName const& typeName)
		{
			uint32 partsCount = 0;
			HSTRING* parts = nullptr;
			check_hresult(RoParseTypeName(static_cast<HSTRING>(winrt::get_abi(typeName.Name)), reinterpret_cast<DWORD*>(&parts), &parts));
			bool result = partsCount > 1;
			for (uint32 index = 0; index < partsCount; ++index) WindowsDeleteString(parts[index]);
			CoTaskMemFree(parts);
			return result;
		}
	};
}

void* winrt_make_OneToolkit_Runtime_TypeNameHelper()
{
	return detach_abi(make<OneToolkit::Runtime::factory_implementation::TypeNameHelper>());
}