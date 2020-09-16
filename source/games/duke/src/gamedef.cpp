//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment
Copyright (C) 2020 - Christoph Oelckers

This file is part of Enhanced Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 1996 - Todd Replogle
Prepared for public release: 03/21/2003 - Charlie Wiederhold, 3D Realms

EDuke enhancements integrated: 04/13/2003 - Matt Saettler

Note: This source file IS NOT USED in EDuke source.  It has been split
into many sub-files.

Modifications for JonoF's port by Jonathon Fowler (jf@jonof.id.au)
*/
//-------------------------------------------------------------------------

#include "ns.h"
#include <string.h>
#include "concmd.h"
#include "cmdlib.h"
#include "memarena.h"
#include "printf.h"
#include "filesystem.h"
#include "mapinfo.h"
#include "menu.h"
#include "global.h"
#include "m_argv.h"
#include "sounds.h"

BEGIN_DUKE_NS


//---------------------------------------------------------------------------
//
// definitions needed by the parser.
//
//---------------------------------------------------------------------------

enum labeltypes {
	LABEL_ANY = -1,
	LABEL_DEFINE = 1,
	LABEL_STATE = 2,
	LABEL_ACTOR = 4,
	LABEL_ACTION = 8,
	LABEL_AI = 16,
	LABEL_MOVE = 32,
};

class labelstring
{
	char text[64];

public:
	char& operator[](size_t pos)
	{
		return text[pos];
	}
	operator const char* () { return text; }
	const char* GetChars() { return text; }
	int compare(const char* c) const { return strcmp(text, c); }
	int comparei(const char* c) const { return stricmp(text, c); }
	labelstring& operator=(const char* c) { strncpy(text, c, 64); text[63] = 0; return *this; }

};

struct TempMusic
{
	int levnum;
	FString music;
};

//---------------------------------------------------------------------------
//
// the actual parser
//
//---------------------------------------------------------------------------

class ConCompiler
{
	char* textptr = nullptr;
	int line_number = 0;
	int errorcount = 0, warningcount = 0;	// was named 'error' and 'warning' which is too generic for public variables and may clash with other code.
	int currentsourcefile = -1;
	unsigned parsing_actor = 0, parsing_event = 0;
	int parsing_state = 0;
	int num_squigilly_brackets = 0;
	int checking_ifelse = 0;
	labelstring parselabel= {};
	// This is for situations where the music gets defined before the map. Since the map records do not exist yet, we need a temporary buffer.
	TArray<TempMusic> tempMusic;
	char parsebuf[1024];
	TArray<char> parsebuffer; // global so that the storage is persistent across calls.

	void ReportError(int error);
	int getkeyword(const char* text);
	FString translatelabeltype(int type);
	bool ispecial(char c);
	void skiptoendofline();
	void skipwhitespace();
	void skipblockcomment();
	bool skipcomments();
	int keyword(void);
	void getlabel(void);
	void appendlabelvalue(labeltypes type, int value);
	void appendlabeladdress(labeltypes type, int offset = 0);
	int transword(void);
	int transnum(int type);
	void checkforkeyword();
	int parsecommand();

public:
	void compilecon(const char* filenam);
	void setmusic();
	int getErrorCount() { return errorcount; }
};

//---------------------------------------------------------------------------
//
// label data
//
//---------------------------------------------------------------------------

static const char* labeltypenames[] = {
	"define",
	"state",
	"actor",
	"action",
	"ai",
	"move"
};

struct labeldef
{
	labelstring name;
	labeltypes type;
	int value;

	int compare(const char* c) const { return name.compare(c); }
	const char* GetChars() { return name.GetChars(); }

};

// These arrays contain the global output from the compiler.
static TArray<labeldef> labels;
TArray<int> ScriptCode;

//---------------------------------------------------------------------------
//
// synthesize the instruction list
//
//---------------------------------------------------------------------------

#define cmd(a) { #a, concmd_ ## a },
#define cmdx(a, b) { b, concmd_ ## a },
#define cmda(a,b) { #a, concmd_ ## b },

struct ConCommand
{
	char cmd[20];	// the longest instruction name is 'ifactornotstayput' at 17 characters so if this changes this field must be enlarged.
	int instr;
};

static ConCommand cmdList[] =
{
#include "condef.h"
};

#undef cmd
#undef cmdx
#undef cmda

static int cmdCmp(const void *a, const void *b)
{
	auto A = (ConCommand*)a;
	auto B = (ConCommand*)b;
	return strcmp(A->cmd, B->cmd);
}

void SortCommands()
{
	qsort(cmdList, countof(cmdList), sizeof(ConCommand), cmdCmp);
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

enum
{
	ERROR_ISAKEYWORD = 1,
	ERROR_PARMUNDEFINED,
	WARNING_DUPLICATEDEFINITION,
	ERROR_COULDNOTFIND,
	ERROR_VARREADONLY,
	ERROR_NOTAGAMEDEF,
	ERROR_NOTAGAMEVAR,
	ERROR_OPENBRACKET,
	ERROR_CLOSEBRACKET,
	ERROR_NOENDSWITCH,
};

void ConCompiler::ReportError(int error)
{
	const char* fn = fileSystem.GetFileFullName(currentsourcefile);
	switch (error)
	{
	case ERROR_ISAKEYWORD:
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Symbol '%s' is a key word.\n",
			fn, line_number, parsebuf);
		break;
	case ERROR_PARMUNDEFINED:
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Parameter '%s' is undefined.\n",
			fn, line_number, parsebuf);
		break;
	case WARNING_DUPLICATEDEFINITION:
		Printf(TEXTCOLOR_YELLOW "  * WARNING.(%s, line %d) Duplicate definition '%s' ignored.\n",
			fn, line_number, parselabel.GetChars());
		break;
	case ERROR_COULDNOTFIND:
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Could not find '%s'.\n",
			fn, line_number, parselabel.GetChars());
		break;
	case ERROR_VARREADONLY:
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Variable '%s' is read-only.\n",
			fn, line_number, parselabel.GetChars());
		break;
	case ERROR_NOTAGAMEDEF:
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Symbol '%s' is not a Game Definition.\n",
			fn, line_number, parselabel.GetChars());
		break;
	case ERROR_NOTAGAMEVAR:
		Printf(TEXTCOLOR_RED "  * ERROR! (%s, line %d) Symbol '%s' is not a defined Game Variable.\n",
			fn, line_number, parselabel.GetChars());
		break;
	case ERROR_OPENBRACKET:
		Printf(TEXTCOLOR_RED "  * ERROR! (%s, line %d) Found more '{' than '}' before '%s'.\n",
			fn, line_number, parsebuf);
		break;
	case ERROR_CLOSEBRACKET:
		Printf(TEXTCOLOR_RED "  * ERROR! (%s, line %d) Found more '}' than '{' before '%s'.\n",
			fn, line_number, parsebuf);
		break;
	case ERROR_NOENDSWITCH:
		Printf(TEXTCOLOR_RED "  * ERROR! (%s, line %d) Did not find endswitch before '%s'.\n",
			fn, line_number, parsebuf);
		break;

	}
}

