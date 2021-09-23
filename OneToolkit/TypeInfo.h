#pragma once
#include "Runtime.TypeInfo.g.h"

namespace winrt::OneToolkit::Runtime
{
    namespace implementation
    {
        struct TypeInfo : TypeInfoT<TypeInfo>
        {
        public:
            TypeInfo(Windows::UI::Xaml::Interop::TypeName typeId);
            DeclareAutoProperty(Windows::UI::Xaml::Interop::TypeName, Id, {});
            DeclareAutoProperty(hstring, Name, {});
            DeclareAutoProperty(hstring, Namespace, {});
            hstring QualifiedName() const noexcept;
            OneToolkit::Runtime::MetadataFile Metadata();
            Windows::UI::Xaml::Interop::TypeKind Kind() const noexcept;
            uint64_t BaseSize();
            IInspectable CreateInstance();
            IInspectable CreateInstance(array_view<IInspectable const> args);
            static OneToolkit::Runtime::TypeInfo GetTypeInfo(hstring const& qualifiedName);
            static bool IsTypePrimitive(hstring const& qualifiedName);
        private:
            OneToolkit::Runtime::MetadataFile m_Metadata = nullptr;
        };
    }

    namespace factory_implementation
    {
        struct TypeInfo : TypeInfoT<TypeInfo, implementation::TypeInfo>
        {
        };
    }
}
