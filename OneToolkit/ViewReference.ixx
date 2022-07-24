module;

#include <inspectable.h>
#include "WindowingBase.h"
#include "UI.Windowing.ViewReference.g.h"
#include <winrt/Windows.System.h>
#include <winrt/OneToolkit.System.h>
#include <winrt/Windows.Foundation.Collections.h>

export module ViewReference;

import ViewReferenceTitleBar;

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace OneToolkit::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

export namespace winrt::OneToolkit::UI::Windowing::implementation
{
	struct ViewReferenceCache : implements<ViewReferenceCache, IMapView<uint64, ViewReference>>, impl::multi_threaded_collection_base
	{
	public:
		IIterator<IKeyValuePair<uint64, ViewReference>> First() const
		{
			auto const guard = acquire_shared();
			return make<Iterator>(m_Vector);
		}

		uint32 Size() const noexcept
		{
			auto const guard = acquire_shared();
			return static_cast<uint32>(m_Vector.size());
		}

		bool HasKey(uint64 key) const
		{
			return Lookup(key) != nullptr;
		}

		ViewReference Lookup(uint64 key) const
		{
			auto const guard = acquire_shared();
			for (auto const& weakView : m_Vector)
			{
				if (auto const strongView = weakView.get())
				{
					if (strongView.Id() == key) return strongView;
				}
			}

			return nullptr;
		}

		void Append(ViewReference const& value)
		{
			auto const guard = acquire_shared();
			m_Vector.emplace_back(value);
		}

		void Split(IMapView<uint64, ViewReference>& first, IMapView<uint64, ViewReference>& second) const noexcept
		{
			first = nullptr;
			second = nullptr;
		}

		void Remove(ViewReference const& value)
		{
			std::erase_if(m_Vector, [value](weak_ref<ViewReference> const& weakView)
				{
					if (auto const strongView = weakView.get()) return strongView == value;
					else return false;
				});
		}

		inline static auto const& s_Instance = make_self<ViewReferenceCache>();
	private:
		std::vector<weak_ref<ViewReference>> m_Vector;

		struct Iterator : implements<Iterator, IIterator<IKeyValuePair<uint64, ViewReference>>>
		{
		public:
			Iterator(std::vector<weak_ref<ViewReference>> const& vector) : m_Vector(vector), m_Iterator(vector.begin())
			{
				MoveNext();
			}

			auto_property<IKeyValuePair<uint64, ViewReference>> Current;

			bool HasCurrent() const noexcept
			{
				return m_Iterator != m_Vector.end();
			}

			bool MoveNext()
			{
				if (m_Iterator == m_Vector.end())
				{
					throw hresult_out_of_bounds();
				}

				++m_Iterator;
				if (m_Iterator == m_Vector.end())
				{
					Current(nullptr);
					return false;
				}
				else
				{
					Current(MakeItemPair(*m_Iterator));
					return true;
				}
			}

			uint32 GetMany(winrt::array_view<IKeyValuePair<uint64, ViewReference>> items)
			{
				for (uint32 index = 0; index < items.size(); ++index)
				{
					items[index] = MakeItemPair(m_Vector[index]);
				}

				return items.size();
			}
		private:
			std::vector<weak_ref<ViewReference>> const& m_Vector;

			std::vector<weak_ref<ViewReference>>::const_iterator m_Iterator;

			static IKeyValuePair<uint64, ViewReference> MakeItemPair(weak_ref<ViewReference> const& weakView)
			{
				auto const strongView = weakView.get();
				return make<impl::key_value_pair<IKeyValuePair<uint64, ViewReference>>>(strongView.Id(), strongView);
			}
		};
	};

	template <typename Derived>
	struct ViewReferenceBase : ViewReferenceT<Derived>
	{
	public:
		auto TitleBar()
		{
			auto titleBar = m_TitleBar.get();
			if (!titleBar && ViewReferenceTitleBar::IsCustomizationSupported())
			{
				titleBar = static_cast<Derived&>(*this).MakeTitleBarWrapper();
				m_TitleBar = titleBar;
			}

			return titleBar;
		}

		~ViewReferenceBase()
		{
			ViewReferenceCache::s_Instance->Remove(*this);
		}
	private:
		weak_ref<OneToolkit::UI::Windowing::ViewReferenceTitleBar> m_TitleBar;
	};

	namespace Universal::Core
	{
		struct ViewReference : ViewReferenceBase<ViewReference>
		{
		public:
			ViewReference(ApplicationView const& appView, CoreApplicationView const& coreAppView, SystemNavigationManager const& navigationManager, SystemNavigationManagerPreview const& navigationManagerPreview) : m_AppView(appView), m_CoreAppView(coreAppView), m_NavigationManager(navigationManager), m_NavigationManagerPreview(navigationManagerPreview) {}

			hstring Title() const
			{
				return m_AppView.Title();
			}

			void Title(hstring const& value)
			{	
				m_AppView.Title(value);
			}

			bool IsVisible() const
			{
				return m_CoreAppView.CoreWindow().Visible();
			}

			uint64 Id() const
			{
				return static_cast<uint64>(m_AppView.Id());
			}

			uint64 Handle() const
			{
				MIDL_INTERFACE("45D64A29-A63E-4CB6-B498-5781D298CB4F") ICoreWindowInterop : ::IUnknown
				{
					STDMETHOD(get_WindowHandle)(HWND * hwnd) = 0;
					STDMETHOD(put_MessageHandled)(bool value) = 0;
				};

				HWND result;
				check_hresult(m_CoreAppView.CoreWindow().as<ICoreWindowInterop>()->get_WindowHandle(&result));
				return as_value<uint64>(result);
			}

