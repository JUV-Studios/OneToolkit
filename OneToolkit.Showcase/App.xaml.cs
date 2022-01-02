using System.Diagnostics;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Notifications;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.Activation;
using Microsoft.Toolkit.Uwp.Notifications;
using Microsoft.AppCenter;
using Microsoft.AppCenter.Crashes;
using Microsoft.AppCenter.Analytics;
using OneToolkit.Showcase.Views;

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
            UnhandledException += App_UnhandledException;
            ToastNotificationManagerCompat.History.Clear();
            AppCenter.Start("c1fa7718-3c52-47a4-b969-abbe4d7816c5", typeof(Analytics), typeof(Crashes));
        }

		private void App_UnhandledException(object sender, UnhandledExceptionEventArgs e)
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
                Window.Current.Content ??= new MainPage();
                Window.Current.Activate();
            }
        }
    }
}

namespace System.Runtime.CompilerServices
{
    internal static class IsExternalInit { }   
}