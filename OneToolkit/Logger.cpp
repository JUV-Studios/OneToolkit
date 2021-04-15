#include "pch.h"
#include "Logger.h"
#if __has_include("Logger.g.cpp")
#include "Logger.g.cpp"
#endif

using namespace winrt;
using namespace OneToolkit::Storage;
using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::Security::Cryptography;
using namespace Windows::Globalization;
using namespace Windows::Globalization::DateTimeFormatting;

namespace winrt::OneToolkit::implementation
{
	Logger::Logger() : m_DateTimeFormatter(L"shortdate longtime")
	{
		
	}

	Logger::Logger(hstring const& dateTimeFormat) : m_DateTimeFormatter(dateTimeFormat)
	{
	}

	Logger::Logger(hstring const& dateTimeFormat, StorageFile const& file) :  m_DateTimeFormatter(dateTimeFormat)
	{
	}

	DateTimeFormatter Logger::Formatter() const noexcept
	{
		return m_DateTimeFormatter;
	}

	StorageFile Logger::WorkingFile() const noexcept
	{
		return m_FileHandle == nullptr ? nullptr : m_FileHandle.UnderlyingFile();
	}

	IVector<ICustomLogger> Logger::CustomLoggers()
	{
		if (m_CustomLoggers == nullptr) m_CustomLoggers = multi_threaded_vector<ICustomLogger>();
		return m_CustomLoggers;
	}

	BinaryStringEncoding Logger::OutputEncoding() const noexcept
	{
		return m_OutputEncoding;
	}

	void Logger::OutputEncoding(BinaryStringEncoding value) noexcept
	{
		m_OutputEncoding = value;
	}

	LineEnding Logger::NewLineEnding() const noexcept
	{
		return m_NewLineEnding;
	}

	void Logger::NewLineEnding(LineEnding value) noexcept
	{
		m_NewLineEnding = value;
	}

	void Logger::Write(hstring const& text)
	{
		WriteAsync(text).get();
	}

	void Logger::Append(hstring const& text)
	{
		AppendAsync(text).get();
	}

	void Logger::Clear()
	{
		ClearAsync().get();
	}

	LoggerContext Logger::GetContext() const noexcept
	{
		return { .IsLoggingToFile = m_FileHandle != nullptr, .IsChangesBatched = m_IsChangesBatched, .IsConsoleSupported = GetConsoleWindow() != nullptr, .IsDebuggerAttached = IsDebuggerPresent() != 0 };
	}

	hstring Logger::CreateLog(hstring const& content)
	{
		std::wstring logContent;
		logContent += m_DateTimeFormatter.Format(clock::now()) + L" " + content;
		auto lastChr = content[content.size() - 1];
		if (lastChr != L'\r' && lastChr != L'\n') logContent += LineEndingHelper::GetNewLineString(m_NewLineEnding);
		return { logContent.data(), static_cast<uint32_t>(logContent.size()) };
	}

	IAsyncAction Logger::WriteAsync(hstring text)
	{
		co_await ClearAsync();
		co_await AppendAsync(text);
	}

	IAsyncAction Logger::ClearAsync()
	{
		auto lifetime = get_strong();
		apartment_context threadContext;
		if (impl::is_sta_thread()) co_await resume_background();
		while (m_IsWriting) continue;
		m_IsWriting = true;
		if (m_IsChangesBatched)
		{
			m_BatchedChanges.clear();
			m_IsChangesBatched = false;
		}

		auto context = GetContext();
		//if (context.IsLoggingToFile) m_FileHandle.FileWriter().WriteBuffer(BufferHelper::EmptyBuffer());
		if (context.IsConsoleSupported) system("cls");
		if (m_CustomLoggers != nullptr)
		{
			for (auto const& logger : m_CustomLoggers)
			{
				if (logger.CanClear()) co_await logger.ClearAsync();
			}
		}

		m_IsWriting = false;
		co_await threadContext;
	}

	IAsyncAction Logger::AppendAsync(hstring text)
	{
		auto lifetime = get_strong();
		apartment_context threadContext;
		if (impl::is_sta_thread()) co_await resume_background();
		while (m_IsWriting) continue;
		m_IsWriting = true;
		if (m_IsChangesBatched) m_BatchedChanges.emplace_back(text);
		else
		{
			auto context = GetContext();
			if (context.IsLoggingToFile) co_await AppendFileAsync(text);
			if (context.IsDebuggerAttached) OutputDebugString(text.data());
			if (context.IsConsoleSupported)
			{
				auto stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
				if (stdOut != nullptr)
				{
					DWORD written = 0;
					WriteConsole(stdOut, text.data(), text.size(), &written, nullptr);
				}
			}

			if (m_CustomLoggers != nullptr)
			{
				for (auto const& logger : m_CustomLoggers) logger.AppendAsync(text);
			}
		}

		m_IsWriting = false;
		co_await threadContext;
	}

	IAsyncAction Logger::AppendFileAsync(hstring content)
	{
		apartment_context threadContext;
		if (impl::is_sta_thread()) co_await resume_background();
		// m_FileHandle.FileAppender().AppendText(content);
		co_await threadContext;
	}

	void Logger::StartBatchingChanges()
	{
		m_IsChangesBatched = true;
	}

	IAsyncAction Logger::CommitBatchedChangesAsync()
	{
		auto lifetime = get_strong();
		m_IsChangesBatched = !m_IsChangesBatched;
		if (!m_BatchedChanges.empty())
		{
			std::wstring text;
			for (auto&& changes : m_BatchedChanges) text += changes;
			m_BatchedChanges.clear();
			co_await AppendAsync(hstring(text));
		}
	}

	bool Logger::StartClosing() const noexcept
	{
		return true;
	}

	void Logger::Close()
	{
		m_DateTimeFormatter = nullptr;
	}

	IAsyncAction Logger::CloseAsync()
	{
		return CommitBatchedChangesAsync();
	}

	fire_and_forget Logger::final_release(std::unique_ptr<Logger> logger) noexcept
	{
		co_await logger->CommitBatchedChangesAsync();
	}
}