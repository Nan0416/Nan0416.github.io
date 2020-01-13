# Markdown

Created: 2020-01-12
Modified: 2020-01-12

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

### <a id="callback">III. ViewModel</a>

### <a id="reference">IV. References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>