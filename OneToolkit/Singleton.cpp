#include "pch.h"
#include "Singleton.h"
#include "Singleton.g.cpp"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::ApplicationModel::Core;

namespace winrt::OneToolkit::implementation
{
	IInspectable Singleton::GetInstance(TypeName const& typeId)
	{
		auto storeTypeName = GetStoreTypeName(typeId.Name);
		return CoreApplication::Properties().TryLookup(storeTypeName);
	}

	IInspectable Singleton::GetInstance(TypeName const& typeId, SingletonInstanceProvider const& provider)
	{
		auto storeTypeName = GetStoreTypeName(typeId.Name);
		if (CoreApplication::Properties().HasKey(storeTypeName))
		{
			return CoreApplication::Properties().Lookup(storeTypeName);
		}
		else
		{
			auto instance = provider(typeId);
			AddInstance(storeTypeName, instance);
			return instance;
		}
	}

	void Singleton::SetInstance(TypeName const& typeId, IInspectable const& value)
	{
		auto storeTypeName = GetStoreTypeName(typeId.Name);
		AddInstance(storeTypeName, value);
	}

	hstring Singleton::GetStoreTypeName(hstring const& typeName)
	{
		return L"OneToolkit.Singleton{" + typeName + L"}";
	}

	void Singleton::AddInstance(hstring const& storeTypeName, IInspectable const& value)
	{
		static slim_mutex singletonLock;
		const slim_lock_guard lockGuard { singletonLock };
		CoreApplication::Properties().Insert(storeTypeName, value);
	}
}