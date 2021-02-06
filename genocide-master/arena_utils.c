

// This is the first file that I created for GrenadeWorld

#include "g_local.h"

// Variables and defines go here.
#define	INI_FILE	"arena.ini"

int			allowBigHealth		= false; 	// Can players pickup large health items
int			allowSmallHealth	= false; 	// Can players pickup small health items
int			allowArmour			= false;	// Can players pickup armour
int			allowInvulnerable	= false;	// Can players be Invulnerable
int			observerAllowed     = true;		// Are players allowed to go into observer mode
int			cameraAllowed       = true;		// Are players allowed to go into camera mode
int			stdLogging			= false;
int			FragRange1			= 10;		// Number of kills required 4 weapon upgrade1
int			FragRange2			= 20;		// Number of kills required 4 weapon upgrade2
int			FragRange3			= 30;		// Number of kills required 4 weapon upgrade3
int			FragPointsKill		= 2;		// Points for killing someone using Fragpower
int			FragPointsKilled	= 4;		// Points for killing a Fragpower user
int			RequiredSpawns		= 10;		// Number of spawns required before new weapon

//
// GrenadeWorld Ini file options
//
typedef struct
{
	char	*ident;
	int		*variable;
	int		MinVariable;
	int		MaxVariable;
	int		DefaultVariable;
} INI_OPTION;

INI_OPTION	option[] = 

//	Noraml GrenadeWorld Options

	{	{"bighealth",		&allowBigHealth,	0,1,1},
		{"smallhealth",		&allowSmallHealth,	0,1,1},
		{"armor",			&allowArmour,		0,1,1},
		{"invuln",			&allowInvulnerable,	0,1,1},
		{"stdlog",			&stdLogging,		0,1,1},	// Dont change ident name
		{"allowcamera",		&cameraAllowed,		0,1,1},
		{"allowobserver",	&observerAllowed,	0,1,1},
		{"powerup1",		&FragRange1,		6,10,10},
		{"powerup2",		&FragRange2,		16,10,20},
		{"powerup3",		&FragRange3,		26,30,30},
		{"points1",			&FragPointsKill,	2,4,2},
		{"points2",			&FragPointsKilled,	3,7,4},
		{"weapspawns",		&RequiredSpawns,	5,30,10}
	};

//*************************************************************************

typedef struct   // Message of the Day
	{
	char textline[100];
	} MOTD_t;

	MOTD_t	MOTD[20];

#define MAX_OPTIONS (sizeof(option)/sizeof(option[0]))
#define MOTD_lines		3

/***********************************************************************
/*
/*	Function:	Displays text on screen upon entry into game
/*
/*	Parameters:	edict_t = main entity structure | pmenu_t = menu structure
/*
/**********************************************************************/
/*void DisplayMOTDMenu(edict_t *ent)
	{

	FILE *motd_file;
	char motd[500];
	char line[80];

	// STEVE changed this bit : read the motd from a file
	if (motd_file = fopen("genocide/motd.txt", "r"))
		{
		// we successfully opened the file "motd.txt"
		if ( fgets(motd, 500, motd_file) )
			{

			// we successfully read a line from "motd.txt" into motd
			// ... read the remaining lines now
			while ( fgets(line, 80, motd_file) )
				{
				// add each new line to motd, to create a BIG message string.
				// we are using strcat: STRing conCATenation function here.
				strcat(motd, line);
				}

			// print our message.
			gi.centerprintf (ent, motd);
			}

		// be good now ! ... close the file
		fclose(motd_file);
		}
	}
*/
void LoadMOTD( void )
	{

	FILE	*motd_file;
	char	line[80];
	int		i;

	// Open the motd file
	if (motd_file = fopen("genocide/motd.txt", "r"))
		{
		i = 0;

		// Read the lines now
		while ( fgets(line, 80, motd_file) )
			{
			// Once we've read a line copy it to the MOTD array.
			strcpy(MOTD[i].textline, line);
			i++;

			// We don't want more than 3 lines so lets piss off.
			if (i>3)
				break;
			}

		// be good now ! ... close the file
		fclose(motd_file);
		}
	}


