#pragma once

//! @brief utf8のstringをsjisのstringに変換する。
std::string UTF8toSjis( std::string srcUTF8 );

//! @brief utf8のstringをwstringに変換する。
std::wstring utf8toWide(const std::string& src );