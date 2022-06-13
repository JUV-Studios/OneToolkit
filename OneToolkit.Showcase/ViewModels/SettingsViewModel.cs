using System;
using System.Threading.Tasks;
using Windows.System;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.Foundation.Metadata;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Core;
using Windows.ApplicationModel.Search;
using Windows.ApplicationModel.Resources;
using Windows.ApplicationModel.DataTransfer;
using OneToolkit.UI.Windowing;
using OneToolkit.System;
using OneToolkit.Storage;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : AppSettingsBase
	{
		private SettingsViewModel() : base(ApplicationData.Current.RoamingSettings)
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

		public static SettingsViewModel Instance { get; } = new();

		public string AboutDisplayText => $"OneToolkit Showcase\n{string.Format(Resources.GetString("VersionText"), Package.Current.Id.Version.AsVersion())}\n{Resources.GetString("CopyrightText")}";

		public string AboutAutomationText => AboutDisplayText.Replace("\n", "");

		public bool ShowTeachingTip
		{
			get => GetAppSetting(nameof(ShowTeachingTip), true);
			set
			{
				if (ShowTeachingTip != value) SettingsContainer.Values[nameof(ShowTeachingTip)] = value;
			}
		}

		public bool DisableSoundEffects
		{
			get => GetAppSetting(nameof(DisableSoundEffects), false);
			set	
			{
				if (DisableSoundEffects != value)
				{
					ElementSoundPlayer.State = value ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
					SettingsContainer.Values[nameof(DisableSoundEffects)] = value;
				}
			}
		}

		public bool PreviewAutoRefresh
		{
			get => GetAppSetting(nameof(PreviewAutoRefresh), true);
			set
			{
				if (PreviewAutoRefresh != value) SettingsContainer.Values[nameof(PreviewAutoRefresh)] = value;
			}
		}

		public string SelectedCodeLanguage
		{
			get => GetAppSetting(nameof(SelectedCodeLanguage), "C#");
			set
			{
				if (SelectedCodeLanguage != value) SettingsContainer.Values[nameof(SelectedCodeLanguage)] = value;
			}
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

		public static async void OpenFeedbackPage() => await LaunchLinkAsync(new($"https://www.nuget.org/packages/OneToolkit/{Package.Current.Id.Version.AsVersion()}/ContactOwners"));

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

		public async void EraseSettings()
		{
			SettingsContainer.Values.Clear();
			await CoreApplication.RequestRestartAsync(string.Empty);
		}

		public static void SetDefaultSettings()
		{
		}

		private T GetAppSetting<T>(string key, T fallback)
		{
			if (SettingsContainer.Values.TryGetValue(key, out object value))
			{
				return (T)value;
			}
			else
			{
				SetAppSetting(key, fallback);
				return fallback;
			}
		}

		private void SetAppSetting(string key, object newValue)
		{
			SettingsContainer.Values[key] = newValue;
		}
	}
}