//---------------------------------------------------------------------------
//
// binary search for keyword
//
//---------------------------------------------------------------------------

int ConCompiler::getkeyword(const char* text)
{
	ptrdiff_t min = 0;
	ptrdiff_t max = countof(cmdList) - 1;
	
	while (min <= max)
	{
		int mid = (min + max) >> 1;
		const int comp = strcmp(text, cmdList[mid].cmd);
		
		if (comp == 0)
		{
			return cmdList[mid].instr;
		}
		else if (comp > 0)
		{
			min = mid + 1;
		}
		else
		{
			max = mid - 1;
		}
	}
	return -1; 
}

//---------------------------------------------------------------------------
//
// label management
//
//---------------------------------------------------------------------------

FString ConCompiler::translatelabeltype(int type)
{
	FString buf;
	for (int i = 0; i < 6; i++) 
	{
		if (!(type & (1 << i))) continue;
		if (buf.Len()) buf += " or ";
		buf += labeltypenames[i];
	}
	return buf;
}

int findlabel(const char* text, bool ignorecase = false)
{
	for (unsigned j = 0; j < labels.Size(); j++)
	{
		if (labels[j].compare(text) == 0)
		{
			return j;
		}
	}
	if (ignorecase)
	{
		for (unsigned j = 0; j < labels.Size(); j++)
		{
			if (labels[j].name.comparei(text) == 0)
			{
				return j;
			}
		}
	}
	return -1;
}

