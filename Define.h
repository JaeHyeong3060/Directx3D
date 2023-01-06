#ifndef Define_h__
#define Define_h__

#define	WINCX		800
#define WINCY		600

extern HWND		g_hWnd;
extern HINSTANCE g_hInst;

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif



#include "ChoppingBoard.h"
#include "CookingStation.h"
#include "FireExtinguisher.h"
#include "MyMap.h"
#include "PirateShip.h"
#include "Plate.h"
#include "DirtyPlate.h"
#include "Pot.h"
#include "ThePass.h"
#include "ThePassExit.h"
#include "Van.h"
#include "WashingStation.h"
#include "WorkTop.h"
#include "Empty.h"
#include "Soup.h"
#include "MenuO.h"
#include "TimeGauge.h"
#include "UITimerBack.h"
#include "UITimerObj.h"
#include "UIScoreBack.h"
#include "UIScoreStar.h"

#include "PlayerCamera.h"

#include "Crate.h"
#include "CrateB.h"
#include "CrateC.h"
#include "Tomato.h"
#include "Onion.h"
#include "Lettuce.h"
#include "SaladTomato.h"
#include "SaladLettuce.h"

#include "UIDigit.h"
#include "DigitMgr.h"
#include "UIResult.h"
#include "UITexture.h"

#include "ResultScene.h"


#include "Chef.h"
#include "ChefB.h"
#include "Effect.h"

#include "NPC_Buck.h"
#include "NPC_Business.h"
#include "NPC_Mel.h"
#include "NPC_Penguin.h"
#include "NPC_Waiter.h"

#include "SoundMgr.h"

#endif // Define_h__
