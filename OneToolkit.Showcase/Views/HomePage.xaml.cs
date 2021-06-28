﻿using System;
using Windows.System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;
using Windows.ApplicationModel.Core;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace OneToolkit.Showcase.Views
{
	public sealed partial class HomePage : UserControl
	{
		public HomePage() => InitializeComponent();

		private async void Install_Click(object sender, RoutedEventArgs e)
		{
			if (!SettingsViewModel.ViewServiceProvider.IsDialogShown)
			{
				SettingsViewModel.ViewServiceProvider.IsDialogShown = true;
				await Launcher.LaunchUriAsync(new("https://www.nuget.org/profiles/JUVStudios"));
				SettingsViewModel.ViewServiceProvider.IsDialogShown = false;
			}
		}
	}
}
