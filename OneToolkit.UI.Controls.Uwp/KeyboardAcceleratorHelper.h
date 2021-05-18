#pragma once

namespace OneToolkit::UI::Controls
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class KeyboardAcceleratorHelper sealed
	{
	public:
		static void SetKeyboardAccelerators(Windows::UI::Xaml::UIElement^ element, Windows::Foundation::Collections::IVector<Windows::UI::Xaml::Input::KeyboardAccelerator^>^ values);
	private:
		KeyboardAcceleratorHelper();
	};
}