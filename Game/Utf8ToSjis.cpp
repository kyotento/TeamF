#include "stdafx.h"
#include "Utf8ToSjis.h"

std::string UTF8toSjis( std::string srcUTF8 ){
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar( CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0 );

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar( CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode );

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte( CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL );

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte( CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL );

	std::string strSJis( bufShiftJis );

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}

std::wstring utf8toWide(const std::string& src ){
	auto const dest_size = ::MultiByteToWideChar( CP_UTF8, 0U, src.data(), -1, nullptr, 0U );
	std::vector<wchar_t> dest( dest_size, L'\0' );
	if( ::MultiByteToWideChar( CP_UTF8, 0U, src.data(), -1, dest.data(), dest.size() ) == 0 ){
		throw std::system_error{ static_cast<int>( ::GetLastError() ), std::system_category() };
	}
	dest.resize( std::char_traits<wchar_t>::length( dest.data() ) );
	dest.shrink_to_fit();
	return std::wstring( dest.begin(), dest.end() );
}