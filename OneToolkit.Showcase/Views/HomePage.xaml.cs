using System;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class HomePage : UserControl
	{
		public HomePage() => InitializeComponent();

		private async void Install_Click(object sender, RoutedEventArgs e) => await Launcher.LaunchUriAsync(new("https://www.nuget.org/profiles/JUVStudios"));
	}
}
