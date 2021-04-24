#pragma once
#include "Logger.g.h"
#include <winrt/OneToolkit.Storage.h>

namespace winrt::OneToolkit
{
    namespace implementation
    {
        struct Logger : LoggerT<Logger>
        {
            Logger();
            Logger(hstring const& dateTimeFormat);
            Logger(hstring const& dateTimeFormat, Windows::Storage::StorageFile const& file);
            Windows::Globalization::DateTimeFormatting::DateTimeFormatter Formatter() const noexcept;
            Windows::Storage::StorageFile WorkingFile() const noexcept;
            Windows::Foundation::Collections::IVector<ICustomLogger> CustomLoggers();
            Windows::Security::Cryptography::BinaryStringEncoding OutputEncoding() const noexcept;
            void OutputEncoding(Windows::Security::Cryptography::BinaryStringEncoding value) noexcept;
            void Clear();
            void Write(hstring const& text);
            void Append(hstring const& text);
            LoggerContext GetContext() const noexcept;
            hstring CreateLog(hstring const& content);
            Windows::Foundation::IAsyncAction ClearAsync();
            Windows::Foundation::IAsyncAction WriteAsync(hstring text);
            Windows::Foundation::IAsyncAction AppendAsync(hstring text);
            void StartBatchingChanges();
            Windows::Foundation::IAsyncAction CommitBatchedChangesAsync();
            bool StartClosing() const noexcept;
            void Close();
            Windows::Foundation::IAsyncAction CloseAsync();
            static fire_and_forget final_release(std::unique_ptr<Logger> logger) noexcept;
        private:
            Windows::Foundation::Collections::IVector<ICustomLogger> m_CustomLoggers;
            Windows::Foundation::IAsyncAction AppendFileAsync(hstring content);
            Windows::Security::Cryptography::BinaryStringEncoding m_OutputEncoding = Windows::Security::Cryptography::BinaryStringEncoding::Utf16LE;
            bool m_IsChangesBatched = false;
            std::atomic_bool m_IsWriting = false;
            std::vector<hstring> m_BatchedChanges;
            Windows::Storage::StorageFile m_WorkingFile = nullptr;
            Windows::Globalization::DateTimeFormatting::DateTimeFormatter m_DateTimeFormatter = nullptr;
        };
    }

    namespace factory_implementation
    {
        struct Logger : LoggerT<Logger, implementation::Logger>
        {
        };
    }
}