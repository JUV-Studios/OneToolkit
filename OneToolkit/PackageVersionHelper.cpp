#include "pch.h"
#include "PackageVersionHelper.h"
#include "ApplicationModel.PackageVersionHelper.g.cpp"

using namespace juv;
using namespace winrt;
using namespace Windows::ApplicationModel;

namespace winrt::OneToolkit::ApplicationModel::implementation
{
	hstring PackageVersionHelper::ToFormattedString(PackageVersion packageVersion)
	{
		return hstring(std::format(L"{0}.{1}.{2}.{3}", packageVersion.Major, packageVersion.Minor, packageVersion.Build, packageVersion.Revision));
	}

	PackageVersion PackageVersionHelper::FromFormattedString(hstring const& formattedString)
	{
		auto SetVersionField = [](std::wstring& token, uint16& field)
		{
			if (!token.empty()) field = static_cast<uint16>(std::stoul(token));
		};

		PackageVersion result;
		uint8 index = 0;
		wchar* state = nullptr;
		std::array<std::wstring, 4> tokens;
		std::wstring stringCopy{ formattedString };
		auto token = wcstok_s(stringCopy.data(), L".", &state);
		while (token != nullptr && index < tokens.size())
		{
			tokens[index] = token;
			token = wcstok_s(nullptr, L".", &state);
			++index;
		}

		SetVersionField(tokens[0], result.Major);
		SetVersionField(tokens[1], result.Minor);
		SetVersionField(tokens[2], result.Build);
		SetVersionField(tokens[3], result.Revision);
		return result;	
	}
}