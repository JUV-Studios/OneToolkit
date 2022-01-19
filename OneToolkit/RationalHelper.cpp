#include "Numerics.RationalHelper.g.h"

import juv;

using namespace juv;

namespace winrt::OneToolkit::Numerics
{
	namespace implementation
	{
		struct RationalHelper : static_t, RationalHelperT<RationalHelper>
		{
			static Windows::Foundation::Numerics::Rational GetSimplestForm(Windows::Foundation::Numerics::Rational rational)
			{
				throw hresult_not_implemented();
			}
		};
	}

	namespace factory_implementation
	{
		struct RationalHelper : RationalHelperT<RationalHelper, implementation::RationalHelper>
		{
		};
	}
}

#include "Numerics.RationalHelper.g.cpp"