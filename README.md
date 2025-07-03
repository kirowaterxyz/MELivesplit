# Livesplit Overlay for Mirror's Edge

A quick-and-dirty DLL overlay to display your **LiveSplit timer** inside **Mirror's Edge** when playing on a **single monitor**.

---

## ❓ What is this?

This is **not** a general-purpose overlay for all fullscreen games.  
It's a **very specific project** made only for **Mirror’s Edge** (2009).

> Show the LiveSplit timer while playing Mirror’s Edge in fullscreen, with only one monitor.

It worked surprisingly well — made in just a few minutes, and then left as-is.  
If you're speedrunning Mirror’s Edge and want to see your timer without a second screen, this might help.

---

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
