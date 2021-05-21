// (c) 2021 JUV Studios. All rights reserved. Included as part of OneToolkit for use in C++/WinRT projects.

#pragma once
#include <Lifecycle.h>
#include <winrt/OneToolkit.Storage.h>
#include <winrt/OneToolkit.Data.Text.h>
#include <winrt/OneToolkit.UI.Converters.h>

namespace juv
{
	template <typename Integer, typename Pointer> requires std::is_integral_v<Integer> && std::is_pointer_v<Pointer>
	auto as_value(Pointer pointer)
	{
		return static_cast<Integer>(reinterpret_cast<size_t>(pointer));
	}

	template <typename Pointer, typename Integer> requires std::is_integral_v<Integer> && std::is_pointer_v<Pointer>
	auto as_pointer(Integer value)
	{
		return reinterpret_cast<Pointer>(static_cast<size_t>(value));
	}
}