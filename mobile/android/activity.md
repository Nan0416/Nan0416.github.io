# Markdown

Created: 2020-01-03
Modified: 2020-01-03

* [Components](#component)
* [Activity](#activity)
* [Lifecycle](#lifecycle)
* [Tasks & Back Stack](#task)
* [Transition & Intent](#transition)
* [Fragment](#fragment)
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


### <a id="reference">VI. References</a>
1. <a href="https://developer.android.com/guide/components/fundamentals" target="_blank">Application Fundamentals</a>
2. Professional Android 4th Edition, Reto Meier & Ian Lake
3. <a href="http://www.dre.vanderbilt.edu/~schmidt/android/android-4.0/out/target/common/docs/doc-comment-check/guide/appendix/api-levels.html" target="_blank">Android API Levels</a>
4. <a href="https://developer.android.com/guide/topics/manifest/uses-sdk-element#ApiLevels" target="_blank">What is API Level?</a>