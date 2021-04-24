//
// FilePicker.xaml.h
// Declaration of the FilePicker class
//

#pragma once
#include "FilePicker.g.h"
#include "../OneToolkit/Exports/ModernCX.h"

namespace OneToolkit::Storage::Pickers
{
	enumeration FilePickerMode
	{
		Open, Save
	};

	[Windows::Foundation::Metadata::WebHostHidden]
	runtimeclass FilePicker sealed
	{
	public:
		FilePicker(FilePickerMode pickerMode);
	private:
		FilePickerMode m_PickerMode;
	};
}
