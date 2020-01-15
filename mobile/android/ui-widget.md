Created: 2020-01-06
Modified: 2020-01-10

* [Introduction](#intro)
* [Custom View](#custom)
* [Common Widgets](#common)
* [References](#reference)
***
### <a id="intro">Introduction</a>

Widgets are the basic control unit (e.g. button, textview) that allow users to interact. 
```
java.lang.Object -> android.view.View -> android.widget.TextView -> android.widget.Button
```
`View` is the base widget class, which defines method to handle touch event, manually dispatch event, set layout, size, style. 

#### View initialization

A view has three constructors, (reference)[https://developer.android.com/reference/android/view/View.html#public-constructors_1]

* View(Context): Create a view by code.
* View(Context, AttributeSet): Create a view within XML file. 

```XML
<LinearLayout>
    <MyView app:text="my text" app:color="@color/red">
</LinearLayout>
```

The `AttributeSet` is such a object that represent all the attributes, e.g. `app:text` and `app:color` in this case.

* View(Context, AttributeSet, defStyle): 


The `Context` is the view's runtime, such as the `Activity` object that is holding the view. 

#### android.view.View

1. Handle touch events. 

`View` class has defined a set of static inner interfaces that facilitates touch event handling. [interfaces](https://developer.android.com/reference/android/view/View.html#summary)

i. When the user click the screen, the first triggered event is `onTouch`, then `onLongClick`, finally, `onClick`.  
ii. If `onTouch` return `true`, the click action will stop here, `onLongClick` and `onClick` will never trigger.  
iii. `onLongClick` can be trigger if the click hover for a second, and if it returns true, the `onClick` method will not be invoked.  
iv. `onClick` method is the final event handler.   
v. The `onHover` needs special device (e.g. a pen) in order to be triggered.

setup click callback: `.setOnClickListener(View.OnClickListener)` and inner static interface `OnClickListener` are used to setup click callback.
```Java
interface OnClickListener{
    void onClick(View v);
}
```

##### onTouch event handler.

```Java
public static interface View.OnTouchListener{
    boolean onTouch(View v, MotionEvent e);
}
```
* Android introduce multi-touch support since Android 2.0 (API level 5)
* Each finger is pointer that associated with a pointer id. The pointer id keep consistent as the finger keep moving on the screen. Once the finger (pointer) leaves the screen, the pointer id is invalidated.
* Pointer id is acquired from pointer index when there is a `ACTION_(POINTER)_DOWN/UP` event.
* A motion event's action encode two parts, the pointer index and the action. <span style="color:red">The pointer index from action is only used to acquire pointer id, it's wrong to use the pointer index to directly ask position.</span>
* Pointer index is used to get the pointer position, but pointer index doesn't keep consistent. So pointer index should be acquired from pointer id `e.findPointerIndex(id);` and then used to find position `e.getX(index)`.

```Java
    private Set<Integer> pointerIds;
    public void logMotion(MotionEvent e){
        int pointerIndex = e.getActionIndex();
        int action = e.getActionMasked();
        switch (action){
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                pointerIds.add(e.getPointerId(pointerIndex));
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                pointerIds.remove(e.getPointerId(pointerIndex));
                break;
            case MotionEvent.ACTION_MOVE:
                for(int id: pointerIds){
                    int index = e.findPointerIndex(id);
                    float x = e.getX(index); // return x in pixel with origin at the left top corner of the view.
                    float y = e.getY(index);
                    Log.d(TAG, "Pointer id: " + id + ", x: " + x + ", y: " + y);
                }
                break;
        }
    }
```

* The return X/Y is in <span style="color:red">pixel</span> unit. The origin of X,Y is at the left top corner of the view. When you touch the view, and move to outside of view. For example, move beyond the top of the view, you will get a <span style="color:red">negative Y value</span> 

2. Dispatch touch event manually.

3. View size, padding, margin.

* View size can be set with XML file or code. The underlying ability of setting view size is from the parent's Layout's inner class `LayoutParams`. For example, the basic
`ViewGroup.LayoutParams` defines `height / android:layout_height` and `width / android:layout_width`. Values can be `match_parent`, `wrap_content` or an exact number. 
* View margin is from `ViewGroup.MarginLayoutParams`, a subclass of `ViewGroup.LayoutParams`. It allows the view to have margin.
<span style="color:red">android:layout_marginStart/End controls the left/right margin. If the layout direction is left-to-right, then start/end = left/right, otherwise, start/end = right/left.</span>

* View padding is from View class itself, `android:padding`, and it occupies view size. For example, when the view has a size of "100dp * 100dp", and also set `paddingTop:80dp`, then the actual content moves down by 80dp and may be overflow. Using `wrap_content` can auto scale the view's size.
* View doesn't define a broder.

### <a id="custom">II. Custom Views</a>


### <a id="common">III. Common Widgets</a>

1. android.widget.TextView (only provide display function)

methods about display text.
```Java
.append(CharSequence s)
.setText(CharSequence s)
.setAllCaps(boolean allCaps)
```


2. android.widget.Button

Button itself doesn't add interesting functionality, most of its functions comes from View and TextView.


3. android.webkit.WebView

WebView extends ViewGroup but it's usually used as a widget to display a webpage. <span style="color:red">WebView is a special browser that can be controlled by the app code.</span> For example, we can modify the default url navigation. In addition, let webpage's javascript to invoke native code.

Classes related to WebView
* android.webkit package
    * WebView
    * WebSettings: control the browser settings. enable JS, text size, user agent string, database api.

    We can the setting at runtime, the setting change becomes effective immediately. (don't need set it back to the webview)

    * WebViewClient: used to set up event handling. when navigation start, onPageFinished, ovrride key.
    * WebResourceRequest

#### WebView
* A WebView has a default `WebSettings`, we don't need to create a new `WebSettings`, instead, get the default websettings and change it.
* A WebView has a `WebViewClient` as the default webview client. We can subclass this class and override corresponding method to customize it.
* restriction (compare to a browser): cannot open a page in _blank, cannot edit or check the url.
* WebView also control the goback and goforward feature, like a browser.


#### WebViewClient
1. public boolean shouldOverrideUrlLoading (WebView view, WebResourceRequest request)

Return `true` indicates ignore the request, `false` means loading the request in the WebView.

<span style="color:red">When the web is developed based on SPA (single page application), the method will not be to control the hyperlink navigation.</span> For example, `developer.android.com` is a SPA. When JS is enabled, this webview will load page in the same host and the above method is not triggered.

2.  public WebResourceResponse shouldInterceptRequest(WebView view, WebResourceRequest request); 

Capture all ajax request (e.g. http:// data://). Default behavior is to return `null`, which leads the `WebView` load resource as usual. However, it's possible to hijack this request by returning our response. 

3. public boolean shouldOverrideKeyEvent (WebView view, KeyEvent event)

<span style="color:red">For current testing, this cannot capture the software keyboard input. It is only invoked when pressing the Back button</span>. My guess: the key-input is first captured by webview so that webview can filter out regular key-input, such as typing username and password in a login page. And use the `WebViewClient` to handle all unkonwn key-input, such as back-key.

The source code
```Java
public boolean shouldOverrideKeyEvent(WebView view, KeyEvent event) {
    return false;
}
```
Because the `WebViewClient` doesn't handle this key, `WebView` will expose this event to the Activity.


### <a id="reference">IV. References</a>
1. <a href="https://www.jianshu.com/p/0c863bbde8eb" target="_blank">Android MotionEvent详解</a>
2. <a href="https://developer.android.com/guide/topics/ui/custom-components#compound" target="_blank">Custom View Components - Compound Controls</a>
3. <a href="https://developer.android.com/training/custom-views/create-view" target="_blank">Creating a View Class</a>
4. <a href="https://blog.danlew.net/2016/07/19/a-deep-dive-into-android-view-constructors/" target="_blank">A deep dive into Android View constructors</a>