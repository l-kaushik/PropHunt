
# TODOs
1. Implement a way to have unique server name or a way to client know that which server is created by whom?
1. Prevent player joining if player try to join a session with max players.

# Issues And Fixes
1. Join session try to join on port 0 and connection timed out, only when I package the game and test, works fine in PIE.

	**Fix:** In `AMenuGameMode` class, the `bUseSeamlessTravel` was set to *true*, which should be only true when you are performing a travel from one map to another map in same server. But in this case the client was getting travel to a remote server, so it should be false. 

2. `NumOpenPublicConnection` is not updating when player join or leave the session.

	**Info:** The `NumOpenPublicConnection` is part of `FOnlineSession` class that display number of availabel slots for player to join a server.

	**Fix:** Implemented `RegisterPlayer()` and `UnregisterPlayer()`. From `AMenuGameMode::PostLogin()` calling the `RegisterPlayer()` that in turn call the one from `Subsystem` class which internally updates the `NumOpenPublicConnections`.

3. Session disappear from SearchResult when `NumOpenPublicConnections` become 0.

4. WHen player changes the mesh, its kinda glitchy now. Happening after adding capsule and mesh location updates.

	**Fix	:** `SpehreTraceSingle()` was tracing the floor mesh and changing to it. Fixed by replacing to `SphereTraceMulti()` and checking if should changed to traced actor or not.

# Logs

1. 17-02-2025 
- Implemented UIManagerr class to load widget blueprint classes. Using singleton pattern to prevent multiple initialization.
- Implemented `ShowError` in `WidgetUtils` class. This will create the ErrorBox UI and also log the error.

1. 03-03-2025
- Added timer for player list in lobby.
- Implemented data asset, that stores mesh location and collision capsule's height and raidus. Udpating them whenever player changes the mesh.

2. 07-03-2025
- Implemented data asset, that store static mesh and capsule related data.
- Upgraded trace fuction to fixed changing to floor mesh.