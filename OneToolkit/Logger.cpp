#include "Storage.Logging.Logger.g.h"

namespace winrt::OneToolkit::Storage::Logging
{
	namespace implementation
	{
		struct Logger : LoggerT<Logger>
		{
			void Write(hstring const& text)
			{
				throw hresult_not_implemented();
			}

			void Write(IInspectable const& value)
			{
				throw hresult_not_implemented();
			}
		};
	}

	namespace factory_implementation
	{
		struct Logger : LoggerT<Logger, implementation::Logger>
		{
		};
	}
}

#include "Storage.Logging.Logger.g.cpp"