// This is for the 'spawn' CCMD.
int getlabelvalue(const char* text)
{
	int lnum = findlabel(text, true);
	if (lnum >= 0 && labels[lnum].type != LABEL_DEFINE) return -1;
	return lnum < 0 ? -1 : labels[lnum].value;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

bool ConCompiler::ispecial(char c)
{
	if (c == 0x0a)
	{
		line_number++;
		return true;
	}

	// oh joy - we need to skip some more characters here to allow running broken scripts.
	if (c == ' ' || c == '\t' || c == 0x0d || c == '(' || c == ')' || c == ',' || c == ';')
		return true;

	return false;
}

static bool isaltok(char c)
{
	// isalnum pukes on negative input.
	return c > 0 && (isalnum(c) || c == '{' || c == '}' || c == '/' || c == '*' || c == '-' || c == '_' || c == '.');
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void ConCompiler::skiptoendofline()
{
	while (*textptr != 0x0a && *textptr != 0x0d && *textptr != 0)
		textptr++;
}

void ConCompiler::skipwhitespace()
{
	while (*textptr == ' ' || *textptr == '\t' || *textptr == '\r' || *textptr == '\n')
	{
		if (*textptr == '\n') line_number++;
		textptr++;
	}
}

void ConCompiler::skipblockcomment()
{
	while (*textptr != '*' || textptr[1] != '/')
	{
		if (*textptr == '\n') line_number++;
		if (*textptr == 0) return;	// reached the end of the file
		textptr++;
	}
	textptr += 2;
}

bool ConCompiler::skipcomments()
{
	while (true)
	{
		skipwhitespace();
		if (*textptr == '/' && textptr[1] == '/')
		{
			skiptoendofline();
			continue;
		}
		if (*textptr == '/' && textptr[1] == '*')
		{
			skipblockcomment();
			continue;
		}
		// stop if we got something else

		// this line got added to skip over a stray semicolon in RR's COOT.CON.
		if (ispecial(*textptr)) textptr++;
		break;
	}
	return *textptr != 0;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int ConCompiler::keyword(void)
{
	int i;
	const char* temptextptr;

	skipcomments();
	temptextptr = textptr;

	while (isaltok(*temptextptr) == 0)
	{
		temptextptr++;
		if (*temptextptr == 0)
			return 0;
	}

	i = 0;
	while (isaltok(*temptextptr))
	{
		parsebuf[i] = *(temptextptr++);
		i++;
	}
	parsebuf[i] = 0;

	return getkeyword(parsebuf);
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

void ConCompiler::getlabel(void)
{
	int i;

	skipcomments();

	while (isalnum(*textptr & 0xff) == 0)
	{
		if (*textptr == 0x0a) line_number++;
		textptr++;
		if (*textptr == 0)
			return;
	}

	i = 0;
	while (ispecial(*textptr) == 0)
		parselabel[i++] = *(textptr++);

	parselabel[i] = 0;
}

//---------------------------------------------------------------------------
//
// script buffer access wrappers.
//
//---------------------------------------------------------------------------

static void setscriptvalue(int offset, int value)
{
	ScriptCode[offset] = value;
}

static int scriptpos()
{
	return ScriptCode.Size();
}

static void appendscriptvalue(int value)
{
	ScriptCode.Push(value);
}

static int popscriptvalue()
{
	decltype(ScriptCode)::value_type p;
	ScriptCode.Pop(p);
	return p;
}

void reservescriptspace(int space)
{
	for (int i = 0; i < space; i++) ScriptCode.Push(0);
}

/*
void pushlabeladdress()
{
	labelcode.Push(script.Size());
}
*/

void ConCompiler::appendlabelvalue(labeltypes type, int value)
{
	labels.Reserve(1);
	labels.Last().type = type;
	labels.Last().name = parselabel;
	labels.Last().value = value;
}

void ConCompiler::appendlabeladdress(labeltypes type, int offset)
{
	appendlabelvalue(type, ScriptCode.Size() + offset);
}


//---------------------------------------------------------------------------
//
//Returns its code #
//
//---------------------------------------------------------------------------

int ConCompiler::transword(void) 
{
	int i, l;

	skipcomments();

	while (isaltok(*textptr) == 0)
	{
		if (*textptr == 0x0a) line_number++;
		if (*textptr == 0)
			return -1;
		textptr++;
	}

	l = 0;
	while (isaltok(*(textptr + l)) && !(*(textptr + l) == '.'))
	{
		if (l < 31)
		{
			parsebuf[l] = textptr[l];
			l++;
		}
	}
	parsebuf[l] = 0;

	i = getkeyword(parsebuf);
	if (i >= 0)
	{
		appendscriptvalue(i);
		textptr += l;
		return i;
	}

	textptr += l;

	const char* fn = fileSystem.GetFileFullName(currentsourcefile);
	if (parsebuf[0] == '{' && parsebuf[1] != 0)
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Expecting a SPACE or CR between '{' and '%s'.\n", fn, line_number, parsebuf + 1);
	else if (parsebuf[0] == '}' && parsebuf[1] != 0)
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Expecting a SPACE or CR between '}' and '%s'.\n", fn, line_number, parsebuf + 1);
	else if (parsebuf[0] == '/' && parsebuf[1] == '/' && parsebuf[2] != 0)
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Expecting a SPACE between '//' and '%s'.\n", fn, line_number, parsebuf + 2);
	else if (parsebuf[0] == '/' && parsebuf[1] == '*' && parsebuf[2] != 0)
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Expecting a SPACE between '/*' and '%s'.\n", fn, line_number, parsebuf + 2);
	else if (parsebuf[0] == '*' && parsebuf[1] == '/' && parsebuf[2] != 0)
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Expecting a SPACE between '*/' and '%s'.\n", fn, line_number, parsebuf + 2);
	else
		Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Keyword expected, got '%s'.\n", fn, line_number, parsebuf + 2);

	errorcount++;
	return -1;
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int ConCompiler::transnum(int type)
{
	int l;

	while (isaltok(*textptr) == 0)
	{
		if (*textptr == 0x0a) line_number++;
		textptr++;
		if (*textptr == 0)
			return 0;
	}


	l = 0;
	while (isaltok(*(textptr + l)))
	{
		if (l < 31)
		{
			parsebuf[l] = textptr[l];
			l++;
		}
	}
	parsebuf[l] = 0;

	if (getkeyword(parsebuf) >= 0)
	{
		errorcount++;
		ReportError(ERROR_ISAKEYWORD);
		textptr += l;
	}


	for (unsigned i = 0; i < labels.Size(); i++)
	{
		if (labels[i].compare(parsebuf) == 0)
		{
			// Non-values can be compared with 0.
			if (labels[i].type & type || (labels[i].value == 0))
			{
				appendscriptvalue(labels[i].value);
				textptr += l;
				return labels[i].value;
			}
			appendscriptvalue(0);
			textptr += l;
			auto el = translatelabeltype(type);
			auto gl = translatelabeltype(labels[i].type);
			const char* fn = fileSystem.GetFileFullName(currentsourcefile);
			Printf(TEXTCOLOR_YELLOW "  * WARNING.(%s, line %d) %s: Expected a '%s' label but found a '%s' label instead.\n", fn, line_number, labels[i].GetChars(), el.GetChars(), gl.GetChars());
			return -1;  // valid label name, but wrong type
		}
	}

	if (isdigit(*textptr) == 0 && *textptr != '-')
	{
		ReportError(ERROR_PARMUNDEFINED);
		errorcount++;
		textptr += l;
#ifdef FOR_LATER
		if (GetDefID(parsebuf) >= 0)
		{
			Printf(TEXTCOLOR_ORANGE "     Game Variable not expected\n");
		}
#endif
		return 0;
	}

	// Now it's getting nasty... With all of C's integer conversion functions we have to look for undefined behavior and truncation problems. This one's the least problematic approach
	// that ignores octal conversion.
	int64_t value;
	char *outp;
	bool ishex = (textptr[0] == '0' && tolower(textptr[1]) == 'x') || (textptr[0] == '-' && textptr[1] == '0' && tolower(textptr[2]) == 'x');
	if (*textptr == '-') value = strtoll(textptr, &outp, ishex? 0 : 10);
	else value = strtoull(textptr, &outp, ishex ? 0 : 10);
	if (*outp != 0)
	{
		// conversion was not successful.
	}
	appendscriptvalue(int(value));	// truncate the parsed value to 32 bit.

	if (type != LABEL_DEFINE && value != 0)
	{
		const char* fn = fileSystem.GetFileFullName(currentsourcefile);
		Printf(TEXTCOLOR_YELLOW "  * WARNING.(%s, line %d) Expected an identifier, got a numeric literal %d.\n", fn, line_number, (int)value);
	}


	textptr += l;
	return int(value);
}

//---------------------------------------------------------------------------
//
// just to reduce some excessive boilerplate. This block reappeared
// endlessly in parsecommand
//
//---------------------------------------------------------------------------

void ConCompiler::checkforkeyword()
{
	if (getkeyword(parselabel) >= 0)
	{
		errorcount++;
		ReportError(ERROR_ISAKEYWORD);
	}
}

//---------------------------------------------------------------------------
//
// 
//
//---------------------------------------------------------------------------

int ConCompiler::parsecommand()
{
	const char* fn = fileSystem.GetFileFullName(currentsourcefile);
	int i, j, k;
	int tempscrptr;
	uint8_t done, temp_ifelse_check;// , tw;
	int temp_line_number;
	int temp_current_file;
	int lnum;

	// Do not count warnings here and allow more errors before bailing out.
	if ((errorcount) > 64 || (*textptr == '\0') || (*(textptr + 1) == '\0')) return 1;
	int tw = transword();

	switch (tw)
	{
	default:
	case -1:
		return 0; //End

	case concmd_state:
		if (parsing_actor == 0 && parsing_state == 0)
		{
			getlabel();
			popscriptvalue();
			appendlabeladdress(LABEL_STATE);

			parsing_state = 1;

			return 0;
		}

		getlabel();
		checkforkeyword();

		lnum = findlabel(parselabel);

		if (lnum < 0)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) State '%s' not found.\n", fn, line_number, parselabel.GetChars());
			errorcount++;
			return 0;
		}
		appendscriptvalue(labels[lnum].value);
		return 0;

	case concmd_ends:
		if (parsing_state == 0)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'ends' with no 'state'.\n", fn, line_number);
			errorcount++;
		}
		//			  else
		{
			if (num_squigilly_brackets > 0)
			{
				ReportError(ERROR_OPENBRACKET);
				errorcount++;
			}
			if (num_squigilly_brackets < 0)
			{
				ReportError(ERROR_CLOSEBRACKET);
				errorcount++;
			}
			parsing_state = 0;
		}
		return 0;

	case concmd_gamevar:
	{
		// syntax: gamevar <var1> <initial value> <flags>
		// defines var1 and sets initial value.
		// flags are used to define usage
		// (see top of this files for flags)
		getlabel();	//GetGameVarLabel();
		// Check to see it's already defined
		popscriptvalue();

		if (getkeyword(parselabel) >= 0)
		{
			errorcount++;
			ReportError(ERROR_ISAKEYWORD);
			return 0;
		}

		transnum(LABEL_DEFINE);	// get initial value
		j = popscriptvalue();

		transnum(LABEL_DEFINE);	// get flags
		lnum = popscriptvalue();
		if (strlen(parselabel) > (MAXVARLABEL - 1))
		{
			warningcount++;
			Printf(TEXTCOLOR_RED "  * WARNING.(%s, line %d) Variable Name '%s' too int (max is %d)\n", fn, line_number, parselabel.GetChars(), MAXVARLABEL - 1);
			return 0;
		}
		int res = AddGameVar(parselabel, j, lnum & (~(GAMEVAR_FLAG_DEFAULT | GAMEVAR_FLAG_SECRET)));
		if (res < 0)
		{
			errorcount++;
			if (res == -1) Printf(TEXTCOLOR_RED "  * ERROR.(%s, line %d) Duplicate game variable definition '%s'.\n", fn, line_number, parselabel.GetChars());
			else if (res == -2) Printf(TEXTCOLOR_RED "  * ERROR.(%s, line %d) '%s' maximum number of game variables exceeded.\n", fn, line_number, parselabel.GetChars());
			return 0;
		}

		return 0;
	}
	case concmd_define:
		getlabel();
		checkforkeyword();
		lnum = findlabel(parselabel);
		if (lnum >= 0)
		{
			warningcount++;
			ReportError(WARNING_DUPLICATEDEFINITION);
			break;
		}

		transnum(LABEL_DEFINE);
		i = popscriptvalue();
		if (lnum < 0)
		{
			appendlabelvalue(LABEL_DEFINE, i);
		}
		popscriptvalue();
		return 0;

	case concmd_palfrom:

		for (j = 0; j < 4; j++)
		{
			if (keyword() == -1)
				transnum(LABEL_DEFINE);
			else break;
		}

		while (j < 4)
		{
			appendscriptvalue(0);
			j++;
		}
		return 0;

	case concmd_move:
		if (parsing_actor || parsing_state)
		{
			transnum(LABEL_MOVE);

			j = 0;
			while (keyword() == -1)
			{
				transnum(LABEL_DEFINE);
				j |= popscriptvalue();
			}
			appendscriptvalue(j);
		}
		else
		{
			popscriptvalue();
			getlabel();
			// Check to see it's already defined

			checkforkeyword();

			for (i = 0; i < (int)labels.Size(); i++)
				if (labels[i].compare(parselabel) == 0)
				{
					warningcount++;
					Printf(TEXTCOLOR_RED "  * WARNING.(%s, line %d) Duplicate move '%s' ignored.\n", fn, line_number, parselabel.GetChars());
					break;
				}
			if (i == labels.Size())
				appendlabeladdress(LABEL_MOVE);
			for (j = 0; j < 2; j++)
			{
				if (keyword() >= 0) break;
				transnum(LABEL_DEFINE);
			}
			for (k = j; k < 2; k++)
			{
				appendscriptvalue(0);
			}
		}
		return 0;

	case concmd_music:
	{
		popscriptvalue();
		transnum(LABEL_DEFINE); // Volume Number (0/4)
		k = popscriptvalue() - 1;
		if (k < 0) specialmusic.Clear();

		i = 0;
		// get the file name...
		while (keyword() == -1)
		{
			while (isaltok(*textptr) == 0)
			{
				if (*textptr == 0x0a) line_number++;
				textptr++;
				if (*textptr == 0) break;
			}
			j = 0;
			parsebuffer.Clear();
			while (isaltok(*(textptr + j)))
			{
				parsebuffer.Push(textptr[j]);
				j++;
			}
			parsebuffer.Push(0);
			if (k >= 0)
			{
				tempMusic.Reserve(1);
				tempMusic.Last().levnum = levelnum(k, i);
				tempMusic.Last().music = parsebuffer.Data();
			}
			else
			{
				specialmusic.Push(parsebuffer.Data());
			}
			textptr += j;
			i++;
		}

		return 0;
	}
	case concmd_include:
	{
		popscriptvalue();
		skipcomments();

		parsebuffer.Clear();
		while (isaltok(*textptr) == 0)
		{
			if (*textptr == 0x0a) line_number++;
			textptr++;
			if (*textptr == 0) break;
		}
		j = 0;
		while (isaltok(*textptr))
		{
			parsebuffer.Push(*(textptr++));
			j++;
		}
		parsebuffer.Push(0);

		auto fni = fileSystem.FindFile(parsebuffer.Data());
		if (fni < 0)
		{
			errorcount++;
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Could not find '%s'.\n", fn, line_number, parsebuffer.Data());

			ReportError(ERROR_COULDNOTFIND);
			return 0;
		}

		auto data = fileSystem.GetFileData(fni, 1);

		temp_current_file = currentsourcefile;
		currentsourcefile = fni;

		temp_line_number = line_number;
		line_number = 1;
		temp_ifelse_check = checking_ifelse;
		checking_ifelse = 0;
		auto origtptr = textptr;
		textptr = (char*)data.Data();

		do
			done = parsecommand();
		while (done == 0);

		textptr = origtptr;
		line_number = temp_line_number;
		checking_ifelse = temp_ifelse_check;
		currentsourcefile = temp_current_file;
		if (*textptr == '"') textptr++;	// needed for RR.

		return 0;
	}

	case concmd_ai:
		if (parsing_actor || parsing_state)
			transnum(LABEL_AI);
		else
		{
			popscriptvalue();
			getlabel();
			checkforkeyword();

			lnum = findlabel(parselabel);
			if (lnum >= 0)
			{
				warningcount++;
				Printf(TEXTCOLOR_RED "  * WARNING.(%s, line %d) Duplicate ai '%s' ignored.\n", fn, line_number, parselabel.GetChars());
			}
			else appendlabeladdress(LABEL_AI);

			for (j = 0; j < 3; j++)
			{
				if (keyword() >= 0) break;
				if (j == 2)
				{
					k = 0;
					while (keyword() == -1)
					{
						transnum(LABEL_DEFINE);
						k |= popscriptvalue();
					}
					appendscriptvalue(k);
					return 0;
				}
				else transnum(j==0? LABEL_ACTION : LABEL_MOVE);
			}
			for (k = j; k < 3; k++)
			{
				appendscriptvalue(0);
			}
		}
		return 0;

	case concmd_action:
		if (parsing_actor || parsing_state)
			transnum(LABEL_ACTION);
		else
		{
			popscriptvalue();
			getlabel();
			checkforkeyword();

			lnum = findlabel(parselabel);
			if (lnum >= 0)
			{
				warningcount++;
				Printf(TEXTCOLOR_RED "  * WARNING.(%s, line %d) Duplicate event '%s' ignored.\n", fn, line_number, parselabel.GetChars());
			}
			else appendlabeladdress(LABEL_ACTION);

			for (j = 0; j < 5; j++)
			{
				if (keyword() >= 0) break;
				transnum(LABEL_DEFINE);
			}
			for (k = j; k < 5; k++)
			{
				appendscriptvalue(0);
			}
		}
		return 0;

	case concmd_actor:
	case concmd_useractor:	// merged with 'actor' because the code is identical except for the added type parameter.
	{
		if (parsing_state)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'actor' within 'state'.\n", fn, line_number);
			errorcount++;
		}

		if (parsing_actor)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'actor' within 'actor'.\n", fn, line_number);
			errorcount++;
		}

		num_squigilly_brackets = 0;
		popscriptvalue();
		parsing_actor = scriptpos();

		if (tw == concmd_useractor)
		{ 
			transnum(LABEL_DEFINE);
			j = popscriptvalue();
		}

		transnum(LABEL_DEFINE);
		lnum = popscriptvalue();

		actorinfo[lnum].scriptaddress = parsing_actor;	// TRANSITIONAL should only store an index
		if (tw == concmd_useractor)
		{
			if (j & 1)
				actorinfo[lnum].flags |= SFLAG_BADGUY;

			if (j & 2)
				actorinfo[lnum].flags |= (SFLAG_BADGUY | SFLAG_BADGUYSTAYPUT);
		}

		for (j = 0; j < 4; j++)
		{
			setscriptvalue(parsing_actor + j, 0);
			if (j == 3)
			{
				j = 0;
				while (keyword() == -1)
				{
					transnum(LABEL_DEFINE);
					
					j |= popscriptvalue();
				}
				appendscriptvalue(j);
				break;
			}
			else
			{
				if (keyword() >= 0)
				{
					reservescriptspace(4 - j);
					break;
				}
				switch (j)
				{
				case 0: transnum(LABEL_DEFINE); break;
				case 1: transnum(LABEL_ACTION); break;
				case 2: transnum(LABEL_MOVE | LABEL_DEFINE); break;
				}
				// This code was originally here but is a no-op, because both source and destination are the same here.			
				//*(parsing_actor + j) = *(scriptaddress - 1);
			}
		}

		checking_ifelse = 0;

		return 0;
		}

	case concmd_onevent:
		if (parsing_state)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'onevent' within 'state'.\n", fn, line_number);
			errorcount++;
		}

		if (parsing_actor)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'onevent' within 'actor'.\n", fn, line_number);
			errorcount++;
		}

		num_squigilly_brackets = 0;
		popscriptvalue();
		parsing_event = parsing_actor = scriptpos();

		transnum(LABEL_DEFINE);
		j = popscriptvalue();
		if (j< 0 || j> EVENT_MAXEVENT)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Invalid Event ID.\n", fn, line_number);
			errorcount++;
			return 0;
		}
		apScriptGameEvent[j] = parsing_event;

		checking_ifelse = 0;

		return 0;


	case concmd_cstat:
		transnum(LABEL_DEFINE);
