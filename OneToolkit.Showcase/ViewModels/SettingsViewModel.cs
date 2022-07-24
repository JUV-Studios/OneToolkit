using System;
using System.Threading.Tasks;
using Windows.System;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.Foundation.Metadata;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.Search;
using Windows.ApplicationModel.Resources;
using Windows.ApplicationModel.DataTransfer;
using OneToolkit.System;
using OneToolkit.UI.Windowing;
using OwlCore.Services;
using OwlCore.AbstractStorage;
using OneToolkit.Showcase.AbstractStorage;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : SettingsBase
	{
		private SettingsViewModel(IFolderData folder) : base(folder, NewtonsoftStreamSerializer.Singleton)
		{
			ElementSoundPlayer.State = DisableSoundEffects ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
		}

		private static bool LaunchingLink = false;

#pragma warning disable CS0618
		public static bool IsSearchCharmSupported => ApiInformation.IsTypePresent("Windows.ApplicationModel.Search.SearchPane");
#pragma warning restore CS0618

#pragma warning disable CS0618
		public static readonly SearchPane SearchCharm = SearchPane.GetForCurrentView();
#pragma warning restore CS0618

		public static readonly SystemNavigationManager NavigationManager = SystemNavigationManager.GetForCurrentView();

		public static readonly ResourceLoader Resources = ResourceLoader.GetForViewIndependentUse();

		public static readonly ViewReference ViewServiceProvider = ViewService.CurrentView;

		public static readonly DataTransferManager TransferManager = DataTransferManager.GetForCurrentView();

		public string AboutDisplayText => $"OneToolkit Showcase\n{string.Format(Resources.GetString("VersionText"), Package.Current.Id.Version)}\n{Resources.GetString("CopyrightText")}";

		public string AboutAutomationText => AboutDisplayText.Replace("\n", "");

		public bool ShowTeachingTip
		{
			get => GetSetting(() => true);
			set => SetSetting(value);
		}

		public bool DisableSoundEffects
		{
			get => GetSetting(() => false);
			set => SetSetting(value);
		}

		public bool PreviewAutoRefresh
		{
			get => GetSetting(() => true);
			set => SetSetting(value);
		}

		public string SelectedCodeLanguage
		{
			get => GetSetting(() => "C#");
			set => SetSetting(value);
		}

		public static void RevealShareCharm() => DataTransferManager.ShowShareUI();

#pragma warning disable CS0618
		public static void RevealSearchCharm()
		{
			if (IsSearchCharmSupported) SearchCharm.Show();
		}
#pragma warning restore CS0618

		public static async void Close() => await ViewServiceProvider.CloseAsync();

		public static async void Suspend() => await ViewServiceProvider.MinimizeAsync();

		public static async void OpenHelpPage() => await LaunchLinkAsync(new("http://discord.com/invite/CZpBpPQjq8"));

		public static async void OpenFeedbackPage() => await LaunchLinkAsync(new($"https://www.nuget.org/packages/OneToolkit/{Package.Current.Id.Version}/ContactOwners"));

		public static async Task<bool> LaunchLinkAsync(Uri link)
		{
			if (!LaunchingLink)
			{
				LaunchingLink = true;
				var result = await Launcher.LaunchUriAsync(link);
				LaunchingLink = false;
				return result;
			}

			return false;
		}
	}
}