#include "pch.h"
#include "StreamHelper.h"
#include "Storage.StreamHelper.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

namespace winrt::OneToolkit::Storage::implementation
{
	IAsyncAction StreamHelper::AppendAsync(IRandomAccessStream const& stream, IBuffer const& buffer)
	{
		stream.Seek(0);
		auto size = static_cast<uint32_t>(stream.Size());
		auto readBuffer = co_await stream.ReadAsync(Buffer(size), size, InputStreamOptions::None);
		co_await stream.WriteAsync(BufferHelper::Concat({ readBuffer, buffer }));
		co_await stream.FlushAsync();
	}
}