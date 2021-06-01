using Windows.ApplicationModel;
using Windows.UI.Xaml.Controls;
using Microsoft.Toolkit.Uwp.Helpers;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace OneToolkit.Showcase
{
	public sealed partial class SettingsPage : UserControl
	{
		public SettingsPage() => InitializeComponent();

		public string VersionText = "Version " + Package.Current.Id.Version.ToFormattedString();
	}
}
