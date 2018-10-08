#pragma once

#include	"HuffmanData.h"

#include	<string>
#include	<map>

//!	@brief	���k���ꂽ�f�[�^�𕡍�����N���X�B
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
	// �����n
	//----------------------------------------------------------

	//!	@brief	�o�͂��ꂽ�t�@�C�����C���|�[�g����
	void Import(const char* _importFilePath);

	//!	@brief	�������Ƃ̃f�[�^���쐬
	void MakeSignData();

	//!	@brief	���k�f�[�^�̃r�b�g��𕶎���Ƃ��ĕ�������
	void MakeCompressDataText();

	//!	@brief	���f�[�^�𕜌�����
	void MakeOriginalData();

	//----------------------------------------------------------
	// �\���n
	//----------------------------------------------------------

	void PrintSignData();

	void PrintCompressData();

	void PrintOriginalData();

public:

	CHuffmanDecoder();
	~CHuffmanDecoder();

};
