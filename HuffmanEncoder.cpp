#include	"HuffmanEncoder.h"

#include	<stdio.h>
#include	<fstream>

CHuffmanEncoder::CHuffmanEncoder(const char* _data) : 
	m_originalDataText(_data)
{

}

void CHuffmanEncoder::Encode(const char* _exportFilePath)
{
	printf("---------------\n");
	printf("Begin encoding.\n");
	printf("---------------\n\n");

	PrintOriginalData();

	CountDataFreq();
	PrintDataFreq();

	CreateHuffmanTree();
	PrintTree();

	CreateSignData();
	PrintSignData();

	CreateCompressData();
	PrintCompressData();

	Export(_exportFilePath);
}

void CHuffmanEncoder::CountDataFreq()
{
	// ���������擾
	m_originalDataTextCount = m_originalDataText.size();

	// �S�Ă̕���������
	for (const auto& datum : m_originalDataText)
	{
		// �������L�[�ɂ��Đ�����
		m_dataFreqs[datum]++;
	}
}

void CHuffmanEncoder::CreateHuffmanTree()
{
	// �S�Ẵm�[�h�������߂�
	size_t leafCount = m_dataFreqs.size();
	size_t nodeCount = leafCount + leafCount - 1;

	// �m�[�h�̃��������m��
	m_nodes.reserve(nodeCount);

	// �f�[�^���m�[�h�ɒǉ�
	for (const auto& datum : m_dataFreqs)
	{
		m_nodes.push_back(SNode(datum.second, datum.first));
	}

	// �����؂��ċA�I�ɍ쐬���A�؂��\������
	MakePartialTree();
}

