using System.ComponentModel;
using System.Linq;
using System.Reflection;

namespace OneToolkit.Mvvm
{
	public enum PropertyEventType : byte
	{
		PropertyChanging, PropertyChanged
	}

	public interface IPropertyEventRaiser
	{
		void Raise(string propertyName, PropertyEventType eventType);
	}

	/// <summary>
	/// Provides a base class for view models and observable objects
	/// </summary>
	public class ObservableBase : INotifyPropertyChanging, INotifyPropertyChanged, IPropertyEventRaiser
	{
		public event PropertyChangedEventHandler PropertyChanged;
		public event PropertyChangingEventHandler PropertyChanging;

		public ObservableBase()
		{
			var propertyFields = from field in GetType().GetRuntimeFields()
								 where field.FieldType.GetInterfaces().Contains(typeof(IWeakHolder<IPropertyEventRaiser>))
								 select field.GetValue(this) as IWeakHolder<IPropertyEventRaiser>;

			foreach (var propertyField in propertyFields)
			{
				propertyField.Holder.Target = this;
			}
		}

		/// <summary>
		/// Override this method to determine whether to raise property changed/property changing or not
		/// </summary>
		protected virtual bool Decide(string propertyName, PropertyEventType eventType)
		{
			return true;
		}

		/// <summary>
		/// Override this method to perform custom actions after raising the property changed event
		/// </summary>
		protected virtual void WhenPropertyChanged(PropertyChangedEventArgs args)
		{
			// Do nothing here
		}

		/// <summary>
		/// Override this method to perform custom actions after raising the property changing event
		/// </summary>
		protected virtual void WhenPropertyChanging(PropertyChangingEventArgs args)
		{
			// Do nothing here
		}

		public void Raise(string propertyName, PropertyEventType type)
		{
			if (Decide(propertyName, type))
			{
				if (type == PropertyEventType.PropertyChanged)
				{
					PropertyChangedEventArgs args = new(propertyName);
					PropertyChanged?.Invoke(this, args);
					WhenPropertyChanged(args);
				}
				else
				{
					PropertyChangingEventArgs args = new(propertyName);
					PropertyChanging?.Invoke(this, args);
					WhenPropertyChanging(args);
				}
			}
		}
	}
}
