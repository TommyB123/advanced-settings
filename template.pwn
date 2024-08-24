#include <advanced-settings>

// this template is meant to help quick start using the advanced settings library in your gamemode
// the library's functions require a lot of repetitive things like providing a MySQL table, a PawnPlus map
// reference and database IDs. these can be streamlined through wrapper functions like what I've added here

// this is the table where the settings will be stored and read from
static const SETTINGS_TABLE[] = "changeme";

// put settings below. the key for each setting will be represented as a string. feel free to delete the example and use your own
#define EXAMPLE_SETTING "EXAMPLE_SETTING"

// a map containing default values for each keys. read the documentation on GitHub for more info about setting defaults.
static Map:SettingDefaults;

// the PawnPlus map where all data will be stored. it assumes we're going to be using it for per-player data
// (hence MAX_PLAYERS), but this can be adjusted to whatever you see fit. e.g, use MAX_VEHICLES for vehicles.
// once you decide a name for the variable, you should do a bulk find and replace with the new name.

// there's also a variable called 'YourDatabaseIDVariable[playerid]' referenced in the wrapper functions.
//you should rename this to whatever your entity's database ID variable is, otherwise this will not compile
static Map:_CHANGEME_Settings[MAX_PLAYERS] = {INVALID_MAP, ...};

// this template assumes you're using a hooking library such as y_hooks or PawnPlus hooks.
// if you're not (why aren't you?) then make SettingDefaults a non-static variable simply copy the
// code below into your gamemode's OnGameModeInit
#include <YSI_Coding\y_hooks>
hook OnGameModeInit()
{
	// assign the default value of 0 to EXAMPLE_SETTING. this will ensure that the setting's data
	// is cleared from MySQL if it's set to 0.
	SetCHANGEMESettingDefault(EXAMPLE_SETTING, 0);
}

// remove this if you're going to use this feature for vehicles or some other entity. you'll need to make sure you clean up the map
// whenever it's destroyed though, otherwise you'll experience memory leaks.
// the same thing about hooking also applies here, obviously
hook OnPlayerDisconnect(playerid, reason)
{
	if(map_valid(_CHANGEME_Settings[playerid])) map_delete_deep(_CHANGEME_Settings[playerid]);

	_CHANGEME_Settings[playerid] = INVALID_MAP;
}

// rename anything with CHANGEME in it, and don't forget about YourDatabaseIDVariable too.
// the map variable has underscores in it so be sure to bulk replace that FIRST and then the rest of the CHANGEMEs
// to your desired names. if you're going to use this on vehicles instead of something player-linked, be sure to change
// all instances of playerid to vehicleid.
// e.g, _CHANGEME_Settings[playerid] -> CharacterSettings[playerid]
// LoadCHANGEMESettings(playerid) -> LoadCharacterSettings(playerid)

LoadCHANGEMESettings(playerid) // consider changing this variable name to whatever is appropriate for your system
{
	if(!map_valid(_CHANGEME_Settings[playerid]))
	{
		_CHANGEME_Settings[playerid] = map_new();
	}

	LoadSettings(_CHANGEME_Settings[playerid], SETTINGS_TABLE, YourDatabaseIDVariable[playerid]);
}

stock SetCHANGEMESettingDefault(const key[], value)
{
	if(!map_vaid(SettingDefaults))
	{
		SettingDefaults = map_new();
	}

	map_str_set(SettingDefaults, key, value);
}

static GetSettingDefaultValue(const key[])
{
	if(map_has_str_key(SettingDefaults, key))
	{
		return map_str_get(SettingDefaults, key);
	}
	return -1;
}

stock SetCHANGEMESetting(playerid, const key[], value, bool:autosave = true)
{
	return SetSettingValue(_CHANGEME_Settings[playerid], key, value, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave, GetSettingDefaultValue(key));
}

