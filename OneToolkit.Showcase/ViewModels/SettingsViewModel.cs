using System;
using System.Collections.Generic;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.ApplicationModel.Core;
using OneToolkit.UI;
using OneToolkit.Mvvm;
using OneToolkit.Storage;

namespace OneToolkit.Showcase.ViewModels
{
	public sealed class SettingsViewModel : ObservableBase
	{
		private readonly SettingsService SettingsService = new(ApplicationData.Current.RoamingSettings);

		private SettingsViewModel()
		{
			ElementSoundPlayer.State = DisableSoundEffects ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
		}

		public static SettingsViewModel Instance { get; } = new();

		public static readonly SystemNavigationManager NavigationManager = SystemNavigationManager.GetForCurrentView();

		public static readonly ViewService ViewServiceProvider = ViewService.GetForCurrentView();

		public static readonly IEnumerable<string> ProgrammingLanguages = new string[]
		{
			"C#", "C++/CX", "C++/WinRT", "Rust", "Visual Basic"
		};

		public GridLength TitleBarHeight => new(ViewServiceProvider.CoreAppView.TitleBar.IsVisible ? ViewServiceProvider.CoreAppView.TitleBar.Height : 0);

		public Thickness TitleBarLeftInset
		{
			get
			{
				if (ViewServiceProvider.CoreAppView.TitleBar.IsVisible) return new(ViewServiceProvider.CoreAppView.TitleBar.SystemOverlayLeftInset, 0, 0, 0);
				else return new(0);
			}
		}

		public Thickness TitleBarRightInset
		{
			get
			{
				if (ViewServiceProvider.CoreAppView.TitleBar.IsVisible) return new(0, 0, ViewServiceProvider.CoreAppView.TitleBar.SystemOverlayRightInset, 0);
				else return new(0);
			}
		}

		public bool DisableSoundEffects
		{
			get => (bool)SettingsService.GetValue(nameof(DisableSoundEffects), false);
			set
			{
				if (DisableSoundEffects != value)
				{
					ElementSoundPlayer.State = value ? ElementSoundPlayerState.Off : ElementSoundPlayerState.On;
					SettingsService.SetValue(nameof(DisableSoundEffects), value);
					Raise(nameof(DisableSoundEffects));
				}
			}
		}

		public string SelectedProgrammingLanguage
		{
			get => SettingsService.GetValue(nameof(SelectedProgrammingLanguage), "C#").ToString();
			set
			{
				if (SelectedProgrammingLanguage != value)
				{
					SettingsService.SetValue(nameof(SelectedProgrammingLanguage), value);
					Raise(nameof(SelectedProgrammingLanguage));
				}
			}
		}

		public void Initialize()
		{
			ViewServiceProvider.CoreAppView.TitleBar.IsVisibleChanged += TitleBar_Changed;
			ViewServiceProvider.CoreAppView.TitleBar.LayoutMetricsChanged += TitleBar_Changed;
		}

		private void TitleBar_Changed(CoreApplicationViewTitleBar sender, object args)
		{
			Raise(nameof(TitleBarHeight));
			Raise(nameof(TitleBarLeftInset));
			Raise(nameof(TitleBarRightInset));
		}
	}
}
