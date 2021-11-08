using Windows.UI.Xaml.Controls;
using Windows.ApplicationModel;
using OneToolkit.System;
using OneToolkit.Showcase.ViewModels;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit.Showcase.Dialogs
{
	public sealed partial class AboutDialog : ContentDialog
	{
		public AboutDialog()
		{
			InitializeComponent();
			Title = SettingsViewModel.Resources.GetString("AppMenuAboutItem/Text");
		}

		public readonly string AboutDisplayText = $"OneToolkit Showcase\n{string.Format(SettingsViewModel.Resources.GetString("VersionText"), PackageVersionHelper.Stringify(Package.Current.Id.Version))}\n{SettingsViewModel.Resources.GetString("CopyrightText")}";

		public string AboutAutomationText => AboutDisplayText.Replace("\n", "");
	}
}
