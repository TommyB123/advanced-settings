#if defined __asettings_included
	#endinput
#endif
#define __asettings_included

#include <a_samp>
#include <a_mysql>
#include <PawnPlus>
#include <json>

//constants
#define SETTING_SQLID "SETTING_SQLID"
#define SETTING_BASE_VALUE "SETTING_BASE_VALUE"

// gotta declare our own MySQL handle here since it's an external library and allow people to reassign if necessary
static MySQL:SettingsMySQLHandle = MYSQL_DEFAULT_HANDLE;

stock AssignSettingsMySQLHandle(MySQL:handle)
{
	SettingsMySQLHandle = handle;
}

stock LoadSettings(Map:map, const table[], entityid)
{
	new query[128];
	mysql_format(SettingsMySQLHandle, query, sizeof(query), "SELECT * FROM %s WHERE entityid = %i", table, entityid);
	mysql_tquery(SettingsMySQLHandle, query, "AS_OnSettingsLoaded", "isi", _:map, table, entityid);
}

forward AS_OnSettingsLoaded(Map:map, const table[], entityid);
public AS_OnSettingsLoaded(Map:map, const table[], entityid)
{
	new key[24], id, value, extra_stringified[2048], Node:node;
	for(new i = 0, r = cache_num_rows(); i < r; i++)
	{
        cache_get_value_name_int(i, "id", id);
		cache_get_value_name(i, "key", key);
		cache_get_value_name_int(i, "value", value);
		cache_get_value_name(i, "extra", extra_stringified);

		new Map:data = map_new();
        map_str_set(data, SETTING_SQLID, id);
		map_str_set(data, SETTING_BASE_VALUE, value);

		JSON_Parse(extra_stringified, node);
		ConvertJSONToMap(node, data);

		JSON_Cleanup(node);
		map_str_add(map, key, data);
	}
}

stock SetSettingValue(Map:map, const key[], value, const table[], entityid, bool:autosave = true, defaultvalue = -1)
{
	if(!map_valid(map)) return false;

	if(value == defaultvalue)
	{
		DeleteSetting(map, key, table, entityid);
		return true;
	}

	if(!map_has_str_key(map, key))
	{
		new Map:setting = map_new();
		map_str_set(setting, SETTING_BASE_VALUE, value);
		map_str_add(map, key, setting);
	}
	else
	{
		new Map:setting = Map:map_str_get(map, key);
		map_str_set(setting, SETTING_BASE_VALUE, value);
	}
	if(autosave) SaveSetting(map, key, table, entityid);
	return true;
}

stock SetSettingExtra(Map:map, const key[], const extrakey[], Variant:value, const table[], entityid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	switch(var_tag_uid(value))
	{
		case tag_uid_cell, tag_uid_float, tag_uid_bool:
		{
			map_str_set(setting, extrakey, var_get(value));
		}
		case tag_uid_char:
		{
			new buffer[256];
			var_get_str(value, buffer);
			map_str_set_str(setting, extrakey, buffer);
		}
	}

	if(autosave) SaveSetting(map, key, table, entityid);
	return true;
}

stock PushSettingList(Map:map, const key[], const extrakey[], Variant:value, const table[], entityid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key), List:list = INVALID_LIST;
	if(!map_has_str_key(setting, extrakey))
	{
		list = list_new();
		map_str_add(setting, extrakey, list);
	}
	else list = List:map_str_get(setting, extrakey);

	switch(var_tag_uid(value))
	{
		case tag_uid_cell, tag_uid_float:
		{
			list_add(list, var_get(value));
		}
		case tag_uid_char:
		{
			new buffer[256];
			var_get_str(value, buffer);
			list_add_str(list, buffer);
		}
	}

	if(autosave) SaveSetting(map, key, table, entityid);
	return true;
}

