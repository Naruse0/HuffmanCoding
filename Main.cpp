/**
 *	@mainpage	Huffman Coding.
 *	@details	�n�t�}�����������s���A���S���Y���̎����B
 */

#include	<stdio.h>
#include	<string>
#include	<vector>
#include	<map>
#include	<Windows.h>



/**
 *	@brief	�萔�f�[�^
 */
const char*	INPUT_DATA = "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";
const char* INPUT_DATA_EXAMPLE = "aabcdeafagagfedadhaeedefahhh";



/**
 *	@brief	�n�t�}�������̖؂��쐬����ۂ̃m�[�h�ƂȂ�N���X
 */
class CNode
{
public:

	//!	@brief	�l�̐ݒ�
	void	SetValue(const int _value) { m_value = _value; }

	//!	@brief	�l�̎擾
	int		GetValue() const { return m_value; }

	//!	@brief	�m�[�h���c���[�ɍ\������Ă��邩���m�F
	bool	IsEnable() const { return m_value > 0; }

	//!	@brief	�e�����݂��邩���m�F
	bool	IsParentExist() const { return m_pParent != nullptr; }



	//!	@brief	�m�[�h�̃Z�b�^�[
	void	SetParent(CNode* _pNode) { m_pParent = _pNode; }
	void	SetLeft(CNode* _pNode) { m_pLeft = _pNode; }
	void	SetRight(CNode* _pNode) { m_pRight = _pNode; }

	//!	@brief	�m�[�h�̃Q�b�^�[
	CNode*	GetParent() const { return m_pParent; }
	CNode*	GetLeft() const { return m_pParent; }
	CNode*	GetRight() const { return m_pParent; }

private:

	int		m_value;		//!	@brief	�l

	CNode*	m_pParent;		//!	@brief	�e�̎Q��
	CNode*	m_pLeft;		//!	@brief	���̎q�̎Q��
	CNode*	m_pRight;		//!	@brief	�E�̎q�̎Q��

public:
	CNode(const int _value) : 
		m_value(_value),
		m_pParent(nullptr), 
		m_pLeft(nullptr),
		m_pRight(nullptr)
	{

	}

	CNode() :
		CNode(-1)	// �s���Ȓl�������l�ɐݒ�
	{

	}

	~CNode()
	{

	}
};



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
		MakeHuffmanTree();

		for (auto& node : m_nodes)
		{
			printf("%d\n", node.GetValue());
		}
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
		printf("---- Data Frequency (Sum = %d)----\n", m_dataCount);
		for (const auto& itr : m_dataFreq)
		{	
			printf("%c : %3d\n", itr.first, itr.second);
		}
	}

