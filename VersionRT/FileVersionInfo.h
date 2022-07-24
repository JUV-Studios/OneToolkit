#pragma once
#define DeclareStringInfoProperty(Name) property Platform::String^ Name \
{\
	Platform::String^ get()\
	{\
		return m_StringInfo[static_cast<uint8>(StringInfoField::Name)]; \
	}\
}

namespace VersionRT
{
	struct VS_FIXEDFILEINFO
	{
		uint32 dwSignature;
		uint32 dwStrucVersion;
		uint32 dwFileVersionMS;
		uint32 dwFileVersionLS;
		uint32 dwProductVersionMS;
		uint32 dwProductVersionLS;
		uint32 dwFileFlagsMask;
		uint32 dwFileFlags;
		uint32 dwFileOS;
		uint32 dwFileType;
		uint32 dwFileSubtype;
		uint32 dwFileDateMS;
		uint32 dwFileDateLS;
	};

	enum class StringInfoField : uint8
	{
		Comments,
		CompanyName,
		FileDescription,
		FileVersion,
		InternalName,
		LegalCopyright,
		LegalTrademarks,
		OriginalFilename,
		PrivateBuild,
		ProductName,
		ProductVersion,
		SpecialBuild
	};

	public ref class FileVersionInfo sealed
	{
	public:
		static FileVersionInfo^ GetVersionInfo(Platform::String^ fileName);

		DeclareStringInfoProperty(Comments);
		DeclareStringInfoProperty(CompanyName);
		DeclareStringInfoProperty(FileDescription);
		DeclareStringInfoProperty(FileVersion);
		DeclareStringInfoProperty(InternalName);
		DeclareStringInfoProperty(LegalCopyright);
		DeclareStringInfoProperty(LegalTrademarks);
		DeclareStringInfoProperty(OriginalFilename);
		DeclareStringInfoProperty(PrivateBuild);
		DeclareStringInfoProperty(ProductName);
		DeclareStringInfoProperty(ProductVersion);
		DeclareStringInfoProperty(SpecialBuild);
	private:
		VS_FIXEDFILEINFO m_FixedInfo;
		Platform::String^ m_StringInfo[12];
	};
}