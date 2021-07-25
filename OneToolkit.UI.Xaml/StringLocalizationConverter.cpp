#include "pch.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::ApplicationModel::Resources;

namespace OneToolkit::UI::Xaml::Converters
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class StringLocalizationConverter sealed : IValueConverter, INotifyPropertyChanged
	{
	public:
		property ResourceLoader^ Context
		{
			ResourceLoader^ get()
			{
				return m_Context;
			}

			void set(ResourceLoader^ value)
			{
				if (m_Context != value)
				{
					m_Context = value;
					PropertyChanged(this, ref new PropertyChangedEventArgs("Context"));
				}
			}
		}

		virtual event PropertyChangedEventHandler^ PropertyChanged;

		virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
		{
			if (auto uri = dynamic_cast<Uri^>(value)) return m_Context->GetStringForUri(uri);
			else return m_Context->GetString(value->ToString());
		}

		virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
		{
			throw ref new NotImplementedException();
		}
	private:	
		ResourceLoader^ m_Context;
	};
}