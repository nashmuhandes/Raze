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
#pragma once
#include "build.h"
#include "common.h"
#include "common_game.h"

#include "blood.h"
#include "db.h"
#include "eventq.h"
#include "dude.h"
#include "player.h"

BEGIN_BLD_NS
void trTriggerSector(unsigned int nSector, XSECTOR *pXSector, int command);
void trMessageSector(unsigned int nSector, EVENT event);
void trTriggerWall(unsigned int nWall, XWALL *pXWall, int command);
void trMessageWall(unsigned int nWall, EVENT event);
void trTriggerSprite(unsigned int nSprite, XSPRITE *pXSprite, int command);
void trMessageSprite(unsigned int nSprite, EVENT event);
void trProcessBusy(void);
void trInit(void);
void trTextOver(int nId);

#ifdef NOONE_EXTENSIONS
// functions required for new features
// -------------------------------------------------------
#define kPlayerCtrlSigStart "<<<<TRPLAYERCTRL{" // save game TRPLAYERCTRL block start
#define kPlayerCtrlSigEnd "}TRPLAYERCTRL>>>>"   // save game TRPLAYERCTRL block end

void pastePropertiesInObj(int type, int nDest, EVENT event);
spritetype* getTargetInRange(spritetype* pSprite, int minDist, int maxDist, short data, short teamMode);
bool isMateOf(XSPRITE* pXDude, XSPRITE* pXSprite);
spritetype* targetIsPlayer(XSPRITE* pXSprite);
bool isTargetAimsDude(XSPRITE* pXTarget, spritetype* pDude);
spritetype* getMateTargets(XSPRITE* pXSprite);
bool isMatesHaveSameTarget(XSPRITE* pXLeader, spritetype* pTarget, int allow);
bool isActive(int nSprite);
bool dudeCanSeeTarget(XSPRITE* pXDude, DUDEINFO* pDudeInfo, spritetype* pTarget);
void disturbDudesInSight(spritetype* pSprite, int max);
int getTargetDist(spritetype* pSprite, DUDEINFO* pDudeInfo, spritetype* pTarget);
int getFineTargetDist(spritetype* pSprite, spritetype* pTarget);
bool IsBurningDude(spritetype* pSprite);
bool IsKillableDude(spritetype* pSprite);
bool isAnnoyingUnit(spritetype* pDude);
bool unitCanFly(spritetype* pDude);
bool isMeleeUnit(spritetype* pDude);
void activateDudes(int rx);
void freeTargets(int nSprite);
void freeAllTargets(XSPRITE* pXSource);
bool affectedByTargetChg(XSPRITE* pXDude);
int getDataFieldOfObject(int objType, int objIndex, int dataIndex);
bool setDataValueOfObject(int objType, int objIndex, int dataIndex, int value);
bool goalValueIsReached(XSPRITE* pXSprite);
bool getDudesForTargetChg(XSPRITE* pXSprite);
void stopWindOnSectors(XSPRITE* pXSource);
void useSectorWindGen(XSPRITE* pXSource, sectortype* pSector);
void useEffectGen(XSPRITE* pXSource, spritetype* pSprite);
void useSeqSpawnerGen(XSPRITE* pXSource, int objType, int index);
void useSpriteDamager(XSPRITE* pXSource, spritetype* pSprite);
void useTeleportTarget(XSPRITE* pXSource, spritetype* pSprite);
void usePropertiesChanger(XSPRITE* pXSource, short objType, int objIndex);
void useObjResizer(XSPRITE* pXSource, short objType, int objIndex);
void TeleFrag(int nKiller, int nSector);
bool valueIsBetween(int val, int min, int max);

void trPlayerCtrlLink(XSPRITE* pXSource, PLAYER* pPlayer);
void trPlayerCtrlStartScene(XSPRITE* pXSource, PLAYER* pPlayer);
void trPlayerCtrlStopScene(XSPRITE* pXSource, PLAYER* pPlayer);
char modernTypeSetSpriteState(int nSprite, XSPRITE* pXSprite, int nState);
// -------------------------------------------------------
#endif// -------------------------------------------------------

END_BLD_NS
