using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.ApplicationModel;
using OneToolkit.System;
using OneToolkit.Showcase.ViewModels;
using System;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit.Showcase.Dialogs
{
	public sealed partial class AboutDialog : ContentDialog
	{
		public AboutDialog() => InitializeComponent();

		public readonly string AboutDisplayText = "OneToolkit Showcase" + Environment.NewLine + 
			string.Format(SettingsViewModel.Resources.GetString("VersionText"), PackageVersionHelper.Stringify(Package.Current.Id.Version)) + Environment.NewLine +
			SettingsViewModel.Resources.GetString("CopyrightText");

		public string AboutAutomationText => AboutDisplayText.Replace("\n", "");

		private void ContentDialog_Loaded(object sender, RoutedEventArgs e)
		{
			Title = SettingsViewModel.Resources.GetString("AppMenuAboutItem/Text");
		}
	}
}
