#include "pch.h"
#include "CropDialog.h"
#include "Imaging.CropDialog.g.cpp"

using namespace winrt;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace OneToolkit::Storage;

namespace winrt::OneToolkit::Imaging::implementation
{
	inline Uri PickerUri { L"microsoft.windows.photos.crop:" };

	IAsyncOperation<bool> CropDialog::CropAsync(StorageFile input, StorageFile destination) const try
	{
		SharedFile sharedInput { input };
		SharedFile sharedDestination { destination };
		auto cropWidth = static_cast<int>(CropSize().Width);
		auto cropHeight = static_cast<int>(CropSize().Height);
		ValueSet parameters;
		parameters.Insert(L"InputToken", box_value(sharedInput.Token()));
		parameters.Insert(L"DestinationToken", box_value(sharedDestination.Token()));
		parameters.Insert(L"CropWidthPixels", box_value(cropWidth));
		parameters.Insert(L"CropHeightPixels", box_value(cropHeight));
		parameters.Insert(L"EllipticalCrop", box_value(IsEllipticalCrop()));
		parameters.Insert(L"ShowCamera", box_value(ShowCamera()));
		LauncherOptions options;
		options.TargetApplicationPackageFamilyName(L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
		auto result = co_await Launcher::LaunchUriForResultsAsync(PickerUri, options);
		if (result.Status() != LaunchUriStatus::Success || result.Result() == nullptr) co_return false;
		else co_return true;
	}
	catch (hresult_error const&) { co_return false;	}

	IAsyncOperation<bool> CropDialog::IsSupportedAsync()
	{
		auto status = co_await Launcher::QueryUriSupportAsync(PickerUri, LaunchQuerySupportType::UriForResults, L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
		co_return status == LaunchQuerySupportStatus::Available;
	}
}