/***********************************************************************
/*
/*	Various bonus message's
/*
/**********************************************************************/
void DisplayBonus0Msg(edict_t *ent)
	{
	char	bon0[]="********************************\n\n"
				   "200 HEALTH. DELICIOUS!!\n\n"
				   "NOW LETS KICK SOME ASS!\n\n"
				   "********************************";

	gi.centerprintf (ent, bon0);
	}

void DisplayBonus1Msg(edict_t *ent)
	{
	char	bon1[]="********************************\n\n"
				   "YOUR ENEMIES ONLY HAVE 10 HEALTH\n\n"
				   "WIPE THEM OUT - ALL OF THEM!\n\n"
				   "********************************";

	gi.centerprintf (ent, bon1);
	}

void DisplayBonus2Msg(edict_t *ent)
	{
	char	bon2[]="********************************\n\n"
				   "OH MY GOD!! DOUBLE DAMAGE HMG.\n\n"
				   "THINGS ARE GONNA HEAT UP NOW!\n\n"
				   "********************************";

	gi.centerprintf (ent, bon2);
	}

void DisplayBonus3Msg(edict_t *ent)
	{
	char	bon3[]="********************************\n\n"
				   "100 HEAVY ARMOUR POINTS.\n\n"
				   "OOO BABY JUST CALL ME ROBOCOP!\n\n"
				   "********************************";

	gi.centerprintf (ent, bon3);
	}

void DisplayBonus4Msg(edict_t *ent)
	{
	char	bon4[]="********************************\n\n"
				   "OH MY GOD!! TRIPLE DAMAGE HMG.\n\n"
				   "NOW WERE REALLY SMOKIN' !\n\n"
				   "********************************";

	gi.centerprintf (ent, bon4);
	}

void DisplayBonus5Msg(edict_t *ent)
	{
	char	bon5[]="******************************\n\n"
				   "30 COOLED HMG SHOTS.\n\n"
				   "SAVES TRYING TO PICK IT UP !\n\n"
				   "******************************";

	gi.centerprintf (ent, bon5);
	}

/***********************************************************************
/*
/*	Function:	Loads all the game settings.
/*
/*	Parameters:	None
/*
/**********************************************************************/
void LoadGrenadeWorldIni( void )
	{	
	FILE	*f;
	cvar_t	*game_dir;
	int		IniOption = 0, Processed = 0;
	char	Buffer[256], filename[256];
	char	*VariableName = NULL, *VariableValue = NULL;
	static	qboolean	AlreadyRead = false;


	if (AlreadyRead)
		return;

	game_dir = gi.cvar ("game", "", 0);

    sprintf(filename, ".\\%s\\%s", game_dir->string, INI_FILE);

	// open the *.ini file

	if ((f = fopen (filename, "r")) == NULL)
		{
		gi.dprintf("Unable to read %s. Using defaults.\n", INI_FILE);
		return;
		}

	gi.dprintf("\nProcessing Grenadeworld %s.. \n", INI_FILE);

	// read 256 characters or until we get to the eof or a return for a newline.

	while (fgets(Buffer, sizeof(Buffer), f) != NULL)
		{

		
		// Ignore this line if it starts with a #, newline, space or [ bracket.

		if (Buffer[0] != '\t' && Buffer[0] != ' ' && Buffer[0] != '\n' && Buffer[0] != '#' && Buffer[0] != '[')
			{

			// Get the variable name, skipping spaces, tabs, and newlines.

			VariableName	= strtok(Buffer, " \t\n");
			IniOption	= 0;

			// If we haven't processed the maximum number of options then keep going
			while (IniOption < MAX_OPTIONS)
				{

				// Find this option in the array of options, if we don't find it tough

				if (!strcmp(VariableName, option[IniOption].ident))
					{

					// Using NULL will continue the search for the value from where the previous
					// strtok for the variable name left off.
					VariableValue = strtok(NULL, " \t\n#");

					// If the variable name is stdlog then we want to set the flag to turn
					// logging on
					if (!strcmp(VariableName, "stdlog"))
						gi.cvar_set("stdlogfile", VariableValue);
					else
						// This will set the valu in the array using string value to integer conversion
						*option[IniOption].variable = atoi(VariableValue);

					Processed++;
					break;
					}

				IniOption++;
				}
			}
		}

	gi.dprintf("%d GrenadeWorld Options processed\n", Processed);
	fclose (f);
	AlreadyRead = true;	
	}

