# Noodle Nodes

A utility AI implementation for the Godot Engine using gdextension. If you don't know what utility AI is, Wikipedia defines it as follows:

*"...utility AI, is a simple but effective way to model behaviours for non-player characters. Using numbers, formulas and scores to rate the relative benefit of possible actions, one can assign utilities to each action. A behaviour can then be selected based on which one scores the highest 'utility'...* - [Wikipedia - Utility system](https://en.wikipedia.org/wiki/Utility_system)

This repository contains the source code and tests. For compiled binaries check the releases page.

Noodle Nodes is an improved fork of [UtilityAI](https://github.com/JarkkoPar/Utility_AI).

## Compatibility

Godot 4.4 or newer.

## Components

* *Utility based AI Agent Behaviours*
* *Behaviour Tree with utility support*
* *State Tree with utility support*
* *Node Query System*


## How to use the nodes

You can find the tutorials [here](https://github.com/Flynsarmy/Noodle_Nodes/tree/main/tutorial).

## Documentation

You can find the documentation [here](https://github.com/Flynsarmy/Noodle_Nodes/blob/main/documentation/Nodes_latest.md).


## Compiling from source

1. Clone the repository
2. Initialize and update submodules
```
git submodule init && git submodule update
```
3. Now you should be able to compile the project with scons
```
scons platform=<valid platform>
```

After compilation, the binaries are created in to the `demo/addons/noodle_nodes/` folder. If all goes well, you should be able to launch the demo project in Godot and start using the addon.

