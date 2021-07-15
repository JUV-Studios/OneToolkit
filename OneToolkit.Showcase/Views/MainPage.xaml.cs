using System;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.ViewManagement;
using Windows.ApplicationModel;
using Microsoft.Toolkit.Uwp.Helpers;
using OneToolkit.Showcase.ViewModels;
using Windows.System;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace OneToolkit.Showcase.Views
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainPage : UserControl
	{
		public MainPage()
		{
			InitializeComponent();
			SettingsViewModel.ViewServiceProvider.AppView.SetPreferredMinSize(new(500, 500));
			SettingsViewModel.ViewServiceProvider.CoreAppView.TitleBar.ExtendViewIntoTitleBar = true;
			var titleBar = SettingsViewModel.ViewServiceProvider.AppView.TitleBar;
			titleBar.BackgroundColor = Colors.Transparent;
			titleBar.ButtonBackgroundColor = Colors.Transparent;
			titleBar.ButtonInactiveBackgroundColor = Colors.Transparent;
		}
	}
}
