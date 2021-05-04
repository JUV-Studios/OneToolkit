#include "pch.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::ApplicationModel::Resources;

namespace OneToolkit::UI::Converters
{
	public ref class StringLocalizationConverter sealed : INotifyPropertyChanged, IValueConverter
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

		virtual Object^ Convert(Object^ value, TypeName, Object^, String^)
		{
			return m_Context->GetString(value->ToString());
		}

		virtual Object^ ConvertBack(Object^, TypeName, Object^, String^)
		{
			throw ref new NotImplementedException();
		}

		virtual event PropertyChangedEventHandler^ PropertyChanged;
	private:
		ResourceLoader^ m_Context = ResourceLoader::GetForViewIndependentUse();
	};
}