#if 0
		// the following checks are being performed by EDuke32 and RedNukem - not sure if this really should be done.
		// DukeGDX and RedneckGDX do not perform these checks. Code pasted here for making a decision later.

		i = popscriptvalue();
		if (i == 32767)
		{
			i = 32768;
			Printf(TEXTCOLOR_RED "  * WARNING!(%s, line %d) tried to set cstat 32767, using 32768 instead.\n", fn, line_number);
			warningcount++;
		}
		else if ((i & 48) == 48)
		{
			Printf(TEXTCOLOR_RED "  * WARNING!(%s, line %d) tried to set cstat %d, using %d instead.\n", fn, line_number, i, i ^ 48);
			i ^= 48;
			warningcount++;
		}
		appendscriptvalue(i);
#endif
		return 0;


	case concmd_sound:
	case concmd_globalsound:
	case concmd_soundonce:
	case concmd_stopsound:
	case concmd_lotsofglass:
	case concmd_strength:
	case concmd_shoot:
	case concmd_addphealth:
	case concmd_spawn:
	case concmd_count:
	case concmd_endofgame:
	case concmd_spritepal:
	case concmd_cactor:
	case concmd_quote:
	case concmd_money:
	case concmd_addkills:
	case concmd_debug:
	case concmd_addstrength:
	case concmd_cstator:
	case concmd_mail:
	case concmd_paper:
	case concmd_sleeptime:
	case concmd_clipdist:
	case concmd_isdrunk:
	case concmd_iseat:
	case concmd_newpic:
		transnum(LABEL_DEFINE);
		return 0;

	case concmd_addammo:
	case concmd_addweapon:
	case concmd_sizeto:
	case concmd_sizeat:
	case concmd_debris:
	case concmd_addinventory:
	case concmd_guts:
		transnum(LABEL_DEFINE);
		transnum(LABEL_DEFINE);
		return 0;

	case concmd_hitradius:
		transnum(LABEL_DEFINE);
		transnum(LABEL_DEFINE);
		transnum(LABEL_DEFINE);
		transnum(LABEL_DEFINE);
		transnum(LABEL_DEFINE);
		break;

	case concmd_else:
		if (checking_ifelse)
		{
			checking_ifelse--;
			tempscrptr = scriptpos();
			reservescriptspace(1); //Leave a spot for the fail location
			parsecommand();
			setscriptvalue(tempscrptr, scriptpos());
		}
		else
		{
			popscriptvalue();
			warningcount++;
			Printf(TEXTCOLOR_YELLOW "  * WARNING.(%s, line %d) Found 'else' with no 'if', ignored.\n", fn, line_number);
		}

		return 0;
	case concmd_setvar:
	case concmd_addvar:
		// syntax: [rand|add|set]var	<var1> <const1>
		// sets var1 to const1
		// adds const1 to var1 (const1 can be negative...)

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();

		// Check to see if it's a keyword
		checkforkeyword();

		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEDEF);
			return 0;
		}
		if (aGameVars[i].dwFlags & GAMEVAR_FLAG_READONLY)
		{
			errorcount++;
			ReportError(ERROR_VARREADONLY);
			return 0;

		}
		appendscriptvalue(i);	// the ID of the DEF (offset into array...)

		transnum(LABEL_DEFINE);	// the number to check against...
		return 0;

	case concmd_setvarvar:
	case concmd_addvarvar:
		// syntax: [add|set]varvar <var1> <var2>
		// sets var1 = var2
		// adds var1 and var2 with result in var1

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();

		checkforkeyword();

		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEDEF);
			return 0;
		}
		if (aGameVars[i].dwFlags & GAMEVAR_FLAG_READONLY)
		{
			errorcount++;
			ReportError(ERROR_VARREADONLY);
			return 0;

		}
		appendscriptvalue(i);	// the ID of the DEF (offset into array...)

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();
		checkforkeyword();

		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEDEF);
			return 0;
		}
	
		appendscriptvalue(i);	// the ID of the DEF (offset into array...)
		return 0;

	case concmd_ifvarvarg:
	case concmd_ifvarvarl:
	case concmd_ifvarvare:

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();

		checkforkeyword();

		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEDEF);
			return 0;
		}
		appendscriptvalue(i);	// the ID of the DEF (offset into array...)

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();

		checkforkeyword();

		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEDEF);
			return 0;
		}
		appendscriptvalue(i);	// the ID of the DEF (offset into array...)
		goto if_common;

	case concmd_ifvarl:
	case concmd_ifvarg:
	case concmd_ifvare:

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();

		checkforkeyword();
		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEVAR);
			return 0;
		}
		appendscriptvalue(i);	// the ID of the DEF (offset into array...)

		transnum(LABEL_DEFINE);	// the number to check against...
		goto if_common;

	case concmd_addlogvar:
		// syntax: addlogvar <var>

		appendscriptvalue(currentsourcefile);
		appendscriptvalue(line_number);

		// get the ID of the DEF
		getlabel();	//GetGameVarLabel();

		checkforkeyword();

		i = GetDefID(parselabel);
		if (i < 0)
		{	// not a defined DEF
			errorcount++;
			ReportError(ERROR_NOTAGAMEDEF);
			return 0;
		}
		appendscriptvalue(i);

		return 0;

	case concmd_addlog:
		// syntax: addlog

		// source file.
		appendscriptvalue(currentsourcefile);

		// prints the line number in the log file.
		appendscriptvalue(line_number);
		return 0;

	case concmd_ifp:
		j = 0;
		do
		{
			transnum(LABEL_DEFINE);
			j |= popscriptvalue();
		} while (keyword() == -1);
		appendscriptvalue(j);
		goto if_common;

	case concmd_ifpinventory:
		transnum(LABEL_DEFINE);
	case concmd_ifrnd:
	case concmd_ifpdistl:
	case concmd_ifpdistg:
	case concmd_ifai:
	case concmd_ifwasweapon:
	case concmd_ifaction:
	case concmd_ifactioncount:
	case concmd_ifmove:
	case concmd_ifcount:
	case concmd_ifactor:
	case concmd_ifstrength:
	case concmd_ifspawnedby:
	case concmd_ifgapzl:
	case concmd_iffloordistl:
	case concmd_ifceilingdistl:
		//		  case 74:
	case concmd_ifphealthl:
	case concmd_ifspritepal:
	case concmd_ifgotweaponce:
	case concmd_ifangdiffl:
	case concmd_ifactorhealthg:
	case concmd_ifactorhealthl:
	case concmd_ifsoundid:
	case concmd_ifsounddist:
		transnum(tw == concmd_ifai? LABEL_AI : tw == concmd_ifaction? LABEL_ACTION : tw == concmd_ifmove? LABEL_MOVE : LABEL_DEFINE);
	case concmd_ifonwater:
	case concmd_ifinwater:
	case concmd_ifactornotstayput:
	case concmd_ifcansee:
	case concmd_ifhitweapon:
	case concmd_ifsquished:
	case concmd_ifdead:
	case concmd_ifcanshoottarget:
	case concmd_ifhitspace:
	case concmd_ifoutside:
	case concmd_ifmultiplayer:
	case concmd_ifinspace:
	case concmd_ifbulletnear:
	case concmd_ifrespawn:
	case concmd_ifinouterspace:
	case concmd_ifnotmoving:
	case concmd_ifawayfromwall:
	case concmd_ifcanseetarget:
	case concmd_ifnosounds:
	case concmd_ifnocover:
	case concmd_ifhittruck:
	case concmd_iftipcow:
	case concmd_ifonmud:
	case concmd_ifcoop:
	case concmd_ifmotofast:
	case concmd_ifwind:
	case concmd_ifonmoto:
	case concmd_ifonboat:
	case concmd_ifsizedown:
	case concmd_ifplaybackon:
	// case concmd_iffindnewspot:	// RRDH
	// case concmd_ifpupwind:

	if_common:	// this code is identical for all 'if...'instructions.
	{
		tempscrptr = scriptpos();
		reservescriptspace(1); //Leave a spot for the fail location

		skipcomments();
		parsecommand();

		setscriptvalue(tempscrptr, scriptpos());
		auto k = keyword();
		// Cannot be done - the code starts misbehaving with this check, it is especially noticeable on the soldiers in NAM.
		// Unfortunately this means one less error check, but ultimately CON is too broken to begin with anyway
#if 0
		if (k == concmd_else)	/ only increment checking_ifelse if there actually is an else. Otherwise this would break the entire checking logic and render it non-functional
#endif
			checking_ifelse++;
		return 0;
	}
	case concmd_leftbrace:
		num_squigilly_brackets++;
		do
			done = parsecommand();
		while (done == 0);
		return 0;
	case concmd_rightbrace:
		num_squigilly_brackets--;
		if (num_squigilly_brackets < 0)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found more '}' than '{'.\n", fn, line_number);
			errorcount++;
		}
		return 1;
	case concmd_betaname:
		popscriptvalue();
		// not used anywhere, just parse over it.
		skiptoendofline();
		return 0;

	case concmd_definevolumename:
		popscriptvalue();
		transnum(LABEL_DEFINE);
		j = popscriptvalue();
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		i = 0;

		parsebuffer.Clear();
		while (*textptr != 0x0a && *textptr != 0x0d && *textptr != 0)		// JBF 20040127: end of file checked
		{
			parsebuffer.Push(*textptr);
			textptr++, i++;
		}
		parsebuffer.Push(0);
		gVolumeNames[j] = FStringTable::MakeMacro(parsebuffer.Data(), i);
		return 0;
	case concmd_defineskillname:
		popscriptvalue();
		transnum(LABEL_DEFINE);
		j = popscriptvalue();
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		i = 0;

		parsebuffer.Clear();
		while (*textptr != 0x0a && *textptr != 0x0d && *textptr != 0)		// JBF 20040127: end of file checked
		{
			parsebuffer.Push(*textptr);
			textptr++, i++;
		}
		parsebuffer.Push(0);
		gSkillNames[j] = FStringTable::MakeMacro(parsebuffer.Data(), i);
		return 0;

	case concmd_definelevelname:
	{
		popscriptvalue();
		transnum(LABEL_DEFINE);
		j = popscriptvalue();
		transnum(LABEL_DEFINE);
		k = popscriptvalue();
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		i = 0;
		parsebuffer.Clear();
		while (*textptr != ' ' && *textptr != '\t' && *textptr != 0x0a && *textptr != 0x0d && *textptr != 0)	// JBF 20040127: end of file checked
		{
			parsebuffer.Push(*textptr);
			textptr++, i++;
		}
		parsebuffer.Push(0);
		auto levnum = levelnum(j, k);
		auto map = FindMapByLevelNum(levnum);
		if (!map) map = AllocateMap();
		map->SetFileName(parsebuffer.Data());

		while (*textptr == ' ' || *textptr == '\t') textptr++;

		map->parTime =
			(((*(textptr + 0) - '0') * 10 + (*(textptr + 1) - '0')) * 26 * 60) +
			(((*(textptr + 3) - '0') * 10 + (*(textptr + 4) - '0')) * 26);

		textptr += 5;
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		map->designerTime =
			(((*(textptr + 0) - '0') * 10 + (*(textptr + 1) - '0')) * 26 * 60) +
			(((*(textptr + 3) - '0') * 10 + (*(textptr + 4) - '0')) * 26);

		map->levelNumber = levnum;

		textptr += 5;
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		i = 0;

		parsebuffer.Clear();
		while (*textptr != 0x0a && *textptr != 0x0d && *textptr != 0)		// JBF 20040127: end of file checked
		{
			parsebuffer.Push(*textptr);
			textptr++, i++;
		}
		parsebuffer.Push(0);
		map->name = FStringTable::MakeMacro(parsebuffer.Data());
		return 0;
	}
	case concmd_definequote:
		popscriptvalue();
		transnum(LABEL_DEFINE);
		k = popscriptvalue();
		if (k >= MAXQUOTES)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Quote number exceeds limit of %d.\n", fn, line_number, MAXQUOTES);
			errorcount++;
		}
		
		i = 0;
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		parsebuffer.Clear();
		while (*textptr != 0x0a && *textptr != 0x0d && *textptr != 0)		// JBF 20040127: end of file checked
		{
			parsebuffer.Push(*textptr);
			textptr++, i++;
		}
		parsebuffer.Push(0);
		quoteMgr.InitializeQuote(k, parsebuffer.Data(), true);
		return 0;
	case concmd_definesound:
	{
		popscriptvalue();
		transnum(LABEL_DEFINE);
		k = popscriptvalue();
		i = 0;
		while (*textptr == ' ' || *textptr == '\t') textptr++;

		parsebuffer.Clear();
		while (*textptr != ' ' && *textptr != '\t' && *textptr != 0)		// JBF 20040127: end of file checked
		{
			parsebuffer.Push(*textptr);
			textptr++, i++;
		}
		parsebuffer.Push(0);

		transnum(LABEL_DEFINE);
		int ps = popscriptvalue();
		transnum(LABEL_DEFINE);
		int pe = popscriptvalue();
		transnum(LABEL_DEFINE);
		int pr = popscriptvalue();
		transnum(LABEL_DEFINE);
		int m = popscriptvalue();
		transnum(LABEL_DEFINE);
		int vo = popscriptvalue();
		S_DefineSound(k, parsebuffer.Data(), ps, pe, pr, m, vo, 1.f);
		return 0;
	}

	case concmd_endevent:
		if (parsing_event == 0)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'endevent' without defining 'onevent'.\n", fn, line_number);
			errorcount++;
		}
		//			  else
		{
			if (num_squigilly_brackets > 0)
			{
				Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found more '{' than '}' before 'endevent'.\n", fn, line_number);
				errorcount++;
			}
			parsing_event = 0;
			parsing_actor = 0;
		}

		return 0;

	case concmd_enda:
		if (parsing_actor == 0)
		{
			Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found 'enda' without defining 'actor'.\n", fn, line_number);
			errorcount++;
		}
		//			  else
		{
			if (num_squigilly_brackets > 0)
			{
				Printf(TEXTCOLOR_RED "  * ERROR!(%s, line %d) Found more '{' than '}' before 'enda'.\n", fn, line_number);
				errorcount++;
			}
			parsing_actor = 0;
		}

		return 0;
	case concmd_break:
	case concmd_fall:
	case concmd_tip:
		//		  case 21:
	case concmd_killit:	//KILLIT
	case concmd_resetactioncount:
	case concmd_pstomp:
	case concmd_resetplayer:
	case concmd_resetcount:
	case concmd_wackplayer:
	case concmd_operate:
	case concmd_respawnhitag:
	case concmd_getlastpal:
	case concmd_pkick:
	case concmd_mikesnd:
	case concmd_tossweapon:
	case concmd_nullop:
	case concmd_destroyit:
	case concmd_larrybird:
	case concmd_strafeleft:
	case concmd_straferight:
	case concmd_slapplayer:
		//case 122:
	case concmd_tearitup:
	case concmd_smackbubba:
	case concmd_soundtagonce:
	case concmd_soundtag:
	case concmd_smacksprite:
	case concmd_fakebubba:
	case concmd_mamatrigger:
	case concmd_mamaspawn:
	case concmd_mamaquake:
	case concmd_mamaend:
	case concmd_garybanjo:
	case concmd_motoloopsnd:
	case concmd_rndmove:
		//case concmd_leavetrax:		// RRDH
		//case concmd_leavedroppings:
		//case concmd_deploybias:
		return 0;
	case concmd_gamestartup:
	{
		// What a mess. The only way to detect which game version we are running is to count the parsed values here.
		int params[34]; // 34 is the maximum for RRRA.
		int pcount = 0;
		for (int i = 0; i < 34; i++)
		{
			transnum(LABEL_DEFINE);
			params[pcount++] = popscriptvalue();
			if (keyword() != -1) break;
		}
		int pget = 0;

		if (!isRR())
		{
			if (pcount == 30) g_gameType |= GAMEFLAG_PLUTOPAK;
			else if (pcount == 31) g_gameType |= GAMEFLAG_PLUTOPAK | GAMEFLAG_WORLDTOUR;
			else if (pcount != 26) I_FatalError("Invalid CONs. Cannot detect version. gamestartup has %d entries", pcount);
		}

		popscriptvalue();
		auto parseone = [&]() { return params[pget++]; };

		ud.const_visibility = parseone();
		impact_damage = parseone();
		max_player_health = parseone();
		max_armour_amount = parseone();
		respawnactortime = parseone();
		respawnitemtime = parseone();
		dukefriction = parseone();
		if (isPlutoPak() || isRR()) gc = parseone();
		rpgblastradius = parseone();
		pipebombblastradius = parseone();
		shrinkerblastradius = parseone();
		tripbombblastradius = parseone();
		morterblastradius = parseone();
		bouncemineblastradius = parseone();
		seenineblastradius = parseone();

		max_ammo_amount[1] = parseone();
		max_ammo_amount[2] = parseone();
		max_ammo_amount[3] = parseone();
		max_ammo_amount[4] = parseone();
		max_ammo_amount[5] = parseone();
		max_ammo_amount[6] = parseone();
		max_ammo_amount[7] = parseone();
		max_ammo_amount[8] = parseone();
		max_ammo_amount[9] = parseone();
		if (isPlutoPak() || isRR()) max_ammo_amount[11] = parseone();
		if (isRR()) max_ammo_amount[12] = parseone();
		camerashitable = parseone();
		numfreezebounces = parseone();
		freezerhurtowner = parseone();
		if (PLUTOPAK || isRR())
		{
			spriteqamount = clamp(parseone(), 0, 1024);
			lasermode = parseone();
		}
		if (isWorldTour()) max_ammo_amount[12] = parseone();
		if (isRRRA())
		{
			max_ammo_amount[13] = parseone();
			max_ammo_amount[14] = parseone();
			max_ammo_amount[16] = parseone();
		}
		return 0;
	}
	}
	return 0;
}

