using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;

namespace OneToolkit.Mvvm
{
	internal interface IObservableHolder
	{
		WeakReference<ObservableBase> Holder { get; }
	}

	/// <summary>
	/// Provides a base class for view models and observable objects.
	/// </summary>
	public class ObservableBase : INotifyPropertyChanging, INotifyPropertyChanged, INotifyValueChanged
	{
		/// <summary>
		/// Creates a new instance of ObservableBase from a derived class. 
		/// </summary>
		/// <param name="suppressEvents">Optional initial value for the SuppressEvents property.</param>
		protected ObservableBase(bool suppressEvents = false)
		{
			SuppressEvents = suppressEvents;
			var propertyFields = from field in GetType().GetRuntimeFields()
								 where field.FieldType.GetInterfaces().Contains(typeof(IObservableHolder))
								 select field.GetValue(this) as IObservableHolder;

			foreach (var propertyField in propertyFields)
			{
				if (propertyField != null) propertyField.Holder.SetTarget(this);
			}
		}

		/// <summary>
		/// Represents whether the property changing/property changed events will be raised or not.
		/// </summary>
		public bool SuppressEvents { get; protected set; }

		/// <summary>
		/// Occurs when a property value changes.
		/// </summary>
		public event PropertyChangedEventHandler PropertyChanged;

		/// <summary>
		/// Occurs when a property value is changing.
		/// </summary>
		public event PropertyChangingEventHandler PropertyChanging;

		/// <summary>
		/// Occurs when the value held by this object changes.
		/// </summary>
		public event EventHandler<ValueChangedEventArgs> ValueChanged;

		/// <summary>
		/// Enables access to the protected SetProperty method from the outside world.
		/// </summary>
		internal static bool SetProperty<T>(ObservableBase @base, ref T field, T newValue, string propertyName) => @base.SetProperty(ref field, newValue, propertyName);

		/// <summary>
		/// Enables access to the protected Raise method from the outside world.
		/// </summary>
		internal static bool Raise(ObservableBase @base, string propertyName, PropertyEventType eventType = PropertyEventType.PropertyChanged) => @base.Raise(propertyName, eventType);

		/// <summary>
		/// Enables access to the protected Raise method from the outside world.
		/// </summary>
		internal static bool Raise(ObservableBase @base, object oldValue, object newValue) => @base.Raise(oldValue, newValue);

		/// <summary>
		/// Override this method to determine whether to raise property changed/property changing or not.
		/// </summary>
		protected virtual bool Decide(string propertyName, PropertyEventType eventType)
		{
			return true;
		}

		/// <summary>
		/// Override this method to perform custom actions after raising the property changed event.
		/// </summary>
		protected virtual void WhenPropertyChanged(PropertyChangedEventArgs args)
		{
			// Do nothing here
		}

		/// <summary>
		/// Override this method to perform custom actions after raising the property changing event.
		/// </summary>
		protected virtual void WhenPropertyChanging(System.ComponentModel.PropertyChangingEventArgs args)
		{
			// Do nothing here
		}

		/// <summary>
		/// Automatically sets a property value and raises property changing/property changed when required.
		/// </summary>
		/// <returns>True if the value was set, false if the value passed was equal to the existing value.</returns>
		protected bool SetProperty<T>(ref T field, T newValue, [CallerMemberName] string propertyName = null)
		{
			if (!EqualityComparer<T>.Default.Equals(field, newValue))
			{
				Raise(propertyName, PropertyEventType.PropertyChanging);
				field = newValue;
				Raise(propertyName, PropertyEventType.PropertyChanged);
				return true;
			}

			return false;
		}

		/// <summary>
		/// Raises the specified property event for a specified property name.
		/// </summary>
		/// <param name="propertyName">The name of the property. Shouldn't be empty or only full of whitespaces.</param>
		/// <param name="eventType">The type of the property event.</param>
		protected bool Raise(string propertyName, PropertyEventType eventType = PropertyEventType.PropertyChanged)
		{
			if (!string.IsNullOrWhiteSpace(propertyName) && Decide(propertyName, eventType) && !SuppressEvents)
			{
				if (eventType == PropertyEventType.PropertyChanging)
				{
					PropertyChangingEventArgs args = new(propertyName);
					PropertyChanging?.Invoke(this, args);
					WhenPropertyChanging(args);
				}
				else
				{
					PropertyChangedEventArgs args = new(propertyName);
					PropertyChanged?.Invoke(this, args);
					WhenPropertyChanged(args);
				}

				return true;
			}

			return false;
		}

		/// <summary>
		/// Raises the value changed event on this object.
		/// </summary>
		protected bool Raise(object oldValue, object newValue)
		{
			if (!SuppressEvents)
			{
				ValueChanged(this, new(oldValue, newValue));
				return true;
			}

			return false;
		}
	}
}