private:

	std::string			m_originalData;		// ���f�[�^
	int					m_dataCount;		// �f�[�^��
	std::map<char, int>	m_dataFreq;			// �f�[�^�̏o���p�x

	std::vector<CNode>	m_nodes;			// �S�Ẵm�[�h��z��ŕێ�����



	//!	@brief	�f�[�^�̏o���p�x�𐔂���
	void	CountDataFreq()
	{
		// �O�̂��ߏ�����
		m_dataFreq.clear();

		// �f�[�^�����擾
		m_dataCount = m_originalData.size();

		// �S�Ă̕�����𑖍�
		for (const auto& datum : m_originalData)
		{
			// �������L�[�ɂ��Đ��𐔂���
			m_dataFreq[datum]++;
		}
	}

	//!	@brief	�n�t�}���c���[���\������
	void	MakeHuffmanTree()
	{
		// �S�Ẵm�[�h�������߂�
		int nodeCount = m_dataCount + m_dataCount - 1;

		// ������
		m_nodes.clear();
		m_nodes.reserve(nodeCount);

		// �f�[�^���m�[�h�ɒǉ�
		for (const auto& datum : m_dataFreq)
		{
			m_nodes.push_back(datum.second);
		}

		// �c���[���\�z����
		MakePartialTree();
	}

	//!	@brief	���i�e�����Ȃ��j�m�[�h�̂����ŏ��̒l��2�Ԗڂɍŏ��̃m�[�h�i���ɗt�ɂȂ�m�[�h�j���擾����
	//!	@param[out]		_pMinNode		�ŏ��̒l�����m�[�h���󂯎��B
	//!	@param[out]		_pMin2Node		2�Ԗڂɍŏ��̃m�[�h���󂯎��B
	//!	@note			������nullptr�̏ꍇ�̓m�[�h�����݂��Ȃ��B
	void	SearchLeafNode(CNode*& _pMinNode, CNode*& _pMin2Node)
	{
		// ---- �ŏ��̒l��T�� ----
		int minValue = INT_MAX;

		// �S�Ẵm�[�h�𑖍�
		for (auto& node : m_nodes)
		{
			// �L���ȃm�[�h�̂����e�����Ȃ��m�[�h��Ώۂɂ���
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// �ŏ��̒l���擾
			if (node.GetValue() < minValue)
			{
				minValue = node.GetValue();
				_pMinNode = &node;			// �Q�Ƃ�ێ�
			}
		}

		// ---- 2�Ԗڂɏ������l��T�� ----
		int min2Value = INT_MAX;

		// �S�Ẵm�[�h�𑖍�
		for (auto& node : m_nodes)
		{
			// �L���ȃm�[�h�̂����e�����Ȃ��m�[�h��Ώۂɂ���
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// Min�������������ɓ����m�[�h��T�����Ȃ��悤�ɂ���B
			if (_pMinNode != nullptr && &node == _pMinNode) { continue; }

			// �ŏ��̒l�ȏォ�������l
			if (node.GetValue() >= minValue && node.GetValue() < min2Value)
			{
				min2Value = node.GetValue();
				_pMin2Node = &node;			// �Q�Ƃ�ێ�
			}
		}
	}

	//!	@brief	�����؂��쐬����
	void	MakePartialTree()
	{
		// �Q�Ƃ̎擾��
		CNode*	pMinNode	= nullptr;
		CNode*	pMin2Node	= nullptr;

		SearchLeafNode(pMinNode, pMin2Node);

		// �T��������
		if (pMinNode != nullptr && pMin2Node != nullptr)
		{
			printf("Add.[Min[%d], Min2[%d]]\n", pMinNode->GetValue(), pMin2Node->GetValue());

			// �ǉ�
			AddNode(pMinNode->GetValue() + pMin2Node->GetValue(), pMinNode, pMin2Node);

			// �ċA�I�Ɏ��s
			MakePartialTree();
			return;
		}
		// �T�����s
		else
		{
			printf("End.\n");

			// �I��
			return;
		}
	}

	//!	@brief	�V�����m�[�h��ǉ�����B
	void	AddNode(const int _value, CNode* _pMinNode, CNode* _pMin2Node)
	{
		bool isAddSuccess = false;

		m_nodes.push_back(_value);
		m_nodes.back().SetLeft(_pMinNode);
		m_nodes.back().SetRight(_pMin2Node);

		_pMinNode->SetParent(&m_nodes.back());
		_pMin2Node->SetParent(&m_nodes.back());

		//// ���ݒ�̃m�[�h��T��
		//for (auto& node : m_nodes)
		//{
		//	// �ݒ�ς݂̃m�[�h�͖���
		//	if (!node.IsEnable()) { continue; }

		//	// �󂢂Ă���m�[�h����������ǉ�����
		//	node.SetValue(_value);
		//	node.SetLeft(_pMinNode);
		//	node.SetRight(_pMin2Node);

		//	// �q�̐e��ݒ肷��
		//	_pMinNode->SetParent(&node);
		//	_pMin2Node->SetParent(&node);

		//	// �����t���O��ݒ�
		//	isAddSuccess = true;
		//	break;
		//}

		//if (isAddSuccess)
		//{
		//	printf("Add Successed.\n");
		//}
		//else
		//{
		//	printf("Add Failed.\n");
		//}
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
	CHuffman	Huffman(INPUT_DATA_EXAMPLE);
	Huffman.PrintOriginalData();
	Huffman.CompressData();
	Huffman.PrintDataFreq();

	// �I������
	printf("Push Enter Key >> ");
	rewind(stdin);
	getchar();
	return 0;
}