# credits

1. Profile photo from @pikisuperstar at freepik.
1. Main Menu background music by royaltyuserecords from pixabay.

# TODOs
1. Implement a way to have unique server name or a way to client know that which server is created by whom?
1. Prevent player joining if player try to join a session with max players.
1. Add free look with alt button, so prop can look around without disturbing hide position.

# Testing Data

1. 12/03/2025
- Map: Test map
- GPU: Intel GPU 610
- Settings: 50% screenpercentage, 60 FPS, all quality sets to low/off, baked light, gpu usage 70%
- Settings: 50% screenpercentage, 30 FPS -- same --, gpu usage 50%
- Settings: 100% screenpercentage, 60 fps, -- same --, gpu usage 100%

2. 13/06/2025
- Map: Test map
- GPU: RTX 3050 4 GB VRAM
- 120+ FPS on low to high settings, 60+ FPS on epic and cinematic settings.

# Major Issues And Fixes
1. Join session try to join on port 0 and connection timed out, only when I package the game and test, works fine in PIE.

	**Fix:** In `AMenuGameMode` class, the `bUseSeamlessTravel` was set to *true*, which should be only true when you are performing a travel from one map to another map in same server. But in this case the client was getting travel to a remote server, so it should be false. 

2. `NumOpenPublicConnection` is not updating when player join or leave the session.

	**Info:** The `NumOpenPublicConnection` is part of `FOnlineSession` class that display number of availabel slots for player to join a server.

	**Fix:** Implemented `RegisterPlayer()` and `UnregisterPlayer()`. From `AMenuGameMode::PostLogin()` calling the `RegisterPlayer()` that in turn call the one from `Subsystem` class which internally updates the `NumOpenPublicConnections`.

3. Session disappear from SearchResult when `NumOpenPublicConnections` become 0.

4. When player changes the mesh, its kinda glitchy now. Happening after adding capsule and mesh location updates.

	**Fix:** `SphereTraceSingle()` was tracing the floor mesh and changing to it. Fixed by replacing to `SphereTraceMulti()` and checking if should changed to traced actor or not.

5. When host quit the game `MenuGameMode::ReturnToMainMenuHost()` send a reason to `PlayerController::ClientReturnToMainMenuWithTextReason_Implementation()`, but the reason is getting overrite by internal function calling it again.

6. Changing menu background image isn't working. Maybe: try sending image to GameInstance and then perform RPC to server so data persist and then try changing

7. Profile image cannot be replicated cuz of limit of 64 KB in TArray element size.

8. BestProp is not showing in the scoreboard.

# notes (for future upgrade reference)
1. Can change texture LOD bias to reduce vram usage
1. remove lumen and use bakelight for low end system
1. disable reflections
1. remove TSR anti aliasing
1. disable shadows completely
1. set screen resolution to 50%, ugly graphics but better FPS
1. use culling, occulsion

1. Load client data in game instance before joining the session, then load data to server's game instance. Then, when travel to game map, again load data to server's game state