using System;
using System.Collections.Generic;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Resources;
using Windows.ApplicationModel.DataTransfer;
using Microsoft.Toolkit.Mvvm.ComponentModel;
using OneToolkit.UI;
using OneToolkit.System;
using OneToolkit.Storage;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : ObservableObject
	{
		private SettingsViewModel()
		{
			ElementSoundPlayer.State = DisableSoundEffects ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
		}

		private readonly SettingsService AppSettings = new(ApplicationData.Current.RoamingSettings);

		public static readonly SystemNavigationManager NavigationManager = SystemNavigationManager.GetForCurrentView();

		public static readonly ResourceLoader Resources = ResourceLoader.GetForViewIndependentUse();

		public static readonly ViewService ViewServiceProvider = ViewService.GetForCurrentView();

		public static readonly DataTransferManager TransferManager = DataTransferManager.GetForCurrentView();

		public static SettingsViewModel Instance { get; } = new();

		public string AboutDisplayText => $"OneToolkit Showcase\n{string.Format(Resources.GetString("VersionText"), PackageVersionHelper.Stringify(Package.Current.Id.Version))}\n{Resources.GetString("CopyrightText")}";

		public string AboutAutomationText => AboutDisplayText.Replace("\n", "");

		public bool ShowTeachingTip
		{
			get => Convert.ToBoolean(AppSettings.GetValue(nameof(ShowTeachingTip), true));
			set
			{
				if (ShowTeachingTip != value) SetAppSetting(nameof(ShowTeachingTip), value);
			}
		}

		public bool DisableSoundEffects
		{
			get => Convert.ToBoolean(AppSettings.GetValue(nameof(DisableSoundEffects), false));
			set	
			{
				if (DisableSoundEffects != value)
				{
					ElementSoundPlayer.State = value ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
					SetAppSetting(nameof(DisableSoundEffects), value);
				}
			}
		}

		public bool PreviewAutoRefresh
		{
			get => Convert.ToBoolean(AppSettings.GetValue(nameof(PreviewAutoRefresh), true));
			set
			{
				if (PreviewAutoRefresh != value) SetAppSetting(nameof(PreviewAutoRefresh), value);
			}
		}

		public string SelectedCodeLanguage
		{
			get => AppSettings.GetValue(nameof(SelectedCodeLanguage), "C#").ToString();
			set
			{
				if (SelectedCodeLanguage != value) SetAppSetting(nameof(SelectedCodeLanguage), value);
			}
		}

		public static async void Close() => await ViewServiceProvider.TryCloseAsync();

		public static async void Suspend() => await ViewServiceProvider.TryMinimizeAsync();

		private void SetAppSetting(string key, object newValue)
		{
			AppSettings.SetValue(key, newValue);
			OnPropertyChanged(key);
		}
	}
}