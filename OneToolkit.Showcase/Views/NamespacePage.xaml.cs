﻿using OneToolkit.Showcase.Models;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace OneToolkit.Showcase.Views
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class NamespacePage : Page
	{
		private TypeGroup GroupData;

		public NamespacePage() => InitializeComponent();

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			GroupData = e.Parameter as TypeGroup;
		}
	}
}
