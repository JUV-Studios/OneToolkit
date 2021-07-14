using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace OneToolkit.Mvvm
{
	/// <summary>
	/// Represents a value that notifies when it's changed.
	/// </summary>
	/// <typeparam name="T">The underlying type of the property.</typeparam>
	public sealed class ObservableValue<T> : ObservableBase, IBackingProvider<T>
	{
		private T BackingField;

		/// <summary>
		/// Creates a new instance of ObservableValue.
		/// </summary>
		public ObservableValue(T value = default, bool suppressEvents = false) : base(suppressEvents)
		{
			BackingField = value;
		}

		/// <summary>
		/// Creates a new instance of ObservableValue based on an existing one.
		/// </summary>
		/// <param name="other">The other instance whose properties will be copied.</param>
		public ObservableValue(ObservableValue<T> other) : base(other.SuppressEvents)
		{
			BackingField = other.BackingField;
		}

		/// <summary>
		/// Represents whether the property changing/property changed events will be raised or not.
		/// </summary>
		public new bool SuppressEvents
		{
			get => base.SuppressEvents;
			set => base.SuppressEvents = value;
		}

		/// <summary>
		/// Gets or sets the underlying value for this ObservableValue instance. Automatically raises PropertyChanging/PropertyChanged when you set it unless suppress is enabled.
		/// </summary>
		public T Value
		{
			get => BackingField;
			set
			{
				var previousValue = BackingField;
				if (SetProperty(ref BackingField, value)) Raise(previousValue, value);
			}
		}

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
			else if (obj is ObservableValue<T> observableValue) return Equals(observableValue.BackingField);
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