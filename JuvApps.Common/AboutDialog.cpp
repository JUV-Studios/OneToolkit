#include "pch.h"
#include "AboutDialog.h"
#include "AboutDialog.g.cpp"
#include <sstream>
#include <winrt/OneToolkit.System.h>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace OneToolkit::System;

namespace winrt::JuvApps::Common::implementation
{
	void AboutDialog::ContentDialog_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{
		auto const appInfo = AppInformation::Current();
		auto const appVersion = appInfo.Version();
		std::wstringstream aboutTextStream;
		aboutTextStream << appInfo.Name().data() << L'\n';
		aboutTextStream << L"Version " << appVersion.Major << '.' << appVersion.Minor << '.' << appVersion.Build << '.' << appVersion.Revision << "\n";
		AboutTextBlock().Text(aboutTextStream.str());
	}
}