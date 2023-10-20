**Game Code Name: Starshot**

**Overview:**
Starshot is a rogue-like space exploration puzzle game that immerses players in a minimalistic terminal-style interface. Players navigate through a symbolic solar system, collecting data from planets and moons. They must manage limited fuel, strategically use thrusts to change their trajectory, and account for the time-based decay of data.

![alt text](https://github.com/pjmuthu/Starshot/blob/main/image.jpg?raw=true)


**Core Mechanics:**
- Navigate a solar system map using a symbol to represent the player's probe.
- Carefully manage limited fuel for thrust-based trajectory changes.
- Utilize thrusts to modify the probe's path and explore different celestial bodies.
- Collect data points from planets and moons to fulfill objectives.
- Account for time-based data decay, encouraging efficient exploration.

**Gameplay Loop:**
1. **Quest Initiation:** Players receive a terminal message outlining the objective, such as gathering data from specific celestial bodies.
2. **Solar System Navigation:** Maneuver the probe symbol through the solar system map, aiming to collect data points.
3. **Thrust Mechanics:** Use terminal commands to fire thrusts, altering the probe's trajectory and conserving fuel.
4. **Data Collection:** Collide with planets and moons to collect data points, depicted as numbers next to celestial bodies.
5. **Jump Planning:** Once enough data is collected, aim for a new solar system, considering jump charges and data decay.
6. **Inter-System Exploration:** Repeat the process in the new system, preserving fuel and managing data decay.
7. **Mission Updates:** Successful data collection prompts terminal messages with updates, objectives, and fuel refills.
8. **Time Pressure:** Data decays logarithmically over time, urging players to gather data quickly while accounting for thrust management.

**Visual Style:**
- Minimalistic terminal-like interface featuring a symbolic solar system map.
- Distinct colors for planets, moons, and the player's probe symbol.
- Minimal UI elements displaying fuel, data points, and objectives.

**Audio Design:**
- Terminal-style electronic sounds for thrusts, collisions, and data collection.
- Audio cues for mission updates and jump sequences.
- Calming ambient music to enhance the exploratory atmosphere.

**Levels and Progression:**
- Each run represents a unique "terminal session" with a distinct quest and objectives.
- Successful data collection provides points for upgrades and new thrust commands.
- Difficulty increases as players progress through different solar systems.

**Controls:**
- Navigate the game using keyboard inputs to change the probe's trajectory.
- Input thrust commands to fire engines and alter the probe's path.
- Immerse players in a retro-inspired terminal experience.


**UI Design**

**Center of the Screen: Radar View**
- The radar occupies the center of the screen and is the main focus.
- **Celestial Bodies:** Planets and moons are represented as circles (planets) and smaller dots (moons).
- **Player Probe:** Represented by a distinctive triangle symbol.
- **Data Points:** Shown as a circular progress bar that starts full and visually decays over time. If the player's probe is near a celestial body, the bar fills up.

**Top Left Corner: System Information**
- **Fuel Indicator:** Numerical display of remaining fuel units.

**Bottom Left Corner: Collected Data**
- **Jump Charges:** Numerical representation of available jump charges, if applicable.

**Top Right Corner: Jump Charge Information**
- **Data Collected:** A level-up style display that shows the amount of data points collected. This could be represented as a progress bar that fills up as data is collected, showing a numerical value alongside.

**Additional Considerations:**
- **Color Palette:** Use distinct colors for the symbols and indicators, ensuring good contrast against the background.
- **Visual Effects:** Implement subtle animations, like fading or pulsing, to indicate data decay and thrust actions.
- **Audio Indicators:** Employ radar-like sound effects for interactions to enhance immersion.

