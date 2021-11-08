#include "pch.h"
#include "Debugger.h"
#include "System.Debugger.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::ApplicationModel::Core;
using namespace OneToolkit::Data::Text;

namespace winrt::OneToolkit::System::implementation
{
	using DebugBreakProcess = int(__stdcall*)(void*);

	using CheckRemoteDebuggerPresent = int(__stdcall*)(void*, int*);

	Debugger::Debugger(uint32 processId, uint32 currentProcessId) : m_ProcessHandle(check_pointer(OpenProcess(PROCESS_QUERY_INFORMATION, false, processId)))
	{
		if (processId != currentProcessId) kernel32 = { L"Kernel32.dll" };
	}

	bool Debugger::IsPresent() const
	{
		if (kernel32)
		{
			int result;
			check_bool(kernel32->GetProcAddress<CheckRemoteDebuggerPresent>("CheckRemoteDebuggerPresent")(m_ProcessHandle.get(), &result));
			return result != 0;
		}
				
		return IsDebuggerPresent() != 0;
	}

	void Debugger::Break() const
	{
		if (kernel32) check_bool(kernel32->GetProcAddress<DebugBreakProcess>("DebugBreakProcess")(m_ProcessHandle.get()));
		else __debugbreak();
	}

	void Debugger::Write(hstring const& text)
	{
		OutputDebugStringW(text.data());
	}

	void Debugger::WriteLine(hstring const& lineText, uint8 lineEnding)
	{
		auto const newLine = LineEndingHelper::GetNewLineString<wchar>(static_cast<LineEnding>(lineEnding));
		Write(lineText + newLine);
	}

	OneToolkit::System::Debugger Debugger::GetForCurrentProcess()
	{
		auto const currentProcessId = GetCurrentProcessId();
		return make<Debugger>(currentProcessId, currentProcessId);
	}

	OneToolkit::System::Debugger Debugger::GetFromProcessId(uint32 const processId)
	{
		if (CoreApplication::Views().Size()) throw hresult_error(COR_E_PLATFORMNOTSUPPORTED, L"This method is for desktop apps only. Universal apps may not debug other processes.");
		auto const currentProcessId = GetCurrentProcessId();
		return make<Debugger>(processId, currentProcessId);
	}
}