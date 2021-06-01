#include "pch.h"
#include "FileHandle.h"
#include "Storage.FileHandle.g.cpp"

using namespace winrt;
using namespace Windows::Storage;

namespace winrt::OneToolkit::Storage::implementation
{
	FileHandle::FileHandle(StorageFile const& file, FileAccessMode accessMode) : m_AccessMode(accessMode)
	{

	}

	FileAccessMode FileHandle::AccessMode() const noexcept
	{
		return m_AccessMode;
	}
}