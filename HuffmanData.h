#pragma once

#include	<bitset>
#include	<vector>
#include	<map>
#include	<string>

//!	@brief	出力する際のデータフォーマット
struct SHuffmanData
{
	struct SHeader
	{
		size_t	dataCount;
		size_t	signCount;
		size_t	bitsetCount;
	} header;

	struct SContent
	{
		struct SSignData
		{
			char		key;
			char		sign[19];
		};

		SSignData*		pSigns;
		unsigned char*	pBitsets;
	} content;

	~SHuffmanData()
	{
		if (content.pSigns != nullptr)
		{
			delete[] content.pSigns;
		}
		if (content.pBitsets != nullptr)
		{
			delete[] content.pBitsets;
		}
	}

	//!	@brief	データをコピーする
	void CopyData(
		const std::map<char, std::string>&	_signs,
		const std::vector<std::bitset<8>>&	_data)
	{
		content.pSigns = new SContent::SSignData[_signs.size()];
		int loopCount = 0;
		for (const auto& sign : _signs)
		{
			content.pSigns[loopCount].key = sign.first;
			
			sign.second.copy(content.pSigns[loopCount].sign, sign.second.size());
			content.pSigns[loopCount].sign[sign.second.size()] = '\0';

			loopCount++;
		}

		content.pBitsets = new unsigned char[_data.size()];
		for (size_t iVec = 0; iVec < _data.size(); ++iVec)
		{
			for (int iBit = 0; iBit < 8; ++iBit)
			{
				content.pBitsets[iVec] <<= 1;
				_data[iVec][iBit] == true ? content.pBitsets[iVec] |= 0x01: content.pBitsets[iVec] |= 0x00;
			}
		}
	}

	//!	@brief	読み込み先のメモリを確保
	void AllocContent(SHeader _header)
	{
		content.pBitsets = new unsigned char[_header.bitsetCount];
		content.pSigns = new SContent::SSignData[_header.signCount];
	}
};
