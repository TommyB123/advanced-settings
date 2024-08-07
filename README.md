# advanced-settings

[![sampctl](https://img.shields.io/badge/sampctl-advanced--settings-2f2f2f.svg?style=for-the-badge)](https://github.com/TommyB123/advanced-settings)

This is a library I wrote for Red County Roleplay to quickly add new account or character settings/attributes without any silly MySQL table tedium.

I'll write up some proper documentation later, but if you know what you're doing, give the template file a read and slap that into your gamemode.

Here's an example of how it can be used in practice.

```pawn
#define VEHICLE_SETTING_MODS "VEHICLE_SETTING_MODS"
#define VEHICLE_SETTING_EXTRA_MODS "VEHICLE_SETTING_EXTRA_MODS


CMD:modmysultan(playerid, params[])
{
    new vehicleid = GetPlayerVehicleID(playerid);

    AddVehicleComponent(vehicleid, 1010); // Nitro
    AddVehicleComponent(vehicleid, 1026); // Right Alien Sideskirt
    AddVehicleComponent(vehicleid, 1027); // Left Alien Sideskirt


    // since the vehicle mods are going to be appended as extra data, we simply assign 1 as the base value of the setting
    // to indicate that it's in use and will be successfully stored to the database
    SetVehicleSetting(vehicleid, VEHICLE_SETTING_MODS, 1);

    // any setting can have arbitrary extra data appended to it, including full PawnPlus lists.
    // since we're storing vehicle mods, we'll be using a list to save all of them.
    // you might notice the usage of `var_new` for the value. we use variants to pass values so
    // a single function can be used to pass ints, strings or floats
    PushVehicleSettingList(vehicleid, VEHICLE_SETTING_MODS, VEHICLE_SETTING_EXTRA_MODS, var_new(1010)); // Nitro
    PushVehicleSettingList(vehicleid, VEHICLE_SETTING_MODS, VEHICLE_SETTING_EXTRA_MODS, var_new(1026)); // Right Alien Sideskirt
    PushVehicleSettingList(vehicleid, VEHICLE_SETTING_MODS, VEHICLE_SETTING_EXTRA_MODS, var_new(1027)); // Left Alien Sideskirt
}
```

##### Dependencies
* [PawnPlus](https://github.com/IllidanS4/PawnPlus)
* [MySQL Plugin](https://github.com/pBlueG/SA-MP-MySQL)
* [pawn-json](https://github.com/Southclaws/pawn-json)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

## Installation

Simply install to your project:

```bash
sampctl package install TommyB123/advanced-settings
```

Include in your code and begin using the library:

```pawn
#include <advancedsettings>
```

## Usage

documentation coming eventually
