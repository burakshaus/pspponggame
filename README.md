# PSP Pong Game

A classic Pong game implementation for the PlayStation Portable (PSP) with AI opponent.

## Features

- **Player vs AI**: Play against a computer-controlled opponent
- **Smooth Graphics**: Runs at 60 FPS with PSP's Graphics Utility (GU) library
- **Simple Controls**: Use D-pad to control your paddle
- **Physics Engine**: Ball bouncing with wall and paddle collision detection

## Game Elements

- **White paddles**: Player (left) and CPU (right)
- **Cyan ball**: Bounces around the screen
- **AI opponent**: Follows the ball with slight delay for balanced gameplay

## Controls

- **UP/DOWN**: Move your paddle
- **START**: Exit game

## Building

### Using Docker (Recommended)

```bash
# Build using PSP SDK Docker container
sudo docker run --rm -v $(pwd):/build ghcr.io/pspdev/pspdev:latest sh -c "cd /build && make"
```

### Manual Build

If you have PSP SDK installed locally:

```bash
make
```

## Installation

1. Build the game (see above)
2. Copy `EBOOT.PBP` to your PSP:
   - Path: `ms0:/PSP/GAME/PongPSP/EBOOT.PBP`
3. Launch from the PSP game menu

## Running on Emulator

You can also run the game using [PPSSPP](https://www.ppsspp.org/):

1. Install PPSSPP
2. Load `EBOOT.PBP` in the emulator
3. Play!

## Technical Details

- **Language**: C
- **Graphics**: PSP GU (Graphics Utility)
- **Screen Resolution**: 480x272
- **Build System**: PSP SDK Makefile

## Project Structure

```
pspponggame/
├── main.c          # Main game logic and rendering
├── Makefile        # Build configuration
├── EBOOT.PBP       # Compiled PSP executable (generated)
├── .gitignore      # Git ignore rules
└── README.md       # This file
```

## Development

The game uses double buffering for smooth rendering and proper frame buffer initialization to ensure graphics display correctly on PSP hardware.

## License

Open source project for PSP homebrew development.

## Credits

Developed for PSP homebrew community.