using Windows.Storage;
using Microsoft.Toolkit.Mvvm.ComponentModel;

namespace OneToolkit.Storage
{
	public class AppSettingsBase : ObservableObject
	{
		protected AppSettingsBase(ApplicationDataContainer settingsContainer)
		{
			settingsContainer.Values.MapChanged += (_, args) => OnPropertyChanged(args.Key);
			SettingsContainer = settingsContainer;
		}

		protected ApplicationDataContainer SettingsContainer { get; init; }
	}
}

namespace System.Runtime.CompilerServices
{
	internal static class IsExternalInit { }
}