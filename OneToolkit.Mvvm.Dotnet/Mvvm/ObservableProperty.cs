using System;
using System.Collections.Generic;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Represents an property which automatically raises property changed/property changing events when it's value is set.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	/// <remarks>A field of this class should be only declared in a class that inherits from ObservableBase.</remarks>
	public sealed class ObservableProperty<T> : IObservableHolder, IBackingProvider<T>
	{
		private T BackingField;

		public ObservableProperty(string propertyName, T value = default, WeakReference<ObservableBase> holder = null)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new ArgumentException("PropertyName for an ObservableProperty instance must not be empty.");
			Holder = holder;
			BackingField = value;
			PropertyName = propertyName;
		}

		public ObservableProperty(string propertyName, T value = default, ObservableBase holder = null)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new ArgumentException("PropertyName for an ObservableProperty instance must not be empty.");
			Holder.SetTarget(holder);
			BackingField = value;
			PropertyName = propertyName;
		}

		/// <summary>
		/// Creates a new instance of ObservableProperty based on an existing one.
		/// </summary>
		/// <param name="other">The other instance whose properties will be copied.</param>
		public ObservableProperty(ObservableProperty<T> other)
		{
			Holder = other.Holder;
			BackingField = other.BackingField;
			PropertyName = other.PropertyName;
		}

		/// <summary>
		/// Gets the name of this ObservableProperty which will be used when raising property events. 
		/// </summary>
		public readonly string PropertyName;

		/// <summary>
		/// Gets or sets the underlying value for this ObservableProperty instance.
		/// </summary>
		public T Value
		{
			get => BackingField;
			set
			{
				if (Holder.TryGetTarget(out ObservableBase holder)) ObservableBase.SetProperty(holder, ref BackingField, value, PropertyName);
				else throw new ObjectDisposedException("Holder for this ObservableProperty instance has been disposed.");
			}
		}

		public WeakReference<ObservableBase> Holder { get; } = new(null);

		public ref T GetBackingField() => ref BackingField;

		public bool Equals(T other) => EqualityComparer<T>.Default.Equals(BackingField, other);

		public override bool Equals(object obj)
		{
			if (ReferenceEquals(this, obj) || ReferenceEquals(BackingField, obj)) return true;
			else if (obj is ObservableProperty<T> observableProperty) return Equals(observableProperty.BackingField);
			else if (obj is T value) return Equals(value);
			else return false;
		}

		public override int GetHashCode() => (BackingField?.GetHashCode()).GetValueOrDefault(base.GetHashCode());
	}
}