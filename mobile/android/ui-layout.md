# Markdown

Created: 2020-01-06
Modified: 2020-01-06

* [Introduction](#intro)
* [References](#reference)
***
### <a id="intro">Introduction</a>

Android provides a couple of layout styles, LinearLayout (Framework API), RelativeLayout (Framework API), and ConstraintLayout (AndroidX).

1. LinearLayout layouts all views in either horizontal or vertical direction, it can "center" child views.
2. RelativeLayout
3. ConstraintLayout layouts all views by constraints. If the view's layout_width/height is set to '0dp' (match_constraint), then the constraintLayout can stretch the size of the view. (In constrast, RelativeLayout and LinearLayout cannot change its child views' size)

### <a id="layout">Layout</a>

#### Example: LinearLayout

LinearLayout arranges views either horizontally or vertically. It has XML attributes
* orientation: set horizontal or vertical
* gravity: control how views should be positioned within the layout.
* weightSum: along with `LayoutParams`'s `layout_weight` to control how to divide the vertical or horizontal space when the orientation is vertical or horizontal, respective.
It also has a inner class `LinearLayout.LayoutParams` that defines `layout_gravity` and `layout_weight`. 
```XML
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical" >
</LinearLayout>
```

#### ConstraintLayout
ConstraintLayout bind a view's position to another view or the parent constraint layout. It also help center views, stretch views (by weights),
```XML
    <Button
        android:id="@+id/digit_1"
        android:layout_width="0dp"
        android:layout_height="0dp"
        android:text="1"
        app:layout_constraintBottom_toTopOf="@+id/digit_0"
        app:layout_constraintEnd_toStartOf="@+id/digit_2"
        app:layout_constraintStart_toStartOf="parent"
        app:layout_constraintTop_toBottomOf="@+id/digit_4"
        app:layout_constraintHorizontal_bias="0.5"
        app:layout_constraintHorizontal_chainStyle="spread"
         />
```

* The above example layout the button accroding to parent and other 3 around view. Its width and height are set to 0dp (match_constraint) so it will be stretched to reach the constraint.
* horizontal bias is effective when the horizontal size (width) is an exact number or wrap content. In this case, the left and right constraint will "apply equal force" to poll the view, and the view will locate at the center. The default value of bias is 0.5, meaning left and right apply the same "force".
A bais = 0.75 will make the right force stronger than the right force, so the view will be location at right a little bit.

* chain style `layout_constraintHorizontal/Vertical_chainStyle` is used to control how a group of views are horizontally/veritcally distributed. When using chainstyle, bais will lose effect. It has three values, `packed`, `spread`, `spread_inside`.
    * When view size are not `match_constraint`, 
        * `pack` will center all views together, it also provides `layout_constraintHorizontal/Vertical_bias` to shift the center.
        * `spread` will distribute views equally, 
        * `spread_inside` will distribute the two terminal views to parent and evenly distribute other views. 
    * When view size is `match_constraint`, chain style are not necessary because all the effect is to stretch views and evenly distribute them. <span style="color:red">In this case, `constraintLayout` provides each view a weight to control the stretch degree.</span>

<p><img src="./img/chain-style.png" width="500" height="200"></p>

```XML
<Button
    android:layout_width="0dp"
    app:layout_constraintHorizontal_chainStyle="spread"
    app:layout_constraintHorizontal_weight="1"
>
<Button
    android:layout_width="0dp"
    app:layout_constraintHorizontal_weight="2"
>
```
The second button's width is twice large as the first button.

4. Layout (position a view)

`getLayoutParams()` and `setLayoutParams(ViewGroup.LayoutParams)` are used on a view to tell its parent how it wants to be layouted.
```java
// the btn's parent is a linear layout.
LinearLayout.LayoutParams btn1LayoutParams = new LinearLayout.LayoutParams(btn1.getLayoutParams());
btn1LayoutParams.weight = 0.5f;
btn1.setLayoutParams(btn1LayoutParams);
```

### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>