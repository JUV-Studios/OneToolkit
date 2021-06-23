#pragma once
#include "Data.Text.TextEncodingHelper.g.h"

namespace winrt::OneToolkit::Data::Text
{
    namespace implementation
    {
        struct TextEncodingHelper : TextEncodingHelperT<TextEncodingHelper>
        {
        public:
            TextEncodingHelper() = delete;
            static bool IsUtf8(Windows::Storage::Streams::IBuffer const& buffer);
            static bool IsUtf16(Windows::Storage::Streams::IBuffer const& buffer, Endianness& endianness);
            static juv::uint8 GetBomLength(Windows::Security::Cryptography::BinaryStringEncoding encoding) noexcept;
            static Windows::Foundation::IReference<Windows::Security::Cryptography::BinaryStringEncoding> TryDetect(Windows::Storage::Streams::IBuffer const& buffer) noexcept;
            static Windows::Foundation::IReference<Windows::Security::Cryptography::BinaryStringEncoding> TryDetectWithBom(Windows::Storage::Streams::IBuffer const& buffer) noexcept;
        private:
            static bool IsUtf16Ascii(std::span<juv::uint8 const> buffer, Endianness& endianness) noexcept;
            static bool IsUtf16Regular(std::span<juv::uint8 const> buffer, Endianness& endianness) noexcept;
        };
    }

    namespace factory_implementation
    {
        struct TextEncodingHelper : TextEncodingHelperT<TextEncodingHelper, implementation::TextEncodingHelper>
        {
        };
    }
}