#include "System.AppInformation.g.h"
#include "FixedFileInfo.h"
#include "FileVersionInfo.h"
#include <filesystem>
#include <functional>
#include <appmodel.h>
#include <wil/stl.h>
#include <wil/win32_helpers.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Foundation.Collections.h>

import juv;

using namespace juv;
using namespace FileVersion;
using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Metadata;

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		namespace Packaged
		{
			struct AppInformation : AppInformationT<AppInformation>
			{
			public:
				AppInformation(AppInfo const& appInfo) : m_AppInfo(appInfo) {}

				hstring Id() const
				{
					return m_AppInfo.Id();
				}

				hstring AppUserModelId() const
				{
					return m_AppInfo.AppUserModelId();
				}

				hstring Name() const
				{
					return m_AppInfo.DisplayInfo().DisplayName();
				}

				hstring Description() const
				{
					return m_AppInfo.DisplayInfo().Description();
				}

				hstring Publisher() const
				{
					return m_AppInfo.Package().PublisherDisplayName();
				}

				PackageVersion Version() const
				{
					return m_AppInfo.Package().Id().Version();
				}
			private:
				AppInfo const m_AppInfo;
			};
		}

		namespace Unpackaged
		{
			auto GetInfoStringOrDefault(FileVersionInfo const& fileVersionInfo, StringInfoField type, std::function<hstring()> const& fallbackProvider)
			{
				if (auto const& text = fileVersionInfo.GetInfoString(type); !text.empty()) return hstring(text);
				else return fallbackProvider();
			}

			struct AppInformation : AppInformationT<AppInformation>
			{
				AppInformation(std::filesystem::path const& executableFilePath, hstring const& appUserModelId) : AppUserModelId(appUserModelId)
				{
					FileVersionInfo fileVersionInfo;
					check_bool(fileVersionInfo.Init(executableFilePath.c_str()));
					Name(GetInfoStringOrDefault(fileVersionInfo, StringInfoField::FileDescription, [executableFilePath]()
						{
							return hstring(executableFilePath.filename().replace_extension(L"").c_str());
						}));

					Description(GetInfoStringOrDefault(fileVersionInfo, StringInfoField::Comments, [this]()
						{
							return Name();
						}));

					auto_property<hstring> property;

					auto& productVersion = fileVersionInfo.GetProductVersion();
					Version({ productVersion.major, productVersion.minor, productVersion.build, productVersion.revision });
				}

				auto_property<hstring> const AppUserModelId;

				auto_property<hstring> Name;

				auto_property<hstring> Description;

				auto_property<hstring> Publisher;

				auto_property<PackageVersion> Version;

				hstring Id() const noexcept
				{
					return AppUserModelId();
				}
			};
		}
	}

	namespace factory_implementation
	{
		struct AppInformation : AppInformationT<AppInformation, AppInformation>
		{
		public:
			static OneToolkit::System::AppInformation Current()
			{
				if (ApiInformation::IsPropertyPresent(name_of<AppInfo>(), L"Current"))
				{
					if (auto const appInfo = AppInfo::Current())
					{
						return make<implementation::Packaged::AppInformation>(appInfo);
					}
				}

				return make<implementation::Unpackaged::AppInformation>(wil::GetModuleFileNameW<std::wstring>(), GetCurrentAppUserModelId());
			}
		private:
			static hstring GetCurrentAppUserModelId()
			{
				wchar_t buffer[APPLICATION_USER_MODEL_ID_MAX_LENGTH];
				uint32 bufferLength = APPLICATION_USER_MODEL_ID_MAX_LENGTH;
				if (!GetApplicationUserModelId(GetCurrentProcess(), &bufferLength, buffer)) return { buffer, bufferLength };
				else return {};
			}
		};
	}

	AppInformation AppInformation::Current()
	{
		return factory_implementation::AppInformation::Current();
	}
}

void* winrt_make_OneToolkit_System_AppInformation()
{
	return detach_abi(make<OneToolkit::System::factory_implementation::AppInformation>());
}