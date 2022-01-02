#include "OneToolkit.h"
#include "Storage.FileOperations.g.h"
#include <winrt/Windows.Foundation.h>

using namespace juv;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

namespace winrt::OneToolkit::Storage::factory_implementation
{
	struct FileOperations : FileOperationsT<FileOperations, FileOperations>
	{
		static IAsyncAction WriteBufferAsync(FileHandle fileHandle, IBuffer buffer)
		{
			co_await resume_background();
			uint32 writtenBytesCount;
			check_bool(WriteFile(as_pointer<HANDLE>(fileHandle.Id().Value), buffer.data(), buffer.Length(), reinterpret_cast<LPDWORD>(&writtenBytesCount), nullptr));
		}
	};
}

void* winrt_make_OneToolkit_Storage_FileOperations()
{
	return detach_abi(make<OneToolkit::Storage::factory_implementation::FileOperations>());
}