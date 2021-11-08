#include "System.Debugger.g.h"

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct Debugger : DebuggerT<Debugger>
		{
		public:
			Debugger(juv::uint32 processId, juv::uint32 currentProcessId);
			bool IsPresent() const;
			void Break() const;
			static void Write(hstring const& text);
			static void WriteLine(hstring const& lineText, juv::uint8 lineEnding = static_cast<juv::uint8>(Data::Text::LineEnding::LF));
			static OneToolkit::System::Debugger GetForCurrentProcess();
			static OneToolkit::System::Debugger GetFromProcessId(juv::uint32 processId);
		private:
			handle m_ProcessHandle;
			std::optional<Runtime::DynamicLibrary> kernel32;
		};
	}

	namespace factory_implementation
	{
		struct Debugger : DebuggerT<Debugger, implementation::Debugger>
		{
		};
	}
}