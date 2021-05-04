#include "pch.h"
#include "PackageHelper.h"
#include "AppModel.PackageHelper.g.cpp"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::ApplicationModel;

namespace winrt::OneToolkit::AppModel::implementation
{
	std::optional<bool> m_IsFullTrust;

	XmlDocument m_PackageManifest = nullptr;

	bool PackageHelper::IsPackaged() noexcept
	{
		std::array<wchar_t, 64> packageFamilyName;
		uint32_t retrivedSize = 64;
		return GetPackageFamilyName(GetCurrentProcess(), &retrivedSize, packageFamilyName.data()) != APPMODEL_ERROR_NO_PACKAGE;
	}

	IAsyncOperation<bool> PackageHelper::CheckIsFullTrustAsync()
	{
		if (!m_IsFullTrust)
		{
			auto manifest = co_await GetPackageManifestAsync();
			std::wstring_view manifestView = manifest.GetXml();
			m_IsFullTrust = manifestView.find(L"runFullTrust") != std::wstring_view::npos;
		}

		co_return *m_IsFullTrust;
	}

	IAsyncOperation<XmlDocument> PackageHelper::GetPackageManifestAsync()
	{
		if (!m_PackageManifest)
		{
			auto manifestFile = co_await Package::Current().InstalledLocation().GetFileAsync(L"AppxManifest.xml");
			m_PackageManifest = co_await XmlDocument::LoadFromFileAsync(manifestFile, {});
		}

		co_return m_PackageManifest;
	}
}