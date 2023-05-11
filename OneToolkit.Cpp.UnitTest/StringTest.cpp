#include <CppUnitTest.h>

import juv;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace winrt::OneToolkit::Cpp::UnitTest
{
	TEST_CLASS(string_test)
	{
		TEST_METHOD(has_only_whitespaces)
		{
			Assert::AreEqual(juv::has_only_whitespaces(""), true);
			Assert::AreEqual(juv::has_only_whitespaces(" "), true);
			Assert::AreEqual(juv::has_only_whitespaces("\f"), true);
			Assert::AreEqual(juv::has_only_whitespaces("\n"), true);
			Assert::AreEqual(juv::has_only_whitespaces("\r"), true);
			Assert::AreEqual(juv::has_only_whitespaces("\t"), true);
			Assert::AreEqual(juv::has_only_whitespaces("\v"), true);
			Assert::AreEqual(juv::has_only_whitespaces("\r\n"), true);
			Assert::AreEqual(juv::has_only_whitespaces("Not blank at all."), false);
		}

		TEST_METHOD(is_space)
		{
			Assert::AreEqual(juv::is_space(' '), true);
			Assert::AreEqual(juv::is_space('\f'), true);
			Assert::AreEqual(juv::is_space('\n'), true);
			Assert::AreEqual(juv::is_space('\r'), true);
			Assert::AreEqual(juv::is_space('\t'), true);
			Assert::AreEqual(juv::is_space('\v'), true);
			Assert::AreEqual(juv::is_space('\0'), false);
		}
	};
}