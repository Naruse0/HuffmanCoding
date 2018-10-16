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
	// �t�@�C���I�[�v��
	std::ifstream	file(_importFilePath, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		printf("Error : Failed to open specifyed file.\n");
		return;
	}

	// �w�b�_�̓ǂݍ���
	file.read((char*)&m_importData.header, sizeof(m_importData.header));

	// �ǂݍ��ݐ�̃��������m��
	m_importData.AllocContent(m_importData.header);

	// �R���e���g�̓ǂݍ���
	file.read(
		(char*)m_importData.content.pSigns,
		sizeof(SHuffmanData::SContent::SSignData) * m_importData.header.signCount);
	file.read(
		(char*)m_importData.content.pBitsets,
		sizeof(unsigned char) * m_importData.header.bitsetCount);

	// �t�@�C���N���[�Y
	file.close();
}

void CHuffmanDecoder::MakeSignData()
{
	// �f�[�^���Ƃ̕��������߂�
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
	// ������
	m_compressDataText = "";

	// �r�b�g��𕶎��񉻂���
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
	int remainingDataCount = m_importData.header.dataCount;	// �������̃f�[�^��
	std::string	tmpRemainingBitText = m_compressDataText;	// �������̃r�b�g��

	// ������
	m_originalDataText = "";

	while (remainingDataCount > 0)
	{
		char	expandData = '\0';
		size_t	minStrLen = UINT_MAX;

		// �S�Ă̕����f�[�^�̂�����v������̂�T��
		for (const auto& sign : m_dataSigns)
		{
			// �擪����ł͂Ȃ��ꍇ�͖���
			size_t offset = tmpRemainingBitText.find(sign.second);
			if (offset != 0) { continue; }

			// �����r�b�g���������ꍇ�͖���
			size_t	strLen = sign.second.length();
			if (strLen > minStrLen) { continue; }

			// �ŏ����X�V
			minStrLen = strLen;

			// �𓀌�̃f�[�^���擾
			expandData = sign.first;
		}

		// �������ς񂾃f�[�^��r��������������擾
		tmpRemainingBitText = tmpRemainingBitText.substr(minStrLen);

		// �𓀌�̃f�[�^����
		m_originalDataText += expandData;

		// �����ς݃f�[�^�J�E���g��i�߂�
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
