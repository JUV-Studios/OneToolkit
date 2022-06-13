#pragma once

struct VS_FIXEDFILEINFO
{
	unsigned long dwSignature;
	unsigned long dwStrucVersion;
	unsigned long dwFileVersionMS;
	unsigned long dwFileVersionLS;
	unsigned long dwProductVersionMS;
	unsigned long dwProductVersionLS;
	unsigned long dwFileFlagsMask;
	unsigned long dwFileFlags;
	unsigned long dwFileOS;
	unsigned long dwFileType;
	unsigned long dwFileSubtype;
	unsigned long dwFileDateMS;
	unsigned long dwFileDateLS;
};