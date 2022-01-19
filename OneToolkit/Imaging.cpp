#include "Media.Imaging.ImageCropUI.g.h"
#include "Media.Imaging.ImageResizer.g.h"
#include <winrt/Windows.System.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Foundation.Collections.h>

import OneToolkit;

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Imaging;
using namespace OneToolkit::Storage;

namespace winrt::OneToolkit::Media::Imaging
{
	namespace implementation
	{
		struct ImageCropUI : ImageCropUIT<ImageCropUI>
		{
		public:
			auto_property<bool> IsEllipticalCrop;

			auto_property<Size> CropSize{ {500, 500} };

			IAsyncOperation<bool> CropAsync(StorageFile origin, StorageFile destination) const
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

			static Uri PickerUri()
			{
				static slim_mutex mutex;
				slim_lock_guard const mutexLock{ mutex };
				if (!m_PickerUri) m_PickerUri = Uri(L"microsoft.windows.photos.crop:");
				return m_PickerUri;
			}

			static IAsyncOperation<bool> IsSupportedAsync()
			{
				auto status = co_await Launcher::QueryUriSupportAsync(PickerUri(), LaunchQuerySupportType::UriForResults, L"Microsoft.Windows.Photos_8wekyb3d8bbwe");
				co_return status == LaunchQuerySupportStatus::Available;
			}
		private:
			inline static Uri m_PickerUri = nullptr;
		};

		struct ImageResizer : static_t, ImageResizerT<ImageResizer>
		{
			static IAsyncAction RescaleAsync(Size desiredSize, BitmapInterpolationMode interpolationMode, IRandomAccessStream const& inputStream, IRandomAccessStream const& outputStream)
			{
				auto decoder = co_await BitmapDecoder::CreateAsync(inputStream);
				auto encoder = co_await BitmapEncoder::CreateForTranscodingAsync(outputStream, decoder);
				auto transform = encoder.BitmapTransform();
				transform.InterpolationMode(BitmapInterpolationMode::Linear);
				transform.ScaledWidth(static_cast<uint32>(desiredSize.Width));
				transform.ScaledHeight(static_cast<uint32>(desiredSize.Height));
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