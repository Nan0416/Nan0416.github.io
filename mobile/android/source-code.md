
Activity's setContentView

an activity has a `Window` object （`PhoneWindow` concrete class), which defines the setContentView
In addition, the `PhoneWindow` has a `mDecor` field of `ViewGroup` type.

PhoneWindow.SetContentView{
    installDecor{
        1. check if the mDecor is null. If it is null, then `mDecor = new DecorView()`.
        // DecorView is a subclass of Framelayout, which is a subclass of ViewGroup.
        2. generate a content view group (`FrameLayout`) inside the mDecor,
    }
    inflate layout to content
    {
        1. based on the XML layout, generate the top viewgroup, .e.g (a constraint layout).
        2. rInflateChildren(xmlParser, topView, attrs, true); // add the XML layout's child view to the top viewgroup

    }
}

ActivityThread{
    Activity{
        PhoneWindow extends Window{
            WindowManageImp implements WindowManager{
                WindowManagerGlobal mGlobal{
                    ViewRootImp root;
                }
            }
        }
    }      
}

Android
ZygoteInit.main{
    
}


performLaunchActivity(ActivityClientRecord r, Intent customIntent)
{
    ComponentName component = r.intent.getComponent();
    ContextImpl appContext = createBaseContextForActivity(r);
    ClassLoader c1 = appContext.getClassLoader();
    Activity activity = mInstrumentation.newActivty(c1, component.getClassName(), r.intent)
    {
        c1.loadClass(className).newInstance();
    }
    appContext.setOuterContext(activity);
    activity.attach(appContext, this[activityThread], mInstructmentation)
}