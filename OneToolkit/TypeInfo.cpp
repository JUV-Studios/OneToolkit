#include "pch.h"
#include "TypeInfo.h"
#include "MetadataFile.h"
#include "Runtime.TypeInfo.g.cpp"
#include <roapi.h>

using namespace juv;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Interop;

namespace winrt::OneToolkit::Runtime::implementation
{
    TypeInfo::TypeInfo(TypeName typeId) : m_Id(typeId)
    {
        std::wstring_view typeNameView = m_Id.Name;
        m_Name = hstring(typeNameView.substr(typeNameView.find_last_of(L'.') + 1));
        m_Namespace = hstring(typeNameView.substr(0, typeNameView.find_last_of(L'.') - 1));
    }

    hstring TypeInfo::QualifiedName() const noexcept
    {
        return m_Id.Name;
    }

    OneToolkit::Runtime::MetadataFile TypeInfo::Metadata()
    {
        if (m_Id.Kind == TypeKind::Metadata)
        {
            if (!m_Metadata) m_Metadata = make<MetadataFile>(m_Id);
        }

        return m_Metadata;
    }

    TypeKind TypeInfo::Kind() const noexcept
    {
        return m_Id.Kind;
    }

    uint64_t TypeInfo::BaseSize()
    {
        if (m_Id == xaml_typename<bool>()) return sizeof(bool);
        else if (m_Id == xaml_typename<char16>()) return sizeof(char16);
        else if (m_Id == xaml_typename<uint8>()) return sizeof(uint8);
        else if (m_Id == xaml_typename<int8>()) return sizeof(int8);
        else if (m_Id == xaml_typename<uint16>()) return sizeof(uint16);
        else if (m_Id == xaml_typename<int16>()) return sizeof(int16);
        else if (m_Id == xaml_typename<uint32>()) return sizeof(uint32);
        else if (m_Id == xaml_typename<int32>()) return sizeof(int32);
        else if (m_Id == xaml_typename<uint64>()) return sizeof(uint64);
        else if (m_Id == xaml_typename<int64>()) return sizeof(int64);
        else if (m_Id == xaml_typename<float>()) return sizeof(float);
        else if (m_Id == xaml_typename<double>()) return sizeof(double);
        else if (m_Id == xaml_typename<hstring>()) return sizeof(hstring);
        else if (m_Id == xaml_typename<guid>()) return sizeof(guid);
        else if (m_Id == xaml_typename<Point>()) return sizeof(Point);
        else if (m_Id == xaml_typename<Size>()) return sizeof(Size);
        else if (m_Id == xaml_typename<Rect>()) return sizeof(Rect);
        else if (m_Id == xaml_typename<DateTime>()) return sizeof(DateTime);
        else if (m_Id == xaml_typename<TimeSpan>()) return sizeof(TimeSpan);
        else return sizeof(size_t); // TODO Calculate type's size from metadata if it's a value type.
    }

    winrt::Windows::Foundation::IInspectable TypeInfo::CreateInstance()
    {
        if (m_Id.Kind == TypeKind::Primitive)
        {
            if (m_Id == xaml_typename<bool>()) return box_value(bool());
            else if (m_Id == xaml_typename<char16>()) return box_value(char16());
            else if (m_Id == xaml_typename<uint8>()) return box_value(uint8());
            else if (m_Id == xaml_typename<int8>()) return box_value(int8());
            else if (m_Id == xaml_typename<uint16>()) return box_value(uint16());
            else if (m_Id == xaml_typename<int16>()) return box_value(int16());
            else if (m_Id == xaml_typename<uint32>()) return box_value(uint32());
            else if (m_Id == xaml_typename<int32>()) return box_value(int32());
            else if (m_Id == xaml_typename<uint64>()) return box_value(uint64());
            else if (m_Id == xaml_typename<int64>()) return box_value(int64());
            else if (m_Id == xaml_typename<float>()) return box_value(float());
            else if (m_Id == xaml_typename<double>()) return box_value(double());
            else if (m_Id == xaml_typename<hstring>()) return box_value(hstring());
            else if (m_Id == xaml_typename<guid>()) return box_value(guid());
            else if (m_Id == xaml_typename<Point>()) return box_value(Point());
            else if (m_Id == xaml_typename<Size>()) return box_value(Size());
            else if (m_Id == xaml_typename<Rect>()) return box_value(Rect());
            else if (m_Id == xaml_typename<DateTime>()) return box_value(DateTime());
            else if (m_Id == xaml_typename<TimeSpan>()) return box_value(TimeSpan());
            else return nullptr;
        }
        else
        {
            // TODO Implementing creating a value type defined in metadata.
            IInspectable result;
            check_hresult(RoActivateInstance(static_cast<HSTRING>(get_abi(m_Id.Name)), reinterpret_cast<::IInspectable**>(put_abi(result))));
            return result;
        }
    }
    
    winrt::Windows::Foundation::IInspectable TypeInfo::CreateInstance(array_view<IInspectable const> args)
    {
        if (args.empty()) return CreateInstance();
        else if (m_Id.Kind == TypeKind::Primitive)
        {
            if (m_Id == xaml_typename<bool>()) return box_value(bool());
            else if (m_Id == xaml_typename<char16>()) return box_value(char16());
            else if (m_Id == xaml_typename<uint8>()) return box_value(uint8());
            else if (m_Id == xaml_typename<int8>()) return box_value(int8());
            else if (m_Id == xaml_typename<uint16>()) return box_value(uint16());
            else if (m_Id == xaml_typename<int16>()) return box_value(int16());
            else if (m_Id == xaml_typename<uint32>()) return box_value(uint32());
            else if (m_Id == xaml_typename<int32>()) return box_value(int32());
            else if (m_Id == xaml_typename<uint64>()) return box_value(uint64());
            else if (m_Id == xaml_typename<int64>()) return box_value(int64());
            else if (m_Id == xaml_typename<float>()) return box_value(float());
            else if (m_Id == xaml_typename<double>()) return box_value(double());
            else if (m_Id == xaml_typename<hstring>()) return box_value(hstring());
            else if (m_Id == xaml_typename<guid>()) return box_value(guid());
            else if (m_Id == xaml_typename<Point>()) return box_value(Point());
            else if (m_Id == xaml_typename<Size>()) return box_value(Size());
            else if (m_Id == xaml_typename<Rect>()) return box_value(Rect());
            else if (m_Id == xaml_typename<DateTime>()) return box_value(DateTime());
            else if (m_Id == xaml_typename<TimeSpan>()) return box_value(TimeSpan());
            else return nullptr;
        }
        else
        {
            throw hresult_not_implemented();
        }
    }

    OneToolkit::Runtime::TypeInfo TypeInfo::GetTypeInfo(hstring const& qualifiedName)
    {
        TypeName typeId;
        typeId.Name = qualifiedName;
        typeId.Kind = IsTypePrimitive(qualifiedName) ? TypeKind::Primitive : TypeKind::Metadata;
        return OneToolkit::Runtime::TypeInfo(typeId);
    }

    bool TypeInfo::IsTypePrimitive(hstring const& qualifiedName)
    {
        return std::find(qualifiedName.begin(), qualifiedName.end(), L'.') == qualifiedName.end();
    }
}