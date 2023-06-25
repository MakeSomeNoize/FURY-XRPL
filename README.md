<h1 align="center">
  FURY-XRPL
</h1>
FURY XRPL - P2E ONLINE BATTLE ROYALE BUILT ON UNREAL ENGINE 5 &amp; XRPL
</p>
<p align="center">
 <img src="https://raw.githubusercontent.com/VO-GAMES/FURY-XRPL/main/Splash.bmp">
  <br />
  </p>

# Информация о проекте
Игра в жанре шутер P2E ONLINE BATTLE ROYALE. Проект создается на базе игрового движка Unreal Engine 5 с использованием языка C++ и визуального программирования Blueprints. В проект уже используются многие новые технологии UE5, в том числе Nanite и Lumen. Дополнительно уже были добавлены плагины DLSS и Online Subsystem Steam.

# Описание архитектуры и кода проекта.
## Core проекта, основный код и все механики расположены в папке FPS.   
### Основные классы:
 - BP_PlayerCharacter. Расположение: /FPS/Blueprints/CharacterAndClasses. 

Базовый класс персонажа от которого потом наследуются все остальные классы персонажей. Здесь происходит обработка логики взаимодействия игрока с миром, его состояние, действия. В данном классе добавлены компоненты: информация о персонаже, система инвентаря, система интерактивного взаимодействия, информация о здоровье и основной Movement персонажа. Также здесь логика удобно разделена на несколько граф: EventGraph - содержит логику, которая вызывается вначале появления игрока; Health&Death - просчет логики, связанной со здоровьем и смертью персонажа; Actions - обработка событий, связанных с действиями игрока; FireWeapon - логика, связанная с оружием и выстрелами; ClassAbilities - навыки и способности персонажа.      
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
  
 - BP_Base_GM. Расположение: /FPS/Blueprints/GameModes

Класс отвечающий за основные правила игры и события, на котороые не могут влиять игроки, обработку событий подключения/отключения игроков, их количество и тд. От него наследуются все игровые режимы, которые используются в игре. Существует только на сервере.   
</p>
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/GM.png">
  <br />
  </p>
  
 - BP_Base_GS. Расположение: /FPS/Blueprints/GameModes

Класс в котором происходит управление информацией о текущем состоянии игры, появление ботов, хранение разной информации. Добавлены компоненты: система отслеживания статуса игры, менеджер ботов, некоторые настройки игры, система появление игроков. Благодаря добавленным компонентам в данном классе и написанной логике, легко создавать новые игровые режимы и быстро настраивать их.    
</p>
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/GS.png">
  <br />
  </p>

  - BP_PlayerState. Расположение: /FPS/Blueprints/PlayerState

Класс который хранит игформацию об игроке, его имени, очках, статусе.
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/PS.png">
  <br />
  </p>

  - BP_BaseWeapon. Расположение: /FPS/Blueprints/Weapons

Базовый класс оружия с основнйо логикой обработки стрельбы, локальных эффектов. От него наследуется все остальное оружие в игре.
<p align="center">
 <img src="https://github.com/VO-GAMES/FURY-XRPL/blob/main/Images/Weapon.png">
  <br />
  </p>

### Информация о располоджении остальныз классов в папке FPS:
- AI - вся логика связанная с ботами
- Blueprints - кроме выше описанных классов, тут хранятся все остальные важные blueprints. Такие как: enums, interfaces, datatables, structures, большая система взаимодействия с физическими материалами и многое другое.
- Animations - анимации персонажей, оружия.
- Effects - основные эффекты
- Environment - основное окружениеm, meshes, декали.
- Gameplay - геймплейные особенности у разных игровых режимов
- Inputs - установка и назначение inputs
- Maps - все карты игры. В проекте используется система с sublevels, что очень сильно уменьшает вес конечного билда.
- Sound - звуковые эффекты
- Textures - основные текстуры
- UI - базовые элементы игрового UI, виджеты.
- Weapons - классы оружий
### Коротко об остальной архитектуре проекта и классах:
- В папках LevelFactory и LevelBrickFactory находятся основные элементы, которые используются только на этиъ картах: окружение, звуки, материалы, 
текстуры.
- В папке ModularSoldiers находятся все классы и ассеты связанные с модульной сборкой и кастомизацией персонажей.  
- ProMainMenu - новая версия UI.
- В папке plugins находятся подключенные к проекту системы DLSS и OnlineSubsystemSteam

(https://drive.google.com/file/d/1qXZ1uLft0-4RlvSWTojfsFFDF7n7yQJ6/view?usp=sharing){:target="_blank"}
