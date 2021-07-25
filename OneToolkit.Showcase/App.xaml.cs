using Windows.ApplicationModel.Activation;
using Windows.ApplicationModel.Core;
using Windows.UI.Xaml;
using Microsoft.AppCenter;
using Microsoft.AppCenter.Crashes;
using Microsoft.AppCenter.Analytics;
using OneToolkit.Lifecycle;
using OneToolkit.Showcase.Views;

namespace OneToolkit.Showcase
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    sealed partial class App : Application
    {
        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            InitializeComponent();
            AppCenter.Start("c1fa7718-3c52-47a4-b969-abbe4d7816c5", typeof(Analytics), typeof(Crashes));
        }

        public static readonly WeakTypeCache PageTypeCache = new();

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
    internal sealed class IsExternalInit
	{
	}
}