# UI & Resource

Created: 2020-01-04
Modified: 2020-01-13

* [Introduction](#intro)
* [XML & resource](#xml)
* [Value resource](#value)
* [Size](#size)
* [Style & Theme](#style)
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

### <a id="xml">II. XML & Resources</a>

* Android use XML format to declare and define resources. Resource includes layout, images, values.
* Organization of resources is by directory. [reference](https://developer.android.com/guide/topics/resources/providing-resources#ResourceTypes)
    * res/layout
    * res/drawable
    * res/mipmap
    * res/values

A resource can be referenced by type + filename or by id in XML or in code.

R.class

adpatable.

#### 1. layout resource

A layout resource is a

They are referenced by filename, e.g. `@layout/layout_1` within XML, `R.layout.layout_1` within code.


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


### <a id="value">III. Value resource</a>

#### 1. Define __value__ resources

Value resources include `string`, `style`, `bool`, `color`, `dimen`, `integer`, `integer-array`, `array` and `id`.
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

#### 2. Reference resources

* Android separates App code and resource. All resource, includes images, icon, string (e.g. button's text), layout, are placed inside the `res` directory.
* Android studio utilizes `aapt` tool to automatically generate a `R` class that used to reference these resource in code. Each resource's id will be an integer in the `R` class.
* Android organizes resouce by type (e.g. id, string, layout), [Resource type](https://developer.android.com/guide/topics/resources/available-resources.html). In the `res` directory, each type of resources are placed in its sub-directory. For example, all `layout` xml files are
placed inside the `layout` directory. In addition, each type of resource are stored within a inner class of the `R` class. For example, all `layout`'s references are accessed via `R.layout` subclass.

Reference resource within XML is using `@[<package_name>:]<resource_type>/<resource_name>` syntax. For example, `android:text="@string/my_button_text"` set the button's text to be the value of `my_button_text`'s reference.


### <a id="size">Size</a>

Android support different size unit.

1. px: pixel, a physical pixel is a unit.
2. dp (dip): density-independent pixel, (virtual pixel). Depends on the resolution of the screen, a unit of dp may equal to several px.  For example, Mi A2's screen is 2160 * 1080 pixel, and 1dp = 3px, so the screen size is also 720 * 360 dp.
3. sp: scale pixel. sp is used with text. By default, a sp size = a dp size, but it resizes based on the user's preferred text size.
4. dpi and ppi: dots per inch/pixel per inch. Discribe the density of pixel. # of pixel in one inch line. Mi A2 403 PPI. *dpi and ppi are used interchangeable, but dpi is originally used for printer and scanner*.

Directly using pixel is not recommended, use dp instead.


#### Resources class

The `Context` abstract class defines a abstract method called `public abstract Resources getResources ()` that return a resources object. `Activity` class extends the `Context` class so the resource object can be acquired from within `Activity` class.

1. get screen metrics.

`public DisplayMetrics getDisplayMetrics ()` return a object that represents screen size, pixel density.

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


### <a id="style">Style & Theme</a>

Android provides styling attributes to its views. These attributes can be applied directly in views (inline-css), or defined in XML files and apply the xml as resource with `android:style` or `android:theme` (external).

In addition, android "style" / "theme" is not restricted within styling, all attributes (e.g. TextView's text) are able to treat as "style".

#### "inline style" and external style.

* All attributes, such as `android:layout_width`, `android:fontFamily`, `android:text`, are "style" and can be specified in the external style resource file.
* View class also defines the `style` and `android:theme` attribute to apply external style resource.

```XML
<TextView
        android:id="@+id/textView2"
        style="@style/myText"
        android:layout_width="wrap_content"
        android:layout_height="191dp"
        android:fontFamily="cursive"
        android:includeFontPadding="false" />
```

* `<application>` and `<activity>` only have the `android:theme` attribute.
* When applying a style as `style`, then the style only applied to the view itself, however, if the style is applied as `android:theme`, then all the inner element will have this style.
* inline-attribute has a higher prority than external attribute.

#### external style

`style`s are defined under the `res/values` directory. 

```XML
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <style name="style_name" parent="@[package:]style/style_to_inherit">
        <item name="[package:]style_property_name">style_value</item>
    </style>

    <style name="myText" parent="TextAppearance.AppCompat">
        <item name="android:textColor">#00FF00</item>
        <item name="android:textSize">24sp</item>
        <item name="android:text">Default TextView text: Hello</item>
        <item name="android:fontFamily">monospace</item>
    </style>
</resources>
```

* When applying a style to a view, the view will apply all recoginzed attributes and ignore unknown attributes. In addition, the item's name cannot be arbitary name, they must be specified by some view, either built-in view or custom view.
* a `style` defines a group of items. To reference the entire style, using `@style/myText`. To reference an item within the style, using `?[<package_name>:][<resource_type>/]<resource_name>` , e.g. `?android:attr/textColor`.

#### Platform & AndroidX defined style and inheritance.

* style (attribute) can be inherited with the `parent` attribute. Derived style can also override inherited attributes.
* Another inheritance syntax is to prefix parent name. 

```XML
<style name="MyText" parent="">
<style name="MyText.LightColor"> <!-- inherit from MyText-->
```

* Android platform defines built-in styles, such as `@android:style/TextAppearance`, but you should never use or inherit these style since compatibility issue, instead, inherit from `androidx`.
(Android platform defined style)[https://github.com/aosp-mirror/platform_frameworks_base/blob/master/core/res/res/values/styles.xml], TextAppearance example

```XML
<style name="TextAppearance">
    <item name="textColor">?textColorPrimary</item>
    <item name="textColorHighlight">?textColorHighlight</item>
    <item name="textColorHint">?textColorHint</item>
    <item name="textColorLink">?textColorLink</item>
    <item name="textSize">16sp</item>
    <item name="textStyle">normal</item>
</style>
```

* AndroidX (values source code)[https://android.googlesource.com/platform/frameworks/support/+/androidx-master-dev/appcompat/appcompat/src/main/res/values/]

#### Practice
* Define a file called `AppTheme.xml` in which you define a `style` inherits from `Theme.AppCompat.*`. And then apply the theme within `<application>`.
* Define a file called `style.xml` in which you define `style` and applied as style. Reference attributes from the `theme` with `?`. 



```XML
<style name="TextAppearance.AppCompat" parent="Base.TextAppearance.AppCompat" />
<style name="Base.TextAppearance.AppCompat" parent="android:TextAppearance">
        <item name="android:textColor">?android:textColorPrimary</item>
        <item name="android:textColorHint">?android:textColorHint</item>
        <item name="android:textColorHighlight">?android:textColorHighlight</item>
        <item name="android:textColorLink">?android:textColorLink</item>
        <item name="android:textSize">@dimen/abc_text_size_body_1_material</item>
</style>
```



### <a id="reference">V. References</a>
1. <a href="https://developer.android.com/guide/topics/ui" target="_blank">User Interface & Navigation</a>
2. <a href="https://developer.android.com/guide/topics/resources/available-resources.html" target="_blank">Resource type overview</a>
3. <a href="https://developer.android.com/guide/topics/ui/look-and-feel/themes" target="_blank">Styles and Themes</a>
4. <a href="https://developer.android.com/reference/android/content/res/Resources.html" target="_blank">Resources API</a>