//-------------------------------------------------------------------------
/*
Copyright (C) 2016 EDuke32 developers and contributors
Copyright (C) 2019 Christoph Oelckers

This is free software; you can redistribute it and/or
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

#include "cheats.h"
#include "compat.h"
#include "demo.h"
#include "duke3d.h"

#include "menus.h"
#include "osdcmds.h"
#include "savegame.h"
#include "game.h"
#include "superfasthash.h"
#include "gamecvars.h"
#include "gamecontrol.h"
#include "c_bind.h"
#include "menu/menu.h"
#include "gstrings.h"
#include "version.h"
#include "namesdyn.h"
#include "../../glbackend/glbackend.h"


BEGIN_RR_NS

#define MENU_MARGIN_REGULAR 40
#define MENU_MARGIN_WIDE    32
#define MENU_MARGIN_CENTER  160
#define MENU_HEIGHT_CENTER  100


enum MenuTextFlags_t
{
	MT_Selected = 1 << 0,
	MT_Disabled = 1 << 1,
	MT_XCenter = 1 << 2,
	MT_XRight = 1 << 3,
	MT_YCenter = 1 << 4,
	MT_Literal = 1 << 5,
	MT_RightSide = 1 << 6,
};


// common font types
// tilenums are set after namesdyn runs.
// These are also modifiable by scripts.
//                                      emptychar x,y       between x,y         zoom                cursorLeft          cursorCenter        cursorScale          textflags
//                                      tilenum             shade_deselected    shade_disabled      pal                 pal_selected        pal_deselected       pal_disabled
MenuFont_t MF_Redfont =               { { 5<<16, 15<<16 },  { 0, 0 },           65536,              20<<16,             110<<16,            65536, 65536, 65536, TEXT_BIGALPHANUM | TEXT_UPPERCASE,
                                        -1,                 10,                 0,                  0,                  0,                  0,                   1,
                                        0,                  0,                  1 };
MenuFont_t MF_Bluefont =              { { 5<<16, 7<<16 },   { 0, 0 },           65536,              10<<16,             110<<16,            32768, 65536, 65536, 0,
                                        -1,                 10,                 0,                  0,                  10,                 10,                  16,
                                        0,                  0,                  16 };
MenuFont_t MF_Minifont =              { { 4<<16, 5<<16 },   { 1<<16, 1<<16 },   65536,              10<<16,             110<<16,            32768, 65536, 65536, 0,
                                        -1,                 10,                 0,                  0,                  2,                  2,                   0,
                                        0,                  0,                  16 };


/*
This function prepares data after ART and CON have been processed.
It also initializes some data in loops rather than statically at compile time.
*/

void Menu_Init(void)
{

	// prepare menu fonts
	// check if tilenum is -1 in case it was set in EVENT_SETDEFAULTS
	if ((unsigned)MF_Redfont.tilenum >= MAXTILES) MF_Redfont.tilenum = BIGALPHANUM;
	if ((unsigned)MF_Bluefont.tilenum >= MAXTILES) MF_Bluefont.tilenum = STARTALPHANUM;
	if ((unsigned)MF_Minifont.tilenum >= MAXTILES) MF_Minifont.tilenum = MINIFONT;
	MF_Redfont.emptychar.y = tilesiz[MF_Redfont.tilenum].y << 16;
	MF_Bluefont.emptychar.y = tilesiz[MF_Bluefont.tilenum].y << 16;
	MF_Minifont.emptychar.y = tilesiz[MF_Minifont.tilenum].y << 16;
	if (!minitext_lowercase)
		MF_Minifont.textflags |= TEXT_UPPERCASE;

#if 0

	// prepare sound setup
#ifndef EDUKE32_STANDALONE
	if (WW2GI)
		ME_SOUND_DUKETALK.name = "GI talk:";
	else if (NAM)
		ME_SOUND_DUKETALK.name = "Grunt talk:";
	ME_SOUND_DUKETALK.name = "Leonard Talk:";

#endif

	// prepare pre-Atomic
	if (!VOLUMEALL || !PLUTOPAK)
	{
		// prepare credits
		M_CREDITS.title = M_CREDITS2.title = M_CREDITS3.title = s_Credits;
	}
#endif
	
    if (RR)
    {
        MF_Redfont.zoom = 32768;
        MF_Redfont.emptychar.x <<= 1;
        MF_Redfont.cursorScale = 13107;
        MF_Redfont.cursorScale2 = 6553;
        //MF_Redfont.emptychar.y <<= 1;
        MF_Bluefont.zoom = 32768;
        MF_Bluefont.emptychar.x <<= 1;
        MF_Bluefont.cursorScale = 6553;
        MF_Bluefont.cursorScale2 = 6553;
        //MF_Bluefont.emptychar.y <<= 1;
        MF_Minifont.zoom = 32768;
        MF_Minifont.emptychar.x <<= 1;
        MF_Minifont.cursorScale = 6553;
        MF_Minifont.cursorScale2 = 6553;
        //MF_Minifont.emptychar.y <<= 1;
    }

}


