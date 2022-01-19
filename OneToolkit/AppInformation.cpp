#include "System.AppInformation.g.h"
#include <filesystem>
#include <Windows.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace winrt::impl;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace OneToolkit::Storage;

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct AppInformation : AppInformationT<AppInformation>
		{
		public:
			AppInformation(std::filesystem::path const& executableFilePath) : m_ExecutableFilePath(executableFilePath.c_str())
			{
				FileVersionInfo versionInfo{ executableFilePath };
				auto const languageInfo = versionInfo.Translations()[0];
				if (auto const displayName = versionInfo.GetStringResource(L"FileDescription", languageInfo); !displayName.empty()) DisplayName(displayName.data());
				else DisplayName(executableFilePath.filename().replace_extension(L"").c_str());
				if (auto const description = versionInfo.GetStringResource(L"Comments", languageInfo); !description.empty()) Description(description.data());
				else Description(DisplayName());
			}

			auto_property<hstring> DisplayName;

			auto_property<hstring> Description;

			IAsyncOperation<StorageFile> GetExecutableFileAsync() const
			{
				return StorageFile::GetFileFromPathAsync(m_ExecutableFilePath);
			}

			static bool IsXamlApplication() noexcept
			{
				try
				{
					if (Windows::UI::Xaml::Application::Current()) return true;
					else if (Microsoft::UI::Xaml::Application::Current()) return true;
				}
				catch (...) {}
				return false;
			}

			static OneToolkit::System::AppInformation Current()
			{
				static OneToolkit::System::AppInformation currentInstance = nullptr;
				static slim_mutex accessLock;
				slim_lock_guard const lockGuard{ accessLock };
				if (!currentInstance)
				{
					std::array<wchar_t, MAX_PATH> filePath;
					check_bool(GetModuleFileName(nullptr, filePath.data(), static_cast<uint32>(filePath.size())));
					currentInstance = make<AppInformation>(filePath.data());
				}

				return currentInstance;
			}
		private:
			hstring const m_ExecutableFilePath;
		};
	}

	namespace factory_implementation
	{
		struct AppInformation : AppInformationT<AppInformation, implementation::AppInformation>
		{
		};
	}
}

#include "System.AppInformation.g.cpp"