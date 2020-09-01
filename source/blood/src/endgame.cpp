//-------------------------------------------------------------------------
/*
Copyright (C) 2010-2019 EDuke32 developers and contributors
Copyright (C) 2019 Nuke.YKT

This file is part of NBlood.

NBlood is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
//-------------------------------------------------------------------------

#include "ns.h"	// Must come before everything else!

#include "build.h"
#include "v_draw.h"
#include "common.h"
#include "mmulti.h"
#include "common_game.h"
#include "blood.h"
#include "endgame.h"
#include "globals.h"
#include "levels.h"
#include "loadsave.h"
#include "network.h"
#include "player.h"
#include "sound.h"
#include "view.h"
#include "messages.h"
#include "statistics.h"
#include "gstrings.h"
#include "gamestate.h"
#include "raze_sound.h"
#include "d_net.h"
#include "screenjob.h"

BEGIN_BLD_NS

enum
{
	kLoadScreenCRC = -2051908571,
	kLoadScreenWideBackWidth = 256,
	kLoadScreenWideSideWidth = 128,

};

static int bLoadScreenCrcMatch = -1;

static void drawTextScreenBackground(void)
{
	if (bLoadScreenCrcMatch == -1) bLoadScreenCrcMatch = tileGetCRC32(kLoadScreen) == kLoadScreenCRC;

	if (bLoadScreenCrcMatch)
	{
		if (ActiveRatio(twod->GetWidth(), twod->GetHeight()) < 1.34f)
		{
			DrawTexture(twod, tileGetTexture(kLoadScreen), 0, 0, DTA_FullscreenEx, FSMode_ScaleToFit43, TAG_DONE);
		}
		else
		{
			int width = scale(xdim, 240, ydim);
			int nCount = (width + kLoadScreenWideBackWidth - 1) / kLoadScreenWideBackWidth;
			for (int i = 0; i < nCount; i++)
			{
				DrawTexture(twod, tileGetTexture(kLoadScreenWideBack), (i * kLoadScreenWideBackWidth), 0,
					DTA_VirtualWidth, width, DTA_VirtualHeight, 200, DTA_KeepRatio, true, TAG_DONE);
			}
			DrawTexture(twod, tileGetTexture(kLoadScreenWideLeft), 0, 0, DTA_VirtualWidth, width, DTA_VirtualHeight, 200, DTA_KeepRatio, true, DTA_TopLeft, true, TAG_DONE);
			DrawTexture(twod, tileGetTexture(kLoadScreenWideRight), width - tileWidth(kLoadScreenWideRight), 0, DTA_TopLeft, true,
				DTA_VirtualWidth, width, DTA_VirtualHeight, 200, DTA_KeepRatio, true, TAG_DONE);
			DrawTexture(twod, tileGetTexture(kLoadScreenWideMiddle), (width - tileWidth(kLoadScreenWideMiddle)) / 2, 0, DTA_TopLeft, true,
				DTA_VirtualWidth, width, DTA_VirtualHeight, 200, DTA_KeepRatio, true, TAG_DONE);
		}
	}
	else
	{
		DrawTexture(twod, tileGetTexture(kLoadScreen), 0, 0, DTA_FullscreenEx, FSMode_ScaleToFit43, TAG_DONE);
	}
}


class DBloodSummaryScreen : public DScreenJob
{
	void DrawKills(void)
	{
		char pBuffer[40];
		if (gGameOptions.nGameType == 0)
		{
			viewDrawText(1, FStringf("%s:", GStrings("KILLS")), 75, 50, -128, 0, 0, 1);
			mysnprintf(pBuffer, 40,"%2d", gKillMgr.Kills);
			viewDrawText(1, pBuffer, 160, 50, -128, 0, 0, 1);
			viewDrawText(1, GStrings("OF"), 190, 50, -128, 0, 0, 1);
			mysnprintf(pBuffer, 40, "%2d", gKillMgr.TotalKills);
			viewDrawText(1, pBuffer, 220, 50, -128, 0, 0, 1);
		}
		else
		{
			viewDrawText(3, "#", 85, 35, -128, 0, 0, 1);
			viewDrawText(3, GStrings("NAME"), 100, 35, -128, 0, 0, 1);
			viewDrawText(3, GStrings("FRAGS"), 210, 35, -128, 0, 0, 1);
			int nStart = 0;
			int nEnd = kMaxPlayers;

			for (int i = nStart; i < nEnd; i++) if (playeringame[i])
			{
				mysnprintf(pBuffer, 40, "%-2d", i);
				viewDrawText(3, pBuffer, 85, 50 + 8 * i, -128, 0, 0, 1);
				mysnprintf(pBuffer, 40, "%s", gProfile[i].name);
				viewDrawText(3, pBuffer, 100, 50 + 8 * i, -128, 0, 0, 1);
				mysnprintf(pBuffer, 40, "%d", gPlayer[i].fragCount);
				viewDrawText(3, pBuffer, 210, 50 + 8 * i, -128, 0, 0, 1);
			}
		}
	}

	void DrawSecrets(void)
	{
		char pBuffer[40];
		viewDrawText(1, FStringf("%s:", GStrings("TXT_SECRETS")), 75, 70, -128, 0, 0, 1);
		mysnprintf(pBuffer, 40, "%2d", gSecretMgr.Founds);
		viewDrawText(1, pBuffer, 160, 70, -128, 0, 0, 1);
		viewDrawText(1, GStrings("OF"), 190, 70, -128, 0, 0, 1);
		mysnprintf(pBuffer, 40, "%2d", gSecretMgr.Total);
		viewDrawText(1, pBuffer, 220, 70, -128, 0, 0, 1);
		if (gSecretMgr.Super > 0)
			viewDrawText(1, GStrings("TXT_SUPERSECRET"), 160, 100, -128, 2, 1, 1);
	}


	int Frame(uint64_t clock, bool skiprequest)
	{
		drawTextScreenBackground();
		if (gGameOptions.nGameType == 0)
		{
			DrawMenuCaption(GStrings("TXTB_LEVELSTATS"));
			if (bPlayerCheated)
			{
				viewDrawText(3, GStrings("TXTB_CHEATED"), 160, 32, -128, 0, 1, 1);
			}
			DrawKills();
			DrawSecrets();
		}
		else
		{
			DrawMenuCaption(GStrings("TXTB_FRAGSTATS"));
			DrawKills();
		}
		int myclock = int(clock * 120 / 1'000'000'000);
		if ((myclock & 32))
		{
			viewDrawText(3, GStrings("PRESSKEY"), 160, 134, -128, 0, 1, 1);
		}
		return skiprequest ? -1 : 1;
	}
};


void ShowSummaryScreen()
{
	JobDesc job = { Create<DBloodSummaryScreen>() };

	STAT_Update(false);
	EndLevel();
	Mus_Stop();
	sndStartSample(268, 128, -1, false);
	RunScreenJob(&job, 1, [](bool)
		{
			int ep = volfromlevelnum(currentLevel->levelNumber);
			gStartNewGame = FindMapByLevelNum(levelnum(ep, gNextLevel));
			gamestate = GS_LEVEL;
			soundEngine->StopAllChannels();
		});
}



CKillMgr::CKillMgr()
{
	Clear();
}

void CKillMgr::SetCount(int nCount)
{
	TotalKills = nCount;
}

void CKillMgr::AddNewKill(int nCount)
{
	TotalKills += nCount;
}

void CKillMgr::AddKill(spritetype* pSprite)
{
	if (pSprite->statnum == kStatDude && pSprite->type != kDudeBat && pSprite->type != kDudeRat && pSprite->type != kDudeInnocent && pSprite->type != kDudeBurningInnocent)
		Kills++;
}

void CKillMgr::CountTotalKills(void)
{
	TotalKills = 0;
	for (int nSprite = headspritestat[kStatDude]; nSprite >= 0; nSprite = nextspritestat[nSprite])
	{
		spritetype* pSprite = &sprite[nSprite];
		if (pSprite->type < kDudeBase || pSprite->type >= kDudeMax)
			ThrowError("Non-enemy sprite (%d) in the enemy sprite list.", nSprite);
		if (pSprite->statnum == kStatDude && pSprite->type != kDudeBat && pSprite->type != kDudeRat && pSprite->type != kDudeInnocent && pSprite->type != kDudeBurningInnocent)
			TotalKills++;
	}
}

void CKillMgr::Clear(void)
{
	TotalKills = Kills = 0;
}

CSecretMgr::CSecretMgr(void)
{
	Clear();
}

void CSecretMgr::SetCount(int nCount)
{
	Total = nCount;
}

void CSecretMgr::Found(int nType)
{
	if (nType == 0) Founds++;
	else if (nType < 0) {
		viewSetSystemMessage("Invalid secret type %d triggered.", nType);
		return;
	}
	else Super++;

	if (gGameOptions.nGameType == 0) {
		viewSetMessage(GStrings(FStringf("TXTB_SECRET%d", Random(2))), 0, MESSAGE_PRIORITY_SECRET);
	}
}

void CSecretMgr::Clear(void)
{
	Total = Founds = Super = 0;
}

class EndGameLoadSave : public LoadSave {
public:
	virtual void Load(void);
	virtual void Save(void);
};

void EndGameLoadSave::Load(void)
{
	Read(&gSecretMgr.Total, 4);
	Read(&gSecretMgr.Founds, 4);
	Read(&gSecretMgr.Super, 4);
	Read(&gKillMgr.TotalKills, 4);
	Read(&gKillMgr.Kills, 4);
}

void EndGameLoadSave::Save(void)
{
	Write(&gSecretMgr.Total, 4);
	Write(&gSecretMgr.Founds, 4);
	Write(&gSecretMgr.Super, 4);
	Write(&gKillMgr.TotalKills, 4);
	Write(&gKillMgr.Kills, 4);
}

CSecretMgr gSecretMgr;
CKillMgr gKillMgr;
static EndGameLoadSave* myLoadSave;

void EndGameLoadSaveConstruct(void)
{
	myLoadSave = new EndGameLoadSave();
}


class DBloodLoadScreen : public DScreenJob
{
	std::function<int(void)> callback;
	const char* pzLoadingScreenText1;
	MapRecord* rec;

public:
	DBloodLoadScreen(const char* caption, MapRecord* maprec, std::function<int(void)> callback_) : DScreenJob(fadein | fadeout), callback(callback_), rec(maprec)
	{
		if (gGameOptions.nGameType == 0) pzLoadingScreenText1 = GStrings("TXTB_LLEVEL");
		else pzLoadingScreenText1 = GStrings(FStringf("TXTB_NETGT%d", gGameOptions.nGameType));
	}

	int Frame(uint64_t clock, bool skiprequest)
	{
		twod->ClearScreen();
		drawTextScreenBackground();
		DrawMenuCaption(pzLoadingScreenText1);
		viewDrawText(1, rec->DisplayName(), 160, 50, -128, 0, 1, 1);
		viewDrawText(3, GStrings("TXTB_PLSWAIT"), 160, 134, -128, 0, 1, 1);

		// Initiate the level load once the page has been faded in completely.
		if (callback && GetFadeState() == visible)
		{
			callback();
			callback = nullptr;
		}
		if (clock > 5'000'000'000) return 0;	// make sure the screen stays long enough to be seen.
		return skiprequest ? -1 : 1;
	}
};

END_BLD_NS