static void Menu_DrawBackground(const DVector2 &origin)
{
	rotatesprite_fs(int(origin.X * 65536) + (MENU_MARGIN_CENTER << 16), int(origin.Y * 65536) + (100 << 16), 65536L, 0, MENUSCREEN, 16, 0, 10 + 64);
}

static void Menu_DrawTopBar(const DVector2 &origin)
{
    rotatesprite_fs(int(origin.X*65536) + (MENU_MARGIN_CENTER<<16), int(origin.Y*65536) + (19<<16), MF_Redfont.cursorScale3, 0,MENUBAR,16,0,10);
}

static void Menu_DrawTopBarCaption(const char* caption, const DVector2& origin)
{
	static char t[64];
	size_t const srclen = strlen(caption);
	size_t const dstlen = min(srclen, ARRAY_SIZE(t) - 1);
	memcpy(t, caption, dstlen);
	t[dstlen] = '\0';
	char* p = &t[dstlen - 1];
	if (*p == ':')
		*p = '\0';
	captionmenutext(int(origin.X * 65536) + (MENU_MARGIN_CENTER << 16), int(origin.Y * 65536) + (24 << 16) + (15 << 15), t);
}

static void Menu_GetFmt(const MenuFont_t* font, uint8_t const status, int32_t* s)
{
	if (status & MT_Selected)
		*s = sintable[((int32_t)totalclock << 5) & 2047] >> 12;
	else
		*s = font->shade_deselected;
	// sum shade values
	if (status & MT_Disabled)
		*s += font->shade_disabled;
}

static vec2_t Menu_Text(int32_t x, int32_t y, const MenuFont_t* font, const char* t, uint8_t status, int32_t ydim_upper, int32_t ydim_lower)
{
	int32_t s, p, ybetween = font->between.y;
	int32_t f = font->textflags | TEXT_RRMENUTEXTHACK;
	if (status & MT_XCenter)
		f |= TEXT_XCENTER;
	if (status & MT_XRight)
		f |= TEXT_XRIGHT;
	if (status & MT_YCenter)
	{
		f |= TEXT_YCENTER | TEXT_YOFFSETZERO;
		ybetween = font->emptychar.y; // <^ the battle against 'Q'
	}
	if (status & MT_Literal)
		f |= TEXT_LITERALESCAPE;

	int32_t z = font->zoom;

	if (status & MT_Disabled)
		p = (status & MT_RightSide) ? font->pal_disabled_right : font->pal_disabled;
	else if (status & MT_Selected)
		p = (status & MT_RightSide) ? font->pal_selected_right : font->pal_selected;
	else
		p = (status & MT_RightSide) ? font->pal_deselected_right : font->pal_deselected;

	Menu_GetFmt(font, status, &s);

	return G_ScreenText(font->tilenum, x, y, z, 0, 0, t, s, p, 2 | 8 | 16 | ROTATESPRITE_FULL16, 0, font->emptychar.x, font->emptychar.y, font->between.x, ybetween, f, 0, ydim_upper, xdim - 1, ydim_lower);
}

