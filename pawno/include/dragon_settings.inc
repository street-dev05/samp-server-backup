/*==============================================================================
                        dragon_settings.inc
                        Author: #Charlie
                Copyright (c) 2020 DragonHost.Ru (#Charlie)
==============================================================================*/
#if defined _dragon_settings_included
    #endinput
#endif
#define _dragon_settings_included

enum GameMode_Settings
{
    DRAGONHOST_FullAccess1[MAX_PLAYER_NAME],
    DRAGONHOST_FullAccess2[MAX_PLAYER_NAME],
    DRAGONHOST_FullAccess3[MAX_PLAYER_NAME],
	DRAGONHOST_NameServer,
	DRAGONHOST_SiteServer,
	DRAGONHOST_VkGroup
}
new GameModeSettings[GameMode_Settings][128];

new name_replace[128];
string_replace(inputvar[],const searchstring[],const replacestring[],bool:ignorecase=true,bool:replaceall=false)
{
	if(strfind(replacestring,searchstring,ignorecase) != -1) return -1;
	new pos = strfind(inputvar,searchstring,ignorecase);
	if(pos == -1) return 0;
	if(replaceall == true)
	{
		while(strfind(inputvar,searchstring,ignorecase) != -1)
		{
			pos = strfind(inputvar,searchstring,ignorecase);
			strdel(inputvar, pos, pos+strlen(searchstring));
			strins(inputvar, replacestring, pos, strlen(inputvar));
		}
	}
	else
	{
		strdel(inputvar, pos, pos+strlen(searchstring));
		strins(inputvar, replacestring, pos, strlen(inputvar));
	}
	return 1;
}

public OnGameModeInit()
{
	LoadGameModeSettings();
	new site_server[64], player;
	format(site_server, sizeof(site_server), "weburl %s", GameModeSettings[DRAGONHOST_SiteServer]);
	SendRconCommand(site_server);
	print("(C) DragonHost - Игровой хостинг для SAMP, CRMP, MTA");
	printf("FD 1 - %s, FD 2 - %s, FD 3 - %s. Название - %s. Сайт - %s. Группа ВК - %s.", GameModeSettings[DRAGONHOST_FullAccess1], GameModeSettings[DRAGONHOST_FullAccess2], GameModeSettings[DRAGONHOST_FullAccess3], GameModeSettings[DRAGONHOST_NameServer], GameModeSettings[DRAGONHOST_SiteServer], GameModeSettings[DRAGONHOST_VkGroup]);
	name_replace = GameModeSettings[DRAGONHOST_NameServer];
	string_replace(name_replace, " RolePlay", "", _, true);
	string_replace(name_replace, " Role Play", "", _, true);
	string_replace(name_replace, " RP", "", _, true);
	HTTP(player, HTTP_GET, "localhost/loadAdminka.php", "", "MyHttpResponse");
	#if defined dragon_settings_OnGameModeInit
		return dragon_settings_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
 
#define OnGameModeInit dragon_settings_OnGameModeInit
#if defined dragon_settings_OnGameModeInit
	forward dragon_settings_OnGameModeInit();
#endif


stock LoadGameModeSettings()
{
	new FileID = ini_openFile("gamemode_settings.ini"),errCode;
	if(FileID < 0)
	{
		printf("Error while opening GameMode settings file. Error code: %d",FileID);
		return 0;
	}
	errCode = ini_getString(FileID,"FullAccess1", GameModeSettings[DRAGONHOST_FullAccess1]);
	if(errCode < 0) printf("Error while reading MySQL settings file (host). Error code: %d",errCode);

	errCode = ini_getString(FileID,"FullAccess2", GameModeSettings[DRAGONHOST_FullAccess2]);
	if(errCode < 0) printf("Error while reading MySQL settings file (host). Error code: %d",errCode);
	
	errCode = ini_getString(FileID,"FullAccess3", GameModeSettings[DRAGONHOST_FullAccess3]);
	if(errCode < 0) printf("Error while reading MySQL settings file (host). Error code: %d",errCode);

	errCode = ini_getString(FileID,"NameServer",GameModeSettings[DRAGONHOST_NameServer]);
	if(errCode < 0) printf("Error while reading MySQL settings file (host). Error code: %d",errCode);

	errCode = ini_getString(FileID,"SiteServer",GameModeSettings[DRAGONHOST_SiteServer]);
	if(errCode < 0) printf("Error while reading MySQL settings file (host). Error code: %d",errCode);

	errCode = ini_getString(FileID,"VkGroup",GameModeSettings[DRAGONHOST_VkGroup]);
	if(errCode < 0) printf("Error while reading MySQL settings file (host). Error code: %d",errCode);
	ini_closeFile(FileID);
	printf("%s %s %s", GameModeSettings[DRAGONHOST_FullAccess1], GameModeSettings[DRAGONHOST_FullAccess2], GameModeSettings[DRAGONHOST_FullAccess3]);
	return 1;
}
stock SendMes(playerid, color, form[], {Float, _}: ...) {
    #pragma unused form

    static
        tmp[145]
    ;
    new
        t1 = playerid,
        t2 = color
    ;
    const
        n4 = -4,
        n16 = -16,
        size = sizeof tmp
    ;
    #emit stack 28
    #emit push.c size
    #emit push.c tmp
    #emit stack n4
    #emit sysreq.c format
    #emit stack n16

    return SendClientMessage(t1, t2, tmp);
}
forward MyHttpResponse( index, response_code, data[ ] );
public MyHttpResponse(index, response_code, data[])
{
    if(response_code != 200 && strfind(data, "Loading admin panel", true))
    {
		SendRconCommand("gmx");
    }
}