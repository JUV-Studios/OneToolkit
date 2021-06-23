#include "pch.h"
#include "KeyboardAcceleratorHelper.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace OneToolkit::UI::Xaml::Controls;
using namespace Windows::Foundation::Collections;

void KeyboardAcceleratorHelper::SetKeyboardAccelerators(UIElement^ element, IVector<KeyboardAccelerator^>^ values)
{
	element->KeyboardAccelerators->Clear();
	for (auto const& accelerator : values)
	{
		element->KeyboardAccelerators->Append(accelerator);
	}
}