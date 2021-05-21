using System;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Represents an property which automatically raises property changed/property changing events when it's value is set.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	/// <remarks>A field of this class should be only declared in a class that inherits from ObservableBase.</remarks>
	public sealed class ObservableProperty<T> : IWeakHolder<ObservableBase>
	{
		private readonly string PropertyName;

		private T BackingField;

		public ObservableProperty(string propertyName, T value = default)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new InvalidOperationException("PropertyName for an ObservableProperty instance must not be empty");
			BackingField = value;
			PropertyName = propertyName;
		}

		/// <summary>
		/// Gets or sets the underlying value for this ObservableProperty instance. Automatically raises PropertyChanged when needed.
		/// </summary>
		public T Value
		{
			get => BackingField;
			set
			{
				if (!ObservableBase.SetProperty(Holder, ref BackingField, value, PropertyName)) throw new ObjectDisposedException("Holder");
			}
		}

		public WeakPointer<ObservableBase> Holder { get; } = new();

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the data storage.</returns>
		public ref T GetBackingField() => ref BackingField;

		public override string ToString() => PropertyName;
	}
}