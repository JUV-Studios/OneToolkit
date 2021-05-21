#pragma once
#include <winerror.h>
#include <winrt/OneToolkit.Lifecycle.h>

namespace winrt::OneToolkit::Lifecycle
{
	/// <summary>
	/// Provides a base class to conveniently implement the IClosable interface.
	/// <remarks>Your derived class must provide a public or protected Dispose method which will be called by Close when the object hasn't been closed yet.</remarks>
	/// </summary>
	template <typename Derived>
	struct Disposable
	{
	public:
		void Close() noexcept
		{
			if (!m_IsDisposed)
			{
				static_cast<Derived*>(this)->Dispose();
				m_IsDisposed = true;
			}
 		}

		virtual ~Disposable()
		{
			Close();
		}
	protected:
		bool m_IsDisposed;

		inline void ThrowIfDisposed() const
		{
			if (m_IsDisposed) throw hresult_error(RPC_E_DISCONNECTED);
		}
	};
}