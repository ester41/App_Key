#ifndef KEY_H_
#define KEY_H_

#include <jendefs.h>
#include <utils.h>

/** @ingroup KEY
 * KEY管理構造体
 */
typedef struct {
	tsTimerContext sTimer; //! PWMタイマーコンテキスト

	uint8 execflg;		//! 実行フラグ
	uint8 counter;		//! カウンター値
	uint8 normalTimer;	//! 正転設定値
//	uint8 sleepTimer;	//! 回転停止設定値
	uint8 reverseTimer;	//! 逆転設定値
} tsKEY;

/*
 * 関数定義
 */

void KEY_vInit(tsKEY *psKEY, tsTimerContext *psTimer);
void KEY_vInt(tsKEY *psKEY);
void KEY_vSet(tsKEY *psKEY);

#endif /* KEY_H_ */
