//-------------------------------------------------------------------------
/*
Copyright (C) 2010 EDuke32 developers and contributors

This file is part of EDuke32.

EDuke32 is free software; you can redistribute it and/or
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

#include "compat.h"
#include "build.h"

#include "namesdyn.h"
#include "global.h"
#include "gamecontrol.h"

BEGIN_DUKE_NS

# define DVPTR(x) &x

int16_t DynamicTileMap[MAXTILES];

struct dynitem
{
    const char *str;
    int32_t *dynvalptr;
    const int16_t staticval;
};

LUNATIC_EXTERN struct dynitem g_dynTileList[] =
{
    { "ACCESS_ICON",         DVPTR(ACCESS_ICON),         ACCESS_ICON__STATIC },
    { "ACCESSCARD",          DVPTR(ACCESSCARD),          ACCESSCARD__STATIC },
    { "ACCESSSWITCH",        DVPTR(ACCESSSWITCH),        ACCESSSWITCH__STATIC },
    { "ACCESSSWITCH2",       DVPTR(ACCESSSWITCH2),       ACCESSSWITCH2__STATIC },
    { "ACTIVATOR",           DVPTR(ACTIVATOR),           ACTIVATOR__STATIC },
    { "ACTIVATORLOCKED",     DVPTR(ACTIVATORLOCKED),     ACTIVATORLOCKED__STATIC },
    { "AIRTANK",             DVPTR(AIRTANK),             AIRTANK__STATIC },
    { "AIRTANK_ICON",        DVPTR(AIRTANK_ICON),        AIRTANK_ICON__STATIC },
    { "ALIENSWITCH",         DVPTR(ALIENSWITCH),         ALIENSWITCH__STATIC },
    { "AMMO",                DVPTR(AMMO),                AMMO__STATIC },
    { "AMMOBOX",             DVPTR(AMMOBOX),             AMMOBOX__STATIC },
    { "AMMOLOTS",            DVPTR(AMMOLOTS),            AMMOLOTS__STATIC },
    { "ANTENNA",             DVPTR(ANTENNA),             ANTENNA__STATIC },
    { "APLAYER",             DVPTR(APLAYER),             APLAYER__STATIC },
    { "APLAYERTOP",          DVPTR(APLAYERTOP),          APLAYERTOP__STATIC },
    { "ARMJIB1",             DVPTR(ARMJIB1),             ARMJIB1__STATIC },
    { "ARROW",               DVPTR(ARROW),               ARROW__STATIC },
    { "ATM",                 DVPTR(ATM),                 ATM__STATIC },
    { "ATMBROKE",            DVPTR(ATMBROKE),            ATMBROKE__STATIC },
    { "ATOMICHEALTH",        DVPTR(ATOMICHEALTH),        ATOMICHEALTH__STATIC },
    { "BANNER",              DVPTR(BANNER),              BANNER__STATIC },
    { "BARBROKE",            DVPTR(BARBROKE),            BARBROKE__STATIC },
    { "BATTERYAMMO",         DVPTR(BATTERYAMMO),         BATTERYAMMO__STATIC },
    { "BETASCREEN",          DVPTR(BETASCREEN),          BETASCREEN__STATIC },
    { "BETAVERSION",         DVPTR(BETAVERSION),         BETAVERSION__STATIC },
    { "BGRATE1",             DVPTR(BGRATE1),             BGRATE1__STATIC },
    { "BIGALPHANUM",         DVPTR(BIGALPHANUM),         BIGALPHANUM__STATIC },
    { "BIGAPPOS",            DVPTR(BIGAPPOS),            BIGAPPOS__STATIC },
    { "BIGCOLIN",            DVPTR(BIGCOLIN),            BIGCOLIN__STATIC },
    { "BIGCOMMA",            DVPTR(BIGCOMMA),            BIGCOMMA__STATIC },
    { "BIGFORCE",            DVPTR(BIGFORCE),            BIGFORCE__STATIC },
    { "BIGHOLE",             DVPTR(BIGHOLE),             BIGHOLE__STATIC },
    { "BIGORBIT1",           DVPTR(BIGORBIT1),           BIGORBIT1__STATIC },
    { "BIGPERIOD",           DVPTR(BIGPERIOD),           BIGPERIOD__STATIC },
    { "BIGQ",                DVPTR(BIGQ),                BIGQ__STATIC },
    { "BIGSEMI",             DVPTR(BIGSEMI),             BIGSEMI__STATIC },
    { "BIGX",                DVPTR(BIGX_),               BIGX__STATIC },
    { "BLANKSCREEN",         DVPTR(BLANKSCREEN),         BLANKSCREEN__STATIC },
    { "BLIMP",               DVPTR(BLIMP),               BLIMP__STATIC },
    { "BLOOD",               DVPTR(BLOOD),               BLOOD__STATIC },
    { "BLOODPOOL",           DVPTR(BLOODPOOL),           BLOODPOOL__STATIC },
    { "BLOODSPLAT1",         DVPTR(BLOODSPLAT1),         BLOODSPLAT1__STATIC },
    { "BLOODSPLAT2",         DVPTR(BLOODSPLAT2),         BLOODSPLAT2__STATIC },
    { "BLOODSPLAT3",         DVPTR(BLOODSPLAT3),         BLOODSPLAT3__STATIC },
    { "BLOODSPLAT4",         DVPTR(BLOODSPLAT4),         BLOODSPLAT4__STATIC },
    { "BLOODYPOLE",          DVPTR(BLOODYPOLE),          BLOODYPOLE__STATIC },
    { "BOLT1",               DVPTR(BOLT1),               BOLT1__STATIC },
    { "BONUSSCREEN",         DVPTR(BONUSSCREEN),         BONUSSCREEN__STATIC },
    { "BOOT_ICON",           DVPTR(BOOT_ICON),           BOOT_ICON__STATIC },
    { "BOOTS",               DVPTR(BOOTS),               BOOTS__STATIC },
    { "BORNTOBEWILDSCREEN",  DVPTR(BORNTOBEWILDSCREEN),  BORNTOBEWILDSCREEN__STATIC },
    { "BOSS1",               DVPTR(BOSS1),               BOSS1__STATIC },
    { "BOSS1LOB",            DVPTR(BOSS1LOB),            BOSS1LOB__STATIC },
    { "BOSS1SHOOT",          DVPTR(BOSS1SHOOT),          BOSS1SHOOT__STATIC },
    { "BOSS1STAYPUT",        DVPTR(BOSS1STAYPUT),        BOSS1STAYPUT__STATIC },
    { "BOSS2",               DVPTR(BOSS2),               BOSS2__STATIC },
    { "BOSS3",               DVPTR(BOSS3),               BOSS3__STATIC },
    { "BOSS4",               DVPTR(BOSS4),               BOSS4__STATIC },
    { "BOSS4STAYPUT",        DVPTR(BOSS4STAYPUT),        BOSS4STAYPUT__STATIC },
    { "BOSSTOP",             DVPTR(BOSSTOP),             BOSSTOP__STATIC },
    { "BOTTLE1",             DVPTR(BOTTLE1),             BOTTLE1__STATIC },
    { "BOTTLE10",            DVPTR(BOTTLE10),            BOTTLE10__STATIC },
    { "BOTTLE11",            DVPTR(BOTTLE11),            BOTTLE11__STATIC },
    { "BOTTLE12",            DVPTR(BOTTLE12),            BOTTLE12__STATIC },
    { "BOTTLE13",            DVPTR(BOTTLE13),            BOTTLE13__STATIC },
    { "BOTTLE14",            DVPTR(BOTTLE14),            BOTTLE14__STATIC },
    { "BOTTLE15",            DVPTR(BOTTLE15),            BOTTLE15__STATIC },
    { "BOTTLE16",            DVPTR(BOTTLE16),            BOTTLE16__STATIC },
    { "BOTTLE17",            DVPTR(BOTTLE17),            BOTTLE17__STATIC },
    { "BOTTLE18",            DVPTR(BOTTLE18),            BOTTLE18__STATIC },
    { "BOTTLE19",            DVPTR(BOTTLE19),            BOTTLE19__STATIC },
    { "BOTTLE2",             DVPTR(BOTTLE2),             BOTTLE2__STATIC },
    { "BOTTLE3",             DVPTR(BOTTLE3),             BOTTLE3__STATIC },
    { "BOTTLE4",             DVPTR(BOTTLE4),             BOTTLE4__STATIC },
    { "BOTTLE5",             DVPTR(BOTTLE5),             BOTTLE5__STATIC },
    { "BOTTLE6",             DVPTR(BOTTLE6),             BOTTLE6__STATIC },
    { "BOTTLE7",             DVPTR(BOTTLE7),             BOTTLE7__STATIC },
    { "BOTTLE8",             DVPTR(BOTTLE8),             BOTTLE8__STATIC },
    { "BOTTOMSTATUSBAR",     DVPTR(BOTTOMSTATUSBAR),     BOTTOMSTATUSBAR__STATIC },
    { "BOUNCEMINE",          DVPTR(BOUNCEMINE),          BOUNCEMINE__STATIC },
    { "BOX",                 DVPTR(BOX),                 BOX__STATIC },
    { "BPANNEL1",            DVPTR(BPANNEL1),            BPANNEL1__STATIC },
    { "BPANNEL3",            DVPTR(BPANNEL3),            BPANNEL3__STATIC },
    { "BROKEFIREHYDRENT",    DVPTR(BROKEFIREHYDRENT),    BROKEFIREHYDRENT__STATIC },
    { "BROKEHYDROPLANT",     DVPTR(BROKEHYDROPLANT),     BROKEHYDROPLANT__STATIC },
    { "BROKENCHAIR",         DVPTR(BROKENCHAIR),         BROKENCHAIR__STATIC },
    { "BULLETHOLE",          DVPTR(BULLETHOLE),          BULLETHOLE__STATIC },
    { "BURNING",             DVPTR(BURNING),             BURNING__STATIC },
    { "BURNING2",            DVPTR(BURNING2),            BURNING2__STATIC },
    { "CACTUS",              DVPTR(CACTUS),              CACTUS__STATIC },
    { "CACTUSBROKE",         DVPTR(CACTUSBROKE),         CACTUSBROKE__STATIC },
    { "CAMCORNER",           DVPTR(CAMCORNER),           CAMCORNER__STATIC },
    { "CAMERA1",             DVPTR(CAMERA1),             CAMERA1__STATIC },
    { "CAMERALIGHT",         DVPTR(CAMERALIGHT),         CAMERALIGHT__STATIC },
    { "CAMERAPOLE",          DVPTR(CAMERAPOLE),          CAMERAPOLE__STATIC },
    { "CAMLIGHT",            DVPTR(CAMLIGHT),            CAMLIGHT__STATIC },
    { "CANWITHSOMETHING",    DVPTR(CANWITHSOMETHING),    CANWITHSOMETHING__STATIC },
    { "CANWITHSOMETHING2",   DVPTR(CANWITHSOMETHING2),   CANWITHSOMETHING2__STATIC },
    { "CANWITHSOMETHING3",   DVPTR(CANWITHSOMETHING3),   CANWITHSOMETHING3__STATIC },
    { "CANWITHSOMETHING4",   DVPTR(CANWITHSOMETHING4),   CANWITHSOMETHING4__STATIC },
    { "CEILINGSTEAM",        DVPTR(CEILINGSTEAM),        CEILINGSTEAM__STATIC },
    { "CHAINGUN",            DVPTR(CHAINGUN),            CHAINGUN__STATIC },
    { "CHAINGUNSPRITE",      DVPTR(CHAINGUNSPRITE),      CHAINGUNSPRITE__STATIC },
    { "CHAIR1",              DVPTR(CHAIR1),              CHAIR1__STATIC },
    { "CHAIR2",              DVPTR(CHAIR2),              CHAIR2__STATIC },
    { "CHAIR3",              DVPTR(CHAIR3),              CHAIR3__STATIC },
    { "CIRCLEPANNEL",        DVPTR(CIRCLEPANNEL),        CIRCLEPANNEL__STATIC },
    { "CIRCLEPANNELBROKE",   DVPTR(CIRCLEPANNELBROKE),   CIRCLEPANNELBROKE__STATIC },
    { "CLOUDYOCEAN",         DVPTR(CLOUDYOCEAN),         CLOUDYOCEAN__STATIC },
    { "CLOUDYSKIES",         DVPTR(CLOUDYSKIES),         CLOUDYSKIES__STATIC },
    { "COLA",                DVPTR(COLA),                COLA__STATIC },
    { "COLAMACHINE",         DVPTR(COLAMACHINE),         COLAMACHINE__STATIC },
    { "COMMANDER",           DVPTR(COMMANDER),           COMMANDER__STATIC },
    { "COMMANDERSTAYPUT",    DVPTR(COMMANDERSTAYPUT),    COMMANDERSTAYPUT__STATIC },
    { "CONE",                DVPTR(CONE),                CONE__STATIC },
    { "COOLEXPLOSION1",      DVPTR(COOLEXPLOSION1),      COOLEXPLOSION1__STATIC },
    { "CRACK1",              DVPTR(CRACK1),              CRACK1__STATIC },
    { "CRACK2",              DVPTR(CRACK2),              CRACK2__STATIC },
    { "CRACK3",              DVPTR(CRACK3),              CRACK3__STATIC },
    { "CRACK4",              DVPTR(CRACK4),              CRACK4__STATIC },
    { "CRACKKNUCKLES",       DVPTR(CRACKKNUCKLES),       CRACKKNUCKLES__STATIC },
    { "CRANE",               DVPTR(CRANE),               CRANE__STATIC },
    { "CRANEPOLE",           DVPTR(CRANEPOLE),           CRANEPOLE__STATIC },
    { "CREDITSTEXT1",        DVPTR(CREDITSTEXT1),        CREDITSTEXT1__STATIC },
    { "CREDITSTEXT2",        DVPTR(CREDITSTEXT2),        CREDITSTEXT2__STATIC },
    { "CREDITSTEXT3",        DVPTR(CREDITSTEXT3),        CREDITSTEXT3__STATIC },
    { "CROSSHAIR",           DVPTR(CROSSHAIR),           CROSSHAIR__STATIC },
    { "CRYSTALAMMO",         DVPTR(CRYSTALAMMO),         CRYSTALAMMO__STATIC },
    { "CYCLER",              DVPTR(CYCLER),              CYCLER__STATIC },
    { "DEVISTATOR",          DVPTR(DEVISTATOR),          DEVISTATOR__STATIC },
    { "DEVISTATORAMMO",      DVPTR(DEVISTATORAMMO),      DEVISTATORAMMO__STATIC },
    { "DEVISTATORSPRITE",    DVPTR(DEVISTATORSPRITE),    DEVISTATORSPRITE__STATIC },
    { "DIGITALNUM",          DVPTR(DIGITALNUM),          DIGITALNUM__STATIC },
    { "DIPSWITCH",           DVPTR(DIPSWITCH),           DIPSWITCH__STATIC },
    { "DIPSWITCH2",          DVPTR(DIPSWITCH2),          DIPSWITCH2__STATIC },
    { "DIPSWITCH3",          DVPTR(DIPSWITCH3),          DIPSWITCH3__STATIC },
    { "DOLPHIN1",            DVPTR(DOLPHIN1),            DOLPHIN1__STATIC },
    { "DOLPHIN2",            DVPTR(DOLPHIN2),            DOLPHIN2__STATIC },
    { "DOMELITE",            DVPTR(DOMELITE),            DOMELITE__STATIC },
    { "DOORSHOCK",           DVPTR(DOORSHOCK),           DOORSHOCK__STATIC },
    { "DOORTILE1",           DVPTR(DOORTILE1),           DOORTILE1__STATIC },
    { "DOORTILE10",          DVPTR(DOORTILE10),          DOORTILE10__STATIC },
    { "DOORTILE11",          DVPTR(DOORTILE11),          DOORTILE11__STATIC },
    { "DOORTILE12",          DVPTR(DOORTILE12),          DOORTILE12__STATIC },
    { "DOORTILE14",          DVPTR(DOORTILE14),          DOORTILE14__STATIC },
    { "DOORTILE15",          DVPTR(DOORTILE15),          DOORTILE15__STATIC },
    { "DOORTILE16",          DVPTR(DOORTILE16),          DOORTILE16__STATIC },
    { "DOORTILE17",          DVPTR(DOORTILE17),          DOORTILE17__STATIC },
    { "DOORTILE18",          DVPTR(DOORTILE18),          DOORTILE18__STATIC },
    { "DOORTILE19",          DVPTR(DOORTILE19),          DOORTILE19__STATIC },
    { "DOORTILE2",           DVPTR(DOORTILE2),           DOORTILE2__STATIC },
    { "DOORTILE20",          DVPTR(DOORTILE20),          DOORTILE20__STATIC },
    { "DOORTILE21",          DVPTR(DOORTILE21),          DOORTILE21__STATIC },
    { "DOORTILE22",          DVPTR(DOORTILE22),          DOORTILE22__STATIC },
    { "DOORTILE23",          DVPTR(DOORTILE23),          DOORTILE23__STATIC },
    { "DOORTILE3",           DVPTR(DOORTILE3),           DOORTILE3__STATIC },
    { "DOORTILE4",           DVPTR(DOORTILE4),           DOORTILE4__STATIC },
    { "DOORTILE5",           DVPTR(DOORTILE5),           DOORTILE5__STATIC },
    { "DOORTILE6",           DVPTR(DOORTILE6),           DOORTILE6__STATIC },
    { "DOORTILE7",           DVPTR(DOORTILE7),           DOORTILE7__STATIC },
    { "DOORTILE8",           DVPTR(DOORTILE8),           DOORTILE8__STATIC },
    { "DOORTILE9",           DVPTR(DOORTILE9),           DOORTILE9__STATIC },
    { "DREALMS",             DVPTR(DREALMS),             DREALMS__STATIC },
    { "DRONE",               DVPTR(DRONE),               DRONE__STATIC },
    { "DUCK",                DVPTR(DUCK),                DUCK__STATIC },
    { "DUKECAR",             DVPTR(DUKECAR),             DUKECAR__STATIC },
    { "DUKEGUN",             DVPTR(DUKEGUN),             DUKEGUN__STATIC },
    { "DUKELEG",             DVPTR(DUKELEG),             DUKELEG__STATIC },
    { "DUKELYINGDEAD",       DVPTR(DUKELYINGDEAD),       DUKELYINGDEAD__STATIC },
    { "DUKENUKEM",           DVPTR(DUKENUKEM),           DUKENUKEM__STATIC },
    { "DUKETAG",             DVPTR(DUKETAG),             DUKETAG__STATIC },
    { "DUKETORSO",           DVPTR(DUKETORSO),           DUKETORSO__STATIC },
    { "EGG",                 DVPTR(EGG),                 EGG__STATIC },
    { "ENDALPHANUM",         DVPTR(ENDALPHANUM),         ENDALPHANUM__STATIC },
    { "EXPLODINGBARREL",     DVPTR(EXPLODINGBARREL),     EXPLODINGBARREL__STATIC },
    { "EXPLODINGBARREL2",    DVPTR(EXPLODINGBARREL2),    EXPLODINGBARREL2__STATIC },
    { "EXPLOSION2",          DVPTR(EXPLOSION2),          EXPLOSION2__STATIC },
    { "EXPLOSION2BOT",       DVPTR(EXPLOSION2BOT),       EXPLOSION2BOT__STATIC },
    { "F1HELP",              DVPTR(F1HELP),              F1HELP__STATIC },
    { "FANSHADOW",           DVPTR(FANSHADOW),           FANSHADOW__STATIC },
    { "FANSHADOWBROKE",      DVPTR(FANSHADOWBROKE),      FANSHADOWBROKE__STATIC },
    { "FANSPRITE",           DVPTR(FANSPRITE),           FANSPRITE__STATIC },
    { "FANSPRITEBROKE",      DVPTR(FANSPRITEBROKE),      FANSPRITEBROKE__STATIC },
    { "FECES",               DVPTR(FECES),               FECES__STATIC },
    { "FEM1",                DVPTR(FEM1),                FEM1__STATIC },
    { "FEM10",               DVPTR(FEM10),               FEM10__STATIC },
    { "FEM2",                DVPTR(FEM2),                FEM2__STATIC },
    { "FEM3",                DVPTR(FEM3),                FEM3__STATIC },
    { "FEM4",                DVPTR(FEM4),                FEM4__STATIC },
    { "FEM5",                DVPTR(FEM5),                FEM5__STATIC },
    { "FEM6",                DVPTR(FEM6),                FEM6__STATIC },
    { "FEM6PAD",             DVPTR(FEM6PAD),             FEM6PAD__STATIC },
    { "FEM7",                DVPTR(FEM7),                FEM7__STATIC },
    { "FEM8",                DVPTR(FEM8),                FEM8__STATIC },
    { "FEM9",                DVPTR(FEM9),                FEM9__STATIC },
    { "FEMMAG1",             DVPTR(FEMMAG1),             FEMMAG1__STATIC },
    { "FEMMAG2",             DVPTR(FEMMAG2),             FEMMAG2__STATIC },
    { "FEMPIC1",             DVPTR(FEMPIC1),             FEMPIC1__STATIC },
    { "FEMPIC2",             DVPTR(FEMPIC2),             FEMPIC2__STATIC },
    { "FEMPIC3",             DVPTR(FEMPIC3),             FEMPIC3__STATIC },
    { "FEMPIC4",             DVPTR(FEMPIC4),             FEMPIC4__STATIC },
    { "FEMPIC5",             DVPTR(FEMPIC5),             FEMPIC5__STATIC },
    { "FEMPIC6",             DVPTR(FEMPIC6),             FEMPIC6__STATIC },
    { "FEMPIC7",             DVPTR(FEMPIC7),             FEMPIC7__STATIC },
    { "FETUS",               DVPTR(FETUS),               FETUS__STATIC },
    { "FETUSBROKE",          DVPTR(FETUSBROKE),          FETUSBROKE__STATIC },
    { "FETUSJIB",            DVPTR(FETUSJIB),            FETUSJIB__STATIC },
    { "FIRE",                DVPTR(FIRE),                FIRE__STATIC },
    { "FIRE2",               DVPTR(FIRE2),               FIRE2__STATIC },
    { "FIREBARREL",          DVPTR(FIREBARREL),          FIREBARREL__STATIC },
    { "FIREEXT",             DVPTR(FIREEXT),             FIREEXT__STATIC },
    { "FIRELASER",           DVPTR(FIRELASER),           FIRELASER__STATIC },
    { "FIREVASE",            DVPTR(FIREVASE),            FIREVASE__STATIC },
    { "FIRSTAID",            DVPTR(FIRSTAID),            FIRSTAID__STATIC },
    { "FIRSTAID_ICON",       DVPTR(FIRSTAID_ICON),       FIRSTAID_ICON__STATIC },
    { "FIRSTGUN",            DVPTR(FIRSTGUN),            FIRSTGUN__STATIC },
    { "FIRSTGUNRELOAD",      DVPTR(FIRSTGUNRELOAD),      FIRSTGUNRELOAD__STATIC },
    { "FIRSTGUNSPRITE",      DVPTR(FIRSTGUNSPRITE),      FIRSTGUNSPRITE__STATIC },
    { "FIST",                DVPTR(FIST),                FIST__STATIC },
    { "FLOORFLAME",          DVPTR(FLOORFLAME),          FLOORFLAME__STATIC },
    { "FLOORPLASMA",         DVPTR(FLOORPLASMA),         FLOORPLASMA__STATIC },
    { "FLOORSLIME",          DVPTR(FLOORSLIME),          FLOORSLIME__STATIC },
    { "FOF",                 DVPTR(FOF),                 FOF__STATIC },
    { "FOODOBJECT16",        DVPTR(FOODOBJECT16),        FOODOBJECT16__STATIC },
    { "FOOTPRINTS",          DVPTR(FOOTPRINTS),          FOOTPRINTS__STATIC },
    { "FOOTPRINTS2",         DVPTR(FOOTPRINTS2),         FOOTPRINTS2__STATIC },
    { "FOOTPRINTS3",         DVPTR(FOOTPRINTS3),         FOOTPRINTS3__STATIC },
    { "FOOTPRINTS4",         DVPTR(FOOTPRINTS4),         FOOTPRINTS4__STATIC },
    { "FORCERIPPLE",         DVPTR(FORCERIPPLE),         FORCERIPPLE__STATIC },
    { "FORCESPHERE",         DVPTR(FORCESPHERE),         FORCESPHERE__STATIC },
    { "FRAGBAR",             DVPTR(FRAGBAR),             FRAGBAR__STATIC },
    { "FRAMEEFFECT1",        DVPTR(FRAMEEFFECT1),        FRAMEEFFECT1__STATIC },
    { "FRAMEEFFECT1_13",     DVPTR(FRAMEEFFECT1_13),     FRAMEEFFECT1_13__STATIC },
    { "FRANKENSTINESWITCH",  DVPTR(FRANKENSTINESWITCH),  FRANKENSTINESWITCH__STATIC },
    { "FREEZE",              DVPTR(FREEZE),              FREEZE__STATIC },
    { "FREEZEAMMO",          DVPTR(FREEZEAMMO),          FREEZEAMMO__STATIC },
    { "FREEZEBLAST",         DVPTR(FREEZEBLAST),         FREEZEBLAST__STATIC },
    { "FREEZESPRITE",        DVPTR(FREEZESPRITE),        FREEZESPRITE__STATIC },
    { "FUELPOD",             DVPTR(FUELPOD),             FUELPOD__STATIC },
    { "GENERICPOLE",         DVPTR(GENERICPOLE),         GENERICPOLE__STATIC },
    { "GENERICPOLE2",        DVPTR(GENERICPOLE2),        GENERICPOLE2__STATIC },
    { "GLASS",               DVPTR(GLASS),               GLASS__STATIC },
    { "GLASS2",              DVPTR(GLASS2),              GLASS2__STATIC },
    { "GLASSPIECES",         DVPTR(GLASSPIECES),         GLASSPIECES__STATIC },
    { "GPSPEED",             DVPTR(GPSPEED),             GPSPEED__STATIC },
    { "GRATE1",              DVPTR(GRATE1),              GRATE1__STATIC },
    { "GREENSLIME",          DVPTR(GREENSLIME),          GREENSLIME__STATIC },
    { "GROWAMMO",            DVPTR(GROWAMMO),            GROWAMMO__STATIC },
    { "GROWSPARK",           DVPTR(GROWSPARK),           GROWSPARK__STATIC },
    { "GROWSPRITEICON",      DVPTR(GROWSPRITEICON),      GROWSPRITEICON__STATIC },
    { "HANDHOLDINGACCESS",   DVPTR(HANDHOLDINGACCESS),   HANDHOLDINGACCESS__STATIC },
    { "HANDHOLDINGLASER",    DVPTR(HANDHOLDINGLASER),    HANDHOLDINGLASER__STATIC },
    { "HANDPRINTSWITCH",     DVPTR(HANDPRINTSWITCH),     HANDPRINTSWITCH__STATIC },
    { "HANDREMOTE",          DVPTR(HANDREMOTE),          HANDREMOTE__STATIC },
    { "HANDSWITCH",          DVPTR(HANDSWITCH),          HANDSWITCH__STATIC },
    { "HANDTHROW",           DVPTR(HANDTHROW),           HANDTHROW__STATIC },
    { "HANGLIGHT",           DVPTR(HANGLIGHT),           HANGLIGHT__STATIC },
    { "HBOMBAMMO",           DVPTR(HBOMBAMMO),           HBOMBAMMO__STATIC },
    { "HEADJIB1",            DVPTR(HEADJIB1),            HEADJIB1__STATIC },
    { "HEALTHBOX",           DVPTR(HEALTHBOX),           HEALTHBOX__STATIC },
    { "HEAT_ICON",           DVPTR(HEAT_ICON),           HEAT_ICON__STATIC },
    { "HEATSENSOR",          DVPTR(HEATSENSOR),          HEATSENSOR__STATIC },
    { "HEAVYHBOMB",          DVPTR(HEAVYHBOMB),          HEAVYHBOMB__STATIC },
    { "HELECOPT",            DVPTR(HELECOPT),            HELECOPT__STATIC },
    { "HOLODUKE",            DVPTR(HOLODUKE),            HOLODUKE__STATIC },
    { "HOLODUKE_ICON",       DVPTR(HOLODUKE_ICON),       HOLODUKE_ICON__STATIC },
    { "HORSEONSIDE",         DVPTR(HORSEONSIDE),         HORSEONSIDE__STATIC },
    { "HOTMEAT",             DVPTR(HOTMEAT),             HOTMEAT__STATIC },
    { "HURTRAIL",            DVPTR(HURTRAIL),            HURTRAIL__STATIC },
    { "HYDRENT",             DVPTR(HYDRENT),             HYDRENT__STATIC },
    { "HYDROPLANT",          DVPTR(HYDROPLANT),          HYDROPLANT__STATIC },
    { "INDY",                DVPTR(INDY),                INDY__STATIC },
    { "INGAMEDUKETHREEDEE",  DVPTR(INGAMEDUKETHREEDEE),  INGAMEDUKETHREEDEE__STATIC },
    { "INNERJAW",            DVPTR(INNERJAW),            INNERJAW__STATIC },
    { "INVENTORYBOX",        DVPTR(INVENTORYBOX),        INVENTORYBOX__STATIC },
    { "IVUNIT",              DVPTR(IVUNIT),              IVUNIT__STATIC },
    { "JETPACK",             DVPTR(JETPACK),             JETPACK__STATIC },
    { "JETPACK_ICON",        DVPTR(JETPACK_ICON),        JETPACK_ICON__STATIC },
    { "JIBS1",               DVPTR(JIBS1),               JIBS1__STATIC },
    { "JIBS2",               DVPTR(JIBS2),               JIBS2__STATIC },
    { "JIBS3",               DVPTR(JIBS3),               JIBS3__STATIC },
    { "JIBS4",               DVPTR(JIBS4),               JIBS4__STATIC },
    { "JIBS5",               DVPTR(JIBS5),               JIBS5__STATIC },
    { "JIBS6",               DVPTR(JIBS6),               JIBS6__STATIC },
    { "JURYGUY",             DVPTR(JURYGUY),             JURYGUY__STATIC },
    { "KILLSICON",           DVPTR(KILLSICON),           KILLSICON__STATIC },
    { "KNEE",                DVPTR(KNEE),                KNEE__STATIC },
    { "LA",                  DVPTR(LA),                  LA__STATIC },
    { "LASERLINE",           DVPTR(LASERLINE),           LASERLINE__STATIC },
    { "LASERSITE",           DVPTR(LASERSITE),           LASERSITE__STATIC },
    { "LEGJIB1",             DVPTR(LEGJIB1),             LEGJIB1__STATIC },
    { "LETTER",              DVPTR(LETTER),              LETTER__STATIC },
    { "LIGHTSWITCH",         DVPTR(LIGHTSWITCH),         LIGHTSWITCH__STATIC },
    { "LIGHTSWITCH2",        DVPTR(LIGHTSWITCH2),        LIGHTSWITCH2__STATIC },
    { "LIZMAN",              DVPTR(LIZMAN),              LIZMAN__STATIC },
    { "LIZMANARM1",          DVPTR(LIZMANARM1),          LIZMANARM1__STATIC },
    { "LIZMANFEEDING",       DVPTR(LIZMANFEEDING),       LIZMANFEEDING__STATIC },
    { "LIZMANHEAD1",         DVPTR(LIZMANHEAD1),         LIZMANHEAD1__STATIC },
    { "LIZMANJUMP",          DVPTR(LIZMANJUMP),          LIZMANJUMP__STATIC },
    { "LIZMANLEG1",          DVPTR(LIZMANLEG1),          LIZMANLEG1__STATIC },
    { "LIZMANSPITTING",      DVPTR(LIZMANSPITTING),      LIZMANSPITTING__STATIC },
    { "LIZMANSTAYPUT",       DVPTR(LIZMANSTAYPUT),       LIZMANSTAYPUT__STATIC },
    { "LIZTROOP",            DVPTR(LIZTROOP),            LIZTROOP__STATIC },
    { "LIZTROOPDUCKING",     DVPTR(LIZTROOPDUCKING),     LIZTROOPDUCKING__STATIC },
    { "LIZTROOPJETPACK",     DVPTR(LIZTROOPJETPACK),     LIZTROOPJETPACK__STATIC },
    { "LIZTROOPJUSTSIT",     DVPTR(LIZTROOPJUSTSIT),     LIZTROOPJUSTSIT__STATIC },
    { "LIZTROOPONTOILET",    DVPTR(LIZTROOPONTOILET),    LIZTROOPONTOILET__STATIC },
    { "LIZTROOPRUNNING",     DVPTR(LIZTROOPRUNNING),     LIZTROOPRUNNING__STATIC },
    { "LIZTROOPSHOOT",       DVPTR(LIZTROOPSHOOT),       LIZTROOPSHOOT__STATIC },
    { "LIZTROOPSTAYPUT",     DVPTR(LIZTROOPSTAYPUT),     LIZTROOPSTAYPUT__STATIC },
    { "LOADSCREEN",          DVPTR(LOADSCREEN),          LOADSCREEN__STATIC },
    { "LOCATORS",            DVPTR(LOCATORS),            LOCATORS__STATIC },
    { "LOCKSWITCH1",         DVPTR(LOCKSWITCH1),         LOCKSWITCH1__STATIC },
    { "LOOGIE",              DVPTR(LOOGIE),              LOOGIE__STATIC },
    { "LUKE",                DVPTR(LUKE),                LUKE__STATIC },
    { "MAIL",                DVPTR(MAIL),                MAIL__STATIC },
    { "MAN",                 DVPTR(MAN),                 MAN__STATIC },
    { "MAN2",                DVPTR(MAN2),                MAN2__STATIC },
    { "MASKWALL1",           DVPTR(MASKWALL1),           MASKWALL1__STATIC },
    { "MASKWALL10",          DVPTR(MASKWALL10),          MASKWALL10__STATIC },
    { "MASKWALL11",          DVPTR(MASKWALL11),          MASKWALL11__STATIC },
    { "MASKWALL12",          DVPTR(MASKWALL12),          MASKWALL12__STATIC },
    { "MASKWALL13",          DVPTR(MASKWALL13),          MASKWALL13__STATIC },
    { "MASKWALL14",          DVPTR(MASKWALL14),          MASKWALL14__STATIC },
    { "MASKWALL15",          DVPTR(MASKWALL15),          MASKWALL15__STATIC },
    { "MASKWALL2",           DVPTR(MASKWALL2),           MASKWALL2__STATIC },
    { "MASKWALL3",           DVPTR(MASKWALL3),           MASKWALL3__STATIC },
    { "MASKWALL4",           DVPTR(MASKWALL4),           MASKWALL4__STATIC },
    { "MASKWALL5",           DVPTR(MASKWALL5),           MASKWALL5__STATIC },
    { "MASKWALL6",           DVPTR(MASKWALL6),           MASKWALL6__STATIC },
    { "MASKWALL7",           DVPTR(MASKWALL7),           MASKWALL7__STATIC },
    { "MASKWALL8",           DVPTR(MASKWALL8),           MASKWALL8__STATIC },
    { "MASKWALL9",           DVPTR(MASKWALL9),           MASKWALL9__STATIC },
    { "MASTERSWITCH",        DVPTR(MASTERSWITCH),        MASTERSWITCH__STATIC },
    { "MENUBAR",             DVPTR(MENUBAR),             MENUBAR__STATIC },
    { "MENUSCREEN",          DVPTR(MENUSCREEN),          MENUSCREEN__STATIC },
    { "MIKE",                DVPTR(MIKE),                MIKE__STATIC },
    { "MINIFONT",            DVPTR(MINIFONT),            MINIFONT__STATIC },
    { "MIRROR",              DVPTR(MIRROR),              MIRROR__STATIC },
    { "MIRRORBROKE",         DVPTR(MIRRORBROKE),         MIRRORBROKE__STATIC },
    { "MONEY",               DVPTR(MONEY),               MONEY__STATIC },
    { "MONK",                DVPTR(MONK),                MONK__STATIC },
    { "MOONSKY1",            DVPTR(MOONSKY1),            MOONSKY1__STATIC },
    { "MORTER",              DVPTR(MORTER),              MORTER__STATIC },
    { "MOVIECAMERA",         DVPTR(MOVIECAMERA),         MOVIECAMERA__STATIC },
    { "MULTISWITCH",         DVPTR(MULTISWITCH),         MULTISWITCH__STATIC },
    { "MUSICANDSFX",         DVPTR(MUSICANDSFX),         MUSICANDSFX__STATIC },
    { "NAKED1",              DVPTR(NAKED1),              NAKED1__STATIC },
    { "NATURALLIGHTNING",    DVPTR(NATURALLIGHTNING),    NATURALLIGHTNING__STATIC },
    { "NEON1",               DVPTR(NEON1),               NEON1__STATIC },
    { "NEON2",               DVPTR(NEON2),               NEON2__STATIC },
    { "NEON3",               DVPTR(NEON3),               NEON3__STATIC },
    { "NEON4",               DVPTR(NEON4),               NEON4__STATIC },
    { "NEON5",               DVPTR(NEON5),               NEON5__STATIC },
    { "NEON6",               DVPTR(NEON6),               NEON6__STATIC },
    { "NEWBEAST",            DVPTR(NEWBEAST),            NEWBEAST__STATIC },
    { "NEWBEASTSTAYPUT",     DVPTR(NEWBEASTSTAYPUT),     NEWBEASTSTAYPUT__STATIC },
    { "NUKEBARREL",          DVPTR(NUKEBARREL),          NUKEBARREL__STATIC },
    { "NUKEBARRELDENTED",    DVPTR(NUKEBARRELDENTED),    NUKEBARRELDENTED__STATIC },
    { "NUKEBARRELLEAKED",    DVPTR(NUKEBARRELLEAKED),    NUKEBARRELLEAKED__STATIC },
    { "NUKEBUTTON",          DVPTR(NUKEBUTTON),          NUKEBUTTON__STATIC },
    { "OCEANSPRITE1",        DVPTR(OCEANSPRITE1),        OCEANSPRITE1__STATIC },
    { "OCEANSPRITE2",        DVPTR(OCEANSPRITE2),        OCEANSPRITE2__STATIC },
    { "OCEANSPRITE3",        DVPTR(OCEANSPRITE3),        OCEANSPRITE3__STATIC },
    { "OCEANSPRITE4",        DVPTR(OCEANSPRITE4),        OCEANSPRITE4__STATIC },
    { "OCEANSPRITE5",        DVPTR(OCEANSPRITE5),        OCEANSPRITE5__STATIC },
    { "OCTABRAIN",           DVPTR(OCTABRAIN),           OCTABRAIN__STATIC },
    { "OCTABRAINSTAYPUT",    DVPTR(OCTABRAINSTAYPUT),    OCTABRAINSTAYPUT__STATIC },
    { "OJ",                  DVPTR(OJ),                  OJ__STATIC },
    { "OOZ",                 DVPTR(OOZ),                 OOZ__STATIC },
    { "OOZ2",                DVPTR(OOZ2),                OOZ2__STATIC },
    { "OOZFILTER",           DVPTR(OOZFILTER),           OOZFILTER__STATIC },
    { "ORDERING",            DVPTR(ORDERING),            ORDERING__STATIC },
    { "ORGANTIC",            DVPTR(ORGANTIC),            ORGANTIC__STATIC },
    { "PANNEL1",             DVPTR(PANNEL1),             PANNEL1__STATIC },
    { "PANNEL2",             DVPTR(PANNEL2),             PANNEL2__STATIC },
    { "PANNEL3",             DVPTR(PANNEL3),             PANNEL3__STATIC },
    { "PAPER",               DVPTR(PAPER),               PAPER__STATIC },
    { "PIGCOP",              DVPTR(PIGCOP),              PIGCOP__STATIC },
    { "PIGCOPDIVE",          DVPTR(PIGCOPDIVE),          PIGCOPDIVE__STATIC },
    { "PIGCOPSTAYPUT",       DVPTR(PIGCOPSTAYPUT),       PIGCOPSTAYPUT__STATIC },
    { "PIPE1",               DVPTR(PIPE1),               PIPE1__STATIC },
    { "PIPE1B",              DVPTR(PIPE1B),              PIPE1B__STATIC },
    { "PIPE2",               DVPTR(PIPE2),               PIPE2__STATIC },
    { "PIPE2B",              DVPTR(PIPE2B),              PIPE2B__STATIC },
    { "PIPE3",               DVPTR(PIPE3),               PIPE3__STATIC },
    { "PIPE3B",              DVPTR(PIPE3B),              PIPE3B__STATIC },
    { "PIPE4",               DVPTR(PIPE4),               PIPE4__STATIC },
    { "PIPE4B",              DVPTR(PIPE4B),              PIPE4B__STATIC },
    { "PIPE5",               DVPTR(PIPE5),               PIPE5__STATIC },
    { "PIPE5B",              DVPTR(PIPE5B),              PIPE5B__STATIC },
    { "PIPE6",               DVPTR(PIPE6),               PIPE6__STATIC },
    { "PIPE6B",              DVPTR(PIPE6B),              PIPE6B__STATIC },
    { "PLAYERONWATER",       DVPTR(PLAYERONWATER),       PLAYERONWATER__STATIC },
    { "PLUG",                DVPTR(PLUG),                PLUG__STATIC },
    { "PLUTOPAKSPRITE",      DVPTR(PLUTOPAKSPRITE),      PLUTOPAKSPRITE__STATIC },
    { "POCKET",              DVPTR(POCKET),              POCKET__STATIC },
    { "PODFEM1",             DVPTR(PODFEM1),             PODFEM1__STATIC },
    { "POT1",                DVPTR(POT1),                POT1__STATIC },
    { "POT2",                DVPTR(POT2),                POT2__STATIC },
    { "POT3",                DVPTR(POT3),                POT3__STATIC },
    { "POWERSWITCH1",        DVPTR(POWERSWITCH1),        POWERSWITCH1__STATIC },
    { "POWERSWITCH2",        DVPTR(POWERSWITCH2),        POWERSWITCH2__STATIC },
    { "PUKE",                DVPTR(PUKE),                PUKE__STATIC },
    { "PULLSWITCH",          DVPTR(PULLSWITCH),          PULLSWITCH__STATIC },
    { "PURPLELAVA",          DVPTR(PURPLELAVA),          PURPLELAVA__STATIC },
    { "QUEBALL",             DVPTR(QUEBALL),             QUEBALL__STATIC },
    { "RADIUSEXPLOSION",     DVPTR(RADIUSEXPLOSION),     RADIUSEXPLOSION__STATIC },
    { "RAT",                 DVPTR(RAT),                 RAT__STATIC },
    { "REACTOR",             DVPTR(REACTOR),             REACTOR__STATIC },
    { "REACTOR2",            DVPTR(REACTOR2),            REACTOR2__STATIC },
    { "REACTOR2BURNT",       DVPTR(REACTOR2BURNT),       REACTOR2BURNT__STATIC },
    { "REACTOR2SPARK",       DVPTR(REACTOR2SPARK),       REACTOR2SPARK__STATIC },
    { "REACTORBURNT",        DVPTR(REACTORBURNT),        REACTORBURNT__STATIC },
    { "REACTORSPARK",        DVPTR(REACTORSPARK),        REACTORSPARK__STATIC },
    { "RECON",               DVPTR(RECON),               RECON__STATIC },
    { "RESPAWN",             DVPTR(RESPAWN),             RESPAWN__STATIC },
    { "RESPAWNMARKERGREEN",  DVPTR(RESPAWNMARKERGREEN),  RESPAWNMARKERGREEN__STATIC },
    { "RESPAWNMARKERRED",    DVPTR(RESPAWNMARKERRED),    RESPAWNMARKERRED__STATIC },
    { "RESPAWNMARKERYELLOW", DVPTR(RESPAWNMARKERYELLOW), RESPAWNMARKERYELLOW__STATIC },
    { "ROTATEGUN",           DVPTR(ROTATEGUN),           ROTATEGUN__STATIC },
    { "RPG",                 DVPTR(RPG),                 RPG__STATIC },
    { "RPGAMMO",             DVPTR(RPGAMMO),             RPGAMMO__STATIC },
    { "RPGGUN",              DVPTR(RPGGUN),              RPGGUN__STATIC },
    { "RPGSPRITE",           DVPTR(RPGSPRITE),           RPGSPRITE__STATIC },
    { "RUBBERCAN",           DVPTR(RUBBERCAN),           RUBBERCAN__STATIC },
    { "SATELITE",            DVPTR(SATELITE),            SATELITE__STATIC },
    { "SCALE",               DVPTR(SCALE),               SCALE__STATIC },
    { "SCRAP1",              DVPTR(SCRAP1),              SCRAP1__STATIC },
    { "SCRAP2",              DVPTR(SCRAP2),              SCRAP2__STATIC },
    { "SCRAP3",              DVPTR(SCRAP3),              SCRAP3__STATIC },
    { "SCRAP4",              DVPTR(SCRAP4),              SCRAP4__STATIC },
    { "SCRAP5",              DVPTR(SCRAP5),              SCRAP5__STATIC },
    { "SCRAP6",              DVPTR(SCRAP6),              SCRAP6__STATIC },
    { "SCREENBREAK1",        DVPTR(SCREENBREAK1),        SCREENBREAK1__STATIC },
    { "SCREENBREAK10",       DVPTR(SCREENBREAK10),       SCREENBREAK10__STATIC },
    { "SCREENBREAK11",       DVPTR(SCREENBREAK11),       SCREENBREAK11__STATIC },
    { "SCREENBREAK12",       DVPTR(SCREENBREAK12),       SCREENBREAK12__STATIC },
    { "SCREENBREAK13",       DVPTR(SCREENBREAK13),       SCREENBREAK13__STATIC },
    { "SCREENBREAK14",       DVPTR(SCREENBREAK14),       SCREENBREAK14__STATIC },
    { "SCREENBREAK15",       DVPTR(SCREENBREAK15),       SCREENBREAK15__STATIC },
    { "SCREENBREAK16",       DVPTR(SCREENBREAK16),       SCREENBREAK16__STATIC },
    { "SCREENBREAK17",       DVPTR(SCREENBREAK17),       SCREENBREAK17__STATIC },
    { "SCREENBREAK18",       DVPTR(SCREENBREAK18),       SCREENBREAK18__STATIC },
    { "SCREENBREAK19",       DVPTR(SCREENBREAK19),       SCREENBREAK19__STATIC },
    { "SCREENBREAK2",        DVPTR(SCREENBREAK2),        SCREENBREAK2__STATIC },
    { "SCREENBREAK3",        DVPTR(SCREENBREAK3),        SCREENBREAK3__STATIC },
    { "SCREENBREAK4",        DVPTR(SCREENBREAK4),        SCREENBREAK4__STATIC },
    { "SCREENBREAK5",        DVPTR(SCREENBREAK5),        SCREENBREAK5__STATIC },
    { "SCREENBREAK6",        DVPTR(SCREENBREAK6),        SCREENBREAK6__STATIC },
    { "SCREENBREAK7",        DVPTR(SCREENBREAK7),        SCREENBREAK7__STATIC },
    { "SCREENBREAK8",        DVPTR(SCREENBREAK8),        SCREENBREAK8__STATIC },
    { "SCREENBREAK9",        DVPTR(SCREENBREAK9),        SCREENBREAK9__STATIC },
    { "SCUBAMASK",           DVPTR(SCUBAMASK),           SCUBAMASK__STATIC },
    { "SECTOREFFECTOR",      DVPTR(SECTOREFFECTOR),      SECTOREFFECTOR__STATIC },
    { "SEENINE",             DVPTR(SEENINE),             SEENINE__STATIC },
    { "SEENINEDEAD",         DVPTR(SEENINEDEAD),         SEENINEDEAD__STATIC },
    { "SELECTDIR",           DVPTR(SELECTDIR),           SELECTDIR__STATIC },
    { "SHARK",               DVPTR(SHARK),               SHARK__STATIC },
    { "SHELL",               DVPTR(SHELL),               SHELL__STATIC },
    { "SHIELD",              DVPTR(SHIELD),              SHIELD__STATIC },
    { "SHOTGUN",             DVPTR(SHOTGUN),             SHOTGUN__STATIC },
    { "SHOTGUNAMMO",         DVPTR(SHOTGUNAMMO),         SHOTGUNAMMO__STATIC },
    { "SHOTGUNSHELL",        DVPTR(SHOTGUNSHELL),        SHOTGUNSHELL__STATIC },
    { "SHOTGUNSPRITE",       DVPTR(SHOTGUNSPRITE),       SHOTGUNSPRITE__STATIC },
    { "SHOTSPARK1",          DVPTR(SHOTSPARK1),          SHOTSPARK1__STATIC },
    { "SHRINKER",            DVPTR(SHRINKER),            SHRINKER__STATIC },
    { "SHRINKEREXPLOSION",   DVPTR(SHRINKEREXPLOSION),   SHRINKEREXPLOSION__STATIC },
    { "SHRINKERSPRITE",      DVPTR(SHRINKERSPRITE),      SHRINKERSPRITE__STATIC },
    { "SHRINKSPARK",         DVPTR(SHRINKSPARK),         SHRINKSPARK__STATIC },
    { "SIDEBOLT1",           DVPTR(SIDEBOLT1),           SIDEBOLT1__STATIC },
    { "SIGN1",               DVPTR(SIGN1),               SIGN1__STATIC },
    { "SIGN2",               DVPTR(SIGN2),               SIGN2__STATIC },
    { "SIXPAK",              DVPTR(SIXPAK),              SIXPAK__STATIC },
    { "SLIDEBAR",            DVPTR(SLIDEBAR),            SLIDEBAR__STATIC },
    { "SLOTDOOR",            DVPTR(SLOTDOOR),            SLOTDOOR__STATIC },
    { "SMALLFNTCURSOR",      DVPTR(SMALLFNTCURSOR),      SMALLFNTCURSOR__STATIC },
    { "SMALLSMOKE",          DVPTR(SMALLSMOKE),          SMALLSMOKE__STATIC },
    { "SOLARPANNEL",         DVPTR(SOLARPANNEL),         SOLARPANNEL__STATIC },
    { "SPACEDOORSWITCH",     DVPTR(SPACEDOORSWITCH),     SPACEDOORSWITCH__STATIC },
    { "SPACELIGHTSWITCH",    DVPTR(SPACELIGHTSWITCH),    SPACELIGHTSWITCH__STATIC },
    { "SPACEMARINE",         DVPTR(SPACEMARINE),         SPACEMARINE__STATIC },
    { "SPEAKER",             DVPTR(SPEAKER),             SPEAKER__STATIC },
    { "SPINNINGNUKEICON",    DVPTR(SPINNINGNUKEICON),    SPINNINGNUKEICON__STATIC },
    { "SPIT",                DVPTR(SPIT),                SPIT__STATIC },
    { "SPOTLITE",            DVPTR(SPOTLITE),            SPOTLITE__STATIC },
    { "STAINGLASS1",         DVPTR(STAINGLASS1),         STAINGLASS1__STATIC },
    { "STALL",               DVPTR(STALL),               STALL__STATIC },
    { "STALLBROKE",          DVPTR(STALLBROKE),          STALLBROKE__STATIC },
    { "STARTALPHANUM",       DVPTR(STARTALPHANUM),       STARTALPHANUM__STATIC },
    { "STATIC",              DVPTR(STATIC),              STATIC__STATIC },
    { "STATUE",              DVPTR(STATUE),              STATUE__STATIC },
    { "STATUEFLASH",         DVPTR(STATUEFLASH),         STATUEFLASH__STATIC },
    { "STEAM",               DVPTR(STEAM),               STEAM__STATIC },
    { "STEROIDS",            DVPTR(STEROIDS),            STEROIDS__STATIC },
    { "STEROIDS_ICON",       DVPTR(STEROIDS_ICON),       STEROIDS_ICON__STATIC },
    { "STRIPEBALL",          DVPTR(STRIPEBALL),          STRIPEBALL__STATIC },
    { "SUSHIPLATE1",         DVPTR(SUSHIPLATE1),         SUSHIPLATE1__STATIC },
    { "SUSHIPLATE2",         DVPTR(SUSHIPLATE2),         SUSHIPLATE2__STATIC },
    { "SUSHIPLATE3",         DVPTR(SUSHIPLATE3),         SUSHIPLATE3__STATIC },
    { "SUSHIPLATE4",         DVPTR(SUSHIPLATE4),         SUSHIPLATE4__STATIC },
    { "SUSHIPLATE5",         DVPTR(SUSHIPLATE5),         SUSHIPLATE5__STATIC },
    { "TAMPON",              DVPTR(TAMPON),              TAMPON__STATIC },
    { "TANK",                DVPTR(TANK),                TANK__STATIC },
    { "TARGET",              DVPTR(TARGET),              TARGET__STATIC },
    { "TECHLIGHT2",          DVPTR(TECHLIGHT2),          TECHLIGHT2__STATIC },
    { "TECHLIGHT4",          DVPTR(TECHLIGHT4),          TECHLIGHT4__STATIC },
    { "TECHLIGHTBUST2",      DVPTR(TECHLIGHTBUST2),      TECHLIGHTBUST2__STATIC },
    { "TECHLIGHTBUST4",      DVPTR(TECHLIGHTBUST4),      TECHLIGHTBUST4__STATIC },
    { "TECHSWITCH",          DVPTR(TECHSWITCH),          TECHSWITCH__STATIC },
    { "TENSCREEN",           DVPTR(TENSCREEN),           TENSCREEN__STATIC },
    { "TEXTBOX",             DVPTR(TEXTBOX),             TEXTBOX__STATIC },
    { "TEXTSTORY",           DVPTR(TEXTSTORY),           TEXTSTORY__STATIC },
    { "THREEBYFIVE",         DVPTR(THREEBYFIVE),         THREEBYFIVE__STATIC },
    { "THREEDEE",            DVPTR(THREEDEE),            THREEDEE__STATIC },
    { "TIP",                 DVPTR(TIP),                 TIP__STATIC },
    { "TIRE",                DVPTR(TIRE),                TIRE__STATIC },
    { "TOILET",              DVPTR(TOILET),              TOILET__STATIC },
    { "TOILETBROKE",         DVPTR(TOILETBROKE),         TOILETBROKE__STATIC },
    { "TOILETWATER",         DVPTR(TOILETWATER),         TOILETWATER__STATIC },
    { "TONGUE",              DVPTR(TONGUE),              TONGUE__STATIC },
    { "TOUCHPLATE",          DVPTR(TOUCHPLATE),          TOUCHPLATE__STATIC },
    { "TOUGHGAL",            DVPTR(TOUGHGAL),            TOUGHGAL__STATIC },
    { "TRANSPORTERBEAM",     DVPTR(TRANSPORTERBEAM),     TRANSPORTERBEAM__STATIC },
    { "TRANSPORTERSTAR",     DVPTR(TRANSPORTERSTAR),     TRANSPORTERSTAR__STATIC },
    { "TRASH",               DVPTR(TRASH),               TRASH__STATIC },
    { "TREE1",               DVPTR(TREE1),               TREE1__STATIC },
    { "TREE2",               DVPTR(TREE2),               TREE2__STATIC },
    { "TRIPBOMB",            DVPTR(TRIPBOMB),            TRIPBOMB__STATIC },
    { "TRIPBOMBSPRITE",      DVPTR(TRIPBOMBSPRITE),      TRIPBOMBSPRITE__STATIC },
    { "TRIPODCAMERA",        DVPTR(TRIPODCAMERA),        TRIPODCAMERA__STATIC },
    { "VACUUM",              DVPTR(VACUUM),              VACUUM__STATIC },
    { "VASE",                DVPTR(VASE),                VASE__STATIC },
    { "VENDMACHINE",         DVPTR(VENDMACHINE),         VENDMACHINE__STATIC },
    { "VICTORY1",            DVPTR(VICTORY1),            VICTORY1__STATIC },
    { "VIEWBORDER",          DVPTR(VIEWBORDER),          VIEWBORDER__STATIC },
    { "VIEWSCREEN",          DVPTR(VIEWSCREEN),          VIEWSCREEN__STATIC },
    { "VIEWSCREEN2",         DVPTR(VIEWSCREEN2),         VIEWSCREEN2__STATIC },
    { "W_FORCEFIELD",        DVPTR(W_FORCEFIELD),        W_FORCEFIELD__STATIC },
    { "W_HITTECHWALL1",      DVPTR(W_HITTECHWALL1),      W_HITTECHWALL1__STATIC },
    { "W_HITTECHWALL10",     DVPTR(W_HITTECHWALL10),     W_HITTECHWALL10__STATIC },
    { "W_HITTECHWALL15",     DVPTR(W_HITTECHWALL15),     W_HITTECHWALL15__STATIC },
    { "W_HITTECHWALL16",     DVPTR(W_HITTECHWALL16),     W_HITTECHWALL16__STATIC },
    { "W_HITTECHWALL2",      DVPTR(W_HITTECHWALL2),      W_HITTECHWALL2__STATIC },
    { "W_HITTECHWALL3",      DVPTR(W_HITTECHWALL3),      W_HITTECHWALL3__STATIC },
    { "W_HITTECHWALL4",      DVPTR(W_HITTECHWALL4),      W_HITTECHWALL4__STATIC },
    { "W_MILKSHELF",         DVPTR(W_MILKSHELF),         W_MILKSHELF__STATIC },
    { "W_MILKSHELFBROKE",    DVPTR(W_MILKSHELFBROKE),    W_MILKSHELFBROKE__STATIC },
    { "W_NUMBERS",           DVPTR(W_NUMBERS),           W_NUMBERS__STATIC },
    { "W_SCREENBREAK",       DVPTR(W_SCREENBREAK),       W_SCREENBREAK__STATIC },
    { "W_TECHWALL1",         DVPTR(W_TECHWALL1),         W_TECHWALL1__STATIC },
    { "W_TECHWALL10",        DVPTR(W_TECHWALL10),        W_TECHWALL10__STATIC },
    { "W_TECHWALL11",        DVPTR(W_TECHWALL11),        W_TECHWALL11__STATIC },
    { "W_TECHWALL12",        DVPTR(W_TECHWALL12),        W_TECHWALL12__STATIC },
    { "W_TECHWALL13",        DVPTR(W_TECHWALL13),        W_TECHWALL13__STATIC },
    { "W_TECHWALL14",        DVPTR(W_TECHWALL14),        W_TECHWALL14__STATIC },
    { "W_TECHWALL15",        DVPTR(W_TECHWALL15),        W_TECHWALL15__STATIC },
    { "W_TECHWALL16",        DVPTR(W_TECHWALL16),        W_TECHWALL16__STATIC },
    { "W_TECHWALL2",         DVPTR(W_TECHWALL2),         W_TECHWALL2__STATIC },
    { "W_TECHWALL3",         DVPTR(W_TECHWALL3),         W_TECHWALL3__STATIC },
    { "W_TECHWALL4",         DVPTR(W_TECHWALL4),         W_TECHWALL4__STATIC },
    { "W_TECHWALL5",         DVPTR(W_TECHWALL5),         W_TECHWALL5__STATIC },
    { "W_TECHWALL6",         DVPTR(W_TECHWALL6),         W_TECHWALL6__STATIC },
    { "W_TECHWALL7",         DVPTR(W_TECHWALL7),         W_TECHWALL7__STATIC },
    { "W_TECHWALL8",         DVPTR(W_TECHWALL8),         W_TECHWALL8__STATIC },
    { "W_TECHWALL9",         DVPTR(W_TECHWALL9),         W_TECHWALL9__STATIC },
    { "WAITTOBESEATED",      DVPTR(WAITTOBESEATED),      WAITTOBESEATED__STATIC },
    { "WALLBLOOD1",          DVPTR(WALLBLOOD1),          WALLBLOOD1__STATIC },
    { "WALLBLOOD2",          DVPTR(WALLBLOOD2),          WALLBLOOD2__STATIC },
    { "WALLBLOOD3",          DVPTR(WALLBLOOD3),          WALLBLOOD3__STATIC },
    { "WALLBLOOD4",          DVPTR(WALLBLOOD4),          WALLBLOOD4__STATIC },
    { "WALLBLOOD5",          DVPTR(WALLBLOOD5),          WALLBLOOD5__STATIC },
    { "WALLBLOOD7",          DVPTR(WALLBLOOD7),          WALLBLOOD7__STATIC },
    { "WALLBLOOD8",          DVPTR(WALLBLOOD8),          WALLBLOOD8__STATIC },
    { "WALLLIGHT1",          DVPTR(WALLLIGHT1),          WALLLIGHT1__STATIC },
    { "WALLLIGHT2",          DVPTR(WALLLIGHT2),          WALLLIGHT2__STATIC },
    { "WALLLIGHT3",          DVPTR(WALLLIGHT3),          WALLLIGHT3__STATIC },
    { "WALLLIGHT4",          DVPTR(WALLLIGHT4),          WALLLIGHT4__STATIC },
    { "WALLLIGHTBUST1",      DVPTR(WALLLIGHTBUST1),      WALLLIGHTBUST1__STATIC },
    { "WALLLIGHTBUST2",      DVPTR(WALLLIGHTBUST2),      WALLLIGHTBUST2__STATIC },
    { "WALLLIGHTBUST3",      DVPTR(WALLLIGHTBUST3),      WALLLIGHTBUST3__STATIC },
    { "WALLLIGHTBUST4",      DVPTR(WALLLIGHTBUST4),      WALLLIGHTBUST4__STATIC },
    { "WATERBUBBLE",         DVPTR(WATERBUBBLE),         WATERBUBBLE__STATIC },
    { "WATERBUBBLEMAKER",    DVPTR(WATERBUBBLEMAKER),    WATERBUBBLEMAKER__STATIC },
    { "WATERDRIP",           DVPTR(WATERDRIP),           WATERDRIP__STATIC },
    { "WATERDRIPSPLASH",     DVPTR(WATERDRIPSPLASH),     WATERDRIPSPLASH__STATIC },
    { "WATERFOUNTAIN",       DVPTR(WATERFOUNTAIN),       WATERFOUNTAIN__STATIC },
    { "WATERFOUNTAINBROKE",  DVPTR(WATERFOUNTAINBROKE),  WATERFOUNTAINBROKE__STATIC },
    { "WATERSPLASH2",        DVPTR(WATERSPLASH2),        WATERSPLASH2__STATIC },
    { "WATERTILE2",          DVPTR(WATERTILE2),          WATERTILE2__STATIC },
    { "WEATHERWARN",         DVPTR(WEATHERWARN),         WEATHERWARN__STATIC },
    { "WINDOWBORDER1",       DVPTR(WINDOWBORDER1),       WINDOWBORDER1__STATIC },
    { "WINDOWBORDER2",       DVPTR(WINDOWBORDER2),       WINDOWBORDER2__STATIC },
    { "WOMAN",               DVPTR(WOMAN),               WOMAN__STATIC },
    { "WOODENHORSE",         DVPTR(WOODENHORSE),         WOODENHORSE__STATIC },
    { "XXXSTACY",            DVPTR(XXXSTACY),            XXXSTACY__STATIC },
 };


int32_t ACCESS_ICON         = ACCESS_ICON__STATIC;
int32_t ACCESSCARD          = ACCESSCARD__STATIC;
int32_t ACCESSSWITCH        = ACCESSSWITCH__STATIC;
int32_t ACCESSSWITCH2       = ACCESSSWITCH2__STATIC;
int32_t ACTIVATOR           = ACTIVATOR__STATIC;
int32_t ACTIVATORLOCKED     = ACTIVATORLOCKED__STATIC;
int32_t AIRTANK             = AIRTANK__STATIC;
int32_t AIRTANK_ICON        = AIRTANK_ICON__STATIC;
int32_t ALIENSWITCH         = ALIENSWITCH__STATIC;
int32_t AMMO                = AMMO__STATIC;
int32_t AMMOBOX             = AMMOBOX__STATIC;
int32_t AMMOLOTS            = AMMOLOTS__STATIC;
int32_t ANTENNA             = ANTENNA__STATIC;
int32_t APLAYER             = APLAYER__STATIC;
int32_t APLAYERTOP          = APLAYERTOP__STATIC;
int32_t ARMJIB1             = ARMJIB1__STATIC;
int32_t ARROW               = ARROW__STATIC;
int32_t ATM                 = ATM__STATIC;
int32_t ATMBROKE            = ATMBROKE__STATIC;
int32_t ATOMICHEALTH        = ATOMICHEALTH__STATIC;
int32_t BANNER              = BANNER__STATIC;
int32_t BARBROKE            = BARBROKE__STATIC;
int32_t BATTERYAMMO         = BATTERYAMMO__STATIC;
int32_t BETASCREEN          = BETASCREEN__STATIC;
int32_t BETAVERSION         = BETAVERSION__STATIC;
int32_t BGRATE1             = BGRATE1__STATIC;
int32_t BIGALPHANUM         = BIGALPHANUM__STATIC;
int32_t BIGAPPOS            = BIGAPPOS__STATIC;
int32_t BIGCOLIN            = BIGCOLIN__STATIC;
int32_t BIGCOMMA            = BIGCOMMA__STATIC;
int32_t BIGFORCE            = BIGFORCE__STATIC;
int32_t BIGHOLE             = BIGHOLE__STATIC;
int32_t BIGORBIT1           = BIGORBIT1__STATIC;
int32_t BIGPERIOD           = BIGPERIOD__STATIC;
int32_t BIGQ                = BIGQ__STATIC;
int32_t BIGSEMI             = BIGSEMI__STATIC;
int32_t BIGX_               = BIGX__STATIC; // "BIGX" clashes on the Wii?
int32_t BLANKSCREEN         = BLANKSCREEN__STATIC;
int32_t BLIMP               = BLIMP__STATIC;
int32_t BLOOD               = BLOOD__STATIC;
int32_t BLOODPOOL           = BLOODPOOL__STATIC;
int32_t BLOODSPLAT1         = BLOODSPLAT1__STATIC;
int32_t BLOODSPLAT2         = BLOODSPLAT2__STATIC;
int32_t BLOODSPLAT3         = BLOODSPLAT3__STATIC;
int32_t BLOODSPLAT4         = BLOODSPLAT4__STATIC;
int32_t BLOODYPOLE          = BLOODYPOLE__STATIC;
int32_t BOLT1               = BOLT1__STATIC;
int32_t BONUSSCREEN         = BONUSSCREEN__STATIC;
int32_t BOOT_ICON           = BOOT_ICON__STATIC;
int32_t BOOTS               = BOOTS__STATIC;
int32_t BORNTOBEWILDSCREEN  = BORNTOBEWILDSCREEN__STATIC;
int32_t BOSS1               = BOSS1__STATIC;
int32_t BOSS1LOB            = BOSS1LOB__STATIC;
int32_t BOSS1SHOOT          = BOSS1SHOOT__STATIC;
int32_t BOSS1STAYPUT        = BOSS1STAYPUT__STATIC;
int32_t BOSS2               = BOSS2__STATIC;
int32_t BOSS3               = BOSS3__STATIC;
int32_t BOSS4               = BOSS4__STATIC;
int32_t BOSS4STAYPUT        = BOSS4STAYPUT__STATIC;
int32_t BOSSTOP             = BOSSTOP__STATIC;
int32_t BOTTLE1             = BOTTLE1__STATIC;
int32_t BOTTLE10            = BOTTLE10__STATIC;
int32_t BOTTLE11            = BOTTLE11__STATIC;
int32_t BOTTLE12            = BOTTLE12__STATIC;
int32_t BOTTLE13            = BOTTLE13__STATIC;
int32_t BOTTLE14            = BOTTLE14__STATIC;
int32_t BOTTLE15            = BOTTLE15__STATIC;
int32_t BOTTLE16            = BOTTLE16__STATIC;
int32_t BOTTLE17            = BOTTLE17__STATIC;
int32_t BOTTLE18            = BOTTLE18__STATIC;
int32_t BOTTLE19            = BOTTLE19__STATIC;
int32_t BOTTLE2             = BOTTLE2__STATIC;
int32_t BOTTLE3             = BOTTLE3__STATIC;
int32_t BOTTLE4             = BOTTLE4__STATIC;
int32_t BOTTLE5             = BOTTLE5__STATIC;
int32_t BOTTLE6             = BOTTLE6__STATIC;
int32_t BOTTLE7             = BOTTLE7__STATIC;
int32_t BOTTLE8             = BOTTLE8__STATIC;
int32_t BOTTOMSTATUSBAR     = BOTTOMSTATUSBAR__STATIC;
int32_t BOUNCEMINE          = BOUNCEMINE__STATIC;
int32_t BOX                 = BOX__STATIC;
int32_t BPANNEL1            = BPANNEL1__STATIC;
int32_t BPANNEL3            = BPANNEL3__STATIC;
int32_t BROKEFIREHYDRENT    = BROKEFIREHYDRENT__STATIC;
int32_t BROKEHYDROPLANT     = BROKEHYDROPLANT__STATIC;
int32_t BROKENCHAIR         = BROKENCHAIR__STATIC;
int32_t BULLETHOLE          = BULLETHOLE__STATIC;
int32_t BURNING             = BURNING__STATIC;
int32_t BURNING2            = BURNING2__STATIC;
int32_t CACTUS              = CACTUS__STATIC;
int32_t CACTUSBROKE         = CACTUSBROKE__STATIC;
int32_t CAMCORNER           = CAMCORNER__STATIC;
int32_t CAMERA1             = CAMERA1__STATIC;
int32_t CAMERALIGHT         = CAMERALIGHT__STATIC;
int32_t CAMERAPOLE          = CAMERAPOLE__STATIC;
int32_t CAMLIGHT            = CAMLIGHT__STATIC;
int32_t CANWITHSOMETHING    = CANWITHSOMETHING__STATIC;
int32_t CANWITHSOMETHING2   = CANWITHSOMETHING2__STATIC;
int32_t CANWITHSOMETHING3   = CANWITHSOMETHING3__STATIC;
int32_t CANWITHSOMETHING4   = CANWITHSOMETHING4__STATIC;
int32_t CEILINGSTEAM        = CEILINGSTEAM__STATIC;
int32_t CHAINGUN            = CHAINGUN__STATIC;
int32_t CHAINGUNSPRITE      = CHAINGUNSPRITE__STATIC;
int32_t CHAIR1              = CHAIR1__STATIC;
int32_t CHAIR2              = CHAIR2__STATIC;
int32_t CHAIR3              = CHAIR3__STATIC;
int32_t CIRCLEPANNEL        = CIRCLEPANNEL__STATIC;
int32_t CIRCLEPANNELBROKE   = CIRCLEPANNELBROKE__STATIC;
int32_t CLOUDYOCEAN         = CLOUDYOCEAN__STATIC;
int32_t CLOUDYSKIES         = CLOUDYSKIES__STATIC;
int32_t COLA                = COLA__STATIC;
int32_t COLAMACHINE         = COLAMACHINE__STATIC;
int32_t COMMANDER           = COMMANDER__STATIC;
int32_t COMMANDERSTAYPUT    = COMMANDERSTAYPUT__STATIC;
int32_t CONE                = CONE__STATIC;
int32_t COOLEXPLOSION1      = COOLEXPLOSION1__STATIC;
int32_t CRACK1              = CRACK1__STATIC;
int32_t CRACK2              = CRACK2__STATIC;
int32_t CRACK3              = CRACK3__STATIC;
int32_t CRACK4              = CRACK4__STATIC;
int32_t CRACKKNUCKLES       = CRACKKNUCKLES__STATIC;
int32_t CRANE               = CRANE__STATIC;
int32_t CRANEPOLE           = CRANEPOLE__STATIC;
int32_t CREDITSTEXT1        = CREDITSTEXT1__STATIC;
int32_t CREDITSTEXT2        = CREDITSTEXT2__STATIC;
int32_t CREDITSTEXT3        = CREDITSTEXT3__STATIC;
int32_t CROSSHAIR           = CROSSHAIR__STATIC;
int32_t CRYSTALAMMO         = CRYSTALAMMO__STATIC;
int32_t CYCLER              = CYCLER__STATIC;
int32_t DEVISTATOR          = DEVISTATOR__STATIC;
int32_t DEVISTATORAMMO      = DEVISTATORAMMO__STATIC;
int32_t DEVISTATORSPRITE    = DEVISTATORSPRITE__STATIC;
int32_t DIGITALNUM          = DIGITALNUM__STATIC;
int32_t DIPSWITCH           = DIPSWITCH__STATIC;
int32_t DIPSWITCH2          = DIPSWITCH2__STATIC;
int32_t DIPSWITCH3          = DIPSWITCH3__STATIC;
int32_t DOLPHIN1            = DOLPHIN1__STATIC;
int32_t DOLPHIN2            = DOLPHIN2__STATIC;
int32_t DOMELITE            = DOMELITE__STATIC;
int32_t DOORSHOCK           = DOORSHOCK__STATIC;
int32_t DOORTILE1           = DOORTILE1__STATIC;
int32_t DOORTILE10          = DOORTILE10__STATIC;
int32_t DOORTILE11          = DOORTILE11__STATIC;
int32_t DOORTILE12          = DOORTILE12__STATIC;
int32_t DOORTILE14          = DOORTILE14__STATIC;
int32_t DOORTILE15          = DOORTILE15__STATIC;
int32_t DOORTILE16          = DOORTILE16__STATIC;
int32_t DOORTILE17          = DOORTILE17__STATIC;
int32_t DOORTILE18          = DOORTILE18__STATIC;
int32_t DOORTILE19          = DOORTILE19__STATIC;
int32_t DOORTILE2           = DOORTILE2__STATIC;
int32_t DOORTILE20          = DOORTILE20__STATIC;
int32_t DOORTILE21          = DOORTILE21__STATIC;
int32_t DOORTILE22          = DOORTILE22__STATIC;
int32_t DOORTILE23          = DOORTILE23__STATIC;
int32_t DOORTILE3           = DOORTILE3__STATIC;
int32_t DOORTILE4           = DOORTILE4__STATIC;
int32_t DOORTILE5           = DOORTILE5__STATIC;
int32_t DOORTILE6           = DOORTILE6__STATIC;
int32_t DOORTILE7           = DOORTILE7__STATIC;
int32_t DOORTILE8           = DOORTILE8__STATIC;
int32_t DOORTILE9           = DOORTILE9__STATIC;
int32_t DREALMS             = DREALMS__STATIC;
int32_t DRONE               = DRONE__STATIC;
int32_t DUCK                = DUCK__STATIC;
int32_t DUKECAR             = DUKECAR__STATIC;
int32_t DUKEGUN             = DUKEGUN__STATIC;
int32_t DUKELEG             = DUKELEG__STATIC;
int32_t DUKELYINGDEAD       = DUKELYINGDEAD__STATIC;
int32_t DUKENUKEM           = DUKENUKEM__STATIC;
int32_t DUKETAG             = DUKETAG__STATIC;
int32_t DUKETORSO           = DUKETORSO__STATIC;
int32_t EGG                 = EGG__STATIC;
int32_t ENDALPHANUM         = ENDALPHANUM__STATIC;
int32_t EXPLODINGBARREL     = EXPLODINGBARREL__STATIC;
int32_t EXPLODINGBARREL2    = EXPLODINGBARREL2__STATIC;
int32_t EXPLOSION2          = EXPLOSION2__STATIC;
int32_t EXPLOSION2BOT       = EXPLOSION2BOT__STATIC;
int32_t F1HELP              = F1HELP__STATIC;
int32_t FANSHADOW           = FANSHADOW__STATIC;
int32_t FANSHADOWBROKE      = FANSHADOWBROKE__STATIC;
int32_t FANSPRITE           = FANSPRITE__STATIC;
int32_t FANSPRITEBROKE      = FANSPRITEBROKE__STATIC;
int32_t FECES               = FECES__STATIC;
int32_t FEM1                = FEM1__STATIC;
int32_t FEM10               = FEM10__STATIC;
int32_t FEM2                = FEM2__STATIC;
int32_t FEM3                = FEM3__STATIC;
int32_t FEM4                = FEM4__STATIC;
int32_t FEM5                = FEM5__STATIC;
int32_t FEM6                = FEM6__STATIC;
int32_t FEM6PAD             = FEM6PAD__STATIC;
int32_t FEM7                = FEM7__STATIC;
int32_t FEM8                = FEM8__STATIC;
int32_t FEM9                = FEM9__STATIC;
int32_t FEMMAG1             = FEMMAG1__STATIC;
int32_t FEMMAG2             = FEMMAG2__STATIC;
int32_t FEMPIC1             = FEMPIC1__STATIC;
int32_t FEMPIC2             = FEMPIC2__STATIC;
int32_t FEMPIC3             = FEMPIC3__STATIC;
int32_t FEMPIC4             = FEMPIC4__STATIC;
int32_t FEMPIC5             = FEMPIC5__STATIC;
int32_t FEMPIC6             = FEMPIC6__STATIC;
int32_t FEMPIC7             = FEMPIC7__STATIC;
int32_t FETUS               = FETUS__STATIC;
int32_t FETUSBROKE          = FETUSBROKE__STATIC;
int32_t FETUSJIB            = FETUSJIB__STATIC;
int32_t FIRE                = FIRE__STATIC;
int32_t FIRE2               = FIRE2__STATIC;
int32_t FIREBARREL          = FIREBARREL__STATIC;
int32_t FIREEXT             = FIREEXT__STATIC;
int32_t FIRELASER           = FIRELASER__STATIC;
int32_t FIREVASE            = FIREVASE__STATIC;
int32_t FIRSTAID            = FIRSTAID__STATIC;
int32_t FIRSTAID_ICON       = FIRSTAID_ICON__STATIC;
int32_t FIRSTGUN            = FIRSTGUN__STATIC;
int32_t FIRSTGUNRELOAD      = FIRSTGUNRELOAD__STATIC;
int32_t FIRSTGUNSPRITE      = FIRSTGUNSPRITE__STATIC;
int32_t FIST                = FIST__STATIC;
int32_t FLOORFLAME          = FLOORFLAME__STATIC;
int32_t FLOORPLASMA         = FLOORPLASMA__STATIC;
int32_t FLOORSLIME          = FLOORSLIME__STATIC;
int32_t FOF                 = FOF__STATIC;
int32_t FOODOBJECT16        = FOODOBJECT16__STATIC;
int32_t FOOTPRINTS          = FOOTPRINTS__STATIC;
int32_t FOOTPRINTS2         = FOOTPRINTS2__STATIC;
int32_t FOOTPRINTS3         = FOOTPRINTS3__STATIC;
int32_t FOOTPRINTS4         = FOOTPRINTS4__STATIC;
int32_t FORCERIPPLE         = FORCERIPPLE__STATIC;
int32_t FORCESPHERE         = FORCESPHERE__STATIC;
int32_t FRAGBAR             = FRAGBAR__STATIC;
int32_t FRAMEEFFECT1        = FRAMEEFFECT1__STATIC;
int32_t FRAMEEFFECT1_13     = FRAMEEFFECT1_13__STATIC;
int32_t FRANKENSTINESWITCH  = FRANKENSTINESWITCH__STATIC;
int32_t FREEZE              = FREEZE__STATIC;
int32_t FREEZEAMMO          = FREEZEAMMO__STATIC;
int32_t FREEZEBLAST         = FREEZEBLAST__STATIC;
int32_t FREEZESPRITE        = FREEZESPRITE__STATIC;
int32_t FUELPOD             = FUELPOD__STATIC;
int32_t GENERICPOLE         = GENERICPOLE__STATIC;
int32_t GENERICPOLE2        = GENERICPOLE2__STATIC;
int32_t GLASS               = GLASS__STATIC;
int32_t GLASS2              = GLASS2__STATIC;
int32_t GLASSPIECES         = GLASSPIECES__STATIC;
int32_t GPSPEED             = GPSPEED__STATIC;
int32_t GRATE1              = GRATE1__STATIC;
int32_t GREENSLIME          = GREENSLIME__STATIC;
int32_t GROWAMMO            = GROWAMMO__STATIC;
int32_t GROWSPARK           = GROWSPARK__STATIC;
int32_t GROWSPRITEICON      = GROWSPRITEICON__STATIC;
int32_t HANDHOLDINGACCESS   = HANDHOLDINGACCESS__STATIC;
int32_t HANDHOLDINGLASER    = HANDHOLDINGLASER__STATIC;
int32_t HANDPRINTSWITCH     = HANDPRINTSWITCH__STATIC;
int32_t HANDREMOTE          = HANDREMOTE__STATIC;
int32_t HANDSWITCH          = HANDSWITCH__STATIC;
int32_t HANDTHROW           = HANDTHROW__STATIC;
int32_t HANGLIGHT           = HANGLIGHT__STATIC;
int32_t HBOMBAMMO           = HBOMBAMMO__STATIC;
int32_t HEADJIB1            = HEADJIB1__STATIC;
int32_t HEALTHBOX           = HEALTHBOX__STATIC;
int32_t HEAT_ICON           = HEAT_ICON__STATIC;
int32_t HEATSENSOR          = HEATSENSOR__STATIC;
int32_t HEAVYHBOMB          = HEAVYHBOMB__STATIC;
int32_t HELECOPT            = HELECOPT__STATIC;
int32_t HOLODUKE            = HOLODUKE__STATIC;
int32_t HOLODUKE_ICON       = HOLODUKE_ICON__STATIC;
int32_t HORSEONSIDE         = HORSEONSIDE__STATIC;
int32_t HOTMEAT             = HOTMEAT__STATIC;
int32_t HURTRAIL            = HURTRAIL__STATIC;
int32_t HYDRENT             = HYDRENT__STATIC;
int32_t HYDROPLANT          = HYDROPLANT__STATIC;
int32_t INDY                = INDY__STATIC;
int32_t INGAMEDUKETHREEDEE  = INGAMEDUKETHREEDEE__STATIC;
int32_t INNERJAW            = INNERJAW__STATIC;
int32_t INVENTORYBOX        = INVENTORYBOX__STATIC;
int32_t IVUNIT              = IVUNIT__STATIC;
int32_t JETPACK             = JETPACK__STATIC;
int32_t JETPACK_ICON        = JETPACK_ICON__STATIC;
int32_t JIBS1               = JIBS1__STATIC;
int32_t JIBS2               = JIBS2__STATIC;
int32_t JIBS3               = JIBS3__STATIC;
int32_t JIBS4               = JIBS4__STATIC;
int32_t JIBS5               = JIBS5__STATIC;
int32_t JIBS6               = JIBS6__STATIC;
int32_t JURYGUY             = JURYGUY__STATIC;
int32_t KILLSICON           = KILLSICON__STATIC;
int32_t KNEE                = KNEE__STATIC;
int32_t LA                  = LA__STATIC;
int32_t LASERLINE           = LASERLINE__STATIC;
int32_t LASERSITE           = LASERSITE__STATIC;
int32_t LEGJIB1             = LEGJIB1__STATIC;
int32_t LETTER              = LETTER__STATIC;
int32_t LIGHTSWITCH         = LIGHTSWITCH__STATIC;
int32_t LIGHTSWITCH2        = LIGHTSWITCH2__STATIC;
int32_t LIZMAN              = LIZMAN__STATIC;
int32_t LIZMANARM1          = LIZMANARM1__STATIC;
int32_t LIZMANFEEDING       = LIZMANFEEDING__STATIC;
int32_t LIZMANHEAD1         = LIZMANHEAD1__STATIC;
int32_t LIZMANJUMP          = LIZMANJUMP__STATIC;
int32_t LIZMANLEG1          = LIZMANLEG1__STATIC;
int32_t LIZMANSPITTING      = LIZMANSPITTING__STATIC;
int32_t LIZMANSTAYPUT       = LIZMANSTAYPUT__STATIC;
int32_t LIZTROOP            = LIZTROOP__STATIC;
int32_t LIZTROOPDUCKING     = LIZTROOPDUCKING__STATIC;
int32_t LIZTROOPJETPACK     = LIZTROOPJETPACK__STATIC;
int32_t LIZTROOPJUSTSIT     = LIZTROOPJUSTSIT__STATIC;
int32_t LIZTROOPONTOILET    = LIZTROOPONTOILET__STATIC;
int32_t LIZTROOPRUNNING     = LIZTROOPRUNNING__STATIC;
int32_t LIZTROOPSHOOT       = LIZTROOPSHOOT__STATIC;
int32_t LIZTROOPSTAYPUT     = LIZTROOPSTAYPUT__STATIC;
int32_t LOADSCREEN          = LOADSCREEN__STATIC;
int32_t LOCATORS            = LOCATORS__STATIC;
int32_t LOCKSWITCH1         = LOCKSWITCH1__STATIC;
int32_t LOOGIE              = LOOGIE__STATIC;
int32_t LUKE                = LUKE__STATIC;
int32_t MAIL                = MAIL__STATIC;
int32_t MAN                 = MAN__STATIC;
int32_t MAN2                = MAN2__STATIC;
int32_t MASKWALL1           = MASKWALL1__STATIC;
int32_t MASKWALL10          = MASKWALL10__STATIC;
int32_t MASKWALL11          = MASKWALL11__STATIC;
int32_t MASKWALL12          = MASKWALL12__STATIC;
int32_t MASKWALL13          = MASKWALL13__STATIC;
int32_t MASKWALL14          = MASKWALL14__STATIC;
int32_t MASKWALL15          = MASKWALL15__STATIC;
int32_t MASKWALL2           = MASKWALL2__STATIC;
int32_t MASKWALL3           = MASKWALL3__STATIC;
int32_t MASKWALL4           = MASKWALL4__STATIC;
int32_t MASKWALL5           = MASKWALL5__STATIC;
int32_t MASKWALL6           = MASKWALL6__STATIC;
int32_t MASKWALL7           = MASKWALL7__STATIC;
int32_t MASKWALL8           = MASKWALL8__STATIC;
int32_t MASKWALL9           = MASKWALL9__STATIC;
int32_t MASTERSWITCH        = MASTERSWITCH__STATIC;
int32_t MENUBAR             = MENUBAR__STATIC;
int32_t MENUSCREEN          = MENUSCREEN__STATIC;
int32_t MIKE                = MIKE__STATIC;
int32_t MINIFONT            = MINIFONT__STATIC;
int32_t MIRROR              = MIRROR__STATIC;
int32_t MIRRORBROKE         = MIRRORBROKE__STATIC;
int32_t MONEY               = MONEY__STATIC;
int32_t MONK                = MONK__STATIC;
int32_t MOONSKY1            = MOONSKY1__STATIC;
int32_t MORTER              = MORTER__STATIC;
int32_t MOVIECAMERA         = MOVIECAMERA__STATIC;
int32_t MULTISWITCH         = MULTISWITCH__STATIC;
int32_t MUSICANDSFX         = MUSICANDSFX__STATIC;
int32_t NAKED1              = NAKED1__STATIC;
int32_t NATURALLIGHTNING    = NATURALLIGHTNING__STATIC;
int32_t NEON1               = NEON1__STATIC;
int32_t NEON2               = NEON2__STATIC;
int32_t NEON3               = NEON3__STATIC;
int32_t NEON4               = NEON4__STATIC;
int32_t NEON5               = NEON5__STATIC;
int32_t NEON6               = NEON6__STATIC;
int32_t NEWBEAST            = NEWBEAST__STATIC;
int32_t NEWBEASTSTAYPUT     = NEWBEASTSTAYPUT__STATIC;
int32_t NUKEBARREL          = NUKEBARREL__STATIC;
int32_t NUKEBARRELDENTED    = NUKEBARRELDENTED__STATIC;
int32_t NUKEBARRELLEAKED    = NUKEBARRELLEAKED__STATIC;
int32_t NUKEBUTTON          = NUKEBUTTON__STATIC;
int32_t OCEANSPRITE1        = OCEANSPRITE1__STATIC;
int32_t OCEANSPRITE2        = OCEANSPRITE2__STATIC;
int32_t OCEANSPRITE3        = OCEANSPRITE3__STATIC;
int32_t OCEANSPRITE4        = OCEANSPRITE4__STATIC;
int32_t OCEANSPRITE5        = OCEANSPRITE5__STATIC;
int32_t OCTABRAIN           = OCTABRAIN__STATIC;
int32_t OCTABRAINSTAYPUT    = OCTABRAINSTAYPUT__STATIC;
int32_t OJ                  = OJ__STATIC;
int32_t OOZ                 = OOZ__STATIC;
int32_t OOZ2                = OOZ2__STATIC;
int32_t OOZFILTER           = OOZFILTER__STATIC;
int32_t ORDERING            = ORDERING__STATIC;
int32_t ORGANTIC            = ORGANTIC__STATIC;
int32_t PANNEL1             = PANNEL1__STATIC;
int32_t PANNEL2             = PANNEL2__STATIC;
int32_t PANNEL3             = PANNEL3__STATIC;
int32_t PAPER               = PAPER__STATIC;
int32_t PIGCOP              = PIGCOP__STATIC;
int32_t PIGCOPDIVE          = PIGCOPDIVE__STATIC;
int32_t PIGCOPSTAYPUT       = PIGCOPSTAYPUT__STATIC;
int32_t PIPE1               = PIPE1__STATIC;
int32_t PIPE1B              = PIPE1B__STATIC;
int32_t PIPE2               = PIPE2__STATIC;
int32_t PIPE2B              = PIPE2B__STATIC;
int32_t PIPE3               = PIPE3__STATIC;
int32_t PIPE3B              = PIPE3B__STATIC;
int32_t PIPE4               = PIPE4__STATIC;
int32_t PIPE4B              = PIPE4B__STATIC;
int32_t PIPE5               = PIPE5__STATIC;
int32_t PIPE5B              = PIPE5B__STATIC;
int32_t PIPE6               = PIPE6__STATIC;
int32_t PIPE6B              = PIPE6B__STATIC;
int32_t PLAYERONWATER       = PLAYERONWATER__STATIC;
int32_t PLUG                = PLUG__STATIC;
int32_t PLUTOPAKSPRITE      = PLUTOPAKSPRITE__STATIC;
int32_t POCKET              = POCKET__STATIC;
int32_t PODFEM1             = PODFEM1__STATIC;
int32_t POT1                = POT1__STATIC;
int32_t POT2                = POT2__STATIC;
int32_t POT3                = POT3__STATIC;
int32_t POWERSWITCH1        = POWERSWITCH1__STATIC;
int32_t POWERSWITCH2        = POWERSWITCH2__STATIC;
int32_t PUKE                = PUKE__STATIC;
int32_t PULLSWITCH          = PULLSWITCH__STATIC;
int32_t PURPLELAVA          = PURPLELAVA__STATIC;
int32_t QUEBALL             = QUEBALL__STATIC;
int32_t RADIUSEXPLOSION     = RADIUSEXPLOSION__STATIC;
int32_t RAT                 = RAT__STATIC;
int32_t REACTOR             = REACTOR__STATIC;
int32_t REACTOR2            = REACTOR2__STATIC;
int32_t REACTOR2BURNT       = REACTOR2BURNT__STATIC;
int32_t REACTOR2SPARK       = REACTOR2SPARK__STATIC;
int32_t REACTORBURNT        = REACTORBURNT__STATIC;
int32_t REACTORSPARK        = REACTORSPARK__STATIC;
int32_t RECON               = RECON__STATIC;
int32_t RESPAWN             = RESPAWN__STATIC;
int32_t RESPAWNMARKERGREEN  = RESPAWNMARKERGREEN__STATIC;
int32_t RESPAWNMARKERRED    = RESPAWNMARKERRED__STATIC;
int32_t RESPAWNMARKERYELLOW = RESPAWNMARKERYELLOW__STATIC;
int32_t ROTATEGUN           = ROTATEGUN__STATIC;
int32_t RPG                 = RPG__STATIC;
int32_t RPGAMMO             = RPGAMMO__STATIC;
int32_t RPGGUN              = RPGGUN__STATIC;
int32_t RPGSPRITE           = RPGSPRITE__STATIC;
int32_t RUBBERCAN           = RUBBERCAN__STATIC;
int32_t SATELITE            = SATELITE__STATIC;
int32_t SCALE               = SCALE__STATIC;
int32_t SCRAP1              = SCRAP1__STATIC;
int32_t SCRAP2              = SCRAP2__STATIC;
int32_t SCRAP3              = SCRAP3__STATIC;
int32_t SCRAP4              = SCRAP4__STATIC;
int32_t SCRAP5              = SCRAP5__STATIC;
int32_t SCRAP6              = SCRAP6__STATIC;
int32_t SCREENBREAK1        = SCREENBREAK1__STATIC;
int32_t SCREENBREAK10       = SCREENBREAK10__STATIC;
int32_t SCREENBREAK11       = SCREENBREAK11__STATIC;
int32_t SCREENBREAK12       = SCREENBREAK12__STATIC;
int32_t SCREENBREAK13       = SCREENBREAK13__STATIC;
int32_t SCREENBREAK14       = SCREENBREAK14__STATIC;
int32_t SCREENBREAK15       = SCREENBREAK15__STATIC;
int32_t SCREENBREAK16       = SCREENBREAK16__STATIC;
int32_t SCREENBREAK17       = SCREENBREAK17__STATIC;
int32_t SCREENBREAK18       = SCREENBREAK18__STATIC;
int32_t SCREENBREAK19       = SCREENBREAK19__STATIC;
int32_t SCREENBREAK2        = SCREENBREAK2__STATIC;
int32_t SCREENBREAK3        = SCREENBREAK3__STATIC;
int32_t SCREENBREAK4        = SCREENBREAK4__STATIC;
int32_t SCREENBREAK5        = SCREENBREAK5__STATIC;
int32_t SCREENBREAK6        = SCREENBREAK6__STATIC;
int32_t SCREENBREAK7        = SCREENBREAK7__STATIC;
int32_t SCREENBREAK8        = SCREENBREAK8__STATIC;
int32_t SCREENBREAK9        = SCREENBREAK9__STATIC;
int32_t SCUBAMASK           = SCUBAMASK__STATIC;
int32_t SECTOREFFECTOR      = SECTOREFFECTOR__STATIC;
int32_t SEENINE             = SEENINE__STATIC;
int32_t SEENINEDEAD         = SEENINEDEAD__STATIC;
int32_t SELECTDIR           = SELECTDIR__STATIC;
int32_t SHARK               = SHARK__STATIC;
int32_t SHELL               = SHELL__STATIC;
int32_t SHIELD              = SHIELD__STATIC;
int32_t SHOTGUN             = SHOTGUN__STATIC;
int32_t SHOTGUNAMMO         = SHOTGUNAMMO__STATIC;
int32_t SHOTGUNSHELL        = SHOTGUNSHELL__STATIC;
int32_t SHOTGUNSPRITE       = SHOTGUNSPRITE__STATIC;
int32_t SHOTSPARK1          = SHOTSPARK1__STATIC;
int32_t SHRINKER            = SHRINKER__STATIC;
int32_t SHRINKEREXPLOSION   = SHRINKEREXPLOSION__STATIC;
int32_t SHRINKERSPRITE      = SHRINKERSPRITE__STATIC;
int32_t SHRINKSPARK         = SHRINKSPARK__STATIC;
int32_t SIDEBOLT1           = SIDEBOLT1__STATIC;
int32_t SIGN1               = SIGN1__STATIC;
int32_t SIGN2               = SIGN2__STATIC;
int32_t SIXPAK              = SIXPAK__STATIC;
int32_t SLIDEBAR            = SLIDEBAR__STATIC;
int32_t SLOTDOOR            = SLOTDOOR__STATIC;
int32_t SMALLFNTCURSOR      = SMALLFNTCURSOR__STATIC;
int32_t SMALLSMOKE          = SMALLSMOKE__STATIC;
int32_t SOLARPANNEL         = SOLARPANNEL__STATIC;
int32_t SPACEDOORSWITCH     = SPACEDOORSWITCH__STATIC;
int32_t SPACELIGHTSWITCH    = SPACELIGHTSWITCH__STATIC;
int32_t SPACEMARINE         = SPACEMARINE__STATIC;
int32_t SPEAKER             = SPEAKER__STATIC;
int32_t SPINNINGNUKEICON    = SPINNINGNUKEICON__STATIC;
int32_t SPIT                = SPIT__STATIC;
int32_t SPOTLITE            = SPOTLITE__STATIC;
int32_t STAINGLASS1         = STAINGLASS1__STATIC;
int32_t STALL               = STALL__STATIC;
int32_t STALLBROKE          = STALLBROKE__STATIC;
int32_t STARTALPHANUM       = STARTALPHANUM__STATIC;
int32_t STATIC              = STATIC__STATIC;
int32_t STATUE              = STATUE__STATIC;
int32_t STATUEFLASH         = STATUEFLASH__STATIC;
int32_t STEAM               = STEAM__STATIC;
int32_t STEROIDS            = STEROIDS__STATIC;
int32_t STEROIDS_ICON       = STEROIDS_ICON__STATIC;
int32_t STRIPEBALL          = STRIPEBALL__STATIC;
int32_t SUSHIPLATE1         = SUSHIPLATE1__STATIC;
int32_t SUSHIPLATE2         = SUSHIPLATE2__STATIC;
int32_t SUSHIPLATE3         = SUSHIPLATE3__STATIC;
int32_t SUSHIPLATE4         = SUSHIPLATE4__STATIC;
int32_t SUSHIPLATE5         = SUSHIPLATE5__STATIC;
int32_t TAMPON              = TAMPON__STATIC;
int32_t TANK                = TANK__STATIC;
int32_t TARGET              = TARGET__STATIC;
int32_t TECHLIGHT2          = TECHLIGHT2__STATIC;
int32_t TECHLIGHT4          = TECHLIGHT4__STATIC;
int32_t TECHLIGHTBUST2      = TECHLIGHTBUST2__STATIC;
int32_t TECHLIGHTBUST4      = TECHLIGHTBUST4__STATIC;
int32_t TECHSWITCH          = TECHSWITCH__STATIC;
int32_t TENSCREEN           = TENSCREEN__STATIC;
int32_t TEXTBOX             = TEXTBOX__STATIC;
int32_t TEXTSTORY           = TEXTSTORY__STATIC;
int32_t THREEBYFIVE         = THREEBYFIVE__STATIC;
int32_t THREEDEE            = THREEDEE__STATIC;
int32_t TIP                 = TIP__STATIC;
int32_t TIRE                = TIRE__STATIC;
int32_t TOILET              = TOILET__STATIC;
int32_t TOILETBROKE         = TOILETBROKE__STATIC;
int32_t TOILETWATER         = TOILETWATER__STATIC;
int32_t TONGUE              = TONGUE__STATIC;
int32_t TOUCHPLATE          = TOUCHPLATE__STATIC;
int32_t TOUGHGAL            = TOUGHGAL__STATIC;
int32_t TRANSPORTERBEAM     = TRANSPORTERBEAM__STATIC;
int32_t TRANSPORTERSTAR     = TRANSPORTERSTAR__STATIC;
int32_t TRASH               = TRASH__STATIC;
int32_t TREE1               = TREE1__STATIC;
int32_t TREE2               = TREE2__STATIC;
int32_t TRIPBOMB            = TRIPBOMB__STATIC;
int32_t TRIPBOMBSPRITE      = TRIPBOMBSPRITE__STATIC;
int32_t TRIPODCAMERA        = TRIPODCAMERA__STATIC;
int32_t VACUUM              = VACUUM__STATIC;
int32_t VASE                = VASE__STATIC;
int32_t VENDMACHINE         = VENDMACHINE__STATIC;
int32_t VICTORY1            = VICTORY1__STATIC;
int32_t VIEWBORDER          = VIEWBORDER__STATIC;
int32_t VIEWSCREEN          = VIEWSCREEN__STATIC;
int32_t VIEWSCREEN2         = VIEWSCREEN2__STATIC;
int32_t W_FORCEFIELD        = W_FORCEFIELD__STATIC;
int32_t W_HITTECHWALL1      = W_HITTECHWALL1__STATIC;
int32_t W_HITTECHWALL10     = W_HITTECHWALL10__STATIC;
int32_t W_HITTECHWALL15     = W_HITTECHWALL15__STATIC;
int32_t W_HITTECHWALL16     = W_HITTECHWALL16__STATIC;
int32_t W_HITTECHWALL2      = W_HITTECHWALL2__STATIC;
int32_t W_HITTECHWALL3      = W_HITTECHWALL3__STATIC;
int32_t W_HITTECHWALL4      = W_HITTECHWALL4__STATIC;
int32_t W_MILKSHELF         = W_MILKSHELF__STATIC;
int32_t W_MILKSHELFBROKE    = W_MILKSHELFBROKE__STATIC;
int32_t W_NUMBERS           = W_NUMBERS__STATIC;
int32_t W_SCREENBREAK       = W_SCREENBREAK__STATIC;
int32_t W_TECHWALL1         = W_TECHWALL1__STATIC;
int32_t W_TECHWALL10        = W_TECHWALL10__STATIC;
int32_t W_TECHWALL11        = W_TECHWALL11__STATIC;
int32_t W_TECHWALL12        = W_TECHWALL12__STATIC;
int32_t W_TECHWALL13        = W_TECHWALL13__STATIC;
int32_t W_TECHWALL14        = W_TECHWALL14__STATIC;
int32_t W_TECHWALL15        = W_TECHWALL15__STATIC;
int32_t W_TECHWALL16        = W_TECHWALL16__STATIC;
int32_t W_TECHWALL2         = W_TECHWALL2__STATIC;
int32_t W_TECHWALL3         = W_TECHWALL3__STATIC;
int32_t W_TECHWALL4         = W_TECHWALL4__STATIC;
int32_t W_TECHWALL5         = W_TECHWALL5__STATIC;
int32_t W_TECHWALL6         = W_TECHWALL6__STATIC;
int32_t W_TECHWALL7         = W_TECHWALL7__STATIC;
int32_t W_TECHWALL8         = W_TECHWALL8__STATIC;
int32_t W_TECHWALL9         = W_TECHWALL9__STATIC;
int32_t WAITTOBESEATED      = WAITTOBESEATED__STATIC;
int32_t WALLBLOOD1          = WALLBLOOD1__STATIC;
int32_t WALLBLOOD2          = WALLBLOOD2__STATIC;
int32_t WALLBLOOD3          = WALLBLOOD3__STATIC;
int32_t WALLBLOOD4          = WALLBLOOD4__STATIC;
int32_t WALLBLOOD5          = WALLBLOOD5__STATIC;
int32_t WALLBLOOD7          = WALLBLOOD7__STATIC;
int32_t WALLBLOOD8          = WALLBLOOD8__STATIC;
int32_t WALLLIGHT1          = WALLLIGHT1__STATIC;
int32_t WALLLIGHT2          = WALLLIGHT2__STATIC;
int32_t WALLLIGHT3          = WALLLIGHT3__STATIC;
int32_t WALLLIGHT4          = WALLLIGHT4__STATIC;
int32_t WALLLIGHTBUST1      = WALLLIGHTBUST1__STATIC;
int32_t WALLLIGHTBUST2      = WALLLIGHTBUST2__STATIC;
int32_t WALLLIGHTBUST3      = WALLLIGHTBUST3__STATIC;
int32_t WALLLIGHTBUST4      = WALLLIGHTBUST4__STATIC;
int32_t WATERBUBBLE         = WATERBUBBLE__STATIC;
int32_t WATERBUBBLEMAKER    = WATERBUBBLEMAKER__STATIC;
int32_t WATERDRIP           = WATERDRIP__STATIC;
int32_t WATERDRIPSPLASH     = WATERDRIPSPLASH__STATIC;
int32_t WATERFOUNTAIN       = WATERFOUNTAIN__STATIC;
int32_t WATERFOUNTAINBROKE  = WATERFOUNTAINBROKE__STATIC;
int32_t WATERSPLASH2        = WATERSPLASH2__STATIC;
int32_t WATERTILE2          = WATERTILE2__STATIC;
int32_t WEATHERWARN         = WEATHERWARN__STATIC;
int32_t WINDOWBORDER1       = WINDOWBORDER1__STATIC;
int32_t WINDOWBORDER2       = WINDOWBORDER2__STATIC;
int32_t WOMAN               = WOMAN__STATIC;
int32_t WOODENHORSE         = WOODENHORSE__STATIC;
int32_t XXXSTACY            = XXXSTACY__STATIC;

static hashtable_t h_names = {512, NULL};

void G_ProcessDynamicTileMapping(const char *szLabel, int32_t lValue)
{
    if ((unsigned)lValue >= MAXTILES || !szLabel)
        return;

    int const i = hash_find(&h_names,szLabel);

    if (i>=0)
    {
        struct dynitem *di = &g_dynTileList[i];
        *di->dynvalptr = lValue;
    }
}

void inithashnames(void)
{
    hash_init(&h_names);

    for (int i=0; i < ARRAY_SSIZE(g_dynTileList); i++)
        hash_add(&h_names, g_dynTileList[i].str, i, 0);
}

void freehashnames(void)
{
    hash_free(&h_names);
}

// This is run after all CON define's have been processed to set up the
// dynamic->static tile mapping.
void G_InitDynamicTiles(void)
{
    Bmemset(DynamicTileMap, 0, sizeof(DynamicTileMap));

    for (auto & i : g_dynTileList)
	{
        DynamicTileMap[*(i.dynvalptr)] = i.staticval;
		NameToTileIndex.Insert(i.str, *(i.dynvalptr));
	}

    g_blimpSpawnItems[0] = RPGSPRITE;
    g_blimpSpawnItems[1] = CHAINGUNSPRITE;
    g_blimpSpawnItems[2] = DEVISTATORAMMO;
    g_blimpSpawnItems[3] = RPGAMMO;
    g_blimpSpawnItems[4] = RPGAMMO;
    g_blimpSpawnItems[5] = JETPACK;
    g_blimpSpawnItems[6] = SHIELD;
    g_blimpSpawnItems[7] = FIRSTAID;
    g_blimpSpawnItems[8] = STEROIDS;
    g_blimpSpawnItems[9] = RPGAMMO;
    g_blimpSpawnItems[10] = RPGAMMO;
    g_blimpSpawnItems[11] = RPGSPRITE;
    g_blimpSpawnItems[12] = RPGAMMO;
    g_blimpSpawnItems[13] = FREEZESPRITE;
    g_blimpSpawnItems[14] = FREEZEAMMO;

    WeaponPickupSprites[0] = KNEE;
    WeaponPickupSprites[1] = FIRSTGUNSPRITE;
    WeaponPickupSprites[2] = SHOTGUNSPRITE;
    WeaponPickupSprites[3] = CHAINGUNSPRITE;
    WeaponPickupSprites[4] = RPGSPRITE;
    WeaponPickupSprites[5] = HEAVYHBOMB;
    WeaponPickupSprites[6] = SHRINKERSPRITE;
    WeaponPickupSprites[7] = DEVISTATORSPRITE;
    WeaponPickupSprites[8] = TRIPBOMBSPRITE;
    WeaponPickupSprites[9] = FREEZESPRITE;
    WeaponPickupSprites[10] = HEAVYHBOMB;
    WeaponPickupSprites[11] = SHRINKERSPRITE;
}
END_DUKE_NS
