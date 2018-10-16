#include	"HuffmanDecoder.h"

#include	<fstream>
#include	<stdio.h>

CHuffmanDecoder::CHuffmanDecoder()
{

}

CHuffmanDecoder::~CHuffmanDecoder()
{

}

void CHuffmanDecoder::Decode(const char* _importFilePath)
{
	printf("---------------\n");
	printf("Begin decoding.\n");
	printf("---------------\n\n");

	Import(_importFilePath);

	MakeSignData();
	PrintSignData();

	MakeCompressDataText();
	PrintCompressData();

	MakeOriginalData();
	PrintOriginalData();
}

void CHuffmanDecoder::Import(const char* _importFilePath)
{
	// ファイルオープン
	std::ifstream	file(_importFilePath, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		printf("Error : Failed to open specifyed file.\n");
		return;
	}

	// ヘッダの読み込み
	file.read((char*)&m_importData.header, sizeof(m_importData.header));

	// 読み込み先のメモリを確保
	m_importData.AllocContent(m_importData.header);

	// コンテントの読み込み
	file.read(
		(char*)m_importData.content.pSigns,
		sizeof(SHuffmanData::SContent::SSignData) * m_importData.header.signCount);
	file.read(
		(char*)m_importData.content.pBitsets,
		sizeof(unsigned char) * m_importData.header.bitsetCount);

	// ファイルクローズ
	file.close();
}

void CHuffmanDecoder::MakeSignData()
{
	// データごとの符号を求める
	for (size_t i = 0; i < m_importData.header.signCount; ++i)
	{
		char		key = m_importData.content.pSigns[i].key;
		const auto&	bitset = m_importData.content.pSigns[i].sign;

		size_t		iBit = 0;
		std::string	bitsetText = "";
		while (bitset[iBit] != '\0')
		{
			bitset[iBit] != '0' ? bitsetText += "1": bitsetText += "0";
			iBit++;
		}

		m_dataSigns[key] = bitsetText;
	}
}

void CHuffmanDecoder::MakeCompressDataText()
{
	// 初期化
	m_compressDataText = "";

	// ビット列を文字列化する
	for (size_t iBitSet = 0; iBitSet < m_importData.header.bitsetCount; ++iBitSet)
	{
		unsigned char	bitset = m_importData.content.pBitsets[iBitSet];
		for (size_t iBit = 0; iBit < 8; ++iBit)
		{
			(bitset & 0x01) != 0 ? m_compressDataText += "1" : m_compressDataText += "0";
			bitset >>= 1;
		}
	}
}

void CHuffmanDecoder::MakeOriginalData()
{
	int remainingDataCount = m_importData.header.dataCount;	// 未処理のデータ数
	std::string	tmpRemainingBitText = m_compressDataText;	// 未処理のビット列

	// 初期化
	m_originalDataText = "";

	while (remainingDataCount > 0)
	{
		char	expandData = '\0';
		size_t	minStrLen = UINT_MAX;

		// 全ての符号データのうち一致するものを探す
		for (const auto& sign : m_dataSigns)
		{
			// 先頭からではない場合は無視
			size_t offset = tmpRemainingBitText.find(sign.second);
			if (offset != 0) { continue; }

			// 符号ビット数が多い場合は無視
			size_t	strLen = sign.second.length();
			if (strLen > minStrLen) { continue; }

			// 最小を更新
			minStrLen = strLen;

			// 解凍後のデータを取得
			expandData = sign.first;
		}

		// 処理が済んだデータを排除した文字列を取得
		tmpRemainingBitText = tmpRemainingBitText.substr(minStrLen);

		// 解凍後のデータを代入
		m_originalDataText += expandData;

		// 処理済みデータカウントを進める
		remainingDataCount--;
	}
}

void CHuffmanDecoder::PrintSignData()
{
	printf("Sign Data.\n");
	for (const auto& datum : m_dataSigns)
	{
		printf("  [%c] : %s\n", datum.first, datum.second.data());
	}
	printf("\n");
}

void CHuffmanDecoder::PrintCompressData()
{
	printf("Compress Data.\n");

	std::string remainingData = m_compressDataText;
	for(size_t i = 0; i< m_importData.header.bitsetCount; ++i)
	{
		printf("  %s\n", remainingData.substr(0, 8).data());
		remainingData = remainingData.substr(8);
	}
	printf("\n");
}

void CHuffmanDecoder::PrintOriginalData()
{
	printf("Original Data.\n");
	printf("  %s\n", m_originalDataText.data());
	printf("\n");
}