static int32_t Menu_CursorShade(void)
{
	return 4 - (sintable[((int32_t)totalclock << 4) & 2047] >> 11);
}

static void Menu_DrawCursorCommon(int32_t x, int32_t y, int32_t z, int32_t picnum, int32_t ydim_upper = 0, int32_t ydim_lower = ydim - 1)
{
	rotatesprite_(x, y, z, 0, picnum, Menu_CursorShade(), 0, 2 | 8, 0, 0, 0, ydim_upper, xdim - 1, ydim_lower);
}

static void Menu_DrawCursorLeft(int32_t x, int32_t y, int32_t z)
{
	const int frames = RR ? 16 : 7;
	Menu_DrawCursorCommon(x, y, z, SPINNINGNUKEICON+(((int32_t) totalclock>>3)%frames));
}

static void Menu_DrawCursorRight(int32_t x, int32_t y, int32_t z)
{
	const int frames = RR ? 16 : 7;
    Menu_DrawCursorCommon(x, y, z, SPINNINGNUKEICON+frames-1-((frames-1+((int32_t) totalclock>>3))%frames));
}

static int Menu_GetFontHeight(int fontnum)
{
	auto& font = fontnum == NIT_BigFont ? MF_Redfont : fontnum == NIT_SmallFont ? MF_Bluefont : MF_Minifont;
	return font.get_yline();
}

//----------------------------------------------------------------------------
//
// Implements the native looking menu used for the main menu
// and the episode/skill selection screens, i.e. the parts
// that need to look authentic
//
//----------------------------------------------------------------------------

class RedneckListMenu : public DListMenu
{
	using Super = DListMenu;
protected:

	void Ticker() override
	{
		// Lay out the menu.
		int32_t y_upper = mDesc->mYpos;
		int32_t y_lower = y_upper + mDesc->mYbotton;
		int32_t y = 0;
		int32_t calculatedentryspacing = 0;
		int32_t const height = Menu_GetFontHeight(mDesc->mNativeFontNum) >> 16;

		int32_t totalheight = 0, numvalidentries = mDesc->mItems.Size();

		for (unsigned e = 0; e < mDesc->mItems.Size(); ++e)
		{
			auto entry = mDesc->mItems[e];
			entry->mHidden = false;
			entry->SetHeight(height);
			totalheight += height;
		}
		if (mDesc->mSpacing <= 0) calculatedentryspacing = std::max(0, (y_lower - y_upper - totalheight) / (numvalidentries > 1 ? numvalidentries - 1 : 1));
		if (calculatedentryspacing <= 0) calculatedentryspacing = mDesc->mSpacing;


		// totalHeight calculating pass
		int totalHeight;
		for (unsigned e = 0; e < mDesc->mItems.Size(); ++e)
		{
			auto entry = mDesc->mItems[e];
			if (!entry->mHidden)
			{
				entry->SetY(y_upper + y);
				y += height;
				totalHeight = y;
				y += calculatedentryspacing;
			}
		}
	}
};

class RedneckMainMenu : public RedneckListMenu
{
	void PreDraw() override
	{
		RedneckListMenu::PreDraw();
		if (RRRA)
			rotatesprite_fs(int(origin.X * 65536) + ((MENU_MARGIN_CENTER - 5) << 16), int(origin.Y * 65536) + ((57) << 16), 16592L, 0, THREEDEE, 0, 0, 10);
		else
			rotatesprite_fs(int(origin.X * 65536) + ((MENU_MARGIN_CENTER + 5) << 16), int(origin.Y * 65536) + ((24) << 16), 23592L, 0, INGAMEDUKETHREEDEE, 0, 0, 10);
	}
};

