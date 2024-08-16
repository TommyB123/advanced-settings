# advanced-settings

[![sampctl](https://img.shields.io/badge/sampctl-advanced--settings-2f2f2f.svg?style=for-the-badge)](https://github.com/TommyB123/advanced-settings)

This is a library I wrote for [Red County Roleplay](https://redcountyrp.com) to quickly add new account or character settings/attributes without any silly MySQL table tedium.

## Dependencies
This library requires MySQL ([table structure](https://github.com/TommyB123/advanced-settings/blob/main/structure.sql) provided) and also heavily uses PawnPlus and the JSON plugin to make managing your data as seamless as possible, so grab those dependencies if you don't already have them.

* [PawnPlus](https://github.com/IllidanS4/PawnPlus)
* [MySQL Plugin](https://github.com/pBlueG/SA-MP-MySQL)
* [pawn-json](https://github.com/Southclaws/pawn-json)

## Installation

Simply install to your project:

```bash
sampctl install TommyB123/advanced-settings
```

Include in your code and begin using the library:

```pawn
#include <advanced-settings>
```

## Introduction
The general design behind this library is relatively simple. Use a PawnPlus map to store arbitrary per-entity data (referred to as settings) by using strings as a key. Further increase flexibility and ease-of-use by also allowing arbitrary extra data to be appended onto a setting as well. When the setting is saved to MySQL, all extra data is converted to a JSON string to make things readable with a familiar format when browsing data on the database. It also makes it very easy to parse upon loading as well. With all of that combined, here's an example of a simple autoamtic account login feature.

Please note that the `SetAccountSetting*` functions are wrapper functions produced by either editing the template file or generating a template with the supplied python script. The actual internal API will be outlined below.

```pawn
// the main key for the auto login setting
#define SETTING_AUTO_LOGIN "SETTING_AUTO_LOGIN"

// two extra keys for storing the player's IP address and GPCI to validate the intergrity of an automatic login request
#define AUTO_LOGIN_EXTRA_IP "AUTO_LOGIN_EXTRA_IP"
#define AUTO_LOGIN_EXTRA_GPCI "AUTO_LOGIN_EXTRA_GPCI"

#include <pp-hooks>
hook OnGameModeInit()
{
	// each and every setting should have a "default" value assigned. this value denotes an unassigned/unused setting that
	// shouldn't be stored onto MySQL. so for instance, if somewhere down the line I set the SETTING_AUTO_LOGIN
	// setting to 0 while it's set to a value OTHER than 0, it will be automatically cleared from the database
	// and deleted from the player's internal setting map as well. any further attempt to fetch SETTING_AUTO_LOGIN
	// would simply return the assigned default value from below. the idea is to only store what's actually necessary to store.
	SetAccountSettingDefault(SETTING_AUTO_LOGIN, 0);
}

SetupAutomaticLogins(playerid)
{
	// set the base value of the SETTING_AUTO_LOGIN setting
	SetAccountSetting(playerid, SETTING_AUTO_LOGIN, 1);

	// save their currently-stored IP and GPCI so they can be referenced later.
    // these are essentiall stored as child data belonging to the main SETTING_AUTO_LOGIN key
	SetAccountSettingExtra(playerid, SETTING_AUTO_LOGIN, AUTO_LOGIN_EXTRA_IP, var_new_str(PlayerIP[playerid]));
	SetAccountSettingExtra(playerid, SETTING_AUTO_LOGIN, AUTO_LOGIN_EXTRA_GPCI, var_new_str(PlayerSerial[playerid]));
}

AutoLoginSecurityCheck(playerid)
{
	// fetch the previously-stored IP and GPCI strings
	new ip[20], serial[128];
	GetAccountSettingExtraStr(playerid, SETTING_AUTO_LOGIN, AUTO_LOGIN_EXTRA_IP, ip);
	GetAccountSettingExtraStr(playerid, SETTING_AUTO_LOGIN, AUTO_LOGIN_EXTRA_GPCI, serial);

	// compare those strings against the player's current data that was fetched upon connection
	// if either of them don't match, don't allow an automatic login
	if(strcmp(PlayerIP[playerid], ip)) return false;
	if(strcmp(PlayerSerial[playerid], serial)) return false;
	return true;
}
```

## Quick Start For Experienced Users
The code was written as a generalized API that can be reapplied to any server feature a scripter desires, such as per-vehicle data, per-account data, per-house data, etc. This requires streamlines wrapper functions to be written, so I've provided a quick and easy [python script](https://github.com/TommyB123/advanced-settings/blob/main/template-generator.py) to snappily generate a template and get started with as little friction as possible. Once you've generated your template file, include it into your server's codebase and get started with the functions below.

If you'd like to see some examples on how this library can be used in practice, take a look at some of the scripts provided in the [samples]() directory in this repo.

## Getting Started
As I touched on above, deploying this library efficiently requires some wrapper functions. The reason why is because the functions for manipulating a setting's data require a MySQL table, a PawnPlus map, an entity ID (such as playerid, vehicleid, etd) and their respective database ID to be passed as arguments. This can get rather tedious, so I threw together a [template file](https://github.com/TommyB123/advanced-settings/blob/main/template.pwn) and also a [template generator](https://github.com/TommyB123/advanced-settings/blob/main/template-generator.py) (requies python) for creating the appropriate wrapper functions needed to make using this library easier.

To give an example on why these wrapper functions are beneficial, let's take a look at the following code which directly calls a "low-level" function of the library.
```pawn
#define ACCOUNT_SETTING_AUTOLOGIN "ACCOUNT_SETTING_AUTOLOGIN"

CMD:autologin(playerid, params[])
{
    SetSettingValue(AccountSettings[playerid], ACCOUNT_SETTING_AUTOLOGIN, 1, "accountsettings", MasterAccount[playerid], true, GetSettingDefaultValue(ACCOUNT_SETTING_AUTOLOGIN));
}
```

There's a lot of arguments in this function call. By using wrapper functions for the API, we can streamline things a lot, turning the above example into this instead.

```pawn
#define ACCOUNT_SETTING_AUTOLOGIN "ACCOUNT_SETTING_AUTOLOGIN"

CMD:autologin(playerid, params[])
{
    SetAccountSettingValue(playerid, ACCOUNT_SETTING_AUTOLOGIN, 1);
}
```

Much, much better, isn't it? This is how the `SetAccountSettingValue` wrapper would look internally.

```pawn
stock SetAccountSetting(playerid, const key[], value, bool:autosave = true)
{
	return SetSettingValue(AccountSettings[playerid], key, value, "accountsettings", MasterAccount[playerid], autosave, GetSettingDefaultValue(key));
}
```

In order to actually procure these wrapper functions, we need to use the previously-mentioned template file. I provide a basic template file and a python script that can be used to generate your own template file. Here's how to use them.

### Manually editing the template file
* Download the [template file](https://github.com/TommyB123/advanced-settings/blob/main/template.pwn).
* Read through the comments in the template file and adjust the wrapper functions to your needs.
* Include the file in your gamemode once you're finished and start using advanced settings!

### Generate a template file with the python script
* Have the python programming language ready on your PC. If you don't have it, get it or consider manually editing the supplied template file as explained above.
* Download the [python script]() and run it via CLI. E.g, `python template-generator.py`
* Follow the instructions given by the script. You will be asked to name your template file at the end.
* Copy your template file into your desired location in your codebase.
* Include the template file.

Once you've generated your template file, you may need to manually add some hooks for cleaning up the per-entity data. Since the data is stored internally using various PawnPlus maps, which are dyamic memory containers, memory leaks are a potential risk. If you don't clean up an entity's settings map when they disconnect (player-linked settings) or are deleted (vehicle-linked settings), you will definitely experience a memory leak. Here's an example on how you can quickly clean up after your players.

```pawn
new Map:AccountSettings[MAX_PLAYERS] = {INVALID_MAP, ...};

#include <pp-hooks>
hook OnPlayerDisconnect(playerid, reason)
{
    if(map_valid(AccountSettings[playerid]))
    {
        // map_delete_deep is used here because multiple PawnPlus data containers can be associated with a single setting.
        // using a `*_deep` function here ensures that all nested containers are properly freed along with the parent container.
        map_delete_deep(AccountSettings[playerid]);
    }

    AccountSettings[playerid] = INVALID_MAP;
}
```

This should cover the basics of implementating the library. If anything else comes to mind or anyone has any questions, I'll be happy to update this documentation to include more examples or explanations.

## Functions
| Function | Description
| - | - |
| `AssignSettingsMySQLHandle(MySQL:handle)` | Reassign the MySQL handle used internally by the library. Generally speaking, if you only use one MySQL handle, you won't need to use this.
| `LoadSettings(Map:map, const table[], entityid)` | Load an entity's settings from MySQL. Usually done after an account login or a saved vehicle being spawned, etc.
| `SetSettingValue(Map:map, const key[], value, const table[], entityid, bool:autosave = true, defaultvalue = -1)` | Assign the base/main value of a setting. `autosave` paramater decides whether or not the change will automatically be saved to MySQL or saved manually later on in your code. All functions with the `autosave` parameter behave this way. The `defaultvalue` argument is the value the fucntion will compare your provided value against. If it matches the default value, nothing will be stored to ensure that only non-default data is stored in the database.
| `AppendSetting(Map:map, const key[], value, const table[], sqlid, bool:autosave = true, defaultvalue = -1)` | Appends a number to the base/main value of a setting.
| `ToggleSetting(Map:map, const key[], const table[], sqlid, bool:autosave = true, defaultvalue = -1)` | Toggle simple settings stored as 0/1 on/off. Useful for things like chat toggles.
| `SetSettingExtra(Map:map, const key[], const extrakey[], Variant:value, const table[], entityid, bool:autosave = true)` | Assign an extra value to a setting. These extra values can be whole integers, floats, strings or even entire PawnPlus lists. Do note that since this function was written to accept various data types, PawnPlus Variants are used for the value argument. This means that when you pass your extra value, you'll need to write `var_new(myIntegerVariable)` or `var_new_str(myStringArray)` instead of `myIntegerVariable` or `myStringArray` directly. This logic applies to all functions that take PawnPlus variants as an argument.
| `AppendSettingExtra(Map:map, const key[], const extrakey[], value, const table[], sqlid, bool:autosave = true)` | Appends a number to a setting extra. Only integers are supported currently.
| `PushSettingList(Map:map, const key[], const extrakey[], Variant:value, const table[], entityid, bool:autosave = true)` | Push a new value into a setting extra represented as a list. If there's no list already found, a new one will be created and the supplied value will be appended to it. Only simple, single data types are supported at this time. If you try to store a complex array inside of a setting extra list, it will not save to the database properly.
| `PopSettingList(Map:map, const key[], const extrakey[], Variant:value, const table[], entityid, bool:autosave = true)` | Remove an existing value from a setting extra list by searching for the value directly. (I know this isn't quite how the term "pop" is used normally, but when factoring for wrapper function names, the pawn compiler's function length limit got hit real fast)
| `RemoveSettingListIndex(Map:map, const key[], const extrakey[], index, const table[], entityid, bool:autosave = true)` | Remove an existing value from a setting extra list via its index.
| `ClearSettingList(Map:map, const key[], const extrakey[], const table[], entityid, bool:autosave = true)` | Clear all entities from a setting extra list.
| `FindSettingListValue(Map:map, const key[], const extrakey[], Variant:value)` | Return a value's index inside of a setting extra list. Returns `-1` if nothing is found.
| `GetSettingListLength(Map:map, const key[], const extrakey[])` | Returns the number of entities inside of a setting extra list.
| `GetSettingList(Map:map, const key[], const extrakey[], &List:list)` | Fetch a setting extra list directly. Returns false if the `extrakey` supplied is not actually linked to a PawnPlus list.
| `GetSettingListInt(Map:map, const key[], const extrakey[], index)` | Fetch the value of a setting extra list at the supplied index as an integer.
| `Float:GetSettingListFloat(Map:map, const key[], const extrakey[], index)` | Fetch the value of a setting extra list at the supplied index as a float.
| `GetSettingListStr(Map:map, const key[], const extrakey[], index, buffer[], len = sizeof(buffer))` | Fetch the value of a setting extra list at the supplied index as a string.
| `RemoveSettingExtra(Map:map, const key[], const extrakey[], const table[], sqlid, bool:autosave = true)` | Remove an extra value from the setting.
| `GetSettingValue(Map:map, const key[], defaultvalue = -1)` | Get the default value of a setting. Default values were explained above, so I won't be repeating myself here.
| `GetSettingExtra(Map:map, const key[], const extrakey[])` | Fetch a setting extra's value as an integer.
| `Float:GetSettingExtraFloat(Map:map, const key[], const extrakey[])` | Fetch a setting extra's value as a float.
| `GetSettingExtraStr(Map:map, const key[], const extrakey[], buffer[], len = sizeof(buffer))` | Fetch a setting extra's value as a string.
| `SaveSetting(Map:map, const key[], const table[], sqlid)` | Manually save a setting's current data to MySQL. This will save the current base value as well as all extra data too. This function is called when `autosave` is set to `true` in any of the above functions.
| `DeleteSetting(Map:map, const key[], const table[], sqlid)` | Delete a setting. This fully cleans up the setting's base value and extra data, then deletes it from MySQL.

## Credits
me
