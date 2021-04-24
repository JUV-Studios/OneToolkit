//
// FilePicker.xaml.cpp
// Implementation of the FilePicker class
//

#include "pch.h"
#include "FilePicker.xaml.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit::Storage::Pickers
{
	FilePicker::FilePicker(FilePickerMode pickerMode) : m_PickerMode(pickerMode)
	{
		InitializeComponent();
		String^ text = pickerMode == FilePickerMode::Open ? "Open" : "Save";
		Title = text;
		PrimaryButtonText = text;
	}
}