void CHuffmanEncoder::SearchLeafNode(SNode*& _pOutLeft, SNode*& _pOutRight)
{
	int	leftValue = INT_MAX;

	// ---- �ŏ��̒l��T�� ----
	for (auto& node : m_nodes)
	{
		// �L���ȃm�[�h�̂����e�����Ȃ��m�[�h��Ώۂɂ���
		if (!node.IsEnable() || node.IsParentExist()) { continue; }

		// �ŏ��̒l���擾
		if (node.value <= leftValue)
		{
			bool isFirstFound	= _pOutLeft == nullptr;	// �ŏ��̔����t���O
			bool isNodeHasChild = node.pLeft != nullptr || node.pRight != nullptr;
			bool isSmaller		= node.value < leftValue;

			// �q�������Ă��Ȃ��ق���ێ�����
			if (isFirstFound || isSmaller || isNodeHasChild)
			{
				leftValue = node.value;
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
		if (node.value <= rightValue)
		{
			bool isFirstFound = _pOutRight == nullptr;	// �ŏ��̔����t���O
			bool isNodeHasChild = node.pLeft != nullptr || node.pRight != nullptr;
			bool isSmaller = node.value < rightValue;

			// �q�������Ă��Ȃ��ق���ێ�����
			if (isFirstFound || isSmaller || isNodeHasChild)
			{
				rightValue = node.value;
				_pOutRight = &node;			// �Q�Ƃ�ێ�
			}
		}
	}
	
	// ---- �q�������Ă��邩�ɂ�����ւ� ----
	if (_pOutLeft != nullptr && _pOutRight != nullptr)
	{
		// �q�������Ă���t���O
		bool isMinHasChild = _pOutLeft->pLeft != nullptr || _pOutLeft->pLeft != nullptr;
		bool isMin2HasChild = _pOutRight->pLeft != nullptr || _pOutRight->pLeft != nullptr;

		// Min2�������q�������Ă���Ƃ���Min������ւ���
		if (isMin2HasChild && !isMinHasChild)
		{
			SNode* workNode = _pOutLeft;
			_pOutLeft = _pOutRight;
			_pOutRight = workNode;
		}
	}
}

void CHuffmanEncoder::MakePartialTree()
{
	// �Q�Ƃ̎擾��
	SNode*	pLeftNode = nullptr;
	SNode*	pRightNode = nullptr;

	SearchLeafNode(pLeftNode, pRightNode);

	// �T��������
	if (pLeftNode != nullptr && pRightNode != nullptr)
	{
		// �ǉ�
		AddNode(pLeftNode->value + pRightNode->value, pLeftNode, pRightNode);

		// �ċA�I�Ɏ��s
		MakePartialTree();
		return;
	}
	// �T�����s
	else
	{
		// �I��
		return;
	}
}

void CHuffmanEncoder::AddNode(const int _value, SNode* _pLeft, SNode* _pRight)
{
	m_nodes.push_back(_value);

	m_nodes.back().pLeft = _pLeft;
	m_nodes.back().pRight = _pRight;

	_pLeft->pParent = &m_nodes.back();
	_pRight->pParent = &m_nodes.back();
}

void CHuffmanEncoder::CreateSignData()
{
	// ���[�g�m�[�h���擾����
	SNode*	pRootNode = nullptr;

	// �m�[�h�̑��݂��m�F
	if (m_nodes.size() > 0)
	{
		pRootNode = m_nodes.front().GetRoot();
	}

	// Root���Ȃ��ꍇ�i�c���[���Ȃ��ꍇ�j�͉������Ȃ��B
	if (pRootNode == nullptr) { return; }

	// �L�[���Ƃ̕������쐬
	FindAndMakeSignData(pRootNode);
}

void CHuffmanEncoder::FindAndMakeSignData(SNode* _pNode, std::string _signString)
{
	// �q�m�[�h���擾
	SNode*	pLeftNode = _pNode->pLeft;
	SNode*	pRightNode = _pNode->pRight;

	// �q�m�[�h�����݂��Ȃ��ꍇ
	if (pLeftNode == nullptr && pRightNode == nullptr)
	{
		// �L�[�ɕ�����o�^
		m_dataSigns[_pNode->key].assign(_signString);

		// �T�����I��
		return;
	}

	// �c���[�̍��������ǂ�
	if (pLeftNode != nullptr)
	{
		// ���̃m�[�h��
		FindAndMakeSignData(pLeftNode, _signString + LEFT_NODE_SIGN);
	}

	// �c���[�̉E���̃m�[�h�����ǂ�
	if (pRightNode != nullptr)
	{
		// ���̃m�[�h��
		FindAndMakeSignData(pRightNode, _signString + RIGHT_NODE_SIGN);
	}
}

void CHuffmanEncoder::CreateCompressData()
{
	// ---- �r�b�g��𕶎���Ƃ��č쐬 ----

	// ������
	m_compressDataText = "";

	// �����𕄍�������������ɒu��������
	for (const auto& datum : m_originalDataText)
	{
		m_compressDataText += m_dataSigns[datum];
	}


	// ---- �쐬������������r�b�g��ɒ��� ----

	// ������
	m_compressBitDataBits.clear();

	// �r�b�g��ɒu���Ȃ���
	bool	isProcEnd = false;
	size_t	idxData = 0;
	while (!isProcEnd)
	{
		// �z���ǉ�
		m_compressBitDataBits.push_back(DataBitSet());
		auto& curBitset = m_compressBitDataBits.back();

		// �S�Ẵr�b�g�ɑ΂��Ēl���Z�b�g����
		for (int iBit = BIT_SET_DIGIT - 1; iBit >= 0; --iBit)
		{
			bool value = m_compressDataText[idxData] != '0';
			curBitset.set(iBit, value);

			// ���̕������
			idxData++;

			// ���̕����񂪂Ȃ��ꍇ
			if (idxData >= m_compressDataText.size())
			{
				isProcEnd = true;
				break;
			}
		}
	}
}	  

void CHuffmanEncoder::Export(const char* _exportFilePath)
{
	// �o�̓f�[�^�̍쐬
	SHuffmanData	exportData;
	exportData.header.dataCount = m_originalDataTextCount;
	exportData.header.signCount = m_dataSigns.size();
	exportData.header.bitsetCount = m_compressBitDataBits.size();

	exportData.CopyData(m_dataSigns, m_compressBitDataBits);

	// �t�@�C���I�[�v��
	std::ofstream	file(_exportFilePath, std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		printf("Error : Failed to open specified file.\n");
		return;
	}

	// �w�b�_�̏�������
	file.write((char*)&exportData.header, sizeof(exportData.header));

	// �R���e���c�̏�������
	file.write(
		(char*)exportData.content.pSigns, 
		sizeof(SHuffmanData::SContent::SSignData) * exportData.header.signCount);
	file.write(
		(char*)exportData.content.pBitsets,
		sizeof(unsigned char) * exportData.header.bitsetCount);

	// �t�@�C���N���[�Y
	file.close();
}

void CHuffmanEncoder::PrintOriginalData() const
{
	printf("Original Data.\n");
	printf("  %s\n", m_originalDataText.data());
	printf("\n");
}

void CHuffmanEncoder::PrintDataFreq() const
{
	printf("OriginalDataTextCount : %3d\n", m_originalDataTextCount);
	printf("DataFrequency\n");
	for (const auto& datum : m_dataFreqs)
	{
		printf("  [%c] : %3d\n", datum.first, datum.second);
	}
	printf("\n");
}

void CHuffmanEncoder::PrintTree()
{
	printf("Huffman Tree.\n");
	PrintTreeRecursivly(m_nodes.front().GetRoot());
	printf("\n");
}

void CHuffmanEncoder::PrintTreeRecursivly(SNode* _pNode, int _depth) const
{
	if (_pNode == nullptr) { return; }

	for (int i = 0; i < _depth + 1; ++i)
	{
		printf("  ");
	}

	printf("[Data : %3d, Key : %3c]\n", _pNode->value, _pNode->key);

	_depth++;
	PrintTreeRecursivly(_pNode->pLeft, _depth);
	PrintTreeRecursivly(_pNode->pRight, _depth);
}

void CHuffmanEncoder::PrintSignData() const
{
	printf("Sign Data.\n");
	for (const auto& datum : m_dataSigns)
	{
		printf("  [%c] : %s\n", datum.first, datum.second.data());
	}
	printf("\n");
}

void CHuffmanEncoder::PrintCompressData() const
{
	printf("Compress Data.\n");
	int i = 0;
	for (const auto& bitset : m_compressBitDataBits)
	{
		printf("  [%2d] : %s\n", i, bitset.to_string().data());
		i++;
	}
	printf("\n");
}