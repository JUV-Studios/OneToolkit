using System;

namespace OneToolkit.Mvvm
{
	public static class ObservableValueHelper
	{
		/// <summary>
		/// Retrieves the value of the specified ObservableValue instance or a default value if it's null.
		/// </summary>
		public static T GetValueOrDefault<T>(this ObservableValue<T> instance, T defaultValue = default)
		{
			return instance == null ? defaultValue : instance.Value;
		}
	}

	/// <summary>
	/// Represents a value that notifies when its value is changed.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	public sealed class ObservableValue<T> : ObservableBase, ICloneable
	{
		private T BackingField;

		public ObservableValue(T value = default, bool suppressEvents = false) : base(suppressEvents)
		{
			BackingField = value;
		}

		/// <summary>
		/// Gets or sets the underlying value for this ObservableValue instance. Automatically raises PropertyChanging/PropertyChanged when you set it unless suppress is enabled.
		/// </summary>
		public T Value
		{
			get => BackingField;
			set => SetProperty(ref BackingField, value);
		}

		public object Clone() => new ObservableValue<T>(BackingField);

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the underlying data storage.</returns>
		public ref T GetBackingField() => ref BackingField;
	}
}
