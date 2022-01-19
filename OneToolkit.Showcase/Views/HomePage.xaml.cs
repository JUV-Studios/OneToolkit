using System;
using Windows.Foundation;
using Windows.System.Threading;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Microsoft.UI.Xaml.Controls;
using OneToolkit.UI;
using OneToolkit.Data.Text;
using OneToolkit.Lifecycle;
using OneToolkit.Showcase.Dialogs;
using OneToolkit.Showcase.ViewModels;

namespace OneToolkit.Showcase.Views
{
	public sealed partial class HomePage : Page, ISuspendable
	{
		public HomePage() => InitializeComponent();

		private GreetingType? PreviousGreetingType;

		private ThreadPoolTimer GreetingUpdateTimer;

		private TimerElapsedHandler TimerElapsedHandler;

		public bool IsSuspended => TimerElapsedHandler == null;

		public event TypedEventHandler<ISuspendable, SuspendableEventType> StateChanged;

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			base.OnNavigatedTo(e);
			RequestResume();
		}

		protected override void OnNavigatedFrom(NavigationEventArgs e)
		{
			base.OnNavigatedFrom(e);
			RequestSuspend();
			Bindings.StopTracking();
			Bindings = null;
#if DEBUG
			GC.Collect();
#endif
		}

		public bool RequestResume()
		{
			TimerElapsedHandler = new(GreetingUpdateTimer_Tick);
			GreetingUpdateTimer = ThreadPoolTimer.CreatePeriodicTimer(TimerElapsedHandler, TimeSpan.FromMilliseconds(130));
			StateChanged?.Invoke(this, SuspendableEventType.Resumed);
			return true;
		}

		public bool RequestSuspend()
		{
			GreetingUpdateTimer.Cancel();
			GreetingUpdateTimer = null;
			TimerElapsedHandler = null;
			StateChanged?.Invoke(this, SuspendableEventType.Suspended);
			return true;
		}

		private async void GreetingUpdateTimer_Tick(ThreadPoolTimer sender)
		{
			var newGreetingType = GreetingHelper.GetTimelyGreeting(DateTime.Now);
			if (newGreetingType != PreviousGreetingType)
			{
				// Only update the hub header if the greeting type has changed to reduce overhead on the UI thread.
				PreviousGreetingType = newGreetingType;
				await Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => (Content as Hub).Header = SettingsViewModel.Resources.GetString($"{GreetingHelper.GetTimelyGreeting(DateTime.Now)}Text"));
			}
		}

		private void Playgrounds_Click(object sender, RoutedEventArgs e) => Frame.Navigate(typeof(Playgrounds), null, App.PageTransition);

		private void ContentInfoSection_ItemClick(object sender, RoutedEventArgs e) => Frame.Navigate(typeof(ApiReference), (sender as FrameworkElement).DataContext, App.PageTransition);

		private async void EraseSettingsButton_Click(object sender, RoutedEventArgs e) => await new EraseSettingsDialog().ShowAsync();

		private void ThemeRadioButtons_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ThemeService.AppThemeOverride = (sender as RadioButtons).SelectedIndex switch
			{
				0 => ElementTheme.Light,
				1 => ElementTheme.Dark,
				_ => ElementTheme.Default,
			};
		}
	}
}