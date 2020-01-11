# Markdown

Created: 2020-01-04
Modified: 2020-01-10

* [Introduction](#intro)
* [Request Perimssion](#request)
* [Example](#example)
* [Custom Permission](#custom)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>

#### Permission
Permission includes accessing hardware (e.g. Bluetooth, Camera), sensitive user data (contacts, SMS), software (custom permissions).

A component (software) can declare its own permission within the `<activity>` `android:permission` attribute. If another component wants to launch this component, it must has the permission.

Permission is a application-scope feature, meaning permission is granted based on the application (instead of activity, or other component). If an application has the permission, then all components of the application will have the permission.

##### Protection Levels
Android divides permissions into 3 protection levels, which affects whether runtime permission requests are required.

1. Normal Permission: access data or resources outside the app's sandbox but it's very little risk to user's privacy. These permissions are automatically granted by Android without notifying user at the install time.
2. Signature Permission: access another app. Suppose A wants to access B. A and B are signed by the same certificate, Android will grant permission at the install time without notifying user.
3. Dangerous Permission: access data or resources that involve user's private information/operation of another app. The request of dangerous permission will prompt to the user at runtime.

Examples, (reference)[https://developer.android.com/reference/android/Manifest.permission]

* Normal Permission
    * android.permission.INTERNET
    * android.permission.KILL_BACKGROUND_PROCESSES
    * android.permission.MODIFY_AUDIO_SETTINGS: allow modify global audio settings.

* Dangerous Perimssion
    * android.permission.ACCESS_FINE_LOCATION: access accurate location
    * android.permission.ACCESS_COARSE_LOCATION
    * android.permission.ACCESS_BACKGROUND_LOCATION: access location in background.

##### Permission Group
Android groups related individual permissions into permission group. When asking an individual dangerous permission, android will ask users for a permission group. Once the user grant the group, all permissions within the group will be automatically granted later.
For example, app asks READ_SMS permission, Android will ask the user SMS group perimssion, which includes both READ_SMS and RECEIVE_SMS. If the group is granted, Android will grant RECEIVE_SMS permission automatically if the app ask later. 

### <a id="request">II. Request Permission</a>

From Android Marshmallow 6.0 (API level 23), permission are requested at runtime (like iOS). All requested permission are declared within the manifest's uses-permission tag.

```XML
<manifest>
    <uses-permission android:name="android.permission.SEND_SMS">
    <uses-permission android:name="android.permission.INTERNET"/>
</manifest>
```

##### uses-feature
`<uses-feature>` is another tag within the manifest. It's a facilitated tag that help to determine if an app can run at a device. For example, declaring 
`<uses-feature android:name="android.hardware.bluetooth" android:required="true"/>` indicates the app will not run at device that doesn't have bluetooth.
The feature can be software or hardware. [features-references](https://developer.android.com/guide/topics/manifest/uses-feature-element#features-reference)

This tag is informational only, meaning Android itself won't check it. Instead, another software may help to check it, e.g. Google Play will check it before installation.

*uses-permission will automatically ask the corresponding features*. For example, if you required `Bluetooth` permission, then it implies you require the `Bluetooth` feature. However, old android system (api 5) support the `Bluetooth` api but doesn't declare the `Bluetooth` feature, which means even though the device has bluetooth hardware and support bluetooth api,
Google Play won't install this app to that device since its api doesn't declare this feature. In order to avoid this circumstance, mark the `Bluetooth` required as false.

So if you don't need to have this permission, do
```XML
<uses-feature android:name="android.hardware.bluetooth" android:required="false" />
```

[reference](https://developer.android.com/guide/topics/manifest/uses-feature-element#permissions)


### <a id="example">III. Example</a>

Getting location service. Declare the location permission in manifest. <span style="color:red">For api level below 23, the permission is request at installation time. For api level >= 23, the permission is request at runtime by code explicitly.</span>
```XML
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"/>
```

#### Practice
1. Before using the location service api, check if the app has the permission.
```Java
if(context.checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_DENIED){
    //...
}
```
<span style="color:red">The best practice should use the AndroidX compat library instead of directly using Framework API.</span>

2. Handling denied situation.
Two situations can result in "denied"
* This is the first time try to ask permission. System denies all dangerous permission by default.
* The user has denied this services before.

Using `activity.shouldShowRequestPermissionRationale(Manifest.permission.ACCESS_FINE_LOCATION)` to distinguish the two situation.
If it is the second situaition, the app usually should explain why requiring the permission.

3. Asking permission

`activity.requestPermissions(String[] permissions, int requestCode);` This api requests a set of permissions at once along with app-defined requestCode, which will be 
used in the callback function. <span style="color:red">Android will prompt the request to user in terms of permission group instead of each individual permission.</span>
For example, asking either `ACCESS_FINE_LOCATION` or `ACCESS_COAESE_LOCATION` will result in "Allow xxx to access this device's location?"

4. Two situation
* User grants the permission: the app can use service api without permission exception. And following asking permission in the same group will grant automatically. But the app still need to ask permission whenever before using the service since 
user can revoke the permission at anytime.
* User denied the permission: the app usually need to explain why and ask again.

Code get notified with a callback function defines in the `Activity` class. The default implementation is empty.
```Java
    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permission, int [] grantResults){
        // the requestCode the app-defined code.
    }
```
### <a id="custom">IV.Custom Permission</a>

TODO

### <a id="reference">V. References</a>
1. <a href="https://developer.android.com/training/permissions/requesting" target="_blank">Request App Permissions</a>