//----------------------------------------------------------------------------
//
// Menu related game interface functions
//
//----------------------------------------------------------------------------

void GameInterface::DrawNativeMenuText(int fontnum, int state, double xpos, double ypos, float fontscale, const char* text, int flags)
{
	int ydim_upper = 0;
	int ydim_lower = ydim - 1;
	//int32_t const indent = 0;	// not set for any relevant menu
	int x = int(xpos * 65536);

	uint8_t status = 0;
	if (state == NIT_SelectedState)
		status |= MT_Selected;
	if (state == NIT_InactiveState)
		status |= MT_Disabled;
	if (flags & LMF_Centered)
		status |= MT_XCenter;

	bool const dodraw = true;
	MenuFont_t& font = fontnum == NIT_BigFont ? MF_Redfont : fontnum == NIT_SmallFont ? MF_Bluefont : MF_Minifont;

	int32_t const height = font.get_yline();
	status |= MT_YCenter;
	int32_t const y_internal = int(ypos * 65536) + ((height >> 17) << 16);// -menu->scrollPos;

	vec2_t textsize;
	if (dodraw)
		textsize = Menu_Text(x, y_internal, &font, text, status, ydim_upper, ydim_lower);

	if (dodraw && (status & MT_Selected) && state != 1)
	{
		if (status & MT_XCenter)
		{
			Menu_DrawCursorLeft(x + font.cursorCenterPosition, y_internal, font.cursorScale);
			Menu_DrawCursorRight(x - font.cursorCenterPosition, y_internal, font.cursorScale);
		}
		else
			Menu_DrawCursorLeft(x /*+ indent*/ - font.cursorLeftPosition, y_internal, font.cursorScale);
	}

}


void GameInterface::MenuOpened()
{
	S_PauseSounds(true);
	if ((!g_netServer && ud.multimode < 2))
	{
		ready2send = 0;
		totalclock = ototalclock;
		screenpeek = myconnectindex;
	}

	auto& gm = g_player[myconnectindex].ps->gm;
	if (gm & MODE_GAME)
	{
		gm |= MODE_MENU;
	}
}

void GameInterface::MenuSound(EMenuSounds snd)
{
	switch (snd)
	{
		case CursorSound:
			S_PlaySound(RR ? 335 : KICK_HIT, CHAN_AUTO, CHANF_UI);
			break;

		case AdvanceSound:
			S_PlaySound(RR? 341 : PISTOL_BODYHIT, CHAN_AUTO, CHANF_UI);
			break;
			
		case CloseSound:
			S_PlaySound(EXITMENUSOUND, CHAN_AUTO, CHANF_UI);
			break;

		default:
			return;
	}
}

void GameInterface::MenuClosed()
{

	auto& gm = g_player[myconnectindex].ps->gm;
	if (gm & MODE_GAME)
	{
		if (gm & MODE_MENU)
			inputState.ClearAllInput();

		// The following lines are here so that you cannot close the menu when no game is running.
		gm &= ~MODE_MENU;

		if ((!g_netServer && ud.multimode < 2) && ud.recstat != 2)
		{
			ready2send = 1;
			totalclock = ototalclock;
			CAMERACLOCK = (int32_t)totalclock;
			CAMERADIST = 65536;

			// Reset next-viewscreen-redraw counter.
			// XXX: are there any other cases like that in need of handling?
			if (g_curViewscreen >= 0)
				actor[g_curViewscreen].t_data[0] = (int32_t)totalclock;
		}

		G_UpdateScreenArea();
		S_PauseSounds(false);
	}
}

bool GameInterface::CanSave()
{
	if (ud.recstat == 2) return false;
	auto &myplayer = *g_player[myconnectindex].ps;
	if (sprite[myplayer.i].extra <= 0)
	{
		P_DoQuote(QUOTE_SAVE_DEAD, &myplayer);
		return false;
	}
	return true;
}

