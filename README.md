<h1 align="center">
  FURY-XRPL 
</h1>
FURY XRPL - P2E ONLINE BATTLE ROYALE BUILT ON UNREAL ENGINE 5 &amp; XRPL
</p>
<p align="center">
 <img src="https://raw.githubusercontent.com/VO-GAMES/FURY-XRPL/main/Splash.bmp">
  <br />
  </p>

# Project Information
A first- and third-person shooter game called P2E ONLINE BATTLE ROYALE is being developed. The project is built using the Unreal Engine 5 game engine, utilizing C++ language and visual programming with Blueprints. The project already incorporates many new UE5 technologies, including Nanite and Lumen. Additionally, DLSS and Online Subsystem Steam plugins have been added.

# Project Architecture and Code Description::
## Core Project, Main Code, and Mechanics are located in the FPS folder.   
### Key Classes:
 - BP_PlayerCharacter. Location: /FPS/Blueprints/CharacterAndClasses. 

This is the base character class from which all other character classes inherit. It handles the player's interaction with the world, their state, and actions. Several components are added to this class: character information, inventory system, interactive interaction system, health information, and the character's main movement. The logic is conveniently divided into several graphs: EventGraph - contains logic that is called upon the player's initial appearance; Health&Death - calculates logic related to the character's health and death; Actions - handles player action-related events; FireWeapon - logic associated with weapons and shooting; ClassAbilities - character's skills and abilities.      
</p>
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/Player.png">
  <br />
  </p>
 
 - BP_BasePlayerController. Расположение: /FPS/Blueprints/PlayerController

Базовый класс контроллера. Используется для установки inputs, настроек, сетевого взаимодействия. Добавлены компоненты: настройки контроллера; система чата; показ счета игрока; UI менеджер; отслеживание смены оружия/класса/способностей. 
</p>
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/PC.png">
  <br />
  </p>
  
 - BP_Base_GM. Location: /FPS/Blueprints/GameModes

This is the base controller class used to set inputs, configurations, and handle network interactions. Components added include controller settings, chat system, player score display, UI manager, and tracking of weapon/class/ability changes.   
</p>
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/GM.png">
  <br />
  </p>
  
 - BP_Base_GS. Location: /FPS/Blueprints/GameModes

This class manages information about the current game state, bot spawning, and storing various information. Components added include game status tracking system, bot manager, game settings, and player spawning system. Thanks to these added components and the implemented logic, it's easy to create new game modes and quickly configure them.    
</p>
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/GS.png">
  <br />
  </p>

  - BP_PlayerState. Location: /FPS/Blueprints/PlayerState

This class stores information about the player, such as their name, points, and status.
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/PS.png">
  <br />
  </p>

  - BP_BaseWeapon. Location: /FPS/Blueprints/Weapons

This is the base weapon class with core logic for handling shooting, local effects, etc. All other weapons in the game inherit from this class.
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/Weapon.png">
  <br />
  </p>

### Location of other classes and assets in the FPS folder:
- AI: Logic related to bots and artificial intelligence.
- Blueprints - besides the classes described above, all other important blueprints are stored here. Such as: enums, interfaces, datatables, structures, a large system of interaction with physical materials and much more.
- Animations - animations of characters and weapons.
- Effects - basic effects
- Environment - the main environment, meshes, decals.
- Gameplay - gameplay features for different game modes
- Inputs - setting and assignment of inputs.
- Maps - all maps of the game. The project uses a system with sublevels, which greatly reduces the weight of the final build.
- Sound - sound effects
- Textures - basic textures
- UI - basic elements of the game UI, widgets.
- Weapons - classes of weapons
### Briefly about the rest of the project architecture and classes:
- The LevelFactory and LevelBrickFactory folders contain the basic elements that are only used on these maps: environments, sounds, materials, 
textures.
- The ModularSoldiers folder contains all the classes and assets related to modular assembly and character customization.  
- ProMainMenu - new version of the UI.
- In the folder plugins are connected to the project systems DLSS and OnlineSubsystemSteam

## `View code video`: [Code](https://www.youtube.com/watch?v=3_MN6-e5yfY)
## `Full project information`: [Furyxrpl](https://furyxrpl.com/)
