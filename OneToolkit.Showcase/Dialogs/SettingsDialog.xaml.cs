using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit.Showcase.Dialogs
{
	public sealed partial class SettingsDialog : ContentDialog
	{
		public SettingsDialog() => InitializeComponent();

		private void ContentDialog_Loaded(object sender, RoutedEventArgs e)
		{
			Title = SettingsViewModel.Resources.GetString("AppMenuSettingsItem/Text");
		}
	}
}
