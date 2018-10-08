/**
 *	@mainpage	Haffuman Coding.
 *	@details	ハフマン符号化を行うアルゴリズムの実装。
 */

//----------------------------------------------------------
// インクルード
//----------------------------------------------------------

#include	"HuffmanEncoder.h"
#include	"HuffmanDecoder.h"

//----------------------------------------------------------
// 定数データ
//----------------------------------------------------------

const char*			INPUT_DATA = "CHFGHEGFCABFGDEGHAEFGHHGHBGFCEFACCFG";
const char*			INPUT_DATA_EXAMPLE = "aabcdeafagagfedadhaeedefahhh";
const size_t		BIT_SET_DIGIT = 8;

const char*			EXPORT_FILE_NAME = "Data.huf";

//----------------------------------------------------------
//!	@brief	エントリポイント
//----------------------------------------------------------
int main(void)
{
	CHuffmanEncoder	encoder(INPUT_DATA);
	encoder.Encode(EXPORT_FILE_NAME);

	CHuffmanDecoder	decoder;
	decoder.Decode(EXPORT_FILE_NAME);

	printf("Push Enter-key >> ");
	rewind(stdin);
	getchar();
	return 0;
}