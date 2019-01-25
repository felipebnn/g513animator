# G513animator

An efficient led controller to be used with the G513 keyboard on Linux. The binary protocol used was based on https://github.com/MatMoul/g810-led.

Comes with an animation (TapGlowAnimation) that sets all keys to a bluish green (#00FFFF) and whenever a key is pressed it goes red (#FF0000) for 2 seconds, it also has a 10 minute timer that turns off all the led (#000000). When you press the Pause key, the animation stops until it's pressed again, in case you want to type a password and there are people around you.
