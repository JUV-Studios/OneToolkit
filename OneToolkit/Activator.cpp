#include "pch.h"
#include "Activator.h"
#include "Reflection.Activator.g.cpp"
#include <roapi.h>

using namespace winrt::Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Reflection::implementation
{
	Windows::Foundation::IInspectable Activator::ActivateInstance(TypeName const& typeId, array_view<IInspectable const> args)
	{
		if (typeId.Name.empty()) throw hresult_invalid_argument(L"TypeId.Name must not be empty.");
		else
		{
			Windows::Foundation::IInspectable result;
			check_hresult(RoActivateInstance(static_cast<HSTRING>(winrt::get_abi(typeId.Name)), reinterpret_cast<::IInspectable**>(winrt::put_abi(result))));
			return result;
		}
	}
}