stock PopSettingList(Map:map, const key[], const extrakey[], Variant:value, const table[], entityid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:settings = Map:map_str_get(map, key);
	if(!map_has_str_key(settings, extrakey)) return false;

	new List:list = List:map_str_get(settings, extrakey);
	if(!list_valid(list)) return false;

	new index = -1;
	switch(var_tag_uid(value))
	{
		case tag_uid_cell, tag_uid_float:
		{
			index = list_find(list, var_get(value));
		}
		case tag_uid_char:
		{
			new buffer[256];
			var_get_str(value, buffer);
			index = list_find_str(list, buffer);
		}
	}

	if(index != -1)
	{
		list_remove(list, index);
		if(autosave) SaveSetting(map, key, table, entityid);
	}
	return true;
}

stock RemoveSettingListIndex(Map:map, const key[], const extrakey[], index, const table[], entityid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return false;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return false;
	if(index < 0 || index > list_size(list)) return false;

	list_remove(list, index);
	if(autosave) SaveSetting(map, key, table, entityid);
	return true;
}

stock ClearSettingList(Map:map, const key[], const extrakey[], const table[], entityid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return false;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return false;

	list_delete(list);
	map_str_remove(setting, extrakey);
	if(autosave) SaveSetting(map, key, table, entityid);
	return true;
}

stock FindSettingListValue(Map:map, const key[], const extrakey[], Variant:value)
{
	if(!map_valid(map)) return -1;
	if(!map_has_str_key(map, key)) return -1;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return -1;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return -1;

	switch(var_tag_uid(value))
	{
		case tag_uid_cell, tag_uid_float, tag_uid_bool:
		{
			return list_find(list, var_get(value));
		}
		case tag_uid_char:
		{
			new buffer[256];
			var_get_str(value, buffer);
			return list_find_str(list, buffer);
		}
	}
	return -1;
}

stock GetSettingListLength(Map:map, const key[], const extrakey[])
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return false;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return false;

	return list_size(list);
}

stock GetSettingList(Map:map, const key[], const extrakey[], &List:list)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return false;

	new List:templist = List:map_str_get(setting, extrakey);
	if(!list_valid(templist)) return false;

	list = templist;
	return true;
}

stock GetSettingListInt(Map:map, const key[], const extrakey[], index)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_valid(setting)) return false;
	if(!map_has_str_key(setting, extrakey)) return false;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return false;
	if(index >= list_size(list)) return -1;

	return list_get(list, index);
}

stock Float:GetSettingListFloat(Map:map, const key[], const extrakey[], index)
{
	if(!map_valid(map)) return 0.0;
	if(!map_has_str_key(map, key)) return 0.0;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_valid(setting)) return 0.0;
	if(!map_has_str_key(setting, extrakey)) return 0.0;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return 0.0;
	if(index >= list_size(list)) return 0.0;

	return Float:list_get(list, index);
}

stock GetSettingListStr(Map:map, const key[], const extrakey[], index, buffer[], len = sizeof(buffer))
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return false;

	new List:list = List:map_str_get(setting, extrakey);
	if(!list_valid(list)) return false;
	if(index >= list_size(list)) return -1;

	buffer[0] = EOS;
	list_get_str(list, index, buffer, len);
	return true;
}

stock RemoveSettingExtra(Map:map, const key[], const extrakey[], const table[], sqlid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return false;

	map_str_remove_deep(setting, extrakey);
	if(autosave) SaveSetting(map, key, table, sqlid);
	return true;
}

stock AppendSetting(Map:map, const key[], value, const table[], sqlid, bool:autosave = true, defaultvalue = -1)
{
	if(!map_valid(map)) return false;

	if(!map_has_str_key(map, key))
	{
		new Map:setting = map_new(), basevalue = defaultvalue;
		basevalue += value;
		map_str_set(setting, SETTING_BASE_VALUE, basevalue);
	}
	else
	{
		new Map:setting = Map:map_str_get(map, key), current = map_str_get(setting, SETTING_BASE_VALUE);
		current += value;
		map_str_set(setting, SETTING_BASE_VALUE, current);
	}

	if(autosave) SaveSetting(map, key, table, sqlid);
	return true;
}

stock AppendSettingExtra(Map:map, const key[], const extrakey[], value, const table[], sqlid, bool:autosave = true)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return false;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey))
	{
		map_str_set(setting, extrakey, value);
	}
	else
	{
		map_str_set(setting, extrakey, map_str_get(setting, extrakey) + value);
	}

	if(autosave) SaveSetting(map, key, table, sqlid);
	return true;
}

