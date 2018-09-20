/**
 *	@mainpage	Huffman Coding.
 *	@details	�n�t�}�����������s���A���S���Y���̎����B
 */

#include	<stdio.h>
#include	<string>
#include	<vector>
#include	<map>
#include	<bitset>
#include	<Windows.h>



/**
 *	@brief	�萔�f�[�^
 */
const char*			INPUT_DATA			= "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";
const char*			INPUT_DATA_EXAMPLE	= "aabcdeafagagfedadhaeedefahhh";
const size_t		BIT_SET_DIGIT		= 8;


/**
 *	@brief	�n�t�}�������̖؂��쐬����ۂ̃m�[�h�ƂȂ�N���X
 */
class CNode
{
public:

	//!	@brief	�l�̐ݒ�
	void	SetValue(const int _value) { m_value = _value; }

	//!	@brief	�L�[�̐ݒ�
	void	SetKey(const char _key) { m_key = _key; }

	//!	@brief	�l�̎擾
	int		GetValue() const { return m_value; }

	//!	@brief	�L�[�̎擾
	char	GetKey() const { return m_key; }

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
	CNode*	GetLeft() const { return m_pLeft; }
	CNode*	GetRight() const { return m_pRight; }
	CNode*	GetRoot() { 
		CNode* pParent = GetParent();
		if (pParent != nullptr)
		{
			return pParent->GetRoot();
		}

		return this;
	}

private:

	int		m_value;		//!	@brief	�l
	char	m_key;			//!	@brief	����������ۂ̃L�[�i�t�ɂ̂ݑ��݂���f�[�^�j

	CNode*	m_pParent;		//!	@brief	�e�̎Q��
	CNode*	m_pLeft;		//!	@brief	���̎q�̎Q��
	CNode*	m_pRight;		//!	@brief	�E�̎q�̎Q��

public:

	CNode(const int _value, const char _key) : 
		m_value(_value),
		m_key(_key),
		m_pParent(nullptr), 
		m_pLeft(nullptr),
		m_pRight(nullptr)
	{

	}

	CNode(const int _value) :
		CNode(_value, '\0')
	{

	}

	CNode() :
		CNode(-1, '\0')	// �s���Ȓl�������l�ɐݒ�
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

		MakeSignedData();
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

	//!	@brief	�L�[���Ƃ̕����������l��\��
	void	PrintSignedData() const
	{
		printf("---- Signed Data ----\n");
		for (const auto& datum : m_dataSign)
		{
			printf("%c : %s\n", datum.first, datum.second.data());
		}
	}

	//!	@brief	�S�Ẵc���[��\��
	void	PrintTree()
	{
		printf("---- Tree Data ----\n");
		PrintTreeData(m_nodes.front().GetRoot());
	}

	//!	@brief		�c���[���ċA�I�ɂ��ׂĕ\��
	//!	@param[in]	_pNode	��ƂȂ�m�[�h�i���̃m�[�h�ȉ��̑S�Ẵm�[�h��\������j
	//!	@param[in]	_depth	�ċA�񐔁i�O������Ăяo�����̓f�t�H���g�̒l���g�p����j
	void	PrintTreeData(CNode* _pNode, int _depth = 0) const
	{
		if (_pNode == nullptr) { return; }

		for (int i = 0; i < _depth; ++i)
		{
			printf("  ");
		}

		printf("[Data : %3d, Key : %3c]\n", _pNode->GetValue(), _pNode->GetKey());

		_depth++;
		PrintTreeData(_pNode->GetLeft(), _depth);
		PrintTreeData(_pNode->GetRight(), _depth);
	}

private:

	static const char	LEFT_NODE_SIGN	= '1';	//!	@brief	����������ۂ̍����m�[�h�����ǂ�ۂɒǉ�����̕���
	static const char	RIGHT_NODE_SIGN = '0';	//!	@brief	����������ۂ̉E���m�[�h�����ǂ�ۂɒǉ�����̕���

