﻿using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;

namespace OneToolkit.Mvvm
{
	public enum PropertyEventType : byte
	{
		PropertyChanging, PropertyChanged
	}

	/// <summary>
	/// Provides a base class for view models and observable objects.
	/// </summary>
	public class ObservableBase : INotifyPropertyChanging, INotifyPropertyChanged
	{
		public ObservableBase()
		{
			var propertyFields = from field in GetType().GetRuntimeFields()
								 where field.FieldType.GetInterfaces().Contains(typeof(IWeakHolder<ObservableBase>))
								 select field.GetValue(this) as IWeakHolder<ObservableBase>;
			foreach (var propertyField in propertyFields) propertyField.Holder.Target = this;
		}

		public bool SuppressEvents { get; set; }

		public event PropertyChangedEventHandler PropertyChanged;

		public event PropertyChangingEventHandler PropertyChanging;

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
		protected virtual void WhenPropertyChanging(PropertyChangingEventArgs args)
		{
			// Do nothing here
		}

		public static bool SetProperty<T>(WeakPointer<ObservableBase> pointer, ref T field, T newValue, string propertyName)
		{
			if (pointer.IsTargetValid)
			{
				pointer.Target.SetProperty(ref field, newValue, propertyName);
				return true;
			}
			else return false;
		}

		public static bool Raise(WeakPointer<ObservableBase> pointer, string propertyName, PropertyEventType type)
		{
			if (pointer.IsTargetValid)
			{
				pointer.Target.Raise(propertyName, type);
				return true;
			}
			else return false;
		}

		protected void SetProperty<T>(ref T field, T newValue, [CallerMemberName] string propertyName = null)
		{
			if (!EqualityComparer<T>.Default.Equals(field, newValue))
			{
				Raise(propertyName, PropertyEventType.PropertyChanging);
				field = newValue;
				Raise(propertyName, PropertyEventType.PropertyChanged);
			}
		}

		/// <summary>
		/// Raises the specified property event for a specified property name.
		/// </summary>
		/// <param name="propertyName">The name of the property. Shouldn't be empty or only full of whitespaces.</param>
		/// <param name="type">The type of the property event.</param>
		protected void Raise(string propertyName, PropertyEventType type)
		{
			if (!string.IsNullOrWhiteSpace(propertyName) && Decide(propertyName, type) && !SuppressEvents)
			{
				if (type == PropertyEventType.PropertyChanging)
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
			}
		}
	}
}