# Markdown

Created: 2020-01-03
Modified: 2020-01-03

* [Components](#component)
* [Activity](#activity)
* [Lifecycle](#lifecycle)
* [Tasks & Back Stack](#task)
* [Transition & Intent](#transition)
* [Fragment](#fragment)
* [Common Methods and Fields](#method)
* [References](#reference)

***
### <a id="intro">I. Components</a>


### <a id="lifecycle">III. Lifecycle</a>

Activity's lifecycle includes `create -> [start -> resume -> ... -> pause -> stop] -> destroy`. When launching an activity, the activity's `onCreate` is invoked, followed by `onStart` and `onResume`. When pressing home button or go to another app, the current activity will go through `onPause` and `onStop`. If the user killed the activity, `onDestroy` is called.

__Rotating screen will destroy the current activity and create a new one.__ The difference between rotating screen and starting a new activity is that rotating screen use the same process (ART).




##### onCreate
onCreate is invoked when launching the activity or screen rotates. It's only invoked once per activity instance.

Things to do:
1. instantiate class-scope variables.
2. set view model `setContentView`.


### <a id="method">Common Methods and Fields</a>

1. public boolean onKeyDown (int keyCode, KeyEvent event); [reference](https://developer.android.com/reference/android/view/KeyEvent)
* Handle the keydown when the focused view ignore the event. If the focused view handles this event, then the activity will not get the event.

* Return true to prevent this event from being propagated further (to upper level).

> KeyEvent
> `KeyEvent` is class used to represent a key event from a device (e.g. on-screen keyboard, joycon). Each KeyEvent encodes
> action (down, up), keycode, scancode, and metastate
> 1. keycode is the meaning of the key, e.g. `KeyEvent.KEYCODE_A, KeyEvent.KEYCODE_BACK, KeyEvent.KEYCODE_SHIFT_LEFT`
> 2. metastate is the state of the input device, e.g. if the `SHIFT` key is also pressed when pressing the input.
> 3. action is the down,up. When a key is held for a while, the `onKeyDown` is repeated invoked and the event will also include the count of key. e.g. `getRepeatCount()`.
> 4.  scancode is the raw key data.

```Java
public boolean onKeyDown(int keyCode, KeyEvent event) {
        Log.d("KeyDown", "Repeated: " + event.getRepeatCount());
        if(event.getKeyCode() == KeyEvent.KEYCODE_BACK){
            Log.d("KeyDown", "KeyCode back");
        }
        if(event.getAction() == KeyEvent.ACTION_DOWN){
            Log.d("KeyDown", "key is pressed");
        }else if(event.getAction() == KeyEvent.ACTION_UP){
            Log.d("KeyDown", "key is released");
        }else if(event.getAction() == KeyEvent.FLAG_CANCELED){
            Log.d("KeyDown", "key pressing is cancelled");
        }else{
            Log.d("KeyDown", "action: " + event.getAction());
        }
        return true;
}
```

### <a id="reference">VI. References</a>
1. <a href="https://developer.android.com/guide/components/fundamentals" target="_blank">Application Fundamentals</a>
2. Professional Android 4th Edition, Reto Meier & Ian Lake
3. <a href="http://www.dre.vanderbilt.edu/~schmidt/android/android-4.0/out/target/common/docs/doc-comment-check/guide/appendix/api-levels.html" target="_blank">Android API Levels</a>
4. <a href="https://developer.android.com/guide/topics/manifest/uses-sdk-element#ApiLevels" target="_blank">What is API Level?</a>