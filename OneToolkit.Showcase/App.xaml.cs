using System;
using System.Linq;
using System.Diagnostics;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.Activation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Notifications;
using Microsoft.Toolkit.Uwp.Notifications;
using Microsoft.AppCenter;
using Microsoft.AppCenter.Crashes;
using Microsoft.AppCenter.Analytics;
using OneToolkit.Showcase.Views;
using OneToolkit.Lifecycle;

namespace OneToolkit.Showcase
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	sealed partial class App : Application
	{
		private bool CrashNotificationSent = false;

		/// <summary>
		/// Initializes the singleton application object.  This is the first line of authored code
		/// executed, and as such is the logical equivalent of main() or WinMain().
		/// </summary>
		public App()
		{
			InitializeComponent();
			EnteredBackground += App_EnteredBackground;
			LeavingBackground += App_LeavingBackground;
			UnhandledException += App_UnhandledException;
			ToastNotificationManagerCompat.History.Clear();
			AppCenter.Start("c1fa7718-3c52-47a4-b969-abbe4d7816c5", typeof(Analytics), typeof(Crashes));
		}

		public static readonly DrillInNavigationTransitionInfo PageTransition = new();

		/// <summary>
		/// Invoked when the application is launched normally by the end user.  Other entry points
		/// will be used such as when the application is launched to open a specific file.
		/// </summary>
		/// <param name="e">Details about the launch request and process.</param>
		protected override void OnLaunched(LaunchActivatedEventArgs e)
		{
			if (e.PrelaunchActivated == false)
			{
				CoreApplication.EnablePrelaunch(true);
				ActivateWindow();
			}
		}

		protected override void OnActivated(IActivatedEventArgs args) => ActivateWindow(() =>
		{
			if (args.Kind == ActivationKind.Protocol)
			{
				var protocolArgs = args as ProtocolActivatedEventArgs;
				if (protocolArgs.Uri.AbsolutePath.Length > 0)
				{
					var nameSpace = ApiReference.FoundNamespaces.Where(nameSpace => nameSpace.GetShortName(string.Empty).ToLower() == protocolArgs.Uri.AbsolutePath.Substring(1).ToLower());
					if (nameSpace.Any())
					{
						(Window.Current.Content as MainPage).Presenter.Navigate(typeof(ApiReference), nameSpace.First(), PageTransition);
					}
				}
			}
		});

		/// <summary>
		/// Activates the main window if it's not already activated.
		/// </summary>
		/// <param name="contentSetCallback">A reference to a delegate that will be invoked before the window is activated.</param>
		private void ActivateWindow(Action contentSetCallback = null)
		{
			var window = Window.Current;
			window.Content ??= new MainPage();
			contentSetCallback?.Invoke();
			window.Activate();
		}

		private void App_EnteredBackground(object sender, EnteredBackgroundEventArgs e)
		{
			var deferral = e.GetDeferral();
			if ((Window.Current.Content as MainPage).Presenter.Content is ISuspendable suspendable) suspendable.RequestSuspend();
			deferral.Complete();
		}

		private void App_LeavingBackground(object sender, LeavingBackgroundEventArgs e)
		{
			var deferral = e.GetDeferral();
			if ((Window.Current.Content as MainPage).Presenter.Content is ISuspendable suspendable) suspendable.RequestResume();
			deferral.Complete();
		}

		private void App_UnhandledException(object sender, Windows.UI.Xaml.UnhandledExceptionEventArgs e)
		{
			if (!CrashNotificationSent && !Debugger.IsAttached)
			{
				e.Handled = true;
				var toast = new ToastContent()
				{
					Visual = new ToastVisual()
					{
						BindingGeneric = new ToastBindingGeneric()
						{
							Children =
							{
								new AdaptiveText() { Text = "Something went wrong!" }, new AdaptiveText() { Text = "OneToolkit Showcase ran into a problem that the developers didn't prepare for yet." }
							},

							Attribution = new()
							{
								Text = e.Exception.Message
							}
						}
					}
				};

				ToastNotificationManager.CreateToastNotifier().Show(new(toast.GetXml()));
				CrashNotificationSent = true;
				Exit();
			}
		}
	}
}

namespace System.Runtime.CompilerServices
{
	internal static class IsExternalInit { }
}