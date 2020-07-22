##Install the Nineva MQTT Utilities plugin first

This is a really simple example of using mosquitto in unreal as controllers, 
I tested it with a couple of gyros tapped on the tip of my feet and walking 
on top of the structure in the frabrication folder.

I used the plugin from NINEVA (really awesome stuff, you should buy them a 
coffee), which you can find here https://github.com/NinevaStudios/MqttUtilities-UnrealEngine/releases

This project works only in C++ projects (due to the plugin); I was using 
the 4.23 version while i was working on it, though i really haven't made 
the time to try it out in newer versions. Let me know how that plays out >:3

##Extra Files

I added a couple of folders that have a 00_ before the name inside of CONTENT, those aren't
part of the unreal engine project but are very necessary for you to understand
and assemble the whole thing, so you can put them outside of the folder if you like