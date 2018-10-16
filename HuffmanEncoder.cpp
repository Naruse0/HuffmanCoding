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
	// 文字数を取得
	m_originalDataTextCount = m_originalDataText.size();

	// 全ての文字を検索
	for (const auto& datum : m_originalDataText)
	{
		// 文字をキーにして数える
		m_dataFreqs[datum]++;
	}
}

void CHuffmanEncoder::CreateHuffmanTree()
{
	// 全てのノード数を求める
	size_t leafCount = m_dataFreqs.size();
	size_t nodeCount = leafCount + leafCount - 1;

	// ノードのメモリを確保
	m_nodes.reserve(nodeCount);

	// データをノードに追加
	for (const auto& datum : m_dataFreqs)
	{
		m_nodes.push_back(SNode(datum.second, datum.first));
	}

	// 部分木を再帰的に作成し、木を構成する
	MakePartialTree();
}

void CHuffmanEncoder::SearchLeafNode(SNode*& _pOutLeft, SNode*& _pOutRight)
{
	int	leftValue = INT_MAX;

	// ---- 最小の値を探索 ----
	for (auto& node : m_nodes)
	{
		// 有効なノードのうち親がいないノードを対象にする
		if (!node.IsEnable() || node.IsParentExist()) { continue; }

		// 最小の値を取得
		if (node.value <= leftValue)
		{
			bool isFirstFound	= _pOutLeft == nullptr;	// 最初の発見フラグ
			bool isNodeHasChild = node.pLeft != nullptr || node.pRight != nullptr;
			bool isSmaller		= node.value < leftValue;

			// 子を持っていないほうを保持する
			if (isFirstFound || isSmaller || isNodeHasChild)
			{
				leftValue = node.value;
				_pOutLeft = &node;			// 参照を保持
			}
		}
	}

	// ---- 2番目に小さい値を探索 ----
	int rightValue = INT_MAX;

	// 全てのノードを走査
	for (auto& node : m_nodes)
	{
		// 有効なノードのうち親がいないノードを対象にする
		if (!node.IsEnable() || node.IsParentExist()) { continue; }

		// Minが検索成功時に同じノードを探索しないようにする。
		if (_pOutLeft != nullptr && &node == _pOutLeft) { continue; }

		// 最小の値以上かつ小さい値
		if (node.value <= rightValue)
		{
			bool isFirstFound = _pOutRight == nullptr;	// 最初の発見フラグ
			bool isNodeHasChild = node.pLeft != nullptr || node.pRight != nullptr;
			bool isSmaller = node.value < rightValue;

			// 子を持っていないほうを保持する
			if (isFirstFound || isSmaller || isNodeHasChild)
			{
				rightValue = node.value;
				_pOutRight = &node;			// 参照を保持
			}
		}
	}
	
	// ---- 子を持っているかによる入れ替え ----
	if (_pOutLeft != nullptr && _pOutRight != nullptr)
	{
		// 子を持っているフラグ
		bool isMinHasChild = _pOutLeft->pLeft != nullptr || _pOutLeft->pLeft != nullptr;
		bool isMin2HasChild = _pOutRight->pLeft != nullptr || _pOutRight->pLeft != nullptr;

		// Min2だけが子を持っているときはMin側入れ替える
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
	// 参照の取得先
	SNode*	pLeftNode = nullptr;
	SNode*	pRightNode = nullptr;

	SearchLeafNode(pLeftNode, pRightNode);

	// 探索成功時
	if (pLeftNode != nullptr && pRightNode != nullptr)
	{
		// 追加
		AddNode(pLeftNode->value + pRightNode->value, pLeftNode, pRightNode);

		// 再帰的に実行
		MakePartialTree();
		return;
	}
	// 探索失敗
	else
	{
		// 終了
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
	// ルートノードを取得する
	SNode*	pRootNode = nullptr;

	// ノードの存在を確認
	if (m_nodes.size() > 0)
	{
		pRootNode = m_nodes.front().GetRoot();
	}

	// Rootがない場合（ツリーがない場合）は何もしない。
	if (pRootNode == nullptr) { return; }

	// キーごとの符号を作成
	FindAndMakeSignData(pRootNode);
}

void CHuffmanEncoder::FindAndMakeSignData(SNode* _pNode, std::string _signString)
{
	// 子ノードを取得
	SNode*	pLeftNode = _pNode->pLeft;
	SNode*	pRightNode = _pNode->pRight;

	// 子ノードが存在しない場合
	if (pLeftNode == nullptr && pRightNode == nullptr)
	{
		// キーに符号を登録
		m_dataSigns[_pNode->key].assign(_signString);

		// 探索を終了
		return;
	}

	// ツリーの左側をたどる
	if (pLeftNode != nullptr)
	{
		// 次のノードへ
		FindAndMakeSignData(pLeftNode, _signString + LEFT_NODE_SIGN);
	}

	// ツリーの右側のノードをたどる
	if (pRightNode != nullptr)
	{
		// 次のノードへ
		FindAndMakeSignData(pRightNode, _signString + RIGHT_NODE_SIGN);
	}
}

void CHuffmanEncoder::CreateCompressData()
{
	// ---- ビット列を文字列として作成 ----

	// 初期化
	m_compressDataText = "";

	// 文字を符号化した文字列に置き換える
	for (const auto& datum : m_originalDataText)
	{
		m_compressDataText += m_dataSigns[datum];
	}


	// ---- 作成した文字列をビット列に直す ----

	// 初期化
	m_compressBitDataBits.clear();

	// ビット列に置きなおす
	bool	isProcEnd = false;
	size_t	idxData = 0;
	while (!isProcEnd)
	{
		// 配列を追加
		m_compressBitDataBits.push_back(DataBitSet());
		auto& curBitset = m_compressBitDataBits.back();

		// 全てのビットに対して値をセットする
		for (int iBit = BIT_SET_DIGIT - 1; iBit >= 0; --iBit)
		{
			bool value = m_compressDataText[idxData] != '0';
			curBitset.set(iBit, value);

			// 次の文字列へ
			idxData++;

			// 次の文字列がない場合
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
	// 出力データの作成
	SHuffmanData	exportData;
	exportData.header.dataCount = m_originalDataTextCount;
	exportData.header.signCount = m_dataSigns.size();
	exportData.header.bitsetCount = m_compressBitDataBits.size();

	exportData.CopyData(m_dataSigns, m_compressBitDataBits);

	// ファイルオープン
	std::ofstream	file(_exportFilePath, std::ios::out | std::ios::binary);
	if (!file.is_open())
	{
		printf("Error : Failed to open specified file.\n");
		return;
	}

	// ヘッダの書き込み
	file.write((char*)&exportData.header, sizeof(exportData.header));

	// コンテンツの書き込み
	file.write(
		(char*)exportData.content.pSigns, 
		sizeof(SHuffmanData::SContent::SSignData) * exportData.header.signCount);
	file.write(
		(char*)exportData.content.pBitsets,
		sizeof(unsigned char) * exportData.header.bitsetCount);

	// ファイルクローズ
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