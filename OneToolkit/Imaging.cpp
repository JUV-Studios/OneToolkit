#include "Media.Imaging.ImageCropUI.g.h"
#include "Media.Imaging.ImageResizer.g.h"
#include <wil/resource.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

import juv;
import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Devices::PointOfService;

namespace winrt::OneToolkit::Media::Imaging
{
	namespace implementation
	{
		struct ImageCropUI : ImageCropUIT<ImageCropUI>
		{
		public:
			auto_property<bool> IsEllipticalCrop;

			auto_property<SizeInt32> CropSize{ {500, 500} };

			inline static auto const PickerUri = Uri(L"microsoft.windows.photos.crop:");

			IAsyncAction CropAsync(StorageFile origin, StorageFile destination) const
			{
				auto const cropSize = CropSize();
				auto const originSharingToken = SharedStorageAccessManager::AddFile(origin);
				auto const destinationSharingToken = SharedStorageAccessManager::AddFile(destination);
				auto const cleanup = wil::scope_exit([&]()
					{
						SharedStorageAccessManager::RemoveFile(originSharingToken);
						SharedStorageAccessManager::RemoveFile(destinationSharingToken);
					});

				ValueSet parameters;
				parameters.Insert(L"InputToken", box_value(originSharingToken));
				parameters.Insert(L"DestinationToken", box_value(originSharingToken));
				parameters.Insert(L"CropWidthPixels", box_value(cropSize.Width));
				parameters.Insert(L"CropHeightPixels", box_value(cropSize.Height));
				parameters.Insert(L"EllipticalCrop", box_value(IsEllipticalCrop()));
				LauncherOptions options;
				options.TargetApplicationPackageFamilyName(L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
				auto result = co_await Launcher::LaunchUriForResultsAsync(PickerUri, options);
				if (result.Status() != LaunchUriStatus::Success || result.Result() == nullptr) throw hresult_error();
			}

			static IAsyncOperation<bool> IsSupportedAsync()
			{
				auto status = co_await Launcher::QueryUriSupportAsync(PickerUri, LaunchQuerySupportType::UriForResults, L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
				co_return status == LaunchQuerySupportStatus::Available;
			}
		private:
			inline static Uri m_PickerUri = nullptr;
		};

		struct ImageResizer : ImageResizerT<ImageResizer>
		{
			ImageResizer() = delete;

			static IAsyncAction RescaleAsync(SizeUInt32 desiredSize, BitmapInterpolationMode interpolationMode, IRandomAccessStream const& inputStream, IRandomAccessStream const& outputStream)
			{
				auto decoder = co_await BitmapDecoder::CreateAsync(inputStream);
				auto encoder = co_await BitmapEncoder::CreateForTranscodingAsync(outputStream, decoder);
				auto transform = encoder.BitmapTransform();
				transform.InterpolationMode(BitmapInterpolationMode::Linear);
				transform.ScaledWidth(desiredSize.Width);
				transform.ScaledHeight(desiredSize.Height);
				co_await encoder.FlushAsync();
			}
		};
	}

	namespace factory_implementation
	{
		struct ImageCropUI : ImageCropUIT<ImageCropUI, implementation::ImageCropUI>
		{
		};

		struct ImageResizer : ImageResizerT<ImageResizer, implementation::ImageResizer>
		{
		};
	}
}

#include "Media.Imaging.ImageCropUI.g.cpp"
#include "Media.Imaging.ImageResizer.g.cpp"