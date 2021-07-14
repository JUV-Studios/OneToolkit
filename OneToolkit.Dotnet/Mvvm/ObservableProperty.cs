using System;
using System.Collections.Generic;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Represents an property which automatically raises property changed/property changing events when it's value is set.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	/// <remarks>A field of this class must be only declared in a class that inherits from ObservableBase if you omit the holder parameter when constructing.</remarks>
	public sealed class ObservableProperty<T> : IObservableHolder, IBackingProvider<T>
	{
		private T BackingField;

		/// <summary>
		/// Creates a new instance of ObservableProperty using an optional strong reference to the holder.
		/// </summary>
		public ObservableProperty(string propertyName, T value = default, ObservableBase holder = null)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new ArgumentException("PropertyName for an ObservableProperty instance must not be empty or only full of whitespaces.");
			_Holder = new(holder);
			BackingField = value;
			PropertyName = propertyName;
		}

		/// <summary>
		/// Creates a new instance of ObservableProperty using an optional weak reference to the holder.
		/// </summary>
		public ObservableProperty(string propertyName, T value = default, WeakReference<ObservableBase> holder = null)
		{
			if (string.IsNullOrWhiteSpace(propertyName)) throw new ArgumentException("PropertyName for an ObservableProperty instance must not be empty or only full of whitespaces.");
			_Holder = holder;
			BackingField = value;
			PropertyName = propertyName;
		}

		/// <summary>
		/// Creates a new instance of ObservableProperty based on an existing one.
		/// </summary>
		/// <param name="other">The other instance whose properties will be copied.</param>
		public ObservableProperty(ObservableProperty<T> other)
		{
			_Holder = other._Holder;
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
				if ((this as IObservableHolder).Holder.TryGetTarget(out ObservableBase holder)) ObservableBase.SetProperty(holder, ref BackingField, value, PropertyName);
				else throw new ObjectDisposedException("Holder for this ObservableProperty instance has been disposed.");
			}
		}

		private WeakReference<ObservableBase> _Holder;

		WeakReference<ObservableBase> IObservableHolder.Holder => _Holder ??= new(null);

		/// <summary>
		/// Enables access to the underlying data storage.
		/// </summary>
		/// <returns>A reference to the underlying data storage.</returns>
		public ref T GetBackingField() => ref BackingField;

		/// <summary>
		/// Indicates whether the current object is equal to another object of the same type.
		/// </summary>
		/// <param name="other">An object to compare with this object.</param>
		/// <returns>true if the current object is equal to the other parameter; otherwise, false.</returns>
		public bool Equals(T other) => EqualityComparer<T>.Default.Equals(BackingField, other);

		/// <summary>
		/// Determines whether the specified object is equal to the current object.
		/// </summary>
		/// <param name="obj">The object to compare with the current object.</param>
		/// <returns>true if the specified object is equal to the current object; otherwise, false.</returns>
		public override bool Equals(object obj)
		{
			if (ReferenceEquals(this, obj) || ReferenceEquals(BackingField, obj)) return true;
			else if (obj is ObservableProperty<T> observableProperty) return Equals(observableProperty.BackingField);
			else if (obj is T value) return Equals(value);
			else return false;
		}

		/// <summary>
		/// Serves as the default hash function.
		/// </summary>
		/// <returns>A hash code for the current object.</returns>
		public override int GetHashCode() => (BackingField?.GetHashCode()).GetValueOrDefault(base.GetHashCode());
	}
}