void GameInterface::StartGame(FGameStartup& gs)
{
	int32_t skillsound = PISTOL_BODYHIT;

	switch (gs.Skill)
	{
	case 0:
		skillsound = 427;
		break;
	case 1:
		skillsound = 428;
		break;
	case 2:
		skillsound = 196;
		break;
	case 3:
		skillsound = 195;
		break;
	case 4:
		skillsound = 197;
		break;
	}

	ud.m_player_skill = gs.Skill + 1;
	if (menu_sounds && skillsound >= 0 && SoundEnabled())
	{
		S_PlaySound(skillsound, CHAN_AUTO, CHANF_UI);

		while (S_CheckSoundPlaying(skillsound))
		{
			S_Update();
			G_HandleAsync();
		}
	}
	ud.m_respawn_monsters = (gs.Skill == 3);
	ud.m_monsters_off = ud.monsters_off = 0;
	ud.m_respawn_items = 0;
	ud.m_respawn_inventory = 0;
	ud.multimode = 1;
	ud.m_volume_number = gs.Episode;
	m_level_number = gs.Level;
	G_NewGame_EnterLevel();

}

FSavegameInfo GameInterface::GetSaveSig()
{
	return { SAVESIG_RR, MINSAVEVER_RR, SAVEVER_RR };
}

void GameInterface::DrawMenuCaption(const DVector2& origin, const char* text)
{
	Menu_DrawTopBar(origin);
	Menu_DrawTopBarCaption(text, origin);
}

void GameInterface::DrawCenteredTextScreen(const DVector2 &origin, const char *text, int position, bool bg)
{
	if (bg) Menu_DrawBackground(origin);
	else
	{
		// Only used for the confirmation screen.
		int lines = 1;
		for (int i = 0; text[i]; i++) if (text[i] == '\n') lines++;
		int height = lines * Menu_GetFontHeight(NIT_SmallFont);
		position -= height >> 17;
	}
	G_ScreenText(MF_Bluefont.tilenum, int((origin.X + 160) * 65536), int((origin.Y + position) * 65536), MF_Bluefont.zoom, 0, 0, text, 0, MF_Bluefont.pal,
		2 | 8 | 16 | ROTATESPRITE_FULL16, 0, MF_Bluefont.emptychar.x, MF_Bluefont.emptychar.y, MF_Bluefont.between.x, MF_Bluefont.between.y,
		MF_Bluefont.textflags | TEXT_XCENTER, 0, 0, xdim - 1, ydim - 1);
}


void GameInterface::DrawPlayerSprite(const DVector2& origin, bool onteam)
{
	if (RR)
		rotatesprite_fs(int(origin.X * 65536) + (260<<16), int(origin.Y * 65536) + ((24+(tilesiz[APLAYER].y>>2))<<16), 24576L,0,3845+36-((((8-((int32_t) totalclock>>4)))&7)*5),0,onteam ? G_GetTeamPalette(playerteam) : G_CheckPlayerColor(playercolor),10);
	else
		rotatesprite_fs(int(origin.X * 65536) + (260<<16), int(origin.Y * 65536) + ((24+(tilesiz[APLAYER].y>>1))<<16), 49152L,0,1441-((((4-((int32_t) totalclock>>4)))&3)*5),0,onteam ? G_GetTeamPalette(playerteam) : G_CheckPlayerColor(playercolor),10);
}

void GameInterface::QuitToTitle()
{
	g_player[myconnectindex].ps->gm = MODE_DEMO;
	if (ud.recstat == 1)
		G_CloseDemoWrite();
	artClearMapArt();
}

END_RR_NS

//----------------------------------------------------------------------------
//
// Class registration
//
//----------------------------------------------------------------------------


static TMenuClassDescriptor<Redneck::RedneckMainMenu> _mm("Redneck.MainMenu");
static TMenuClassDescriptor<Redneck::RedneckListMenu> _lm("Redneck.ListMenu");

void RegisterRedneckMenus()
{
	menuClasses.Push(&_mm);
	menuClasses.Push(&_lm);
}
