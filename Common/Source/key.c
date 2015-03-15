#include <string.h>
#include <jendefs.h>
#include <utils.h>
#include <AppHardwareApi.h>

#include "key.h"
#include "common.h"
#include "utils.h"
#include "modbus_ascii.h"

#define DBGMSG
#ifdef DBGMSG
#endif

/**
 * KEY を初期化する
 * @param psTimer
 */
void KEY_vInit(tsKEY *psKEY, tsTimerContext *psTimer) {
	memset(psKEY, 0, sizeof(tsKEY)); // 管理構造体の初期化
	memcpy(&psKEY->sTimer, psTimer, sizeof(tsTimerContext)); // タイマ管理構造体をコピーする

	psKEY->normalTimer = 33;	// 正転設定値
	psKEY->reverseTimer = 25;	// 逆転設定値

	psKEY->counter = 0;	// カウンター値初期化
	psKEY->sTimer.u16duty = 0;	// PWMのデューティー値設定
	psKEY->sTimer.u16Hz = 120;	// PWMのHz設定
	vTimerChangeHz(&psKEY->sTimer);
	vPortAsOutput(PORT_OUT1);
	vPortAsOutput(PORT_OUT2);
	vPortSetHi(PORT_OUT1);
	vPortSetHi(PORT_OUT2);
}

/**
 * KEY の主処理部
 * @param psKEY
 */
void KEY_vInt(tsKEY *psKEY) {
	if (!psKEY->execflg)
		return;

	// 処理終了
	if (psKEY->counter > psKEY->normalTimer + psKEY->reverseTimer) {
		psKEY->sTimer.u16duty = 0;
		vTimerChangeHz(&psKEY->sTimer);
		vPortSetHi(PORT_OUT1);
		vPortSetHi(PORT_OUT2);
		psKEY->counter = 0;
		psKEY->execflg = 0;
		return;
	}
	if (psKEY->counter <= psKEY->normalTimer) {
		// 正転
		psKEY->sTimer.u16duty = 1024;
		vTimerChangeHz(&psKEY->sTimer);
		vPortSetLo(PORT_OUT1);
		vPortSetHi(PORT_OUT2);
	} else if (psKEY->counter <= psKEY->normalTimer + psKEY->reverseTimer) {
		// 逆転
		psKEY->sTimer.u16duty = 1024;
		vTimerChangeHz(&psKEY->sTimer);
		vPortSetHi(PORT_OUT1);
		vPortSetLo(PORT_OUT2);
	}
	psKEY->counter++;
}

/**
 * KEY の開始処理部
 * @param psKEY
 */
void KEY_vSet(tsKEY *psKEY) {
	if (psKEY->execflg)
		return;

	// 処理を開始フラグを立てる
	psKEY->counter = 0;
	psKEY->execflg = TRUE;
}
