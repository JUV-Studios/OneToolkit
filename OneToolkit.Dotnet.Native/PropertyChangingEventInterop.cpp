using namespace Platform;
using namespace OneToolkit::Mvvm;
using namespace Windows::Foundation;

namespace OneToolkit::Details
{
	public ref class PropertyChangingEventInterop sealed
	{
	public:
		PropertyChangingEventInterop(Object^ sender) : Sender(sender)
		{
		}

		EventRegistrationToken Add(PropertyChangingEventHandler^ handler)
		{
			return PropertyChanging += handler;
		}

		void Remove(EventRegistrationToken token)
		{
			PropertyChanging -= token;
		}

		void Raise(String^ propertyName)
		{
			PropertyChangingEventArgs args;
			args.PropertyName = propertyName;
			PropertyChanging(Sender.Resolve<Object>(), args);
		}
	private:
		WeakReference Sender;
		event PropertyChangingEventHandler^ PropertyChanging;
	};
}