stock ToggleSetting(Map:map, const key[], const table[], sqlid, bool:autosave = true, defaultvalue = -1)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key))
	{
		SetSettingValue(map, key, !defaultvalue, table, sqlid, autosave, defaultvalue);
	}
	else
	{
		DeleteSetting(map, key, table, sqlid);
	}
	return true;
}

stock GetSettingValue(Map:map, const key[], defaultvalue = -1)
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return defaultvalue;

	new Map:setting = Map:map_str_get(map, key);
	return map_str_get(setting, SETTING_BASE_VALUE);
}

stock GetSettingExtra(Map:map, const key[], const extrakey[])
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return 0;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return 0;

	return map_str_get(setting, extrakey);
}

stock Float:GetSettingExtraFloat(Map:map, const key[], const extrakey[])
{
	if(!map_valid(map)) return 0.0;
	if(!map_has_str_key(map, key)) return 0.0;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return 0.0;

	return Float:map_str_get(setting, extrakey);
}

stock GetSettingExtraStr(Map:map, const key[], const extrakey[], buffer[], len = sizeof(buffer))
{
	if(!map_valid(map)) return false;
	if(!map_has_str_key(map, key)) return 0;

	new Map:setting = Map:map_str_get(map, key);
	if(!map_has_str_key(setting, extrakey)) return 0;

	return map_str_get_str(setting, extrakey, buffer, len);
}

stock SaveSetting(Map:map, const key[], const table[], sqlid)
{
	if(sqlid == 0) return true;
	if(!map_valid(map)) return true;
	if(!map_has_str_key(map, key)) return true;

	new Map:setting = Map:map_str_get(map, key), extra_stringified[2048];
	new value = map_str_get(setting, SETTING_BASE_VALUE);
	StringifyEntityExtraMap(setting, extra_stringified);

	new query[2048 + 256];
	if(!map_has_str_key(setting, SETTING_SQLID))
	{
		//insert new row
		mysql_format(SettingsMySQLHandle, query, sizeof(query), "INSERT INTO %s (sqlid, setting, settingval, extra) VALUES (%i, '%e', %i, '%e')", table, sqlid, key, value, extra_stringified);
		mysql_tquery(SettingsMySQLHandle, query, "AS_OnSettingInserted", "is", _:map, key);
	}
	else
	{
		//update existing row
		new id = map_str_get(setting, SETTING_SQLID);
		mysql_format(SettingsMySQLHandle, query, sizeof(query), "UPDATE %s SET settingval = %i, extra = '%e' WHERE id = %i", table, value, extra_stringified, id);
		mysql_tquery(SettingsMySQLHandle, query);
	}
	return true;
}

forward AS_OnSettingInserted(Map:map, const key[]);
public AS_OnSettingInserted(Map:map, const key[])
{
	if(map_valid(map) && map_has_str_key(map, key))
	{
		new Map:setting = Map:map_str_get(map, key);
		map_str_set(setting, SETTING_SQLID, cache_insert_id());
	}
}

stock DeleteSetting(Map:map, const key[], const table[], sqlid)
{
	new query[128];
	mysql_format(SettingsMySQLHandle, query, sizeof(query), "DELETE FROM %e WHERE sqlid = %i AND setting = '%e'", table, sqlid, key);
	mysql_tquery(SettingsMySQLHandle, query);
	if(map_valid(map) && map_has_str_key(map, key))
	{
		map_str_remove_deep(map, key);
	}
	return true;
}

