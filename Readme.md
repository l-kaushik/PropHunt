
# Issues And Fixes
1. Join session try to join on port 0 and connection timed out, only when I package the game and test, works fine in PIE.

	**Fix:** In `AMenuGameMode` class, the `bUseSeamlessTravel` was set to *true*, which should be only true when you are performing a travel from one map to another map in same server. But in this case the client was getting travel to a remote server, so it should be false. 

2. `NumOpenPublicConnection` is not updating when player join or leave the session.

	**Info:** The `NumOpenPublicConnection` is part of `FOnlineSession` class that display number of availabel slots for player to join a server.

	**Fix:** Implemented `RegisterPlayer()` and `UnregisterPlayer()`. From `AMenuGameMode::PostLogin()` calling the `RegisterPlayer()` that in turn call the one from `Subsystem` class which internally updates the `NumOpenPublicConnections`.

3. Session disappear from SearchResult when `NumOpenPublicConnections` become 0.