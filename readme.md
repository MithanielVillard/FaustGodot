# FaustGodot

`FaustGodot` is [Faust](https://faust.grame.fr) DSP programming language integration in Godot Game Engine.

> [!NOTE]
> `FaustGodot` is used for real-time audio applications. It is not intended for creating gameplay logic using Faust.
> It allows DSP programs to manipulate audio through `AudioStreamPlayer` and `AudioEffect`.

### Installation
Download the extension from the `release` tab and just extract it to your root Godot project
directory.
Just launch your Godot project and make sure the extension is **enabled** in the project settings.

>[!NOTE]
>Requires Godot version >= 4.1

| Platform | Notes                                                                                                                 |
|----------|-----------------------------------------------------------------------------------------------------------------------|
| Macos    | Working ✅ (tested on Intel Mac Tahoe 26)                                                                              |
| Linux    | Working ✅ (tested on CachyOs - Arch based distro)                                                                     |
| Windows  | ⚠️ Faust Libraries not working due to a bug on Windows.<br/>Appart from that it works normally (tested on Windows 11) |

### Usage

FaustGodot adds Faust as a new kind of script. They can be created the same way as a regular GDScript or C# Script
but can't but directly attached to a node or a resource. They can only be referenced by an `AudioStreamFaust` or `AudioEffectFaust`.

>[!NOTE]
> In order to have MIDI working you **_must_** place a `MidiHandlerFaust` node anywhere in your scene. Having multiple instances is useless. 

### Features

- Create and edit Faust DSP Scripts directly inside Godot's script editor.
- Custom color theme with syntax highlighting
- Use DSP as both `AudioStream` and `AudioEffect`
- In editor and script access for modifying DSP parameters

### To-do

- Add Faust documentation on hover in the script editor.
- Syntax completion in script editor

### FaustGodot vs Faust2Godot

[Faust2Godot](https://github.com/MithanielVillard/Faust2Godot)  is the static version of this project. That means that once exported you can not modify the DSP 
code outside its parameters. Being compiled, [Faust2Godot](https://github.com/MithanielVillard/Faust2Godot) have much better performances. It is recommended to use it
if you have only a few DSP scripts and performance is critical to your applications.

## Building project

`faust2godot` use the [CMake](https://cmake.org) project generator.
Building project can take a while due to having to compile godot-cpp sdk and libfaust. Multithreaded compilation is highly recommended (`-j nbrThread`).

Building the GDExtension :
```shell
mkdir build && cd build
cmake ..
cmake --build . -j 10
```

Building the GDExtension (in release) :
```shell
mkdir build && cd build
cmake ..
cmake --build . -j 10 --config Release
```

Building the GDExtension (Changing Godot target) :

_Possible values :_
- template_release
- template_debug
- editor

```shell
mkdir build && cd build
cmake .. -DGODOTCPP_TARGET=template_release
cmake --build . -j 10 --config Release
```

Building the GDExtension (with tests) :
```shell
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build . -j 10
```

Run tests :

```shell
cd build
ctest -V
```