/***********************************************************************
/*
/*	Function:	Loads all the game settings.
/*
/*	Parameters:	None
/*
/**********************************************************************/
void VerifyIniFileValues( void )
	{	
	int	Loop;

	for ( Loop=0; Loop<MAX_OPTIONS; Loop++ )
		{

		// If the value which has been set isn't in the normal range then
		// set it to a default value.

		if ((*option[Loop].variable < option[Loop].MinVariable) ||
			(*option[Loop].variable > option[Loop].MaxVariable))
			{
			*option[Loop].variable = option[Loop].DefaultVariable;
			}
		}
	}

/***********************************************************************
/*
/*	Function:	Displays message on screen when using the blaster
/*
/**********************************************************************/
void Blaster_Message(edict_t *ent)
	{
	char	Blast_Msg[]="=================================\n"
				        "Hey! Just because the blaster\n"
						"works doesn't mean you should\n"
						"use it. Try a GrenadeLauncher :)\n"
						"=================================";

	gi.centerprintf (ent, Blast_Msg);

	}

/***********************************************************************
/*
/*	Function:	Displays a superb message of the day 
/*
/**********************************************************************/
void MOTDScoreboardMessage (edict_t *ent)
	{
	char	entry[1024];
	char	string[1400];
	int		stringlength;
	int		i, j;
	int		yofs;
	char	*seperator = "++++++++++++++++++++++++++++++++++";

	char	*selectheader[] =
			{
			"Atrophy Presents",
			"-<> GENOCIDE: HMG v1.3 Final <>-",
			"By Rat Instinct",
			"https://hub.86it.us",
			NULL
			};

	string[0] = 0;
	stringlength = 0;

	yofs = 80 - MOTD_lines * 10;
	if (yofs < 0 )
		yofs = 0;

	for (i=0; selectheader[i]; i++)
		{
		Com_sprintf (entry, sizeof(entry),
			"xm %i yv %i dmstr 752 \"%s\" ",
			-5*strlen(selectheader[i]), yofs + (int)(-60.0+-3.5*14), selectheader[i] );

		j = strlen(entry);
		strcpy (string + stringlength, entry);
		stringlength += j;

		yofs += 20;
		}

	yofs += 10;
	Com_sprintf (entry, sizeof(entry),
		"xm %i yv %i dmstr 772 \"%s\" ",
		-5*strlen(seperator), yofs + (int)(-60.0+-3.5*14), seperator );

	j = strlen(entry);
	strcpy (string + stringlength, entry);
	stringlength += j;

	yofs += 30;

	for (i=0; i< MOTD_lines; i++)
		{
		Com_sprintf (entry, sizeof(entry),
			"xm %i yv %i dmstr 842 \"%s\" ",
			-5*strlen(MOTD[i].textline), yofs + (int)(-60.0+-3.5*14), MOTD[i].textline );

		j = strlen(entry);
		if (stringlength + j < 1024)
			{
			strcpy (string + stringlength, entry);
			stringlength += j;
			}

		yofs += 20;
		}

	Com_sprintf (entry, sizeof(entry),
		"xm %i yv %i dmstr 772 \"%s\" ",
		-5*strlen(seperator), yofs + (int)(-60.0+-3.5*14), seperator );

	j = strlen(entry);

	if (stringlength + j < 1024)
		{
		strcpy (string + stringlength, entry);
		stringlength += j;
		}

	gi.WriteByte (svc_layout);
	gi.WriteString (string);

	}
