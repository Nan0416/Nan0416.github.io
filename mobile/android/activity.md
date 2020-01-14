# Markdown

Created: 2020-01-03
Modified: 2020-01-11

* [Components](#component)
* [Activity](#activity)
* [Lifecycle](#lifecycle)
* [Lifecycle Awareness Component](#lifecycle-awareness)
* [Tasks & Back Stack](#task)
* [Intent](#intent)
* [Common Methods and Fields](#method)
* [References](#reference)

***
### <a id="intro">I. Components</a>

### <a id="activity">II. Activity</a>

```
java.lang.Object
   ↳	android.content.Context
 	   ↳	android.content.ContextWrapper
 	 	   ↳	android.view.ContextThemeWrapper
 	 	 	   ↳	android.app.Activity (Framework Define Activity: support lifecycle, add view and layout)
 	 	 	 	   ↳	androidx.activity.ComponentActivity (AndroidX: support lifecycleowner, viewmodelstore owner)
 	 	 	 	 	   ↳	androidx.fragment.app.FragmentActivity (AndroidX: support fragment)
 	 	 	 	 	 	   ↳	androidx.appcompat.app.AppCompatActivity
```

Application
Context

### <a id="lifecycle">III. Lifecycle</a>

Activity's lifecycle includes `create -> [start -> resume -> ... -> pause -> stop] -> destroy`. When launching an activity, the activity's `onCreate` is invoked, followed by `onStart` and `onResume`. When pressing home button or go to another app, the current activity will go through `onPause` and `onStop`. If the user killed the activity, `onDestroy` is called.

__Rotating screen will destroy the current activity and create a new one.__ The difference between rotating screen and starting a new activity is that rotating screen use the same process (ART).


##### 1. onCreate
onCreate is invoked when launching the activity or screen rotates. It's only invoked once per activity instance.

Things to do:
1. instantiate class-scope variables.
2. set view model `setContentView`.

`Activity` class source code
```Java
public void onCreate(Bundle savedInstanceState){
    if (mLastNonConfigurationInstances != null) {
        mFragments.restoreLoaderNonConfig(mLastNonConfigurationInstances.loaders);
    }

}

```

##### 2. onStart()
onStart is invoked when the view is visible to user.

Things to do:
1. start animation.

`Activity` class source code
```Java
protected void onStart(){
    // mFragments.doLoaderStart(); // deprecated since 28. 
    mCalled = true;
    getApplication().dispatchActivityStarted(this);
    if(mAutoFillResetNeeded){
        getAutofillManager().onVisibleForAutofill();
    }
    if (mActivityInfo.parentActivityName != null) {
        //...
    }
    if (savedInstanceState != null) {
        //...
    }
    // mFragments.dispatchCreate(); deprecated.
    getApplication().dispatchActivityCreated(this, savedInstanceState);
    if (mVoiceInteractor != null) {
        mVoiceInteractor.attachActivity(this);
    }
    mRestoredFromBundle = savedInstanceState != null;
    mCalled = true;
}
```



#### 3. onResume()
onResume is a point where the user can start interaction with the UI.

```Java
    protected void onResume() {
        getApplication().dispatchActivityResumed(this);
        mActivityTransitionState.onResume(this, isTopOfTask());
        if (mAutoFillResetNeeded) {
            if (!mAutoFillIgnoreFirstResumePause) {
                View focus = getCurrentFocus();
                if (focus != null && focus.canNotifyAutofillEnterExitEvent()) {
                    getAutofillManager().notifyViewEntered(focus);
                }
            }
        }
        mCalled = true;
    }
```

### <a id="lifecycle-awareness">IV. Lifecycle Awareness Component</a>

<span style="color:red">Lifecycle awareness component: decouple activity/fragment with dependent functions.</span>

Activity and Fragment are able to expose their lifecycle events and allow other object to subscribe to these event and query current state. *Lifecycle awareness component is not natively supported by Framework API, instead, it is supported by AndroidX library*.

`ComponentActivity` and `Fragment` implements the `LifecycleOwner` interface, which defines a single method
```Java
public abstract Lifecycle getLifecycle ();
```
to return a `Lifecycle` object. The `Lifecycle` object allows observer to subscribe/unsubscribe and query current state.
```Java
public abstract void addObserver (LifecycleObserver observer);
public abstract Lifecycle.State getCurrentState ();
public abstract void removeObserver (LifecycleObserver observer);
```
The `LifecycleObserver` is a mark interface, it marks the class as a observer. __The real callback method is marked with Annotation `OnLifecycleEvent`__. Observer can ignore some unnecessary method when using annotation instead of interface.

```Java
public class Examplebserver implements LifecycleObserver {
    @OnLifecycleEvent(Lifecycle.Event.ON_RESUME)
    public void connectListener() {
        ...
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_PAUSE)
    public void disconnectListener() {
        ...
    }
}
```




### <a id="method">V. Common Methods and Fields</a>

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

### <a id="intent">V. Intent</a>

Intent object is used for delivering data to component or launching an component. In precise,
* starting an activity with input and get back result <span style="color:red">When an activity starts another activity, the target activity will shift up from the bottom. (Android 9.0)</span>
* starting a service component,
* delivering a broadcast

The components can from the same application or cross-application.

Two types of intent:
* Explicit intent with component's class name.
* Implicit intent to let Android OS select the target activity. When creating a component (activity), we also need to declare the Intents that the component is able to handle in the manifest file. If no intent is specified, the component can only be started by explicit intent.

Important Intent fields
1. ComponentName: if this field is set, then it's a explicit component.
2. Action: a important field used to select the target, such as 
    * `Intent.ACTION_VIEW` view url (broswer), setup alarm, play video.
    * `Intent.ACTION_VIEW` play video,
    * `Intent.ACTION_VIEW` show a position on a map
    * `Intent.ACTION_CALL` make a phone call.
    * `Intent.ACTION_MAIN` start as a main entry point, doesn't receive data.

In addition, we can also define our own action. A action is a String type. A practice is to add the package name as prefix to avoid duplication with others.

3. Category: used with implicit intent. The purpose of category is to add an additional filter field for Android to select the proper component. <span style="color:red">A intent can include multiple categories.</span>Examples,
    * `CATEGORY_BROWSABLE`: means the target activity can be started by a web browser to display (VIEW) a link.
    * `CATEGORY_LAUNCHER`: initial activity of an application.
    * `CATEGORY_DEFAULT`: if a intent doesn't include a category, or an activity's intent filter doesn't define a category, then its value is default.

4. Data: A Uri data (reference) whose scheme is an important factor used to select implicit activity. e.g. the uri = "http://www.google.com", `getScheme()` will return `http`, which is used to select the implicity activity. In addition, you can set the `MIME` type (Multipurpose Internet Mail Extension). e.g. `text/plain`.

5. Extras: an intent object can also store key-value pair. There exists a few pre-defined keys. such as 
    * `Intent.EXTRA_EMAIL`: email recipient addresses, value is type of String[]
    * `Intent.EXTRA_SUBJECT`: set subject.

#### Bundle objects


#### Explicit intent

For example, define a activity called `IntentSenderActivity` that has a button whose click callback will start explicitly itself.

```Java
public class IntentSenderActivity extends Activity {
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_intent_sender);

        Intent intent = getIntent(); // as target acitivity to get the intent used to start itself.
        final int count = intent.getIntExtra(IntentSenderActivity.EXTRA_KEK, 0);
        
        TextView textView = findViewById(R.id.sender_text);
        textView.setText("count " + count);
        Button btn = findViewById(R.id.sender_btn);
        btn.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent();
                intent.putExtra(IntentSenderActivity.EXTRA_KEK, count + 1);
                // explicitly set the recipient activity's application package and its fullname (including package)
                intent.setClassName("com.example.myapplication", IntentSenderActivity.class.getName()); 
                startActivity(intent);
            }
        });
    }
}
```

1. `getIntent()` is a activity defined method used to get the intent used to current activity itself.


#### Implicit Intent (Recipient side)

Activity should advertise itself for receiving corresponding intent (action, category, data uri scheme, and mime type) in the manifest file.
[reference](https://developer.android.com/guide/components/intents-filters#Resolution)

#### Implicit intent

ToDo



### <a id="reference">VI. References</a>
1. <a href="https://developer.android.com/guide/components/fundamentals" target="_blank">Application Fundamentals</a>
2. Professional Android 4th Edition, Reto Meier & Ian Lake
3. <a href="http://www.dre.vanderbilt.edu/~schmidt/android/android-4.0/out/target/common/docs/doc-comment-check/guide/appendix/api-levels.html" target="_blank">Android API Levels</a>
4. <a href="https://developer.android.com/guide/topics/manifest/uses-sdk-element#ApiLevels" target="_blank">What is API Level?</a>
5. <a href="https://developer.android.com/guide/components/intents-filters" target="_blank">Intents and Intent Filters</a>
6. <a href="https://developer.android.com/topic/libraries/architecture/lifecycle" target="_blank">Lifecycle-Aware Components</a>