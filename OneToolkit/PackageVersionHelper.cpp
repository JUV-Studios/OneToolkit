#include "System.PackageVersionHelper.g.h"

import juv;

using namespace juv;
using namespace winrt;
using namespace Windows::ApplicationModel;

namespace winrt::OneToolkit::System
{
	namespace implementation
	{
		struct PackageVersionHelper : static_t, PackageVersionHelperT<PackageVersionHelper>
		{
			static hstring Stringify(PackageVersion packageVersion)
			{
				return std::format(L"{0}.{1}.{2}.{3}", packageVersion.Major, packageVersion.Minor, packageVersion.Build, packageVersion.Revision).data();
			}

			static PackageVersion Parse(hstring const& formattedString)
			{
				auto setVersionField = [](std::wstring& token, uint16& field)
				{
					if (!token.empty()) field = static_cast<uint16>(std::stoul(token));
				};

				PackageVersion result;
				uint8 index = 0;
				wchar_t* state = nullptr;
				std::array<std::wstring, 4> tokens;
				std::wstring stringCopy{ formattedString };
				auto token = wcstok_s(stringCopy.data(), L".", &state);
				while (index < tokens.size() && token)
				{
					tokens[index] = token;
					token = wcstok_s(nullptr, L".", &state);
					++index;
				}

				setVersionField(tokens[0], result.Major);
				setVersionField(tokens[1], result.Minor);
				setVersionField(tokens[2], result.Build);
				setVersionField(tokens[3], result.Revision);
				return result;
			}
		};
	}

	namespace factory_implementation
	{
		struct PackageVersionHelper : PackageVersionHelperT<PackageVersionHelper, implementation::PackageVersionHelper>
		{
		};
	}
}

#include "System.PackageVersionHelper.g.cpp"