			Rect Bounds() const
			{
				return m_CoreAppView.CoreWindow().Bounds();
			}

			com_array<IInspectable> BackingObjects() const
			{
				return { m_AppView, m_CoreAppView, m_NavigationManager, m_NavigationManagerPreview };
			}

			void Resize(Size const& newSize) const
			{
				auto const result = m_AppView.TryResizeView(newSize);
				if (!result) throw hresult_access_denied();
			}

			void SetMinimumSize(Size const& minSize) const
			{
				m_AppView.SetPreferredMinSize(minSize);
			}

			IAsyncAction CloseAsync() const
			{
				auto const result = co_await m_AppView.TryConsolidateAsync();
				if (!result) throw hresult_access_denied();
			}

			IAsyncAction MinimizeAsync() const
			{
				if (CoreApplication::Views().Size() == 1) return AppLifecycleHelper::SuspendAsync();
				else throw hresult_access_denied();
			}

			OneToolkit::UI::Windowing::ViewReferenceTitleBar MakeTitleBarWrapper()
			{
				return make<ViewReferenceTitleBar>(m_AppView.TitleBar(), m_CoreAppView.TitleBar());
			}

			~ViewReference()
			{
				auto const coreWindow = m_CoreAppView.CoreWindow();
				coreWindow.SizeChanged(m_SizedChangedToken);
				coreWindow.VisibilityChanged(m_VisibilityChangedToken);
			}
		private:
			event_token m_SizedChangedToken;
			event_token m_VisibilityChangedToken;
			ApplicationView const m_AppView;
			CoreApplicationView const m_CoreAppView;
			SystemNavigationManager const m_NavigationManager;
			SystemNavigationManagerPreview const m_NavigationManagerPreview;
		};
	}

	namespace Universal::Preview
	{
		struct ViewReference : ViewReferenceBase<ViewReference>
		{
		public:
			ViewReference(AppWindow const& appWindow) : m_AppWindow(appWindow) {}

			hstring Title() const
			{
				return m_AppWindow.Title();
			}

			void Title(hstring const& value) const
			{
				m_AppWindow.Title(value);
			}

			bool IsVisible() const
			{
				return m_AppWindow.IsVisible();
			}
			
			uint64 Id() const
			{
				return static_cast<uint64>(CoreAppWindowPreview::GetIdFromWindow(m_AppWindow));
			}

			uint64 Handle() const
			{
				MIDL_INTERFACE("B74EA3BC-43C1-521F-9C75-E5C15054D78C") IApplicationWindow_HwndInterop : ::IInspectable
				{
					STDMETHOD(get_WindowHandle)(uint64* windowHandle) = 0;
				};

				uint64 result;
				check_hresult(m_AppWindow.as<IApplicationWindow_HwndInterop>()->get_WindowHandle(&result));
				return result;
			}

			Rect Bounds() const
			{
				auto const placement = m_AppWindow.GetPlacement();
				return { placement.Offset(), placement.Size() };
			}

			com_array<IInspectable> BackingObjects() const
			{
				return { m_AppWindow };
			}

			void Resize(Size const& newSize) const
			{
				m_AppWindow.RequestSize(newSize);
			}

			void SetMinimumSize(Size const& minSize) const
			{
				WindowManagementPreview::SetPreferredMinSize(m_AppWindow, minSize);
			}

			IAsyncAction CloseAsync() const
			{
				return m_AppWindow.CloseAsync();
			}

			IAsyncAction MinimizeAsync() const
			{
				return nullptr;
			}

			OneToolkit::UI::Windowing::ViewReferenceTitleBar MakeTitleBarWrapper()
			{
				return make<ViewReferenceTitleBar>(m_AppWindow.TitleBar());
			}
		private:
			AppWindow const m_AppWindow;
		};
	}

	namespace Desktop
	{
		struct ViewReference : ViewReferenceBase<ViewReference>
		{
		public:
			ViewReference(AppWindow const& appWindow) : m_AppWindow(appWindow) {}

			hstring Title() const
			{
				return m_AppWindow.Title();
			}

			void Title(hstring const& value) const
			{
				m_AppWindow.Title(value);
			}

			bool IsVisible() const
			{
				return m_AppWindow.IsVisible();
			}

			uint64 Id() const
			{
				return m_AppWindow.Id().Value;
			}

			uint64 Handle() const
			{
				// TODO
				throw hresult_not_implemented();
			}

			Rect Bounds() const
			{
				auto const size = m_AppWindow.Size();
				auto const position = m_AppWindow.Position();
				return { static_cast<float>(position.X), static_cast<float>(position.Y), static_cast<float>(size.Width), static_cast<float>(size.Height) };
			}

			com_array<IInspectable> BackingObjects() const
			{
				return { m_AppWindow };
			}

			void Resize(Size const& newSize) const
			{
				m_AppWindow.Resize({ static_cast<int>(newSize.Width), static_cast<int>(newSize.Height) });
			}
			
			void SetMinimumSize(Size const& minSize) const
			{
				throw hresult_not_implemented();
			}

			IAsyncAction CloseAsync() const
			{
				m_AppWindow.Destroy();
				co_return;
			}

			IAsyncAction MinimizeAsync() const
			{
				m_AppWindow.Hide();
				co_return;
			}

			OneToolkit::UI::Windowing::ViewReferenceTitleBar MakeTitleBarWrapper()
			{
				return make<ViewReferenceTitleBar>(m_AppWindow);
			}
		private:
			AppWindow const m_AppWindow;
		};
	}
}