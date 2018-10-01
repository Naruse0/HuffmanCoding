/**
 *	@mainpage	Huffman Coding.
 *	@details	ハフマン符号化を行うアルゴリズムの実装。
 */

#include	<stdio.h>
#include	<iostream>
#include	<fstream>
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

const char*			EXPORT_FILE_NAME	= "Export.txt";

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

	//----------------------------------------------------------
	// 主な機能
	//----------------------------------------------------------

	//!	@brief	データの圧縮を行う
	void	CompressData()
	{
		printf("---- Commpress Data ----\n");
		CountDataFreq();
		MakeHuffmanTree();
		MakeSignedData();
		MakeCompressedData();
	}

	//!	@brief	圧縮したデータを出力する
	void	ExportData(const char* _fileName)
	{
		printf("---- Export ----\n");

		CExporter	exporter(_fileName, m_dataCount, m_dataSign, m_compressedBit);
		exporter.Export();

		// 終了
		printf("Export success.\n");
	}

	//!	@brief	出力したデータを入力する
	void	ImportData(const char* _fileName)
	{
		printf("---- Import ----\n");

		CImporter	importer(_fileName, m_dataCount, m_originalData, m_dataSign);

		importer.Import();

		printf("Import success.\n");
	}

	//----------------------------------------------------------
	// 表示関係
	//----------------------------------------------------------

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

	//!	@brief	圧縮したデータを表示
	void	PrintCompressedData() const
	{
		printf("---- Compressed Data ----\n");
		printf("%s\n", m_compressedData.data());

		printf("-- Bit --\n");
		for (const auto& bitset : m_compressedBit)
		{
			printf("%s\n", bitset.to_string().data());
		}
		printf("\n");


		printf("-- Bit(char, int) --\n");
		for (const auto& bitset : m_compressedBit)
		{
			printf("%3d ", bitset.to_ulong());
		}
		printf("\n");

	}

