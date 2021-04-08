# step-series-example-openFramework
[STEP400](https://github.com/ponoor/STEP400) example code for openFrameworks.


In default, [ofxOscSender](https://openframeworks.cc//documentation/ofxOsc/ofxOscSender/#!show_sendMessage) uses OSCBundles which STEP400 can't parse.
Please specify to use OSCMessages for an osc sender.

```
sender.sendMessage(m, false);
```
