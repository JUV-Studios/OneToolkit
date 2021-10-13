﻿#include "pch.h"#include "TypeDetails.h"#include "MetadataFile.h"#include "Runtime.TypeDetails.g.cpp"#include <roapi.h>namespace winrt::OneToolkit::Runtime::implementation{	using namespace juv;	using namespace Windows::Foundation;	using namespace Windows::Foundation::Collections;	using namespace Windows::UI::Xaml::Interop;	IVector<CustomTypeActivationHandler> m_CustomActivators = multi_threaded_vector<CustomTypeActivationHandler>();	TypeDetails::TypeDetails(TypeName typeId) : m_Id(typeId)	{		std::wstring_view typeNameView = m_Id.Name;		m_Name = hstring(typeNameView.substr(typeNameView.find_last_of(L'.') + 1));		m_Namespace = hstring(typeNameView.substr(0, typeNameView.find_last_of(L'.') - 1));	}	hstring TypeDetails::QualifiedName() const noexcept	{		return m_Id.Name;	}	OneToolkit::Runtime::MetadataFile TypeDetails::Metadata()	{		if (m_Id.Kind == TypeKind::Metadata)		{			if (!m_Metadata) m_Metadata = make<MetadataFile>(m_Id);		}		return m_Metadata;	}	TypeKind TypeDetails::Kind() const noexcept	{		return m_Id.Kind;	}	uint64_t TypeDetails::BaseSize()	{		if (m_Id == xaml_typename<bool>()) return sizeof(bool);		else if (m_Id == xaml_typename<char16>()) return sizeof(char16);		else if (m_Id == xaml_typename<uint8>()) return sizeof(uint8);		else if (m_Id == xaml_typename<int8>()) return sizeof(int8);		else if (m_Id == xaml_typename<uint16>()) return sizeof(uint16);		else if (m_Id == xaml_typename<int16>()) return sizeof(int16);		else if (m_Id == xaml_typename<uint32>()) return sizeof(uint32);		else if (m_Id == xaml_typename<int32>()) return sizeof(int32);		else if (m_Id == xaml_typename<uint64>()) return sizeof(uint64);		else if (m_Id == xaml_typename<int64>()) return sizeof(int64);		else if (m_Id == xaml_typename<float>()) return sizeof(float);		else if (m_Id == xaml_typename<double>()) return sizeof(double);		else if (m_Id == xaml_typename<hstring>()) return sizeof(hstring);		else if (m_Id == xaml_typename<guid>()) return sizeof(guid);		else if (m_Id == xaml_typename<Point>()) return sizeof(Point);		else if (m_Id == xaml_typename<Size>()) return sizeof(Size);		else if (m_Id == xaml_typename<Rect>()) return sizeof(Rect);		else if (m_Id == xaml_typename<DateTime>()) return sizeof(DateTime);		else if (m_Id == xaml_typename<TimeSpan>()) return sizeof(TimeSpan);		else return sizeof(size_t); // TODO Calculate type's size from metadata if it's a value type.	}	IInspectable TypeDetails::CreateInstance()	{		if (m_Id == xaml_typename<bool>()) return box_value(bool());		else if (m_Id == xaml_typename<char16>()) return box_value(char16());		else if (m_Id == xaml_typename<uint8>()) return box_value(uint8());		else if (m_Id == xaml_typename<int8>()) return box_value(int8());		else if (m_Id == xaml_typename<uint16>()) return box_value(uint16());		else if (m_Id == xaml_typename<int16>()) return box_value(int16());		else if (m_Id == xaml_typename<uint32>()) return box_value(uint32());		else if (m_Id == xaml_typename<int32>()) return box_value(int32());		else if (m_Id == xaml_typename<uint64>()) return box_value(uint64());		else if (m_Id == xaml_typename<int64>()) return box_value(int64());		else if (m_Id == xaml_typename<float>()) return box_value(float());		else if (m_Id == xaml_typename<double>()) return box_value(double());		else if (m_Id == xaml_typename<hstring>()) return box_value(hstring());		else if (m_Id == xaml_typename<guid>()) return box_value(guid());		else if (m_Id == xaml_typename<Point>()) return box_value(Point());		else if (m_Id == xaml_typename<Size>()) return box_value(Size());		else if (m_Id == xaml_typename<Rect>()) return box_value(Rect());		else if (m_Id == xaml_typename<DateTime>()) return box_value(DateTime());		else if (m_Id == xaml_typename<TimeSpan>()) return box_value(TimeSpan());		else		{			// TODO Implementing creating a value type defined in metadata.			IInspectable result;			if (RoActivateInstance(static_cast<HSTRING>(get_abi(m_Id.Name)), reinterpret_cast<::IInspectable**>(put_abi(result))) != 0)			{				for (auto const& customActivator : m_CustomActivators)				{					result = customActivator(*this);					if (result) break;				}			}			return result;		}	}	IVector<CustomTypeActivationHandler> TypeDetails::CustomActivators() noexcept	{		return m_CustomActivators;	}	OneToolkit::Runtime::TypeDetails TypeDetails::GetTypeDetails(IInspectable value)	{		if (value)		{			TypeName typeId;			typeId.Name = get_class_name(value);			typeId.Kind = std::find(typeId.Name.begin(), typeId.Name.end(), L'.') == typeId.Name.end() ? TypeKind::Primitive : TypeKind::Metadata;			return OneToolkit::Runtime::TypeDetails(typeId);		}		return nullptr;	}}