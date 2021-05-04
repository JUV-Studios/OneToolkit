#include "pch.h"
#include "PackageHelper.h"
#include "AppModel.PackageHelper.g.cpp"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::AppModel::implementation
{
	std::optional<bool> m_IsFullTrust;

	bool PackageHelper::IsPackaged() noexcept
	{
		std::array<wchar_t, 64> packageFamilyName;
		uint32_t retrivedSize = 64;
		return GetPackageFamilyName(GetCurrentProcess(), &retrivedSize, packageFamilyName.data()) != APPMODEL_ERROR_NO_PACKAGE;
	}

	IAsyncOperation<bool> PackageHelper::CheckIsFullTrust()
	{
		if (!m_IsFullTrust)
		{
			auto manifest = co_await PathIO::ReadTextAsync(L"ms-appx:///AppxManifest.xml");
			std::wstring_view manifestView = manifest;
			m_IsFullTrust = manifestView.find(L"runFullTrust") != std::wstring_view::npos;
		}

		co_return *m_IsFullTrust;
	}
}