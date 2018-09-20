/**
 *	@mainpage	Huffman Coding.
 *	@details	ハフマン符号化を行うアルゴリズムの実装。
 */

#include	<stdio.h>
#include	<string>
#include	<vector>
#include	<map>
#include	<bitset>
#include	<Windows.h>



/**
 *	@brief	定数データ
 */
const char*			INPUT_DATA			= "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";
const char*			INPUT_DATA_EXAMPLE	= "aabcdeafagagfedadhaeedefahhh";
const size_t		BIT_SET_DIGIT		= 8;


/**
 *	@brief	ハフマン符号の木を作成する際のノードとなるクラス
 */
class CNode
{
public:

	//!	@brief	値の設定
	void	SetValue(const int _value) { m_value = _value; }

	//!	@brief	キーの設定
	void	SetKey(const char _key) { m_key = _key; }

	//!	@brief	値の取得
	int		GetValue() const { return m_value; }

	//!	@brief	キーの取得
	char	GetKey() const { return m_key; }

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

	int		m_value;		//!	@brief	値
	char	m_key;			//!	@brief	符号化する際のキー（葉にのみ存在するデータ）

	CNode*	m_pParent;		//!	@brief	親の参照
	CNode*	m_pLeft;		//!	@brief	左の子の参照
	CNode*	m_pRight;		//!	@brief	右の子の参照

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
		CNode(-1, '\0')	// 不正な値を初期値に設定
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

		MakeSignedData();
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

	//!	@brief	キーごとの符号化した値を表示
	void	PrintSignedData() const
	{
		printf("---- Signed Data ----\n");
		for (const auto& datum : m_dataSign)
		{
			printf("%c : %s\n", datum.first, datum.second.data());
		}
	}

	//!	@brief	全てのツリーを表示
	void	PrintTree()
	{
		printf("---- Tree Data ----\n");
		PrintTreeData(m_nodes.front().GetRoot());
	}

	//!	@brief		ツリーを再帰的にすべて表示
	//!	@param[in]	_pNode	基準となるノード（このノード以下の全てのノードを表示する）
	//!	@param[in]	_depth	再帰回数（外部から呼び出す時はデフォルトの値を使用する）
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

	static const char	LEFT_NODE_SIGN	= '1';	//!	@brief	符号化する際の左側ノードをたどる際に追加するの符号
	static const char	RIGHT_NODE_SIGN = '0';	//!	@brief	符号化する際の右側ノードをたどる際に追加するの符号

	std::string			m_originalData;		//!	@brief	元データ
	int					m_dataCount;		//!	@brief	データ数
	std::map<char, int>	m_dataFreq;			//!	@brief	データの出現頻度

	std::vector<CNode>	m_nodes;			//!	@brief	全てのノードを配列で保持する

	std::map<char, std::string>	m_dataSign;	//!	@brief	データごとの符号


