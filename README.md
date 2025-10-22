# Livesplit Overlay for Mirror's Edge

A DLL overlay to display your **LiveSplit timer** inside **Mirror's Edge** when playing on a **single monitor**.

## Preview

[![IMAGE ALT TEXT HERE](https://i.ytimg.com/vi/RHCmfNBMeT4/maxresdefault.jpg)](https://youtu.be/RHCmfNBMeT4)

## ⚙️ How it works

- You must **inject the DLL** into the game (Mirror’s Edge).
- It renders a section of the screen where your LiveSplit timer is.
- The overlay is **hardcoded** for a specific size and position.

---

## 📐 LiveSplit Setup

To match the hardcoded overlay area, make sure your LiveSplit layout is:

- **Horizontal**
- **Width:** `1940`
- **Height:** `44`

These values match the coordinates found in `render.cpp` (line 108):

```cpp
{ 1940.0f, 44.0f, 0.0f, 1.0f, 1.0f, 1.0f }, // tip



