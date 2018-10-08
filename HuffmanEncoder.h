#pragma once

#include	<bitset>
#include	<map>
#include	<string>
#include	<vector>

#include	"HuffmanData.h"

//!	@brief	ハフマン符号化を行い圧縮し、データを出力するクラス
class CHuffmanEncoder
{
public:

	//!	@brief	圧縮し出力する
	void Encode(const char* _exportFilePath);

private:

	static const size_t	BIT_SET_DIGIT	= 8;	//!	@brief	bitsetの桁数
	static const char	LEFT_NODE_SIGN	= '1';	//!	@brief	符号化する際の左側ノードをたどる際に追加するの符号
	static const char	RIGHT_NODE_SIGN = '0';	//!	@brief	符号化する際の右側ノードをたどる際に追加するの符号

	//!	@brief	圧縮後のビットデータを格納する型
	using DataBitSet = std::bitset<BIT_SET_DIGIT>;

	//!	@brief	ハフマンツリーを構成するノード構造体
	struct SNode
	{
		int		value;		
		char	key;

		SNode*	pParent;
		SNode*	pLeft;
		SNode*	pRight;

		//!	@brief	ノードがツリーに構成されているかを確認
		bool	IsEnable() const { return value > 0; }

		//!	@brief	親が存在するか
		bool	IsParentExist() const { return pParent != nullptr; }

		//!	@brief	根ノードの参照を取得する
		SNode*	GetRoot()
		{
			if (pParent != nullptr)
			{
				return pParent->GetRoot();
			}
			return this;
		}

		//!	@brief	葉ノードを作成する際のコンストラクタ
		SNode(const int _value, const char _key) :
			value(_value),
			key(_key),
			pParent(nullptr),
			pLeft(nullptr),
			pRight(nullptr)
		{

		}

		//!	@brief	内部ノードを作成する際のコンストラクタ
		SNode(const int _value) :
			SNode(_value, '\0')
		{

		}

		//!	@brief	無効な値を設定するコンストラクタ
		SNode() :
			SNode(-1)
		{

		}
	};

	std::string					m_originalDataText;			//!	@brief	元データ
	std::string					m_compressDataText;			//!	@brief	圧縮データ
	std::vector<DataBitSet>		m_compressBitDataBits;		//!	@brief	圧縮データを符号化したビット列

	size_t						m_originalDataTextCount;	//!	@brief	元データのデータ数
	std::map<char, size_t>		m_dataFreqs;				//!	@brief	データごとの出現頻度
	std::map<char, std::string>	m_dataSigns;				//!	@brief	データごとの符号

	std::vector<SNode>			m_nodes;					//!	@brief	ハフマンツリーのノード

	//----------------------------------------------------------
	// ハフマンツリー作成関係
	//----------------------------------------------------------

	//!	@brief	データの出現頻度を数える
	void CountDataFreq();

	//!	@brief	ハフマンツリーを構成する
	void CreateHuffmanTree();

	//!	@brief	根（親がいない）ノードのうち最小の値と2番目に最小のノード（次に葉になるノード）を取得する
	//!	@param[out]		_pOutLeft		最小の値を持つノードを受け取る。
	//!	@param[out]		_pOutRight		2番目に最小のノードを受け取る。
	//!	@note			引数がnullptrの場合はノードが存在しない。
	void SearchLeafNode(SNode*& _pOutLeft, SNode*& _pOutRight);

	//!	@brief	部分木を作成する
	void MakePartialTree();

	//!	@brief		新しいノードを追加する。
	//!	@param[in]	_value		新しいノードに設定する値
	//!	@param[in]	_pLeft		新しいノードの左側の子にするノード
	//!	@param[in]	_pRight		新しいノードの右側の子にするノード
	void AddNode(const int _value, SNode* _pLeft, SNode* _pRight);

	//----------------------------------------------------------
	// ハフマン符号化関係
	//----------------------------------------------------------

	//!	@brief	データごとの符号を作成する
	void CreateSignData();

	//!	@brief	葉を探しながら符号データを作成
	void FindAndMakeSignData(SNode* _pNode, std::string _signString = "");

	//----------------------------------------------------------
	// 圧縮・出漁
	//----------------------------------------------------------

	//!	@brief	元データの圧縮
	void CreateCompressData();

	//!	@brief	圧縮データをフォーマットに直し出力
	void Export(const char* _exportFilePath);

	//----------------------------------------------------------
	// 表示系
	//----------------------------------------------------------

	void PrintOriginalData() const;

	void PrintDataFreq() const;

	void PrintTree();

	void PrintTreeRecursivly(SNode* _pNode, int _depth = 0) const;

	void PrintSignData() const;

	void PrintCompressData() const;

public:

	//!	@brief	圧縮するデータを設定するコンストラクタ
	CHuffmanEncoder(const char* _data);

	~CHuffmanEncoder()
	{

	}
};