#pragma once

#include	"HuffmanData.h"

#include	<string>
#include	<map>

//!	@brief	圧縮されたデータを複合するクラス。
class CHuffmanDecoder
{
public:

	void Decode(const char* _importFilePath);

private:

	std::string					m_originalDataText;
	std::string					m_compressDataText;
	std::map<char, std::string>	m_dataSigns;

	SHuffmanData				m_importData;

	//----------------------------------------------------------
	// 複合系
	//----------------------------------------------------------

	//!	@brief	出力されたファイルをインポートする
	void Import(const char* _importFilePath);

	//!	@brief	符号ごとのデータを作成
	void MakeSignData();

	//!	@brief	圧縮データのビット列を文字列として複合する
	void MakeCompressDataText();

	//!	@brief	元データを復元する
	void MakeOriginalData();

	//----------------------------------------------------------
	// 表示系
	//----------------------------------------------------------

	void PrintSignData();

	void PrintCompressData();

	void PrintOriginalData();

public:

	CHuffmanDecoder();
	~CHuffmanDecoder();

};