//---------------------------------------------------------------------------
//
// split in two to allow multiple CON files.
//
//---------------------------------------------------------------------------

void ConCompiler::compilecon(const char *filenam)
{
	currentsourcefile = fileSystem.FindFile(filenam);
	if (currentsourcefile < 0)
	{
		I_FatalError("%s: Missing con file(s).", filenam);
	}
	Printf("Compiling: '%s'.\n", filenam);
	auto data = fileSystem.GetFileData(currentsourcefile, 1);
	textptr = (char*)data.Data();

	line_number = 1;
	errorcount = warningcount = 0;

	while (parsecommand() == 0);

	if ((errorcount) > 64)
		Printf(TEXTCOLOR_RED  "  * ERROR! Too many errors.");
	else if (warningcount || errorcount)
		Printf(TEXTCOLOR_ORANGE "Found %d warning(s), %d error(s).\n", warningcount, errorcount);
	if (errorcount > 0) I_FatalError("Failed to compile %s", filenam);


	// Install the crosshair toggle messages in the CVAR.
	cl_crosshair.SetToggleMessages(quoteMgr.GetRawQuote(QUOTE_CROSSHAIR_OFF), quoteMgr.GetRawQuote(QUOTE_CROSSHAIR_OFF-1));
}

//==========================================================================
//
// Fallback in case nothing got defined.
//
//==========================================================================

