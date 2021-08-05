using System;
using System.IO;
using System.ComponentModel;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Markup;
using Windows.ApplicationModel;
using Windows.System.Threading;
using OneToolkit.UI.Xaml.Controls;
using OneToolkit.Showcase.ViewModels;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class Playground : UserControl, INotifyPropertyChanged
	{
		private ThreadPoolTimer Timer;

		private string PreviousText;

		public Playground()
		{
			InitializeComponent();
			SettingsViewModel.Instance.PropertyChanged += Settings_PropertyChanged;
		}

		public bool IsInTallMode => ContentView.Mode == Microsoft.UI.Xaml.Controls.TwoPaneViewMode.Tall;

		public event PropertyChangedEventHandler PropertyChanged;

		private void HubPanel_Loaded(object sender, RoutedEventArgs e)
		{
			var target = sender as HubPanel;
			target.Header = (MainPage.NavigationMenu.SelectedItem as ComboBoxItem).Content;
			target.Loaded -= HubPanel_Loaded;
		}

		private async void Editor_Loaded(object sender, RoutedEventArgs e)
		{
			if (Editor.Text == string.Empty) Editor.Text = await PathIO.ReadTextAsync(Path.Combine(Package.Current.InstalledLocation.Path, "Assets", "Playground.xaml"));
		}

		private void Editor_GotFocus(object sender, RoutedEventArgs e)
		{
			if (SettingsViewModel.Instance.PreviewAutoRefresh) Timer = ThreadPoolTimer.CreatePeriodicTimer(HandleTimerTick, TimeSpan.FromMilliseconds(400));
		}

		private void Editor_LostFocus(object sender, RoutedEventArgs e)
		{
			if (Timer != null)
			{
				Timer.Cancel();
				Timer = null;
			}
		}

		private void HubPanel_SectionHeaderClick(object sender, HubSectionHeaderClickEventArgs e)
		{
			RefreshPreview();
		}

		private void ContentView_ModeChanged(Microsoft.UI.Xaml.Controls.TwoPaneView sender, object args)
		{
			PropertyChanged?.Invoke(this, new(nameof(IsInTallMode)));
		}

		private void Settings_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			if (e.PropertyName == "PreviewAutoRefresh")
			{
				if (!SettingsViewModel.Instance.PreviewAutoRefresh) Editor_LostFocus(null, null);
			}
		}

		private async void HandleTimerTick(ThreadPoolTimer timer)
		{
			await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, RefreshPreview);
		}

		private void RefreshPreview()
		{
			var editorText = Editor.Text;
			if (PreviousText != editorText)
			{
				try
				{
					PreviewHub.DataContext = XamlReader.Load(editorText) as UIElement;
				}
				catch (Exception ex)
				{
					PreviewHub.DataContext = new TextBlock() { Text = ex.Message };
				}

				PreviousText = editorText;
			}
		}
	}
}
