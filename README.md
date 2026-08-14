# IvyEngine

A lightweight, opinionated 2D game engine built from scratch to strip away unnecessary abstractions and chase maximum performance for my specific game needs.

## Why another engine?

I love [raylib](https://github.com/raysan5/raylib) – it's an amazing library. But while building my game and debugging deep into its internals (like stepping into `InitWindow` and exploring OpenGL calls), I realized I only need a small subset of its features. 

So I built IvyEngine as my personal learning ground and production tool, tailored exactly to what I use.

## Features

- **Platforms**: Linux (X11/Wayland) & Windows (Win32).  
- **Input**: Keyboard & Mouse only (no joystick/gamepad).  
- **Audio**: WAV & MP3 support.  
- **Graphics**: OpenGL with BC3 (DDS) texture compression.  
- **Performance**: Custom memory allocators and minimal internal state.  
- **No bloat**: Stripped-down architecture with zero unnecessary dependencies.
