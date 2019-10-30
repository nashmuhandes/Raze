//
// Definitions of common game-only data structures/functions
// (and declarations of data appearing in both)
// for EDuke32 and Mapster32
//

#ifndef EDUKE32_COMMON_GAME_H_
#define EDUKE32_COMMON_GAME_H_

#include "collections.h"
#include "grpscan.h"
#include "gamecontrol.h"

#include "vfs.h"

BEGIN_DUKE_NS


extern struct grpfile_t const *g_selectedGrp;

extern int32_t g_gameType;
extern int     g_addonNum;

#define DUKE                (g_gameType & GAMEFLAG_DUKE)
#define NAM                 (g_gameType & GAMEFLAG_NAM)
#define NAPALM              (g_gameType & GAMEFLAG_NAPALM)
#define WW2GI               (g_gameType & GAMEFLAG_WW2GI)
#define NAM_WW2GI           (g_gameType & (GAMEFLAG_NAM|GAMEFLAG_WW2GI))
#define SHAREWARE           (g_gameType & GAMEFLAG_SHAREWARE)
#define DUKEBETA            ((g_gameType & GAMEFLAG_DUKEBETA) == GAMEFLAG_DUKEBETA)
#define FURY                (g_gameType & GAMEFLAG_FURY)

enum Games_t {
    GAME_DUKE = 0,
    GAME_NAM,
    GAME_NAPALM,
    GAME_WW2GI,
    GAMECOUNT
};

enum searchpathtypes_t {
    SEARCHPATH_REMOVE = 1<<0,
    SEARCHPATH_NAM    = 1<<1,
    SEARCHPATH_WW2GI  = 1<<2,
};

typedef enum basepal_ {
    BASEPAL = 0,
    WATERPAL,
    SLIMEPAL,
    DREALMSPAL,
    TITLEPAL,
    ENDINGPAL,  // 5
    ANIMPAL,
    BASEPALCOUNT
} basepal_t;

#define OSDTEXT_DEFAULT   "^00"
#define OSDTEXT_DARKRED   "^10"
#define OSDTEXT_GREEN     "^11"
#define OSDTEXT_RED       "^21"
#define OSDTEXT_YELLOW    "^23"

#define OSDTEXT_BRIGHT    "^S0"

#define OSD_ERROR OSDTEXT_DARKRED OSDTEXT_BRIGHT

extern const char *g_gameNamePtr;

extern char *g_grpNamePtr;
extern char *g_scriptNamePtr;

extern const char *G_DefaultGrpFile(void);
extern const char *G_GrpFile(void);

extern const char *G_DefaultConFile(void);
extern const char *G_ConFile(void);

extern GrowArray<char *> g_scriptModules;

extern void G_AddCon(const char *buffer);
extern void G_AddConModule(const char *buffer);

extern void clearGrpNamePtr(void);
extern void clearScriptNamePtr(void);

extern int loaddefinitions_game(const char *fn, int32_t preload);
extern int32_t g_groupFileHandle;

//////////

extern void G_InitMultiPsky(int CLOUDYOCEAN__DYN, int MOONSKY1__DYN, int BIGORBIT1__DYN, int LA__DYN);
extern void G_SetupGlobalPsky(void);

//////////

extern void G_AddSearchPaths(void);
extern void G_CleanupSearchPaths(void);

extern void G_ExtInit(void);
extern void G_ScanGroups(void);
extern void G_LoadGroups();

extern const char * G_GetInstallPath(int32_t insttype);

//////////

void G_LoadGroupsInDir(const char *dirname);
void G_DoAutoload(const char *dirname);

//////////

extern void G_LoadLookups(void);

//////////

# define FORMAT_UPGRADE_ELIGIBLE
extern FileReader S_OpenAudio(const char *fn, char searchfirst, uint8_t ismusic);

void G_AddGroup(const char* buffer);
void G_AddPath(const char* buffer);

END_DUKE_NS
#endif