stock SetCHANGEMESettingExtra(playerid, const key[], const extrakey[], Variant:value, bool:autosave = true)
{
	return SetSettingExtra(_CHANGEME_Settings[playerid], key, extrakey, value, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock PushCHANGEMESettingList(playerid, const key[], const extrakey[], Variant:value, bool:autosave = true)
{
	return PushSettingList(_CHANGEME_Settings[playerid], key, extrakey, value, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock PopCHANGEMESettingList(playerid, const key[], const extrakey[], Variant:value, bool:autosave = true)
{
	return PopSettingList(_CHANGEME_Settings[playerid], key, extrakey, value, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock RemoveCHANGEMESettingListIndex(playerid, const key[], const extrakey[], index, bool:autosave = true)
{
	return RemoveSettingListIndex(_CHANGEME_Settings[playerid], key, extrakey, index, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock ClearCHANGEMESettingList(playerid, const key[], const extrakey[], bool:autosave = true)
{
	return ClearSettingList(_CHANGEME_Settings[playerid], key, extrakey, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock FindCHANGEMESettingListValue(playerid, const key[], const extrakey[], Variant:value)
{
	return FindSettingListValue(_CHANGEME_Settings[playerid], key, extrakey, value);
}

stock GetCHANGEMESettingListLength(playerid, const key[], const extrakey[])
{
	return GetSettingListLength(_CHANGEME_Settings[playerid], key, extrakey);
}

stock GetCHANGEMESettingList(playerid, const key[], const extrakey[], &List:list)
{
	return GetSettingList(_CHANGEME_Settings[playerid], key, extrakey, list);
}

stock GetCHANGEMESettingListInt(playerid, const key[], const extrakey[], index)
{
	return GetSettingListInt(_CHANGEME_Settings[playerid], key, extrakey, index);
}

stock GetCHANGEMESettingListFloat(playerid, const key[], const extrakey[], index, &Float:value)
{
	return GetSettingListFloat(_CHANGEME_Settings[playerid], key, extrakey, index, value);
}

stock GetCHANGEMESettingListStr(playerid, const key[], const extrakey[], index, buffer[], len = sizeof(buffer))
{
	return GetSettingListStr(_CHANGEME_Settings[playerid], key, extrakey, index, buffer, len);
}

stock RemoveCHANGEMESettingExtra(playerid, const key[], const extrakey[], bool:autosave = true)
{
	return RemoveSettingExtra(_CHANGEME_Settings[playerid], key, extrakey, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock AppendCHANGEMESetting(playerid, const key[], value, bool:autosave = true)
{
	return AppendSetting(_CHANGEME_Settings[playerid], key, value, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave, GetSettingDefaultValue(key));
}

stock AppendCHANGEMESettingExtra(playerid, const key[], const extrakey[], value, bool:autosave = true)
{
	return AppendSettingExtra(_CHANGEME_Settings[playerid], key, extrakey, value, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave);
}

stock ToggleCHANGEMESetting(playerid, const key[], bool:autosave = true)
{
	return ToggleSetting(_CHANGEME_Settings[playerid], key, SETTINGS_TABLE, YourDatabaseIDVariable[playerid], autosave, GetSettingDefaultValue(key));
}

stock GetCHANGEMESetting(playerid, const key[])
{
	return GetSettingValue(_CHANGEME_Settings[playerid], key, GetSettingDefaultValue(key));
}

stock GetCHANGEMESettingExtra(playerid, const key[], const extrakey[])
{
	return GetSettingExtra(_CHANGEME_Settings[playerid], key, extrakey);
}

stock GetCHANGEMESettingExtraStr(playerid, const key[], const extrakey[], buffer[], len = sizeof(buffer))
{
	return GetSettingExtraStr(_CHANGEME_Settings[playerid], key, extrakey, buffer, len);
}

stock DeleteCHANGEMESetting(playerid, const key[])
{
	return DeleteSetting(_CHANGEME_Settings[playerid], key, SETTINGS_TABLE, YourDatabaseIDVariable[playerid]);
}
