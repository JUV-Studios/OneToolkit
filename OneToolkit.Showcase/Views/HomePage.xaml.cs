using System;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class HomePage : Page
	{
		public HomePage() => InitializeComponent();

		private void ReferenceGridItem_Click(object sender, RoutedEventArgs e)
		{
			Frame.Navigate(typeof(ApiReference), (sender as FrameworkElement).DataContext, App.PageTransition);
		}

		private void Playgrounds_Click(object sender, RoutedEventArgs e)
		{
			Frame.Navigate(typeof(Playgrounds), null, App.PageTransition);
		}
	}
}