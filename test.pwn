#pragma dynamic 32768

#include <a_samp>
#include <a_mysql>
#include <PawnPlus>
#include <json>

#include "advanced-settings"

new TEST_TABLE[12] = "test";
new MySQL:connection; // just a dummy connection
new TestEntitySQLID = 12345;
new Map:SettingsMap;
new Map:DefaultValues;

#define SETTING_TEST "SETTING_TEST"
#define SETTING_TEST_TOGGLE "SETTING_TEST_TOGGLE"
#define SETTING_EXTRA_TEST_INT "SETTING_EXTRA_TEST_INT"
#define SETTING_EXTRA_TEST_FLOAT "SETTING_EXTRA_TEST_FLOAT"
#define SETTING_EXTRA_TEST_STRING "SETTING_EXTRA_TEST_STRING"
#define SETTING_EXTRA_TEST_LIST "SETTING_EXTRA_TEST_LIST"

main()
{
	// this is just a jank test i wrote to make sure all of the code worked as a standalone library since it was originally
	// just another component of a larger sa-mp gamemode. it's not really a proper unit test because i don't care enough

	AssignSettingsMySQLHandle(connection);

	DefaultValues = map_new();
	AddSettingDefault(SETTING_TEST, 0);
	AddSettingDefault(SETTING_TEST_TOGGLE, 1);

	SettingsMap = map_new();
	LoadSettings(SettingsMap, TEST_TABLE, TestEntitySQLID);

	SetSettingValue(SettingsMap, SETTING_TEST, 5, TEST_TABLE, TestEntitySQLID);
	AppendSetting(SettingsMap, SETTING_TEST, 5, TEST_TABLE, TestEntitySQLID);
	printf("%s base value: %i (should be 10)", SETTING_TEST, GetSettingValue(SettingsMap, SETTING_TEST), map_str_get(DefaultValues, SETTING_TEST));

	SetSettingExtra(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_INT, var_new(12), TEST_TABLE, TestEntitySQLID);
	AppendSettingExtra(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_INT, 12, TEST_TABLE, TestEntitySQLID);
	printf("%s value: %i (should be 24)", SETTING_EXTRA_TEST_INT, GetSettingExtra(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_INT));

	SetSettingExtra(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_FLOAT, var_new(5.0), TEST_TABLE, TestEntitySQLID);
	printf("%s value: %f (should be 5.0)", SETTING_EXTRA_TEST_FLOAT, GetSettingExtraFloat(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_FLOAT));

	new testingstring[24];
	SetSettingExtra(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_STRING, var_new_str("testing"), TEST_TABLE, TestEntitySQLID);
	GetSettingExtraStr(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_STRING, testingstring);
	printf("%s value: %s (should be \"testing\")", SETTING_EXTRA_TEST_STRING, testingstring);

	PushSettingList(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, var_new(1), TEST_TABLE, TestEntitySQLID);
	printf("%s index 0 int value: %i (should be 1)", SETTING_EXTRA_TEST_LIST, GetSettingListInt(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, 0));

	PushSettingList(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, var_new_str("nested_testing"), TEST_TABLE, TestEntitySQLID);
	GetSettingListStr(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, 1, testingstring);
	printf("%s index 1 string value: %s (should be \"nested_testing\")", SETTING_EXTRA_TEST_LIST, testingstring);

	PushSettingList(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, var_new(6.9), TEST_TABLE, TestEntitySQLID);
	printf("%s index 2 float value: %i (should be 1)", SETTING_EXTRA_TEST_LIST, GetSettingListFloat(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, 1));

	RemoveSettingExtra(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_FLOAT, TEST_TABLE, TestEntitySQLID);

	new stringindex = FindSettingListValue(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, var_new_str("nested_testing"));
	if(stringindex != -1)
	{
		RemoveSettingListIndex(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, stringindex, TEST_TABLE, TestEntitySQLID);
	}

	new items = GetSettingListLength(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST);
	printf("%s item count: %i (should be 2)", SETTING_EXTRA_TEST_LIST, items);

	PopSettingList(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, var_new(3), TEST_TABLE, TestEntitySQLID);

	new List:list;
	GetSettingList(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, list);
	list_add(list, 1234);

	ClearSettingList(SettingsMap, SETTING_TEST, SETTING_EXTRA_TEST_LIST, TEST_TABLE, TestEntitySQLID);

	ToggleSetting(SettingsMap, SETTING_TEST_TOGGLE, TEST_TABLE, TestEntitySQLID);
	print("done");

}

AddSettingDefault(const key[], value)
{
	if(map_valid(DefaultValues))
	{
		map_str_set(DefaultValues, key, value);
	}
}
