module;

#include <inspectable.h>
#include "WindowingBase.h"
#include <winrt/Windows.System.h>
#include <winrt/OneToolkit.System.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/OneToolkit.UI.Windowing.h>

export module Window;

using namespace juv;
using namespace winrt;
using namespace Windows::System;
using namespace OneToolkit::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

export namespace winrt::OneToolkit::UI::Windowing::implementation
{
	/* struct ViewReferenceCache : implements<ViewReferenceCache, IMapView<uint64, ViewReference>>, impl::multi_threaded_collection_base
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
	}; */

	template <typename Derived>
	struct WindowBase : implements<Derived, IWindow>
	{
	public:
		auto TitleBar() const
		{
			return ViewService::GetTitleBarFromController(static_cast<Derived const&>(*this).MakeTitleBarController());
		}

		~WindowBase()
		{
			//ViewReferenceCache::s_Instance->Remove(*this);
		}
	private:
		weak_ref<OneToolkit::UI::Windowing::IWindowTitleBar> m_TitleBar;
	};

	namespace Universal::Core
	{
		struct Window : WindowBase<Window>
		{
		public:
			Window(WindowController const& controller) : Controller(controller) {}

			auto_property<WindowController> const Controller;

			hstring Title() const
			{
				return Controller().AppView().Title();
			}

			void Title(hstring const& value)
			{	
				Controller().AppView().Title(value);
			}

			bool IsVisible() const
			{
				return Controller().CoreAppView().CoreWindow().Visible();
			}

			uint64 Id() const
			{
				return static_cast<uint64>(Controller().AppView().Id());
			}

			uint64 Handle() const
			{
				MIDL_INTERFACE("45D64A29-A63E-4CB6-B498-5781D298CB4F") ICoreWindowInterop : ::IUnknown
				{
					STDMETHOD(get_WindowHandle)(HWND * hwnd) = 0;
					STDMETHOD(put_MessageHandled)(bool value) = 0;
				};

				HWND result;
				check_hresult(Controller().CoreAppView().CoreWindow().as<ICoreWindowInterop>()->get_WindowHandle(&result));
				return as_value<uint64>(result);
			}

			Rect Bounds() const
			{
				return Controller().CoreAppView().CoreWindow().Bounds();
			}

			void Resize(Size const& newSize) const
			{
				auto const result = Controller().AppView().TryResizeView(newSize);
				if (!result) throw hresult_access_denied();
			}

			void SetMinimumSize(Size const& minSize) const
			{
				Controller().AppView().SetPreferredMinSize(minSize);
			}

			IAsyncAction CloseAsync() const
			{
				auto const result = co_await Controller().AppView().TryConsolidateAsync();
				if (!result) throw hresult_access_denied();
			}

			IAsyncAction MinimizeAsync() const
			{
				if (CoreApplication::Views().Size() == 1) return AppLifecycleHelper::SuspendAsync();
				else throw hresult_access_denied();
			}

			WindowTitleBarController MakeTitleBarController() const
			{
				return { Controller().AppView().TitleBar(), Controller().CoreAppView().TitleBar() };
			}

			~Window()
			{
				auto const coreWindow = Controller().CoreAppView().CoreWindow();
				coreWindow.SizeChanged(m_SizedChangedToken);
				coreWindow.VisibilityChanged(m_VisibilityChangedToken);
			}
		private:
			event_token m_SizedChangedToken;
			event_token m_VisibilityChangedToken;
		};
	}

	namespace Universal::Preview
	{
		struct Window : WindowBase<Window>
		{
			Window(AppWindow const& controller) : Controller(controller) {}

			auto_property<AppWindow> const Controller;

			hstring Title() const
			{
				return Controller().Title();
			}

			void Title(hstring const& value) const
			{
				Controller().Title(value);
			}

			bool IsVisible() const
			{
				return Controller().IsVisible();
			}
			
			uint64 Id() const
			{
				return static_cast<uint64>(CoreAppWindowPreview::GetIdFromWindow(Controller()));
			}

			uint64 Handle() const
			{
				MIDL_INTERFACE("B74EA3BC-43C1-521F-9C75-E5C15054D78C") IApplicationWindow_HwndInterop : ::IInspectable
				{
					STDMETHOD(get_WindowHandle)(uint64* windowHandle) = 0;
				};

				uint64 result;
				check_hresult(Controller().as<IApplicationWindow_HwndInterop>()->get_WindowHandle(&result));
				return result;
			}

			Rect Bounds() const
			{
				auto const placement = Controller().GetPlacement();
				return { placement.Offset(), placement.Size() };
			}

			void Resize(Size const& newSize) const
			{
				Controller().RequestSize(newSize);
			}

			void SetMinimumSize(Size const& minSize) const
			{
				WindowManagementPreview::SetPreferredMinSize(Controller(), minSize);
			}

			IAsyncAction CloseAsync() const
			{
				return Controller().CloseAsync();
			}

			IAsyncAction MinimizeAsync() const
			{
				return nullptr;
			}

			auto MakeTitleBarController() const
			{
				return Controller().TitleBar();
			}
		};
	}

	namespace Desktop
	{
		struct Window : WindowBase<Window>
		{
			Window(AppWindow const& controller) : Controller(controller) {}

			auto_property<AppWindow> const Controller;

			hstring Title() const
			{
				return Controller().Title();
			}

			void Title(hstring const& value) const
			{
				Controller().Title(value);
			}

			bool IsVisible() const
			{
				return Controller().IsVisible();
			}

			uint64 Id() const
			{
				return Controller().Id().Value;
			}

			uint64 Handle() const
			{
				// TODO
				throw hresult_not_implemented();
			}

			Rect Bounds() const
			{
				auto const size = Controller().Size();
				auto const position = Controller().Position();
				return { static_cast<float>(position.X), static_cast<float>(position.Y), static_cast<float>(size.Width), static_cast<float>(size.Height) };
			}

			void Resize(Size const& newSize) const
			{
				Controller().Resize({ static_cast<int>(newSize.Width), static_cast<int>(newSize.Height) });
			}
			
			void SetMinimumSize(Size const& minSize) const
			{
				throw hresult_not_implemented();
			}

			IAsyncAction CloseAsync() const
			{
				Controller().Destroy();
				co_return;
			}

			IAsyncAction MinimizeAsync() const
			{
				Controller().Hide();
				co_return;
			}

			auto MakeTitleBarController() const
			{
				return Controller().TitleBar();
			}
		};
	}
}