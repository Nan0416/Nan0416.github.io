# Markdown

Created: 2020-01-12
Modified: 2020-01-13

* [Introduction - onSave/RestoreInstanceState](#intro)
* [Parcelable & Bundle](#parcelable)
* [ViewModel](#viewmodel)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>

1. Home Button vs. Back Buttom

Pressing the home button will place the activity in background (onPause -> onStop)
Pressing the back button will pop this activity out of the task stack and destroy it (onPause -> onStop -> onDestroy)

2. In some case, the user may expect to have the same activity when the user rotate the screen or resume a background activity from background. However, configuration change (rotation) will definitive destroy and recreate the activity. Resuming a background activity may also create a new activity since Android may kill the background task to reclaim resource.

In the prespective of system, an activity will be destroied when
* invoke activity.finish()
* configuration change
* kill a background task in background

In the prespective of a user, an activity will be destroied when
* click Back button
* clear the background activity in task manager.
* control app to invoke activity.finish()

3. `onSaveInstanceState` method is a callback method used to save UI related data, such as TextView's text, used to restore UI. It is invoked when the activity may be destroied but the user doesn't expect.
* configuration change.
* switch to background.
* start a new activity.

The method is usually invoked after `onStop`.

It's not invoked when
* invoke .finish.
* press Back button.
In these cases, the user wants a new activity instance when launch the activity again.

4. `onRestoreInstanceState` method is a callback method used to restore UI based on saved data.
This method is invoked
* configuration change
* previous switch to background and killed by system (not by user).

The default implementation of Activity's `onSave/RestoreInstanceState` is to invoke each view's `onSave/RestoreInstanceState` in the view tree. However, each concrete widget may has different implmenetation of this method.
For example, TextView, by default, doesn't save the text unless the freeze text is set to true.

### <a id="parcelable">II. Parcelable & Bundle</a>

Bundle is a class used to passing information between activities.

Parcelable is an interface for serializing and deserializing objects. It has three 

```Java
public interface Parcelable{
    // a Parcelable class must implement the inner interface, it deserializes the object from a parcel stream.
    public interface Creator<T> {
        public T createFromParcel(Parcel source);
        // Returns an array of the Parcelable class, with every entry initialized to null.
        public T[] newArray(int size);
    }
    // describe the object. e.g. if includes file descriptor. return 0 in most case.
    public abstract int describeContents ();
    // serialize the object's contents to the Parcel object.
    public void writeToParcel(Parcel dest, int flags);
}
```
`Parcel` is an input/output handler that deserialize/serialize object to/from a shared memory. It can and only can read/write primitive types and primitive type arrays `readDouble` and `writeDouble`. Complex types are handled by the `Parcelable`. Under the `Parcel` class, it's native C/C++ code.

Parcel can write/read data in the same order.

#### Implementation of Parcelable
* `Bundle` is common implementation of `Parcelable` interface, which provides supports for primitives types. It also provides a Map like interface for storing and retrieving data.

* `Intent` is another implementation of `Parcelable`. The main goal of Intent is to select and launch a target activity. But since the target activity should receive the `Intent`, so the `Intent` also implements the `Parcelable` interface. In addition, it also provides some extra methods for storing primitive types, Bundle, Parcelable.

```Java
public Intent putExtra (String name, String value)
public Intent putExtra (String name, long[] value)
public Intent putExtra (String name, Parcelable value)
public Intent putExtra (String name, Bundle value)

public Bundle getExtras (); // it aggregate all extras into a single Bundle.
public int getIntExtra (String name, int defaultValue) // convenient method for get int extra
public T getParcelableExtra (String name);
public * get***Extra() 
```

#### When is serialization happens?
The serialization happens when doing cross-activity information pass. For example, 
* using `startActivity` will launch serialize the `Intent` object, which will flatten inner `Parcelable` objects. In this case, the Parcelable's `.writeToParcel` will be called.

* system killed a background activity. In this case, it will serialize the bundle saved on `onSaveInstanceState`.

#### Example

```Java
public class Address implements Parcelable {
    private String state, city, zipcode, line1, line2;
    public Address(String state, String city, String zipcode, String line1, String line2){
        this.state = state; this.city =city; this.zipcode = zipcode;this.line1 = line1; this.line2 = line2;
    }
    @Override
    public int describeContents(){
        return 0;
    }
    @Override
    public void writeToParcel(Parcel dest, int flags){
        Log.d("Parcelable/writeToParcel", "writeToParcel");
        dest.writeString(this.state); dest.writeString(this.city);
        dest.writeString(this.zipcode); dest.writeString(this.line1); dest.writeString(this.line2);
    }
    public static final Parcelable.Creator<Address> CREATOR = new AddressCreator();
    public static class AddressCreator implements Creator<Address>{
        public Address createFromParcel(Parcel source){
            String state = source.readString(); String city = source.readString();
            String zipcode = source.readString(); String line1 = source.readString();
            String line2 = source.readString();
            return new Address(state, city, zipcode, line1, line2);
        }
        public Address[] newArray(int size){
            return new Address[size];
        }
    }
}
```

### <a id="viewmodel">III. ViewModel</a>

ViewModel is a class used to retain data due to app configuration change (e.g. rotation). 

For example,
```Java
public class ViewModelPracticeActivity extends ComponentActivity {
    private MyViewModel model;
    protected void onCreate(Bundle savedInstanceState) {
        //...
        ViewModelStoreOwner storeOwner = (ViewModelStoreOwner)(this);
        ViewModelStore store = storeOwner.getViewModelStore();
        ViewModelProvider provider = new ViewModelProvider(store, new ViewModelProvider.NewInstanceFactory());
        model = provider.get(MyViewModel.class);
    }
}
```

After the activity is recreated due to configuration change, the model object is same even thought the activity changed.


#### Under the hood.

Configuration change causes the entire activity to be re-created. <span style="color:red">It will invoke the activity's constructor to create a brand new activity.</span> However, the app may need to only recreate UI related data and objects, such as the view tree. It's not necessary to re-create buiness logic data. A ViewModel is a class for retaining data during configuration change.


A `ViewModel` object is bound to a `ViewModelStore` object. The `ViewModelStoreOwner` interface defines a method to get the `ViewModelStore` object.
```Java
public abstract ViewModelStore getViewModelStore ()
```
`ComponentActivity` implements this interface. Source code
```Java
public ViewModelStore getViewModelStore() {
        if (getApplication() == null) {
            throw new IllegalStateException("Your activity is not yet attached to the "
                    + "Application instance. You can't request ViewModel before onCreate call.");
        }
        if (mViewModelStore == null) {
            NonConfigurationInstances nc = (NonConfigurationInstances) mLastNonConfigurationInstances.activity;
            // see below retain process;
            if (nc != null) {
                mViewModelStore = nc.viewModelStore;
            }
            if (mViewModelStore == null) {
                mViewModelStore = new ViewModelStore();
            }
        }
        return mViewModelStore;
    }
```

During the first time to launch an activity, `NonConfigurationInstances` is null, so the activity creates a `new VideModelStore()` object. After configuration change, the application's `ActivityThread` invokes `performDestroyActivity` method, which gets and retains a `Activity.NonConfigurationInstances` from the activity and then the activity is destroied.

During the second time to launch the activity, the activity will be given the last retained object through the `.attach` object.

```Java
ActivityClientRecord performDestroyActivity(...){
    // ...
    ActivityClientRecord r = mActivities.get(token);
    // ... 
    r.lastNonConfigurationInstances = r.activity.retainNonConfigurationInstances();
    /**
    Activity.retainNonConfigurationInstances{
        Object activity = onRetainNonConfigurationInstance(){
            @Override by ComponentActivity.onRetainNonConfigurationInstance{
                Object custom = onRetainCustomNonConfigurationInstance(); // null
                return new ComponentActivity.NonConfigurationInstances{
                    custom = null;
                    viewModelStore = viewmodelstore;
                }
            }
        }
        return {
            activity{ // from above ComponentActivity implementation.
                custom = next subclass.
                viewModelStore.
            }
            fragment // should be deprecated and implemented by FragmentActivity?.
            ...
        }
    }
    */
}
```
And followed by the `performLaunchActivity`, where the input r is the return value of `performDestoryActivity`.
```Java
private Activity performLaunchActivity(ActivityClientRecord r, Intent customIntent) {
    // ...
    activity.attach(... r.lastNonConfigurationInstances, ...);
    // update activity to new activity.
     r.activity = activity; 
}
```

In summary, the ViewModelStore is created by activity, retain and re-attached to the activity by the `ActivityThread`.
Because `ViewModelStore` and `ViewModel` objects can has a longer life-scope than `Activity`, it's dangerous to let `ViewModel` depend on `Activity`. Instead, let `Activity` use `ViewModel` as a data container for views, or communication channel between fragments.


#### Usage
`ViewModel` is the base class to define a customized ViewModel. It defines a `.onCleared` callback, which is invoked when the activity should be finished, such as `.finish` method or use click back button.

`ViewModelStore` is the `ViewModel` object container. It's created by activity during the first launch, and retain/re-attach to activity by `ActivityThread`. Its core is a hashmap to put/get viewmodel object. For example, the put method store/update viewmodel and clear the old viewmodel
```Java
final void put(String key, ViewModel viewModel) {
        ViewModel oldViewModel = mMap.put(key, viewModel);
        if (oldViewModel != null) {
            oldViewModel.onCleared();
        }
    }
```

`ViewModelProvider` is a helper layer on top of `ViewModeStore` used to create/manage `ViewModel`.


### <a id="reference">IV. References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>

