using System;

namespace OneToolkit.Mvvm
{
	public static class ObservablePropertyHelper
	{
		/// <summary>
		/// Retrieves the value of the specified ObservableProperty instance or a default value if it's null.
		/// </summary>
		public static T GetValueOrDefault<T>(this ObservableProperty<T> instance, T defaultValue = default)
		{
			return instance == null ? defaultValue : instance.Value;
		}
	}

	/// <summary>
	/// Represents an property which automatically raises property changed/property changing events when it's value is set.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	/// <remarks>A field of this class should be only declared in a class that inherits from ObservableBase.</remarks>
	public sealed class ObservableProperty<T> : IWeakHolder<ObservableBase>, ICloneable
	{
		private readonly string PropertyName;

		private T BackingField;

		public ObservableProperty(string propertyName, T value = default, ObservableBase holder = null)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new InvalidOperationException("PropertyName for an ObservableProperty instance must not be empty.");
			BackingField = value;
			Holder.Target = holder;
			PropertyName = propertyName;
		}

		/// <summary>
		/// Gets or sets the underlying value for this ObservableProperty instance. Automatically raises PropertyChanging/PropertyChanged when you set it unless suppress is enabled.
		/// </summary>
		public T Value
		{
			get => BackingField;
			set
			{
				if (!ObservableBase.SetProperty(Holder, ref BackingField, value, $"{PropertyName}.Value")) throw new ObjectDisposedException("The holder for this ObservableProperty instance has been disposed.");
			}
		}

		public WeakPointer<ObservableBase> Holder { get; } = new();

		public object Clone() => new ObservableProperty<T>(PropertyName, BackingField, Holder.Target);

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the underlying data storage.</returns>
		public ref T GetBackingField() => ref BackingField;

		public override string ToString() => PropertyName;
	}
}