static const char* ConFile(void)
{
	if (userConfig.DefaultCon.IsNotEmpty()) return userConfig.DefaultCon.GetChars();

	// WW2GI anf NAM special con names got introduced by EDuke32.
	// Do we really need these?
	if (g_gameType & GAMEFLAG_WW2GI)
	{
		if (fileSystem.FindFile("ww2gi.con") >= 0) return "ww2gi.con";
	}

	if (g_gameType & GAMEFLAG_NAM)
	{
		if (fileSystem.FindFile("nam.con") >= 0) return "nam.con";
		if (fileSystem.FindFile("napalm.con") >= 0) return "napalm.con";
	}

	if (g_gameType & GAMEFLAG_NAPALM)
	{
		if (fileSystem.FindFile("napalm.con") >= 0) return "napalm.con";
		if (fileSystem.FindFile("nam.con") >= 0) return "nam.con";
	}

	// This got introduced by EDuke 2.0.
	if (g_gameType & GAMEFLAG_DUKE)
	{
		if (fileSystem.FindFile("eduke.con") >= 0) return "eduke.con";	
	}

	// the other games only use game.con.
	return "game.con";
}

//---------------------------------------------------------------------------
//
// process the music definitions after all map records are set up.
//
//---------------------------------------------------------------------------

void ConCompiler::setmusic()
{
	for (auto& tm : tempMusic)
	{
		auto map = FindMapByLevelNum(tm.levnum);
		if (map) map->music = tm.music;
	}
	tempMusic.Clear();
}

