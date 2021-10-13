#include "pch.h"

#ifdef WinUI_3
using namespace Microsoft::WRL;
using namespace Microsoft::UI::Xaml;
#else
using namespace Windows::UI::Xaml;
#endif

namespace Component
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class WindowHelper sealed
	{
	public:
		static WindowId GetWindowId(Window^ window)
		{
#ifdef WinUI_3
			ComPtr<IWindowNative> nativeWindow;
			if (reinterpret_cast<IUnknown*>(window)->QueryInterface(nativeWindow.GetAddressOf()) == 0)
			{
				WindowId result;
				__abi_ThrowIfFailed(nativeWindow->get_WindowHandle(reinterpret_cast<HWND*>(&result.Value)));
				return result;
			}
			
#endif
			return ViewService::GetCoreWindowId(window->CoreWindow);
		}
	private:
		WindowHelper();
	};
}