ConvertJSONToMap(Node:node, Map:map)
{
	new index, keystr[32];
	while(!JSON_Keys(node, index, keystr))
	{
		switch(JSON_GetType(node, keystr))
		{
			case JSON_NODE_NUMBER:
			{
				new value;
				new ret = JSON_GetInt(node, keystr, value);
				if(ret) //GetInt failed, grab a float instead
				{
					new Float:floatvalue;
					JSON_GetFloat(node, keystr, floatvalue);
					map_str_set(map, keystr, floatvalue);
				}
				else map_str_set(map, keystr, value);
			}
			case JSON_NODE_BOOLEAN:
			{
				new bool:value;
				JSON_GetBool(node, keystr, value);
				map_str_set(map, keystr, value);
			}
			case JSON_NODE_STRING:
			{
				new value[256];
				JSON_GetString(node, keystr, value);
				map_str_set_str_s(map, keystr, str_new(value));
			}
			case JSON_NODE_ARRAY:
			{
				new List:items = list_new(), Node:array, arrayindex = -1, Node:item;
				JSON_GetArray(node, keystr, array);
				while(!JSON_ArrayIterate(array, arrayindex, item))
				{
					switch(JSON_NodeType(item))
					{
						case JSON_NODE_NUMBER:
						{
							new number;
							JSON_GetNodeInt(item, number);
							list_add(items, number);
						}
						case JSON_NODE_BOOLEAN:
						{
							new bool:boolean;
							JSON_GetNodeBool(item, boolean);
							list_add(items, boolean);
						}
						case JSON_NODE_STRING:
						{
							new value[256];
							JSON_GetNodeString(item, value);
							list_add_str(items, value);
						}
						case JSON_NODE_NULL:
						{
							++index;
							continue;
						}
						default:
						{
							++index;
							continue;
						}
					}
				}

				map_str_add(map, keystr, items);
				++index;
				continue;
			}
			case JSON_NODE_NULL:
			{
				++index;
				continue;
			}
			default:
			{
				++index;
				continue;
			}
		}
		++index;
	}
}

ConvertMapToJSON(Map:map, Node:node)
{
	JSON_ToggleGC(node, false);

	new keystr[32];
	for(new Iter:i = map_iter(map); iter_inside(i); iter_move_next(i))
	{
		iter_get_key_str(i, keystr);
		switch(iter_tag_uid(i))
		{
			case tag_uid_bool:
			{
				JSON_SetBool(node, keystr, bool:iter_get(i));
			}
			case tag_uid_char: //pawn string, probably
			{
				new buffer[256];
				iter_get_arr(i, buffer);
				JSON_SetString(node, keystr, buffer);
			}
			case tag_uid_string: //pawnplus string
			{
				new buffer[256];
				str_get(iter_get_str_s(i), buffer);
				JSON_SetString(node, keystr, buffer);
			}
			case tag_uid_cell:
			{
				JSON_SetInt(node, keystr, iter_get(i));
			}
			case tag_uid_float:
			{
				JSON_SetFloat(node, keystr, Float:iter_get(i));
			}
			case tag_uid_list:
			{
				new Node:array = JSON_Array();
				JSON_SetArray(node, keystr, array);

				new List:list = List:iter_get(i);
				for(new Iter:l = list_iter(list); iter_inside(l); iter_move_next(l))
				{
					switch(iter_tag_uid(l))
					{
						case tag_uid_bool:
						{
							JSON_ArrayAppend(node, keystr, JSON_Bool(bool:iter_get(l)));
						}
						case tag_uid_char:
						{
							new buffer[256];
							iter_get_arr(l, buffer);
							JSON_ArrayAppend(node, keystr, JSON_String(buffer));
						}
						case tag_uid_cell:
						{
							JSON_ArrayAppend(node, keystr, JSON_Int(iter_get(l)));
						}
					}
				}
			}
			default:
			{
				printf("Unsupported tag found when trying to convert PawnPlus map to JSON. Key: %s. Tag: %i", keystr, iter_tag_uid(i));
			}
		}
	}
}

StringifyEntityExtraMap(Map:map, string[], bool:delete = false, len = sizeof(string))
{
	string[0] = EOS;
	if(map_valid(map) && map_size(map))
	{
		new Node:node = JSON_Object();
		ConvertMapToJSON(map, node);
		JSON_Remove(node, SETTING_BASE_VALUE); //don't store base value
		JSON_Stringify(node, string, len);
		JSON_Cleanup(node);
		if(delete) map_delete_deep(map);
	}
	else format(string, len, "{}");
}
