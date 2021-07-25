#pragma once

namespace OneToolkit::UI::Xaml
{
	public ref class ElementResourceManager
	{
	public:
		static void Dispose(Windows::UI::Xaml::UIElement^ control);
	private:
		ElementResourceManager();
	};
}