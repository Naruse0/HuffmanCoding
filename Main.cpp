/**
 *	@mainpage	Huffman Coding.
 *	@details	�n�t�}�����������s���A���S���Y���̎����B
 */

#include	<stdio.h>
#include	<string>
#include	<vector>
#include	<map>



/**
 *	@brief	�萔�f�[�^
 */
const char*	INPUT_DATA = "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";



/**
 *	@brief	�n�t�}�����������s���N���X
 */
class CHuffman
{
public:

	//!	@brief	�f�[�^�̈��k���s��
	void	CompressData()
	{
		printf("---- Commpress Data ----\n");
		CountDataFreq();
	}

	// �\���֌W
	//!	@brief	���f�[�^�̕\��
	void	PrintOriginalData() const
	{
		printf("---- Original Data ----\n");
		printf("%s\n", m_originalData.data());
	}
	
	//!	@brief	�f�[�^�̏o���p�x�̕\��
	void	PrintDataFreq() const
	{
		printf("---- Data Frequency ----\n");
		for (const auto& itr : m_dataFreq)
		{
			printf("%c : %3d\n", itr.first, itr.second);
		}
	}

private:

	std::string			m_originalData;		// ���f�[�^
	std::map<char, int>	m_dataFreq;			// �f�[�^�̏o���p�x

	//!	@brief	�f�[�^�̏o���p�x�𐔂���
	void	CountDataFreq()
	{
		// �O�̂��ߏ�����
		m_dataFreq.clear();

		// �S�Ă̕�����𑖍�
		for (const auto& datum : m_originalData)
		{
			// �������L�[�ɂ��Đ��𐔂���
			m_dataFreq[datum]++;
		}
	}

public:

	CHuffman(const char* _data) : 
		m_originalData(_data)
	{

	}

	~CHuffman()
	{

	}
};



/** 
 *	@brief	�G���g���|�C���g
 */
int main()
{
	CHuffman	Huffman(INPUT_DATA);
	Huffman.PrintOriginalData();
	Huffman.CompressData();

	Huffman.PrintDataFreq();

	// �I������
	printf("Push any key >> ");
	rewind(stdin);
	getchar();
	return 0;
}