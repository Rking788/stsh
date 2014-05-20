stsh - StarkShell
====

The idea behind this project is to be able to connect to a machine through an iPhone or other iOS device. Currently, the goal of the project is to connect to a Mac with a built-in iSight camera. The motivation behind this app is the scene at the beginning of IronMan 2 where Tony Stark connects to the TVs with his cell phone.

The current approach is to install a "drone" application on the target machines. The client on the cell phone will attempt to connect to a socket the drone is listening on. Then when a connection is made it will tell the drone to blink the iSight camera. The client will detect the blink (possibly using OpenCV) and that is how it will know which computer the phone is currently pointed at. Then when a connection has been made it will open up a remote terminal to the machine in front of the user.

At this point all of this is theoretical and not much has been implemented yet.
