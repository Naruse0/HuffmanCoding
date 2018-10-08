#pragma once

#include	<bitset>
#include	<map>
#include	<string>
#include	<vector>

#include	"HuffmanData.h"

//!	@brief	�n�t�}�����������s�����k���A�f�[�^���o�͂���N���X
class CHuffmanEncoder
{
public:

	//!	@brief	���k���o�͂���
	void Encode(const char* _exportFilePath);

private:

	static const size_t	BIT_SET_DIGIT	= 8;	//!	@brief	bitset�̌���
	static const char	LEFT_NODE_SIGN	= '1';	//!	@brief	����������ۂ̍����m�[�h�����ǂ�ۂɒǉ�����̕���
	static const char	RIGHT_NODE_SIGN = '0';	//!	@brief	����������ۂ̉E���m�[�h�����ǂ�ۂɒǉ�����̕���

	//!	@brief	���k��̃r�b�g�f�[�^���i�[����^
	using DataBitSet = std::bitset<BIT_SET_DIGIT>;

	//!	@brief	�n�t�}���c���[���\������m�[�h�\����
	struct SNode
	{
		int		value;		
		char	key;

		SNode*	pParent;
		SNode*	pLeft;
		SNode*	pRight;

		//!	@brief	�m�[�h���c���[�ɍ\������Ă��邩���m�F
		bool	IsEnable() const { return value > 0; }

		//!	@brief	�e�����݂��邩
		bool	IsParentExist() const { return pParent != nullptr; }

		//!	@brief	���m�[�h�̎Q�Ƃ��擾����
		SNode*	GetRoot()
		{
			if (pParent != nullptr)
			{
				return pParent->GetRoot();
			}
			return this;
		}

		//!	@brief	�t�m�[�h���쐬����ۂ̃R���X�g���N�^
		SNode(const int _value, const char _key) :
			value(_value),
			key(_key),
			pParent(nullptr),
			pLeft(nullptr),
			pRight(nullptr)
		{

		}

		//!	@brief	�����m�[�h���쐬����ۂ̃R���X�g���N�^
		SNode(const int _value) :
			SNode(_value, '\0')
		{

		}

		//!	@brief	�����Ȓl��ݒ肷��R���X�g���N�^
		SNode() :
			SNode(-1)
		{

		}
	};

	std::string					m_originalDataText;			//!	@brief	���f�[�^
	std::string					m_compressDataText;			//!	@brief	���k�f�[�^
	std::vector<DataBitSet>		m_compressBitDataBits;		//!	@brief	���k�f�[�^�𕄍��������r�b�g��

	size_t						m_originalDataTextCount;	//!	@brief	���f�[�^�̃f�[�^��
	std::map<char, size_t>		m_dataFreqs;				//!	@brief	�f�[�^���Ƃ̏o���p�x
	std::map<char, std::string>	m_dataSigns;				//!	@brief	�f�[�^���Ƃ̕���

	std::vector<SNode>			m_nodes;					//!	@brief	�n�t�}���c���[�̃m�[�h

	//----------------------------------------------------------
	// �n�t�}���c���[�쐬�֌W
	//----------------------------------------------------------

	//!	@brief	�f�[�^�̏o���p�x�𐔂���
	void CountDataFreq();

	//!	@brief	�n�t�}���c���[���\������
	void CreateHuffmanTree();

	//!	@brief	���i�e�����Ȃ��j�m�[�h�̂����ŏ��̒l��2�Ԗڂɍŏ��̃m�[�h�i���ɗt�ɂȂ�m�[�h�j���擾����
	//!	@param[out]		_pOutLeft		�ŏ��̒l�����m�[�h���󂯎��B
	//!	@param[out]		_pOutRight		2�Ԗڂɍŏ��̃m�[�h���󂯎��B
	//!	@note			������nullptr�̏ꍇ�̓m�[�h�����݂��Ȃ��B
	void SearchLeafNode(SNode*& _pOutLeft, SNode*& _pOutRight);

	//!	@brief	�����؂��쐬����
	void MakePartialTree();

	//!	@brief		�V�����m�[�h��ǉ�����B
	//!	@param[in]	_value		�V�����m�[�h�ɐݒ肷��l
	//!	@param[in]	_pLeft		�V�����m�[�h�̍����̎q�ɂ���m�[�h
	//!	@param[in]	_pRight		�V�����m�[�h�̉E���̎q�ɂ���m�[�h
	void AddNode(const int _value, SNode* _pLeft, SNode* _pRight);

	//----------------------------------------------------------
	// �n�t�}���������֌W
	//----------------------------------------------------------

	//!	@brief	�f�[�^���Ƃ̕������쐬����
	void CreateSignData();

	//!	@brief	�t��T���Ȃ��畄���f�[�^���쐬
	void FindAndMakeSignData(SNode* _pNode, std::string _signString = "");

	//----------------------------------------------------------
	// ���k�E�o��
	//----------------------------------------------------------

	//!	@brief	���f�[�^�̈��k
	void CreateCompressData();

	//!	@brief	���k�f�[�^���t�H�[�}�b�g�ɒ����o��
	void Export(const char* _exportFilePath);

	//----------------------------------------------------------
	// �\���n
	//----------------------------------------------------------

	void PrintOriginalData() const;

	void PrintDataFreq() const;

	void PrintTree();

	void PrintTreeRecursivly(SNode* _pNode, int _depth = 0) const;

	void PrintSignData() const;

	void PrintCompressData() const;

public:

	//!	@brief	���k����f�[�^��ݒ肷��R���X�g���N�^
	CHuffmanEncoder(const char* _data);

	~CHuffmanEncoder()
	{

	}
};