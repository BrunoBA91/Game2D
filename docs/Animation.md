# Animation

**File:** `Animation.h / Animation.cpp`  
**Purpose:** Represents a frame-based animation from a sprite sheet, including timing and frame logic.

---

## Class: `Animation`

### Description
Handles frame progression from a sprite sheet based on elapsed time. Each animation corresponds to a row in the sprite sheet and has optional margins and spacing.

---

### Constructor
```cpp
Animation(int frameW, int frameH, int numFrames, int frameTime, int row,
          int spacingX = 0, int spacingY = 0, int marginX = 0, int marginY = 0);
```
- Initializes animation parameters.
- Each frame has the same width and height.
- `row` defines the vertical location in the sprite sheet.
- Optional values adjust spacing and margins in the texture.

---

### Methods

- `void update()`  
  Advances the animation frame based on elapsed time.

- `void reset()`  
  Resets the animation to the first frame.

- `void setFrame(int frame)`  
  Manually sets the animation to a specific frame index.

- `void setRow(int row)`  
  Changes which row of the sprite sheet is used for this animation.

- `SDL_Rect getCurrentFrameRect() const`  
  Returns the source rectangle for the current frame of the animation.

---

### Private Fields

| Field         | Description                                      |
|---------------|--------------------------------------------------|
| `frameWidth`  | Width of each frame in pixels                    |
| `frameHeight` | Height of each frame in pixels                   |
| `totalFrames` | Total number of frames in the animation          |
| `currentFrame`| Current animation frame index                    |
| `frameDuration`| Duration of each frame in milliseconds          |
| `elapsedTime` | Time elapsed since the last frame switch         |
| `rowIndex`    | Sprite sheet row index                           |
| `spacingX/Y`  | Horizontal/vertical spacing between frames       |
| `marginX/Y`   | Left/top margin before the first frame in a row  |