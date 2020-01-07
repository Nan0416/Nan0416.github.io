# Markdown

Created: 2020-01-04
Modified: 2020-01-04

* [Introduction](#intro)
* [Resources](#resource)
* [Layout](#layout)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>

Android UI can be created at runtime, declared in XML file, or mixed with two.
XML file is like html file with android define layout (ViewGroup) and widget (View).

#### View
1. View is the basic class for creating UI component. All widgets and layouts, such as button, text fields, linearlayout, are its subclasses.
2. It represents a rectangle area on the screen. 
3. It is responsible for drawing and event handling.

A view object can be created by code at runtime or declared in the XML file.

#### ViewGroup
1. ViewGroup is a subclass of View, which is invisible and used to layout other views.
2. A ViewGroup also defines a inner class, LayoutParams, used by child views to tell the ViewGroup how they want to be layout.

#### ViewGroup.LayoutParams
Each ViewGroup has a inner subclass of LayoutParams. This inner class is used by views inside the ViewGroup to control its position within the ViewGroup.

The base LayoutParams supports `layout_height` and `layout_width`, which inner views can use them to set their height and width. The supported values are `match_parent` or `wrap_content`.
For example, the TextView can use `layout_width` and `layout_height` since the outer ViewGroup is LinearLayout whose LayoutParams inherit ViewGroup.LayoutParams. 
```XML
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android" android:orientation="vertical">
    <TextView android:layout_width="wrap_content" android:layout_height="wrap_content"/>
</LinearLayout>
```


#### class hierachy.

```
Object
    View (focus, listeners, visibility, get position, size, padding, margins)
        ViewGroup
            LinearLayout, RelativeLayout
```

#### ID attributes

A view can have attributes, like html attributes.

```XML
<Button android:id="@+id/my_button" android:text="@string/my_button_text"/>
```


This button widget (view) defines an id field `@+id/my_button` that can be referenced by code or XML.
* `@` indicates the following is a reference to resource.
* `+` means to create the id in the R.class if not existed. If the id already existed, then just reference the existed id.
* `id` means the following literal is an id.

```java
// The button object can be referenced in code.
Button myButton = (Button) findViewById(R.id.my_button);
```

### <a id="resource">II. Resouces</a>

####  Define resources

1. Define __`layout`__ resource.

`layout` resource are stored within `res/layout` directory, for example,

```
/res
    /layout
       /layout_1.xml
       /layout_2.xml
```

They are referenced by filename, e.g. `@layout/layout_1` within XML, `R.layout.layout_1` within code.


2. Define __value__ resources, value resources include `string`, `style`, `bool`, `color`, `dimen`, `integer`, `integer-array`, `array` and `id`.
All values resources are placed within `res/values` directory. __They are defined with the corresponding type and referenced by `name` attributes__.

```
/res
    /layout
        /file1.xml
        /file2.xml
```

The filename doesn't matter, value resources are referenced by their `name` attributes. For example, file1.xml can defines a variety of resources.
```XML
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <item type="id" name="button_ok" />
    <string name="my_button_text">Click Me!</string>
    <bool name="my_boool">true</string>
    <dimen name="out_layout_height">10dp</dimen>
</resources>
```

`id` (item) resource is used to define a id, and later can be assigned to something, e.g. a view. For example,
`<Button android:id="@id/button_ok">`, in this case, you don't need the `+` sign since the id is already declared.

__The best practice is to place each type of value within its own file.__ For example, placing all `string` within `res/values/string.xml` file.

#### Reference resources

* Android separates App code and resource. All resource, includes images, icon, string (e.g. button's text), layout, are placed inside the `res` directory.
* Android studio utilizes `aapt` tool to automatically generate a `R` class that used to reference these resource in code. Each resource's id will be an integer in the `R` class.
* Android organizes resouce by type (e.g. id, string, layout), [Resource type](https://developer.android.com/guide/topics/resources/available-resources.html). In the `res` directory, each type of resources are placed in its sub-directory. For example, all `layout` xml files are
placed inside the `layout` directory. In addition, each type of resource are stored within a inner class of the `R` class. For example, all `layout`'s references are accessed via `R.layout` subclass.

Reference resource within XML is using `@[<package_name>:]<resource_type>/<resource_name>` syntax. For example, `android:text="@string/my_button_text"` set the button's text to be the value of `my_button_text`'s reference.

#### Resources class

The `Context` abstract class defines a abstract method called `public abstract Resources getResources ()` that return a resources object. `Activity` class extends the `Context` class so the resource object can be acquired from within `Activity` class.

1. get screen metrics.

`public DisplayMetrics getDisplayMetrics ()` return a object that represents screen size, pixel density.

##### Size

Android support different size unit.

1. px: pixel, a physical pixel is a unit.
2. dp (dip): density-independent pixel, (virtual pixel). Depends on the resolution of the screen, a unit of dp may equal to several px.  For example, Mi A2's screen is 2016 * 1080 pixel, and 1dp = 3px, so the screen size is also 672 * 360 dp. <span style="color:red">Xiaomi declares the screen is 2160 * 1080, wrong?</span>
3. sp: scale pixel. sp is used with text. By default, a sp size = a dp size, but it resizes based on the user's preferred text size.
4. dpi and ppi: dots per inch/pixel per inch. Discribe the density of pixel. # of pixel in one inch line. Mi A2 403 PPI. *dpi and ppi are used interchangeable, but dpi is originally used for printer and scanner*.

Directly using pixel is not recommended, use dp instead.

```Java
public void logScreenMetric(){
        DisplayMetrics dm = getResources().getDisplayMetrics();
        StringBuilder sb = new StringBuilder();
        sb.append("px per dp: " + dm.density);
        sb.append(", px per font pixel: " + dm.scaledDensity); // depends on the user font setting, it's same as dp by default.
        // the height and width are dynamic depends on the screen is portrait or landscape.
        sb.append(", screen height in pixel : " + dm.heightPixels); // Mi A2, 2016
        sb.append(", screen width in pixel : " + dm.widthPixels); // Mi A2, 1080
        sb.append(", screen dpi: " + dm.densityDpi); // Mi A2 480.
        sb.append(", pixel per inch in X: " + dm.xdpi); // 403.411
        sb.append(", pixel per inch in Y: " + dm.ydpi); // 403.411
        Log.d(TAG, sb.toString());
}
```




### <a id="reference">V. References</a>
1. <a href="https://developer.android.com/guide/topics/ui" target="_blank">User Interface & Navigation</a>
2. <a href="https://developer.android.com/guide/topics/resources/available-resources.html" target="_blank">Resource type overview</a>
4. <a href="https://developer.android.com/reference/android/content/res/Resources.html" target="_blank">Resources API</a>