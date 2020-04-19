Created: 2020-04-18
Modified: 2020-04-18

* [Java System Class](#java-system-class)
* [Classloader](#classloader)
* [References](#reference)
***
### <a id="java-system-class">I. Java System Class</a>
<p class="reference-box">
Among the facilities provided by the System class are <br>
standard input, standard output, and error output streams;<br>
access to externally defined properties and environment variables; <br>
a means of loading files and libraries; <br>
and a utility method for quickly copying a portion of an array.
</p>

#### System Properties
The `System` class has a static method called `.getProperties()`, which returns a `Properties` object that includes both JVM (and vender) specified key-value and user specified key-value pair through `java -Dkey=value` JVM arguments.


##### JVM specified key-value pair
(JRE related)
1. java.vendor: `Oracle Corporation`
2. java.version: e.g. `1.8.0_231`, `1.8.0_141`
3. java.runtime.version: e.g. `1.8.0_231-b11`,`1.8.0_141-b16`
4. java.runtime.name: e.g. `Java(TM) SE Runtime Environment`, `OpenJDK Runtime Environment`
5. java.vm.version: 25.231-b11
6. java.vm.vendor: Oracle Corporation
7. java.vm.name: e.g. `Java HotSpot(TM) 64-Bit Server VM`, `OpenJDK 64-Bit Server VM`
8. java.vm.specification.name: `Java Virtual Machine Specification`
9. java.vm.specification.vendor: `Oracle Corporation`
10. java.vm.specification.version: `1.8`

(JRE path)
1. java.home: the path to JRE, e.g. `/Library/Java/JavaVirtualMachines/jdk1.8.0_231.jdk/Contents/Home/jre`, which includes `/bin`, `/lib`, `/Welcome.html` and so on.
2. java.library.path: the path includes all native (C/C++) libraries e.g. `/Users/nqqin/Library/Java/Extensions:/Library/Java/Extensions:/Network/Library/Java/Extensions:/System/Library/Java/Extensions:/usr/lib/java:`
3. java.class.path: this is the classpath. When you run `java -cp ...`, it will set this properties. You can also do `java -Djava.class.path` to setup the classpath. If no classpath given, it will set to `.` (the current directory). classpath can include both directory and jar.

    You can set classpath by 
    * -Djava.class.path
    * -classpath (-cp)
    * export CLASSPATH=  // environment variable

4. java.ext.dirs: used by the java Extension Mechanism Architecture

These paths are initialized at the begining when launch the JVM. The `java.class.path` classpath then will initialize the system `ClassLoader` object, which is used to load 
.jar and classes to the runtime. You can update the `java.class.path` at any time, but the `ClassLoader` is not being refreshed. So updating `java.class.path` at runtime doesn't affect the effective classpath.  

(User related)
1. user.home: user's home directory, e.g. `/Users/nqqin`
2. user.name: username, `nqqin`
3. user.language: `en`
4. user.country: `US`
5. user.dir: the current working directory (directory where you run java)
6. user.variant: 
7. user.timezone:

(OS related)
1. os.name: e.g.`Mac OS X`, `Linux` ...
2. os.version: e.g. `10.14.6`, `3.10.0-1062.1.1.el7.x86_64`
3. os.arch: e.g. `x86_64`, `amd64`



#### Enviroment variable
Environment variables are converted to a map `Map<String, String> envs = System.getenv();`

#### Others
System provides .load and .loadLibrary methods. They load __native (C/C++) library__. They cannot load .jar.


### <a id="classloader">ClassLoader</a>
When start running java, one of tasks is to initialize the system classloader. Different vendors may have different strategies.

`Oracle JVM` will invoke `sun.misc.Launcher` to initialize the classloader.
```java
public class Launcher{
    public Launcher(){
        Launcher.ExtClassLoader var1 = Launcher.ExtClassLoader.getExtClassLoader(){
            URL[] var1 = System.getProperties("java.ext.dirs"); // pusedo code.
            return new Launcher.ExtClassLoader(var1); // a subclass of URLClassLoader
        }
        ClassLoader applicationLoader = Launcher.AppClassLoader.getAppClassLoader(var1){
            URL[] urls = System.getProperties("java.class.path");
            return new Launcher.AppClassLoader(urls, var0); // a subclass of URLClassLoader
        }
    }
}

```
The `java.lang.ClassLoader` is an abstract base class for loading java binary at runtime. It's bound to some native methods, for example,
* `private native Class<?> defineClass1(String name, byte[] b, int off, int len, ProtectionDomain pd, String source);`, the byte[] contains the bytecode of .class file, this native method convert the bytecode to a Class object.
* `private native Class<?> findBootstrapClass(String name);`, which invoke the root bootstrap class loader to load classes.
* `private native final Class<?> findLoadedClass0(String name);`, a native cache contains loaded class.

#### ClassLoader structure and hierarchy
A classloader usually has a parent classloader (`Launcher.ExtClassLoader` doesn't have one). When you invoking .loadClass, classloader first uses its parent to load classes, if the parent doesn't find, then the classload find class by its own.

```java
public class URLClassLoader{
    protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        Class<?> c = findLoadedClass(name); // find from native cache if already loaded.
        if(c != null){
            return c;
        }
        if(parent != null){
            c = parent.loadClass(name, false);
        }else{
            c = findBootstrapClassOrNull(name); // The only classloader that doesn't have parent classloader is Launcher.ExtClassLoader, which use the native code to load class.
        }
        if(c != null){
            return c;
        }
        c = findClass(name); // find class by the class itself. depends on subclass implementation.
        return c;
    }
}
```
The real structure will is
```java
{findBootstrapClass} // load class from rt.jar, which defines java.lang.String and others basic class. 
Launcher.ExtClassLoader // load from `java.ext.dirs` properties.
Launcher.AppClassLoader // load from classpath.
```


URLClassLoader is commonly used classloader. It defined the findClass method that find class definition according to the classname and a list of urls. And the urls can be modified at runtime, which means you can dynamically load class definition at runtime.
```java
public class URLClassLoader extends SecureClassLoader{
    public class SecureClassLoader extends ClassLoader {

    }
    protected Class<?> findClass(final String name){
        // name is in form of "nqqin.app.loader.MyClassLoader", it will convert it to nqqin/app/loader/MyClassLoader and then find it
        // against a list of URLs (Launcher.AppClassLoader set this URLs to be classpath.) and then read the bytes from the .class
        // using the native `defineClass1` to convert bytes to Class object.
    }
}
```

#### Dynamically load class & Write your own classloader

`ClassLoader.getSystemClassLoader();` return the application classloader (Launcher.AppClassLoader), which inherits the functions of URLClassLoader. So you can add new path to this classloader and load class at runtime. However, the `addURL(URL url)` method is protected in the `URLClassLoader`, so you have to either use reflection or create a subclass.

Reflection
```java
public static void addURL(URL u) throws IOException {
    Class[] parameters = new Class[]{URL.class};
    Method method = URLClassLoader.class.getDeclaredMethod("addURL", parameters);
    method.setAccessible(true);
    method.invoke(URLClassLoader) ClassLoader.getSystemClassLoader();, new Object[]{u});
}
```

Subclass Example
```java
class DynamicClassLoader extends URLClassLoader{
    private static DynamicClassLoader loader;
    protected DynamicClassLoader(ClassLoader parent) {
        super(new URL[0], parent);
    }

    public void addPath(String path) throws MalformedURLException {
        File file = new File(path);
        super.addURL(file.toURI().toURL());
    }

    public static DynamicClassLoader getClassLoader(){
        if(loader != null){
            return loader;
        }
        loader = new DynamicClassLoader(ClassLoader.getSystemClassLoader());
        return loader;
    }
}
```

In addition, each Class object also has a classloader property that binds to the classloader that loads this class.

### <a id="reference">References</a>
1. <a href="https://docs.oracle.com/javase/7/docs/api/java/lang/System.html#getProperties()" target="_blank">System public static Properties getProperties() API</a>