//---------------------------------------------------------------------------
//
// why was this called loadefs?
//
//---------------------------------------------------------------------------

void loadcons()
{
	for (int i = 0; i < MAXTILES; i++)
	{
		memset(&actorinfo[i], 0, sizeof(actorinfo));
	}

	ScriptCode.Clear();
	labels.Clear();

	SortCommands();

	ClearGameEvents();
	ClearGameVars();
	AddSystemVars();

	auto before = I_nsTime();

	ScriptCode.Push(0);
	ConCompiler comp;
	comp.compilecon(ConFile()); //Tokenize

	if (userConfig.AddCons) for (FString& m : *userConfig.AddCons.get())
	{
		comp.compilecon(m);
	}
	ScriptCode.ShrinkToFit();
	labels.ShrinkToFit();
	userConfig.AddCons.reset();
	setscriptvalue(0, scriptpos());

	if (comp.getErrorCount())
	{
		I_FatalError("Failed to compile CONs.");
	}
	else
	{
		auto after = I_nsTime();
		Printf("Compilation time:%.2f ms, Code Size:%u bytes. %u labels. %d/%d Variables.\n", (after-before) / 1000000.,
			(ScriptCode.Size() << 2) - 4,
			labels.Size(),
			0,//iGameVarCount,
			MAXGAMEVARS
		);
	}

	// These can only be retrieved AFTER loading the scripts.
	InitGameVarPointers();
	ResetSystemDefaults();
	S_WorldTourMappingsForOldSounds(); // create a sound mapping for World Tour.
	if (isRRRA())
	{
		// RRRA goes directly to the second episode after E1L7 to continue the game.
		int num = fileSystem.CheckNumForName("e1l7.map");
		int file = fileSystem.GetFileContainer(num);
		if (file <= fileSystem.GetMaxIwadNum())
		{
			auto maprec = FindMapByName("e1l7");
			if (maprec) maprec->nextLevel = levelnum(1, 0);
		}
	}
	else if (isRR())
	{
		// RR does not define its final level and crudely hacked it into the progression. This puts it into the E2L8 slot so that the game can naturally progress there.
		auto maprec1 = FindMapByLevelNum(levelnum(1, 6));
		auto maprec2 = FindMapByLevelNum(levelnum(1, 7));
		auto maprec3 = FindMapByName("endgame");
		int num3 = fileSystem.FindFile("endgame.map");
		if (maprec1 && !maprec2 && !maprec3 && num3 >= 0)
		{
			auto maprec = AllocateMap();
			maprec->designerTime = 0;
			maprec->parTime = 0;
			maprec->SetFileName("endgame.map");
			maprec->SetName("$TXT_CLOSEENCOUNTERS");
			maprec->levelNumber = levelnum(1, 7);
		}
	}
	comp.setmusic();
}

END_DUKE_NS