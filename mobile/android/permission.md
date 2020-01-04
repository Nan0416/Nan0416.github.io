# Markdown

Created: 2020-01-04
Modified: 2020-01-04

* [Introduction](#intro)
* [Request Perimssion](#request)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>

#### Permission
Permission includes accessing hardware (e.g. Bluetooth, Camera), sensitive user data (contacts, SMS), software (custom permissions).

A component (software) can declare its own permission within the `<activity>` `android:permission` attribute. If another component wants to launch this component, it must has the permission.
##### Protection Levels
Android divides permissions into 3 protection levels, which affects whether runtime permission requests are required.

1. Normal Permission: access data or resources outside the app's sandbox but it's very little risk to user's privacy. These permissions are automatically granted by Android without notifying user at the install time.
2. Signature Permission: access another app. Suppose A wants to access B. A and B are signed by the same certificate, Android will grant permission at the install time without notifying user.
3. Dangerous Permission: access data or resources that involve user's private information/operation of another app. The request of dangerous permission will prompt to the user at runtime.

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

*uses-feature will automatically ask the corresponding permission*.



### <a id="reference">References</a>