	std::string			m_originalData;		//!	@brief	���f�[�^
	int					m_dataCount;		//!	@brief	�f�[�^��
	std::map<char, int>	m_dataFreq;			//!	@brief	�f�[�^�̏o���p�x

	std::vector<CNode>	m_nodes;			//!	@brief	�S�Ẵm�[�h��z��ŕێ�����

	std::map<char, std::string>	m_dataSign;	//!	@brief	�f�[�^���Ƃ̕���


	//----------------------------------------------------------
	// �n�t�}���c���[�̍쐬�֌W
	//----------------------------------------------------------

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
			m_nodes.push_back(CNode(datum.second, datum.first));
		}

		// �c���[���\�z����
		MakePartialTree();
	}

	//!	@brief	���i�e�����Ȃ��j�m�[�h�̂����ŏ��̒l��2�Ԗڂɍŏ��̃m�[�h�i���ɗt�ɂȂ�m�[�h�j���擾����
	//!	@param[out]		_pOutLeft		�ŏ��̒l�����m�[�h���󂯎��B
	//!	@param[out]		_pOutRight		2�Ԗڂɍŏ��̃m�[�h���󂯎��B
	//!	@note			������nullptr�̏ꍇ�̓m�[�h�����݂��Ȃ��B
	void	SearchLeafNode(CNode*& _pOutLeft, CNode*& _pOutRight)
	{
		// ---- �ŏ��̒l��T�� ----
		int leftValue = INT_MAX;

		// �S�Ẵm�[�h�𑖍�
		for (auto& node : m_nodes)
		{
			// �L���ȃm�[�h�̂����e�����Ȃ��m�[�h��Ώۂɂ���
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// �ŏ��̒l���擾
			if (node.GetValue() <= leftValue)
			{
				bool isFirstFound		= _pOutLeft == nullptr;	// �ŏ��̔����t���O
				bool isNodeHasChild		= node.GetLeft() != nullptr || node.GetRight() != nullptr;
				bool isSmaller			= node.GetValue() < leftValue;

				// �q�������Ă��Ȃ��ق���ێ�����
				if (isFirstFound || isSmaller || isNodeHasChild)
				{
					leftValue = node.GetValue();
					_pOutLeft = &node;			// �Q�Ƃ�ێ�
				}
			}
		}

		// ---- 2�Ԗڂɏ������l��T�� ----
		int rightValue = INT_MAX;

		// �S�Ẵm�[�h�𑖍�
		for (auto& node : m_nodes)
		{
			// �L���ȃm�[�h�̂����e�����Ȃ��m�[�h��Ώۂɂ���
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// Min�������������ɓ����m�[�h��T�����Ȃ��悤�ɂ���B
			if (_pOutLeft != nullptr && &node == _pOutLeft) { continue; }

			// �ŏ��̒l�ȏォ�������l
			if (node.GetValue() <= rightValue)
			{
				bool isFirstFound			= _pOutRight == nullptr;	// �ŏ��̔����t���O
				bool isNodeHasChild			= node.GetLeft() != nullptr || node.GetRight() != nullptr;
				bool isSmaller				= node.GetValue() < rightValue;

				// �q�������Ă��Ȃ��ق���ێ�����
				if (isFirstFound || isSmaller || isNodeHasChild)
				{
					rightValue = node.GetValue();
					_pOutRight = &node;			// �Q�Ƃ�ێ�
				}
			}
		}

		// �q�������Ă��邩�ɂ�����ւ�
		if (_pOutLeft != nullptr && _pOutRight != nullptr)
		{
			// �q�������Ă���t���O
			bool isMinHasChild	= _pOutLeft->GetLeft() != nullptr || _pOutLeft->GetLeft() != nullptr;
			bool isMin2HasChild = _pOutRight->GetLeft() != nullptr || _pOutRight->GetLeft() != nullptr;

			// Min2�������q�������Ă���Ƃ���Min������ւ���
			if (isMin2HasChild && !isMinHasChild)
			{
				CNode* workNode = _pOutLeft;
				_pOutLeft = _pOutRight;
				_pOutRight = workNode;
			}
		}
	}

	//!	@brief	�����؂��쐬����
	void	MakePartialTree()
	{
		// �Q�Ƃ̎擾��
		CNode*	pLeftNode	= nullptr;
		CNode*	pRightNode	= nullptr;

		SearchLeafNode(pLeftNode, pRightNode);

		// �T��������
		if (pLeftNode != nullptr && pRightNode != nullptr)
		{
			printf("Add.[Left[%3d, %2c], Right[%3d, %2c], New[%3d]]\n",
				   pLeftNode->GetValue(),
				   pLeftNode->GetKey(),
				   pRightNode->GetValue(), 
				   pRightNode->GetKey(),
				   pLeftNode->GetValue() + pRightNode->GetValue());

			// �ǉ�
			AddNode(pLeftNode->GetValue() + pRightNode->GetValue(), pLeftNode, pRightNode);

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

	//!	@brief		�V�����m�[�h��ǉ�����B
	//!	@param[in]	_value		�V�����m�[�h�ɐݒ肷��l
	//!	@param[in]	_pLeft		�V�����m�[�h�̍����̎q�ɂ���m�[�h
	//!	@param[in]	_pRight		�V�����m�[�h�̉E���̎q�ɂ���m�[�h
	void	AddNode(const int _value, CNode* _pLeft, CNode* _pRight)
	{
		m_nodes.push_back(_value);

		m_nodes.back().SetLeft(_pLeft);
		m_nodes.back().SetRight(_pRight);

		_pLeft->SetParent(&m_nodes.back());
		_pRight->SetParent(&m_nodes.back());
	}


	//----------------------------------------------------------
	// �n�t�}���������֌W
	//----------------------------------------------------------

	//!	@brief	���k��̕������쐬����
	void	MakeSignedData()
	{
		// ���[�g�m�[�h���擾����
		CNode*	pRootNode = nullptr;

		// �m�[�h�̑��݂��m�F
		if (m_nodes.size() > 0)
		{
			pRootNode = m_nodes.front().GetRoot();
		}

		// Root���Ȃ��ꍇ�i�c���[���Ȃ��ꍇ�j�͉������Ȃ��B
		if (pRootNode == nullptr) { return; }

		// �L�[���Ƃ̕������쐬
		FindAndMakeSignedData(pRootNode);
	}
	
	//!	@brief	�t��T���Ȃ���f�[�^���쐬
	void	FindAndMakeSignedData(CNode* _pNode, std::string _signString = "")
	{
		// �q�m�[�h���擾
		CNode*	pLeftNode = _pNode->GetLeft();
		CNode*	pRightNode = _pNode->GetRight();

		// �q�m�[�h�����݂��Ȃ��ꍇ
		if (pLeftNode == nullptr && pRightNode == nullptr)
		{
			// �L�[�ɕ�����o�^
			m_dataSign[_pNode->GetKey()].assign(_signString);

			// �T�����I��
			return;
		}
		
		// �c���[�̍��������ǂ�
		if (pLeftNode != nullptr)
		{
			// ���̃m�[�h��
			FindAndMakeSignedData(pLeftNode, _signString + LEFT_NODE_SIGN);
		}

		// �c���[�̉E���̃m�[�h�����ǂ�
		if (pRightNode != nullptr)
		{
			// ���̃m�[�h��
			FindAndMakeSignedData(pRightNode, _signString + RIGHT_NODE_SIGN);
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
	CHuffman	Huffman(INPUT_DATA_EXAMPLE);
	Huffman.PrintOriginalData();
	Huffman.CompressData();
	Huffman.PrintDataFreq();
	Huffman.PrintSignedData();
	Huffman.PrintTree();

	// �I������
	printf("Push Enter Key >> ");
	rewind(stdin);
	getchar();
	return 0;
}