	//----------------------------------------------------------
	// ハフマンツリーの作成関係
	//----------------------------------------------------------

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
			m_nodes.push_back(CNode(datum.second, datum.first));
		}

		// ツリーを構築する
		MakePartialTree();
	}

	//!	@brief	根（親がいない）ノードのうち最小の値と2番目に最小のノード（次に葉になるノード）を取得する
	//!	@param[out]		_pOutLeft		最小の値を持つノードを受け取る。
	//!	@param[out]		_pOutRight		2番目に最小のノードを受け取る。
	//!	@note			引数がnullptrの場合はノードが存在しない。
	void	SearchLeafNode(CNode*& _pOutLeft, CNode*& _pOutRight)
	{
		// ---- 最小の値を探索 ----
		int leftValue = INT_MAX;

		// 全てのノードを走査
		for (auto& node : m_nodes)
		{
			// 有効なノードのうち親がいないノードを対象にする
			if (!node.IsEnable() || node.IsParentExist()) { continue; }

			// 最小の値を取得
			if (node.GetValue() <= leftValue)
			{
				bool isFirstFound		= _pOutLeft == nullptr;	// 最初の発見フラグ
				bool isNodeHasChild		= node.GetLeft() != nullptr || node.GetRight() != nullptr;
				bool isSmaller			= node.GetValue() < leftValue;

				// 子を持っていないほうを保持する
				if (isFirstFound || isSmaller || isNodeHasChild)
				{
					leftValue = node.GetValue();
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
			if (node.GetValue() <= rightValue)
			{
				bool isFirstFound			= _pOutRight == nullptr;	// 最初の発見フラグ
				bool isNodeHasChild			= node.GetLeft() != nullptr || node.GetRight() != nullptr;
				bool isSmaller				= node.GetValue() < rightValue;

				// 子を持っていないほうを保持する
				if (isFirstFound || isSmaller || isNodeHasChild)
				{
					rightValue = node.GetValue();
					_pOutRight = &node;			// 参照を保持
				}
			}
		}

		// 子を持っているかによる入れ替え
		if (_pOutLeft != nullptr && _pOutRight != nullptr)
		{
			// 子を持っているフラグ
			bool isMinHasChild	= _pOutLeft->GetLeft() != nullptr || _pOutLeft->GetLeft() != nullptr;
			bool isMin2HasChild = _pOutRight->GetLeft() != nullptr || _pOutRight->GetLeft() != nullptr;

			// Min2だけが子を持っているときはMin側入れ替える
			if (isMin2HasChild && !isMinHasChild)
			{
				CNode* workNode = _pOutLeft;
				_pOutLeft = _pOutRight;
				_pOutRight = workNode;
			}
		}
	}

	//!	@brief	部分木を作成する
	void	MakePartialTree()
	{
		// 参照の取得先
		CNode*	pLeftNode	= nullptr;
		CNode*	pRightNode	= nullptr;

		SearchLeafNode(pLeftNode, pRightNode);

		// 探索成功時
		if (pLeftNode != nullptr && pRightNode != nullptr)
		{
			printf("Add.[Left[%3d, %2c], Right[%3d, %2c], New[%3d]]\n",
				   pLeftNode->GetValue(),
				   pLeftNode->GetKey(),
				   pRightNode->GetValue(), 
				   pRightNode->GetKey(),
				   pLeftNode->GetValue() + pRightNode->GetValue());

			// 追加
			AddNode(pLeftNode->GetValue() + pRightNode->GetValue(), pLeftNode, pRightNode);

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

	//!	@brief		新しいノードを追加する。
	//!	@param[in]	_value		新しいノードに設定する値
	//!	@param[in]	_pLeft		新しいノードの左側の子にするノード
	//!	@param[in]	_pRight		新しいノードの右側の子にするノード
	void	AddNode(const int _value, CNode* _pLeft, CNode* _pRight)
	{
		m_nodes.push_back(_value);

		m_nodes.back().SetLeft(_pLeft);
		m_nodes.back().SetRight(_pRight);

		_pLeft->SetParent(&m_nodes.back());
		_pRight->SetParent(&m_nodes.back());
	}


	//----------------------------------------------------------
	// ハフマン符号化関係
	//----------------------------------------------------------

	//!	@brief	圧縮後の符号を作成する
	void	MakeSignedData()
	{
		// ルートノードを取得する
		CNode*	pRootNode = nullptr;

		// ノードの存在を確認
		if (m_nodes.size() > 0)
		{
			pRootNode = m_nodes.front().GetRoot();
		}

		// Rootがない場合（ツリーがない場合）は何もしない。
		if (pRootNode == nullptr) { return; }

		// キーごとの符号を作成
		FindAndMakeSignedData(pRootNode);
	}
	
	//!	@brief	葉を探しながらデータを作成
	void	FindAndMakeSignedData(CNode* _pNode, std::string _signString = "")
	{
		// 子ノードを取得
		CNode*	pLeftNode = _pNode->GetLeft();
		CNode*	pRightNode = _pNode->GetRight();

		// 子ノードが存在しない場合
		if (pLeftNode == nullptr && pRightNode == nullptr)
		{
			// キーに符号を登録
			m_dataSign[_pNode->GetKey()].assign(_signString);

			// 探索を終了
			return;
		}
		
		// ツリーの左側をたどる
		if (pLeftNode != nullptr)
		{
			// 次のノードへ
			FindAndMakeSignedData(pLeftNode, _signString + LEFT_NODE_SIGN);
		}

		// ツリーの右側のノードをたどる
		if (pRightNode != nullptr)
		{
			// 次のノードへ
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
 *	@brief	エントリポイント
 */
int main()
{
	CHuffman	Huffman(INPUT_DATA_EXAMPLE);
	Huffman.PrintOriginalData();
	Huffman.CompressData();
	Huffman.PrintDataFreq();
	Huffman.PrintSignedData();
	Huffman.PrintTree();

	// 終了処理
	printf("Push Enter Key >> ");
	rewind(stdin);
	getchar();
	return 0;
}