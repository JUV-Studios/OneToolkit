#include "pch.h"
#include "ImageResizer.h"
#include "Media.Imaging.ImageResizer.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Imaging;

namespace winrt::OneToolkit::Media::Imaging::implementation
{
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