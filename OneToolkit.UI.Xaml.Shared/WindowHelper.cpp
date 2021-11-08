#include "pch.h"
#ifdef WINUI3
using namespace Microsoft::WRL;
#endif

using namespace Framework;

namespace OneToolkit::UI::Xaml::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class WindowHelper sealed
	{
	public:
		static WindowHandle GetWindowHandle(Window^ window)
		{
#ifdef WINUI3
			ComPtr<IWindowNative> nativeWindow;
			if (window->__abi_QueryInterface(reinterpret_cast<Platform::Guid&>(const_cast<_GUID&>(__uuidof(IWindowNative))), reinterpret_cast<void**>(nativeWindow.GetAddressOf())) == 0)
			{
				WindowHandle result;
				__abi_ThrowIfFailed(nativeWindow->get_WindowHandle(reinterpret_cast<HWND*>(&result.Value)));
				return result;
			}
#endif
			return ViewService::GetHandleFromCoreWindow(window->CoreWindow);
		}
	private:
		WindowHelper();
	};
}