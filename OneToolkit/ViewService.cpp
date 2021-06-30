#include "pch.h"
#include "ViewService.h"
#include "ViewServiceDesktop.h"
#include "ViewServiceUniversal.h"
#include "UI.ViewService.g.cpp"

using namespace winrt;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::UI::ViewManagement;
using namespace OneToolkit::Runtime;
using namespace OneToolkit::ApplicationModel;

namespace winrt::OneToolkit::UI::implementation
{
	typedef int(__stdcall* RegGetValueW)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);

	UserInteractionMode ViewService::InteractionMode()
	{
		if (AppInformation::IsCoreApplication()) return UIViewSettings::GetForCurrentView().UserInteractionMode();
		else
		{
			unsigned long result;
			unsigned long size = sizeof(result);
			DynamicModule advapi32{ L"Advapi32.dll" };
			check_win32(advapi32.GetProcAddress<RegGetValueW>("RegGetValueW")(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\ImmersiveShell", L"TabletMode", 
				RRF_RT_REG_DWORD, nullptr, &result, &size));
			return static_cast<UserInteractionMode>(result);
		}
	}

	IViewServiceProvider ViewService::GetDefault()
	{
		return make<ViewServiceDesktop>(0);
	}

	IViewServiceProvider ViewService::GetForCurrentView()
	{
		return make<ViewServiceUniversal>();
	}

	IViewServiceProvider ViewService::GetForWindowId(int64_t windowHandle)
	{
		if (AppInformation::IsCoreApplication()) throw hresult_illegal_method_call(L"GetForWindowId must be called from desktop apps only. UWP apps should call GetForCurrentView instead.");
		return make<ViewServiceDesktop>(windowHandle);
	}
}