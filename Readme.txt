Github Repo:https://github.com/BWPearson/FIT2097Assignment2
Video Walkthrough in the GameWalkthrough.7z

Missing functionality includes a Ui (Messages are sent to all players via debug)
extra functionality is incomplete due to lack of UI.
Replication was buggy, as such the doors did not always animate due to the Rep_notify not triggering baack up to the server. Best to play as server for complete experience.
Remote Unlock was buggy because of this and as such was abandoned as it would have prvented playign the game altogether.
The fuse box lacks a particle emitter and sound due to running out of time.
Interaction with objects is driven by a trace written in c++
All existing actors are replicated as best as they can be, these include Doors, duses, fuseboxes and switches.
The onyl Actor replicated via c++ was the Third Person Character.
The Ui does indicate whether the player is on the client or server.

The extra funcitonality was to race your opponent ot the end across a low gravity parkour map.
The timer was implemented and would keep track of who was the fastest, but as there was no UI as I ran out of time, it was not fully completed.
The extra funcitonality also includes and improved and complex vertical level design for racing your opponent through.

How to Play:

WASD to move, space to jump, E to interact when the debug message shows up for an interactable.