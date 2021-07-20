#include "pch.h"

using namespace Windows::UI::Xaml;
using namespace Windows::Foundation::Metadata;

namespace OneToolkit::UI::Xaml
{
	[WebHostHidden]
	public ref class WindowHelper sealed
	{
	public:
		static int64 GetWindowHandle(Window^ window)
		{
			return ViewService::GetWindowHandle(window->CoreWindow);
		}
	private:
		WindowHelper();
	};
}