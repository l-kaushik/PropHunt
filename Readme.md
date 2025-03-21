# notes
1. Can change texture LOD bias to reduce vram usage
1. remove lumen and use bakelight for low end system
1. disable reflections
1. remove TSR anti aliasing
1. disable shadows completely
1. set screen resolution to 50%, ugly graphics but better FPS
1. use culling, occulsion

# TODOs
1. Implement a way to have unique server name or a way to client know that which server is created by whom?
1. Prevent player joining if player try to join a session with max players.
1. Implement TravelError and NetworkError handling in GameInstance class.
1. Improve prop hunt shooting mechanism to detect hit based on mesh not on collision box.

# Testing Data

1. 12/03/2025
- Map: Test map
- GPU: Intel GPU 610
- Settings: 50% screenpercentage, 60 FPS, all quality sets to low/off, baked light, gpu usage 70%
- Settings: 50% screenpercentage, 30 FPS -- same --, gpu usage 50%
- Settings: 100% screenpercentage, 60 fps, -- same --, gpu usage 100%

# Issues And Fixes
1. Join session try to join on port 0 and connection timed out, only when I package the game and test, works fine in PIE.

	**Fix:** In `AMenuGameMode` class, the `bUseSeamlessTravel` was set to *true*, which should be only true when you are performing a travel from one map to another map in same server. But in this case the client was getting travel to a remote server, so it should be false. 

2. `NumOpenPublicConnection` is not updating when player join or leave the session.

	**Info:** The `NumOpenPublicConnection` is part of `FOnlineSession` class that display number of availabel slots for player to join a server.

	**Fix:** Implemented `RegisterPlayer()` and `UnregisterPlayer()`. From `AMenuGameMode::PostLogin()` calling the `RegisterPlayer()` that in turn call the one from `Subsystem` class which internally updates the `NumOpenPublicConnections`.

3. Session disappear from SearchResult when `NumOpenPublicConnections` become 0.

4. When player changes the mesh, its kinda glitchy now. Happening after adding capsule and mesh location updates.

	**Fix	:** `SphereTraceSingle()` was tracing the floor mesh and changing to it. Fixed by replacing to `SphereTraceMulti()` and checking if should changed to traced actor or not.

5. When host quit the game `MenuGameMode::ReturnToMainMenuHost()` send a reason to `PlayerController::ClientReturnToMainMenuWithTextReason_Implementation()`, but the reason is getting overrite by internal function calling it again.

# Logs

1. 17-02-2025 
- Implemented UIManagerr class to load widget blueprint classes. Using singleton pattern to prevent multiple initialization.
- Implemented `ShowError` in `WidgetUtils` class. This will create the ErrorBox UI and also log the error.

2. 03-03-2025
- Added timer for player list in lobby.
- Implemented data asset, that stores mesh location and collision capsule's height and raidus. Udpating them whenever player changes the mesh.

3.  07-03-2025
- Implemented data asset, that store static mesh and capsule related data.
- Upgraded trace fuction to fixed changing to floor mesh.
- Implemented throbber on session search and also added refresh capability.

4. 9-03-2025
- Implemented handling of client leaving a session and host leaving a sesson.
- Implemented a variant of `ShowError` that can take a callback function bind to the ok button.

5. 10-03-2025
- Implemented better handling of host leaving session or game.
- Binded functions to GEngine's OnNetworkFailure and OnTravelFailure.
- Not using previous method now, but code is not removed either.