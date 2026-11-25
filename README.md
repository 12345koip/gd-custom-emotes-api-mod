### Custom Emotes API
Custom Emotes API is an API mod providing a central support framework for adding custom emotes to RobTop's in-game system.

### Using this mod
First, specify it as a dependency in your `mod.json`:
```json
{
  "dependencies": {
    "kx_.custom-emotes-api": ">=1.0.0"
  }
}
```

Then, include the provided header:
```cpp
#include <kx_.custom-emotes-api/CustomEmotes.hpp>
```

This will provide you with three classes you can use to create and customise your own emotes:

- `CustomEmoteContainer`, which is used to group custom emotes of the same type together. One mod can create many custom emote containers, and each container will show as a separate section under an `EmotePickerPopup`.
- `CustomEmoteDef`, which represents a custom emote.
- `CustomEmoteOptions`, which are used to specify extra options when creating a `CustomEmoteDef`, such as scale or positional offset.

To create custom emotes, first create a `CustomEmoteContainer`, and then add your own emotes to it.

### Example code
Here is an example file used to create a custom emote using `image1.png`.
```cpp
#include <Geode/Geode.hpp>
#include <kx_.custom-emotes-api/CustomEmotes.hpp>

using namespace geode::prelude;
using namespace CustomEmotes;

$on_game(Loaded) {
    auto containerResult = CustomEmotesContainer::create("my emotes");
    if (!containerResult.ok()) return; //the container couldn't be created
    
    CustomEmotesContainer* container = containerResult.unwrap();
    
    //creating the emote with some extra options
    CustomEmoteOptions options {};
    options.color = ccColor3B {150, 150, 150};
    options.scale = 1.0f;
    
    auto emoteResult = container->NewEmoteFromImage(
        "image1.png"_spr, //file to use
        "my_emote", //name of emote (typing :name: will resolve this emote)
        options //extra options we specified earlier
    );

    if (emoteResult.ok()) { //created successfully
        log::info("custom emote created successfully");
    } else { //something went wrong.
        log::warn("custom emote could not be created. Reason: {}", emoteResult.unwrapErr();
    }
}
```

### Notes

- If you are creating an emote from a sprite in a spritesheet, use `CustomEmotesContainer::NewEmoteFromSpritesheet`.
- Emote aliases can be created using `CustomEmoteDef::AddAlias` or `CustomEmotesContainer::AddAliasForEmote`.
- Each `CustomEmotesContainer` has a size limit of 150 emotes.
- `CustomEmotesContainer`s support both relative and absolute emote indices, relative being an emote index based from the container's base offset, and absolute being an emote index based from 0. Emote indices are managed internally to prevent collisions.
- When reading properties from a `CustomEmoteDef` that direct getter methods aren't available for, use `GetEmoteDef` and read them from there.
- Custom emotes will **not** be placed in the recents section of the picker popup. This is because the emote's owner can't be guaranteed between sessions, so there's no way to ensure it is resolved to the correct emote.
- View the header file for further documentation.

### Bug Reports and Feature Requests
If you find an issue with this mod, or would like to request for a feature to be added, please open up an Issue on the GitHub repository!