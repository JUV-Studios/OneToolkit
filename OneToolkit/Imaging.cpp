#include "OneToolkit.h"
#include "Imaging.h"
#include "Media.Imaging.ImageCropUI.g.cpp"
#include "Media.Imaging.ImageResizer.g.cpp"
#include <winrt/Windows.System.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace OneToolkit::Storage;

namespace winrt::OneToolkit::Media::Imaging::implementation
{
	IAsyncOperation<bool> ImageCropUI::CropAsync(StorageFile origin, StorageFile destination) const
	{
		try
		{
			SharedFile sharedInput{ origin };
			SharedFile sharedDestination{ destination };
			auto cropWidth = static_cast<int>(CropSize().Width);
			auto cropHeight = static_cast<int>(CropSize().Height);
			ValueSet parameters;
			parameters.Insert(L"InputToken", box_value(sharedInput.Token()));
			parameters.Insert(L"DestinationToken", box_value(sharedDestination.Token()));
			parameters.Insert(L"CropWidthPixels", box_value(cropWidth));
			parameters.Insert(L"CropHeightPixels", box_value(cropHeight));
			parameters.Insert(L"EllipticalCrop", box_value(IsEllipticalCrop()));
			LauncherOptions options;
			options.TargetApplicationPackageFamilyName(L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
			auto result = co_await Launcher::LaunchUriForResultsAsync(PickerUri(), options);
			if (result.Status() != LaunchUriStatus::Success || result.Result() == nullptr) co_return false;
			else co_return true;
		}
		catch (hresult_error const&)
		{
			co_return false;
		}
	}

	Uri ImageCropUI::PickerUri()
	{
		return Uri(L"microsoft.windows.photos.crop:");
	}

	IAsyncOperation<bool> ImageCropUI::IsSupportedAsync()
	{
		auto status = co_await Launcher::QueryUriSupportAsync(PickerUri(), LaunchQuerySupportType::UriForResults, L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
		co_return status == LaunchQuerySupportStatus::Available;
	}

	IAsyncAction ImageResizer::RescaleAsync(Size desiredSize, BitmapInterpolationMode interpolationMode, IRandomAccessStream const& inputStream, IRandomAccessStream const& outputStream)
	{
		auto decoder = co_await BitmapDecoder::CreateAsync(inputStream);
		auto encoder = co_await BitmapEncoder::CreateForTranscodingAsync(outputStream, decoder);
		auto transform = encoder.BitmapTransform();
		transform.InterpolationMode(BitmapInterpolationMode::Linear);
		transform.ScaledWidth(static_cast<uint32>(desiredSize.Width));
		transform.ScaledHeight(static_cast<uint32>(desiredSize.Height));
		co_await encoder.FlushAsync();
	}
}