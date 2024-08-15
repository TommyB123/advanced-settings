content = '''#include <advanced-settings>

// this is the table where the settings will be stored and read from
static const SETTINGS_TABLE[] = "$tablename";

// put settings below. the key for each setting will be represented as a string. feel free to delete the example and use your own
#define EXAMPLE_SETTING "EXAMPLE_SETTING"

// a map containing default values for each keys. read the documentation on GitHub for more info about setting defaults.
static Map:SettingDefaults;

// the map where the entity's settings will be stored
static Map:$settingmap[$settingconstant] = {INVALID_MAP, ...};

// call this function when your entity is loaded, such as when a player connects or a saved vehicle is loaded.
Load$settingnameSettings($entityvariable)
{
    if(!map_valid($settingmap[$entityvariable])) $settingmap[$entityvariable] = map_new();

    LoadSettings($settingmap[$entityvariable], SETTINGS_TABLE, $entitydatabasevariable[$entityvariable]);
}

stock Set$settingnameSettingDefault(const key[], value)
{
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

stock Set$settingnameSetting($entityvariable, const key[], value, bool:autosave = true)
{
    return SetSettingValue($settingmap[$entityvariable], key, value, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave, GetSettingDefaultValue(key));
}

stock Set$settingnameSettingExtra($entityvariable, const key[], const extrakey[], Variant:value, bool:autosave = true)
{
    return SetSettingExtra($settingmap[$entityvariable], key, extrakey, value, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Push$settingnameSettingList($entityvariable, const key[], const extrakey[], Variant:value, bool:autosave = true)
{
    return PushSettingList($settingmap[$entityvariable], key, extrakey, value, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Pop$settingnameSettingList($entityvariable, const key[], const extrakey[], Variant:value, bool:autosave = true)
{
    return PopSettingList($settingmap[$entityvariable], key, extrakey, value, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Remove$settingnameSettingListIndex($entityvariable, const key[], const extrakey[], index, bool:autosave = true)
{
    return RemoveSettingListIndex($settingmap[$entityvariable], key, extrakey, index, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Clear$settingnameSettingList($entityvariable, const key[], const extrakey[], bool:autosave = true)
{
    return ClearSettingList($settingmap[$entityvariable], key, extrakey, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Find$settingnameSettingListValue($entityvariable, const key[], const extrakey[], Variant:value)
{
    return FindSettingListValue($settingmap[$entityvariable], key, extrakey, value);
}

stock Get$settingnameSettingListLength($entityvariable, const key[], const extrakey[])
{
    return GetSettingListLength($settingmap[$entityvariable], key, extrakey);
}

stock Get$settingnameSettingList($entityvariable, const key[], const extrakey[], &List:list)
{
    return GetSettingList($settingmap[$entityvariable], key, extrakey, list);
}

stock Get$settingnameSettingListInt($entityvariable, const key[], const extrakey[], index)
{
    return GetSettingListInt($settingmap[$entityvariable], key, extrakey, index);
}

stock Get$settingnameSettingListFloat($entityvariable, const key[], const extrakey[], index, &Float:value)
{
    return GetSettingListFloat($settingmap[$entityvariable], key, extrakey, index, value);
}

stock Get$settingnameSettingListStr($entityvariable, const key[], const extrakey[], index, buffer[], len = sizeof(buffer))
{
    return GetSettingListStr($settingmap[$entityvariable], key, extrakey, index, buffer, len);
}

stock Remove$settingnameSettingExtra($entityvariable, const key[], const extrakey[], bool:autosave = true)
{
    return RemoveSettingExtra($settingmap[$entityvariable], key, extrakey, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Append$settingnameSetting($entityvariable, const key[], value, bool:autosave = true)
{
    return AppendSetting($settingmap[$entityvariable], key, value, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave, GetSettingDefaultValue(key));
}

stock Append$settingnameSettingExtra($entityvariable, const key[], const extrakey[], value, bool:autosave = true)
{
    return AppendSettingExtra($settingmap[$entityvariable], key, extrakey, value, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave);
}

stock Toggle$settingnameSetting($entityvariable, const key[], bool:autosave = true)
{
    return ToggleSetting($settingmap[$entityvariable], key, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable], autosave, GetSettingDefaultValue(key));
}

stock Get$settingnameSetting($entityvariable, const key[])
{
    return GetSettingValue($settingmap[$entityvariable], key, GetSettingDefaultValue(key));
}

stock Get$settingnameSettingExtra($entityvariable, const key[], const extrakey[])
{
    return GetSettingExtra($settingmap[$entityvariable], key, extrakey);
}

stock Get$settingnameSettingExtraStr($entityvariable, const key[], const extrakey[], buffer[], len = sizeof(buffer))
{
    return GetSettingExtraStr($settingmap[$entityvariable], key, extrakey, buffer, len);
}

stock Delete$settingnameSetting($entityvariable, const key[])
{
    return DeleteSetting($settingmap[$entityvariable], key, SETTINGS_TABLE, $entitydatabasevariable[$entityvariable]);
}

'''

setting_name = input("Enter your desired name for the setting entity. E.g, Character, Account, Vehicle. This value will be used for the function names. If you entered Vehicle, all functions would be set to \"SetVehicleSetting\", etc:\n")
content = content.replace("$settingname", setting_name)

table_name = input("\nEnter the name of your desired MySQL table for the setting data. E.g, vehiclesettings:\n")
content = content.replace("$tablename", table_name)

setting_constant = input("\nEnter the desired entity constant for your setting. E.g, MAX_PLAYERS, MAX_VEHICLES, MAX_HOUSES:\n")
content = content.replace("$settingconstant", setting_constant)

setting_map = input("\nEnter the desired variable that will be used for the entity's map container. For example, if you desire to store vehicle settings, write VehicleSettings and you'll get new Map:VehicleSettings[MAX_VEHICLES], assuming you entered MAX_VEHICLES as a constant previously:\n")
content = content.replace('$settingmap', setting_map)

entity_variable = input("\nEnter the entity variable that will be passed to the setting functions. E.g, playerid, vehicleid, houseid, etc etc:\n")
content = content.replace("$entityvariable", entity_variable)

entity_database_variable = input("\nEnter the database variable for your entity. E.g, PlayerAccountID will give you PlayerAccountID[playerid] and this will be passed to the functions:\n")
content = content.replace('$entitydatabasevariable', entity_database_variable)

file_name = input("\nEnter the name for your template file. For example, writing vehicle-settings will write to vehicle-settings.pwn:\n")

with open(f'{file_name}.pwn', "x") as file:
    file.write(content)
    print(f'The template with your modifications has been written to {file_name}.pwn')
