/**
 *	@mainpage	Huffman Coding.
 *	@details	ハフマン符号化を行うアルゴリズムの実装。
 */

#include	<stdio.h>
#include	<string>
#include	<vector>
#include	<map>



/**
 *	@brief	定数データ
 */
const char*	INPUT_DATA = "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";



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
		printf("---- Data Frequency ----\n");
		for (const auto& itr : m_dataFreq)
		{
			printf("%c : %3d\n", itr.first, itr.second);
		}
	}

private:

	std::string			m_originalData;		// 元データ
	std::map<char, int>	m_dataFreq;			// データの出現頻度

	//!	@brief	データの出現頻度を数える
	void	CountDataFreq()
	{
		// 念のため初期化
		m_dataFreq.clear();

		// 全ての文字列を走査
		for (const auto& datum : m_originalData)
		{
			// 文字をキーにして数を数える
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
 *	@brief	エントリポイント
 */
int main()
{
	CHuffman	Huffman(INPUT_DATA);
	Huffman.PrintOriginalData();
	Huffman.CompressData();

	Huffman.PrintDataFreq();

	// 終了処理
	printf("Push any key >> ");
	rewind(stdin);
	getchar();
	return 0;
}