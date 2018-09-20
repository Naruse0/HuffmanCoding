/**
 *	@mainpage	Huffman Coding.
 *	@details	ハフマン符号化を行うアルゴリズムの実装。
 */

#include	<stdio.h>
#include	<string>
#include	<vector>
#include	<map>
#include	<Windows.h>



/**
 *	@brief	定数データ
 */
const char*	INPUT_DATA = "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";
const char* INPUT_DATA_EXAMPLE = "aabcdeafagagfedadhaeedefahhh";



/**
 *	@brief	ハフマン符号の木を作成する際のノードとなるクラス
 */
class CNode
{
public:

	//!	@brief	値の設定
	void	SetValue(const int _value) { m_value = _value; }

	//!	@brief	値の取得
	int		GetValue() const { return m_value; }

	//!	@brief	ノードがツリーに構成されているかを確認
	bool	IsEnable() const { return m_value > 0; }

	//!	@brief	親が存在するかを確認
	bool	IsParentExist() const { return m_pParent != nullptr; }



	//!	@brief	ノードのセッター
	void	SetParent(CNode* _pNode) { m_pParent = _pNode; }
	void	SetLeft(CNode* _pNode) { m_pLeft = _pNode; }
	void	SetRight(CNode* _pNode) { m_pRight = _pNode; }

	//!	@brief	ノードのゲッター
	CNode*	GetParent() const { return m_pParent; }
	CNode*	GetLeft() const { return m_pParent; }
	CNode*	GetRight() const { return m_pParent; }

private:

	int		m_value;		//!	@brief	値

	CNode*	m_pParent;		//!	@brief	親の参照
	CNode*	m_pLeft;		//!	@brief	左の子の参照
	CNode*	m_pRight;		//!	@brief	右の子の参照

public:
	CNode(const int _value) : 
		m_value(_value),
		m_pParent(nullptr), 
		m_pLeft(nullptr),
		m_pRight(nullptr)
	{

	}

	CNode() :
		CNode(-1)	// 不正な値を初期値に設定
	{

	}

	~CNode()
	{

	}
};



/**
 *	@brief	ハフマン符号化を行うクラス
 */
class CHuffman
{
public:

	//!	@brief	データの圧縮を行う
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

	// 表示関係
	//!	@brief	元データの表示
	void	PrintOriginalData() const
	{
		printf("---- Original Data ----\n");
		printf("%s\n", m_originalData.data());
	}
	
	//!	@brief	データの出現頻度の表示
	void	PrintDataFreq() const
	{
		printf("---- Data Frequency (Sum = %d)----\n", m_dataCount);
		for (const auto& itr : m_dataFreq)
		{	
			printf("%c : %3d\n", itr.first, itr.second);
		}
	}

private:

	std::string			m_originalData;		// 元データ
	int					m_dataCount;		// データ数
	std::map<char, int>	m_dataFreq;			// データの出現頻度

	std::vector<CNode>	m_nodes;			// 全てのノードを配列で保持する



	//!	@brief	データの出現頻度を数える
	void	CountDataFreq()
	{
		// 念のため初期化
		m_dataFreq.clear();

		// データ数を取得
		m_dataCount = m_originalData.size();

		// 全ての文字列を走査
		for (const auto& datum : m_originalData)
		{
			// 文字をキーにして数を数える
			m_dataFreq[datum]++;
		}
	}

	//!	@brief	ハフマンツリーを構成する
	void	MakeHuffmanTree()
	{
		// 全てのノード数を求める
		int nodeCount = m_dataCount + m_dataCount - 1;

		// 初期化
		m_nodes.clear();
		m_nodes.reserve(nodeCount);

		// データをノードに追加
		for (const auto& datum : m_dataFreq)
		{
			m_nodes.push_back(datum.second);
		}

		// ツリーを構築する
		MakePartialTree();
	}

	//!	@brief	根（親がいない）ノードのうち最小の値と2番目に最小のノード（次に葉になるノード）を取得する
	//!	@param[out]		_pMinNode		最小の値を持つノードを受け取る。
	//!	@param[out]		_pMin2Node		2番目に最小のノードを受け取る。
	//!	@note			引数がnullptrの場合はノードが存在しない。
	void	SearchLeafNode(CNode*& _pMinNode, CNode*& _pMin2Node)
	{
		// ---- 最小の値を探索 ----
		int minValue = INT_MAX;

		// 全てのノードを走査
		for (auto& node : m_nodes)
		{
			// 有効なノードのうち親がいないノードを対象にする
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// 最小の値を取得
			if (node.GetValue() < minValue)
			{
				minValue = node.GetValue();
				_pMinNode = &node;			// 参照を保持
			}
		}

		// ---- 2番目に小さい値を探索 ----
		int min2Value = INT_MAX;

		// 全てのノードを走査
		for (auto& node : m_nodes)
		{
			// 有効なノードのうち親がいないノードを対象にする
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// Minが検索成功時に同じノードを探索しないようにする。
			if (_pMinNode != nullptr && &node == _pMinNode) { continue; }

			// 最小の値以上かつ小さい値
			if (node.GetValue() >= minValue && node.GetValue() < min2Value)
			{
				min2Value = node.GetValue();
				_pMin2Node = &node;			// 参照を保持
			}
		}
	}

	//!	@brief	部分木を作成する
	void	MakePartialTree()
	{
		// 参照の取得先
		CNode*	pMinNode	= nullptr;
		CNode*	pMin2Node	= nullptr;

		SearchLeafNode(pMinNode, pMin2Node);

		// 探索成功時
		if (pMinNode != nullptr && pMin2Node != nullptr)
		{
			printf("Add.[Min[%d], Min2[%d]]\n", pMinNode->GetValue(), pMin2Node->GetValue());

			// 追加
			AddNode(pMinNode->GetValue() + pMin2Node->GetValue(), pMinNode, pMin2Node);

			// 再帰的に実行
			MakePartialTree();
			return;
		}
		// 探索失敗
		else
		{
			printf("End.\n");

			// 終了
			return;
		}
	}

	//!	@brief	新しいノードを追加する。
	void	AddNode(const int _value, CNode* _pMinNode, CNode* _pMin2Node)
	{
		bool isAddSuccess = false;

		m_nodes.push_back(_value);
		m_nodes.back().SetLeft(_pMinNode);
		m_nodes.back().SetRight(_pMin2Node);

		_pMinNode->SetParent(&m_nodes.back());
		_pMin2Node->SetParent(&m_nodes.back());

		//// 未設定のノードを探索
		//for (auto& node : m_nodes)
		//{
		//	// 設定済みのノードは無視
		//	if (!node.IsEnable()) { continue; }

		//	// 空いているノードを見つけたら追加する
		//	node.SetValue(_value);
		//	node.SetLeft(_pMinNode);
		//	node.SetRight(_pMin2Node);

		//	// 子の親を設定する
		//	_pMinNode->SetParent(&node);
		//	_pMin2Node->SetParent(&node);

		//	// 成功フラグを設定
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
 *	@brief	エントリポイント
 */
int main()
{
	CHuffman	Huffman(INPUT_DATA_EXAMPLE);
	Huffman.PrintOriginalData();
	Huffman.CompressData();
	Huffman.PrintDataFreq();

	// 終了処理
	printf("Push Enter Key >> ");
	rewind(stdin);
	getchar();
	return 0;
}