private:

	static const char	LEFT_NODE_SIGN	= '1';					//!	@brief	符号化する際の左側ノードをたどる際に追加するの符号
	static const char	RIGHT_NODE_SIGN = '0';					//!	@brief	符号化する際の右側ノードをたどる際に追加するの符号

	std::string			m_originalData;							//!	@brief	元データ
	std::string			m_compressedData;						//!	@brief	圧縮済みデータ(ビットの文字列)
	int					m_dataCount;							//!	@brief	データ数
	std::map<char, int>	m_dataFreq;								//!	@brief	データの出現頻度

	std::vector<CNode>	m_nodes;								//!	@brief	全てのノードを配列で保持する

	std::map<char, std::string>	m_dataSign;						//!	@brief	データごとの符号

	std::vector<std::bitset<BIT_SET_DIGIT>>	m_compressedBit;	//!	@brief	ビット配列（余白は0埋め）

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

	//----------------------------------------------------------
	// 圧縮・解凍関係
	//----------------------------------------------------------

	//!	@brief	元データを圧縮
	void	MakeCompressedData()
	{
		// 初期化
		m_compressedData = "";

		// 文字を符号化した文字列に置き換える
		for (const auto& datum : m_originalData)
		{
			m_compressedData += m_dataSign[datum];
		}

		// データをビット列として入れていく
		m_compressedBit.clear();


		// ビット列に置きなおす
		bool	isProcEnd = false;
		size_t	idxData = 0;
		while (!isProcEnd)
		{
			// 配列を追加
			m_compressedBit.push_back(std::bitset<BIT_SET_DIGIT>());
			auto& curBitset = m_compressedBit.back();

			// 全てのビットに対して値をセットする
			//for (int iBit = 0; iBit < BIT_SET_DIGIT; ++iBit)
			for(int iBit = BIT_SET_DIGIT - 1; iBit >= 0; --iBit)
			{
				bool value = m_compressedData[idxData] != '0';
				curBitset.set(iBit, value);

				// 次の文字列へ
				idxData++; 

				// 次の文字列がない場合
				if (idxData >= m_compressedData.size())
				{
					isProcEnd = true;
					break;
				}
			}
		}
	}


	//----------------------------------------------------------
	// データ出力用クラス
	//----------------------------------------------------------

	class CExporter
	{
	public:

		//!	@brief	データの出力
		void Export() const
		{
			//---- 出力用ファイルオープン ----
			std::ofstream	file(m_fileName, std::ios::out);
			if (!file.is_open())
			{
				printf("Error : ファイルオープン失敗。\n");
				return;
			}

			//---- 書き込み ----

			// データ数
			file << m_dataCount << std::endl;

			// データごとの符号
			file << m_dataSign.size() << std::endl;
			for (const auto& datum : m_dataSign)
			{
				file << datum.first << "," << datum.second << std::endl;
			}

			// 圧縮データ
			file << m_exportData .size() << std::endl;
 			for (const auto& bitset : m_exportData)
			{
				//char outChar = static_cast<const char>(bitset.to_ulong());
				const int outChar = static_cast<const int>(bitset.to_ulong());
				file << outChar << std::endl;
			}

			//---- 終了 ----
			file.close();
		}

	private:

		std::string										m_fileName;		//!	@brief	出力するファイル名
		const int&										m_dataCount;	//!	@brief	出力するデータ数
		const std::map<char, std::string>&				m_dataSign;		//!	@brief	データごとの符号
		const std::vector<std::bitset<BIT_SET_DIGIT>>&	m_exportData;	//!	@brief	出力するデータ

	public:

		//!	@brief	出力するデータの参照を受け取るコンストラクタ
		CExporter(
			const char*										_fileName,
			const int&										_dataCount,
			const std::map<char, std::string>&				_dataSign,
			const std::vector<std::bitset<BIT_SET_DIGIT>>&	_compressedData) : 
			m_fileName(_fileName),
			m_dataCount(_dataCount),
			m_dataSign(_dataSign),
			m_exportData(_compressedData)
		{

		}

		~CExporter()
		{
		
		}

	};


	//----------------------------------------------------------
	// データ入力用クラス
	//----------------------------------------------------------

	class CImporter
	{
	public:

		void	Import()
		{
			//---- 入力用ファイルオープン ----
			std::ifstream	file(m_fileName, std::ios::in);
			if (!file.is_open())
			{
				printf("Error : ファイルオープン失敗。\n");
				return;
			}

			//---- 読み込み ----
			std::string	line;		// 行の読み込み先

			// データ数
			file >> line;
			m_outDataCount = std::stoi(line);

			// データの符号
			file >> line;
			int signDataCount = std::stoi(line);

			m_outDataSign.clear();
			for (int i = 0; i < signDataCount; ++i)
			{
				file >> line;

				// 行のデータを、キーと値に分割
				std::string	key, value;
				size_t		keyOffset = line.find_first_of(',');
				key = line.substr(0, keyOffset);
				value = line.substr(keyOffset + 1);

				char	inChar = key.front();		// 1文字取得

				m_outDataSign[inChar] = value;
			}

			// 圧縮データ
			file >> line;
			int bitsetCount = std::stoi(line);
			m_importData.resize(bitsetCount);
			for (int i = 0; i < bitsetCount; ++i)
			{
				// 参照を取得
				auto& bitset = m_importData.at(i);

				// 文字取得
				file >> line;

				// 文字列をビットに置き換え 
				int		bitValue = 0;
				int		curDigit = 1;
				for (int iNum = line.size() - 1; iNum >= 0 ; --iNum)
				{
					int num = line.data()[iNum] - '0';
					bitValue += num * curDigit;
					curDigit *= 10;
				}

				for (int iBit = BIT_SET_DIGIT - 1; iBit >= 0; --iBit)
				{
					int targetBit = (1 << iBit);
					bitset.set(iBit, bitValue & targetBit);
				}
			}

			// ビット配列を文字列化する
			std::string		bitString = "";
			printf("---- Import Bit ----\n");
			for (const auto& bitset : m_importData)
			{
				printf("%s\n", bitset.to_string().data());
				bitString += bitset.to_string();
			}

			// 複合
			m_outOrigData.clear();
			int remainingDataCount = m_outDataCount;
			while (remainingDataCount > 0)
			{
				char	expandData = '\0';
				size_t	minStrLen = UINT_MAX;

				// 全ての符号データのうち一致するものを探す
				for (const auto& sign : m_outDataSign)
				{
					// 先頭からではない場合は無視
					size_t offset = bitString.find(sign.second);
					if (offset != 0) { continue; }

					// 符号ビット数が多い場合は無視
					size_t	strLen = sign.second.length();
					if (strLen > minStrLen) { continue; }

					// 最小を更新
					minStrLen = strLen;

					// 解凍後のデータを取得
					expandData = sign.first;
				}

				// 処理が済んだデータを排除した文字列を取得
				bitString = bitString.substr(minStrLen);

				// 解凍後のデータを代入
				m_outOrigData += expandData;

				// 処理済みデータカウントを進める
				remainingDataCount--;
			}

			//---- 終了 ----
			file.close();
		}

	private:

		std::string									m_fileName;
		int&										m_outDataCount;
		std::string&								m_outOrigData;
		std::map<char, std::string>&				m_outDataSign;
		std::vector<std::bitset<BIT_SET_DIGIT>>		m_importData;

	public:

		//!	@brief	入力したデータを格納する変数への参照を設定するコンストラクタ
		CImporter(
			const char*						_fileName,
			int&							_outDataCount,
			std::string&					_outOrigData,
			std::map<char, std::string>&	_outDataSign) : 
			m_fileName(_fileName),
			m_outDataCount(_outDataCount),
			m_outOrigData(_outOrigData),
			m_outDataSign(_outDataSign)
		{

		}

		~CImporter()
		{

		}
	};

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
	CHuffman	huf(INPUT_DATA);
	huf.CompressData();
	huf.PrintOriginalData();
	huf.PrintDataFreq();
	huf.PrintSignedData();
	huf.PrintTree();

	CHuffman	inHuf("");
	huf.ExportData(EXPORT_FILE_NAME);
	inHuf.ImportData(EXPORT_FILE_NAME);
	huf.PrintOriginalData();
	inHuf.PrintOriginalData();


	// 終了処理
	printf("Push Enter Key >> ");
	rewind(stdin);
	getchar();
	return 0;
}