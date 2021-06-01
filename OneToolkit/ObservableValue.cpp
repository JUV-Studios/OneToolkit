#include "pch.h"
#include "ObservableValue.h"
#include "Mvvm.NonGeneric.ObservableValue.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::OneToolkit::Mvvm::NonGeneric::implementation
{
	ObservableValue::ObservableValue(IInspectable const& val, bool suppressEvents)
	{
		Value(val);
		m_SuppressEvents = suppressEvents;
	}

	bool ObservableValue::IsEqual(IInspectable const& another) const noexcept
	{
		return Value() == another;
	}

	IInspectable ObservableValue::Clone()
	{
		return make<ObservableValue>(Value());
	}
}