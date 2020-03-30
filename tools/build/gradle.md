Created: 2020-03-29
Modified: 2020-03-29

* [Introduction](#intro)
* [Project](#project)
* [Task](#task)
* [Plugin](#plugin)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>
Gradle is a build system. (The target projects can be in any programming language.)

It is the 3rd major build tool in the java ecosystem (Ant, Maven). Initial released in 2004. The latest gradle version is 6.3 (2020-03-29).

Gradle itself is written in Java, and groovy script can be translate to java instruction.

#### Pros
* Use Groovy or Kotlin DSL to write build script, more expressive than XML.
* Have the same concept of tasks (Ant's target). 
* Support plugins.
* Gradle wrapper, a wrapper + .jar: you don't need to install Gradle onto your system to run gradle.

#### File structure

Gradle defines a default file structure as following:

```
src
    main
        java
        resource
    test
        java
        resource
build.gradle // the build script
settings.gradle
```

The gradle command line is `gradle task`. When running gradle, the first thing is to initiate the settings object from the `setting.gradle`, and then initiate the project object from the build.gradle script. The scope of the build.gradle is mapped into a project object.

```
// Settings Object: org.gradle.api.initialization.Settings
@HasInternalProtocol
public interface Settings extends PluginAware, ExtensionAware
    ProjectDescriptor getRootProject();
    File getRootDir();
    // ...
}
```
It can be mapped into the following settins.gradle (<span style="color:red">ToDo: How does the mapping work?</span>)
```
// the script scope is inside the Settings object
rootProject.name = 'SpringFrameworkWebApplication'
println rootDir.path
```

```
// Project object is created according to the Settings object
@HasInternalProtocol
public interface Project extends Comparable<Project>, ExtensionAware, PluginAware {
    Project getRootProject();
    File getRootDir();
    File getBuildDir();
    String getName();

    void buildscript(Closure configureClosure);
    void repositories(Closure configureClosure);
}
```
The `PluginAware` interface enable project to apply plugin
```
public interface PluginAware {
    void apply(Map<String, ?> options);
}
```

### <a id="project">II. Project</a>
A project object has a few important handler to handle different configurations.
1. `RepositoryHandler` manages a set of repositiories, allowing repo to be defined and queried. e.g. mavencentral(), mavenlocal()
2. `DependencyHandler` used to declare dependencies. Dependencies are put into different configuration and then used at different time, e.g. compile.
```
dependencies {
     configurationName dependencyNotation
 }
``` 
3. `ConfigurationHandler`
4. `ArtifactHandler`

### <a id="task">III. Task</a>
A task is a collection of actions.
* simple task: defines a list of actions.
* enhance task: inherit from another Task class, which may define some dedicated functions.

#### task interface
`task` is a member function defined in `Project` class. By applying task function, you can add tasks to the project.
```
public interface Project extends Comparable<Project>, ExtensionAware, PluginAware {
    Task task(String name) throws InvalidUserDataException;
    Task task(Map<String, ?> args, String name) throws InvalidUserDataException;
    Task task(Map<String, ?> args, String name, Closure configureClosure);
    Task task(String name, Closure configureClosure);
}
```

##### Closure
A closure is list of actions/statements. In Gradle syntax, they are group by {}, and the task object is also passed into the {}.
For example, the standard syntax is
```
task("task-name", (type:Copy, dependsOn: "another-task"), {
    println("this is task-name")
})
```
* If the closure is the last argument, it can be put outside.
* Groovy can ignore () if it has at least one argument.
* Map is in form of (key-1:value-1, key-2:value-2)

So the above task definition can be written as
```
task taskname (type:Copy, dependsOn: "another-task") {
    println("this is task-name)
}
```

##### enhanced task

An enhanced task is a task inherit from anothe Task Class. When you use `task name {}`, you actually define a task instance.

Task Class define a few action that you can supply argument to them. For example, `Copy` task class defines `from` and `into` so that you can copy files.
```
task copyExample (type: Copy) {
    from "source/file.txt"
    to "destination"
}
```

### <a id="plugin">IV. Plugin</a>
Plugin allows to reuse tasks. You can download others plugin, which may define the tasks you need, into your build project.

#### Example of defining a plugin in Java.
A plugin can contains tasks, extension and configuration. (extension is like the configuration of tasks).

```
package nqqin.app.gradleplugin;

import org.gradle.api.Plugin;
import org.gradle.api.Project;

public class DemoPlugin implements Plugin<Project> {
    @Override
    public void apply(Project project) {
        project.getExtensions().create("demoSetting", DemoPluginExtension.class); // add an extension instance.
        project.getTasks().create("demo", DemoTask.class); // add an task instance.
    }
}
```

A plugin should implements `Plugin<Project>`, and the `apply` is the entry point of the 3rd plugin. When the user writes `apply plugin org.plugin.name`, the `apply` method is invoked.

##### Extension and Task
```
//Extension
package nqqin.app.gradleplugin;
public class DemoPluginExtension {
    private int repeat = 10;
    public int getRepeat() { return repeat; }
    public void setRepeat(int repeat) { this.repeat = repeat; }
}
```

```
// Task
package nqqin.app.gradleplugin;
import org.gradle.api.DefaultTask;
import org.gradle.api.tasks.TaskAction;

public class DemoTask extends DefaultTask {
    private String message = "what";

    public void setMessage(String message){
        this.message = message;
    }
    @TaskAction
    public void greet() {
        DemoPluginExtension extension = getProject().getExtensions().findByType(DemoPluginExtension.class); // get extension and find values.
        if (extension == null) {
            extension = new DemoPluginExtension();
        }

        @TaskAction
        public void greet() {
            DemoPluginExtension extension = getProject().getExtensions().findByType(DemoPluginExtension.class);
            if (extension == null) {
                extension = new DemoPluginExtension();
            }
            for(int i = 0; i < extension.getRepeat(); i++){
                System.out.println(message);
            }
        }
    }
}
```

The above defines a `Task Class`. It can have its own properties and can also get configuration from extension.
The `@TaskAction` method will be execution when use invoke the task and the tasks that inherit from this class.
##### Test
See reference

##### Build
Of course, the plugin should have a name.

Create a file `plugin-name.properties` under the directory `/resources/META-INF.gradle-plugins`.
Inside the file, indicate the implementation class as an example
`/resources/META-INF.gradle-plugins/myplugin.properties`
```
implementation-class=nqqin.app.gradleplugin.DemoPlugin
```
The best practice of naming plugins is to use java package style to avoid name conflict.

###### Build
You can build the plugin and install it into local maven repo using the following gradle script.
```
plugins {
    id 'java'
    id 'groovy'
    id 'maven'
}

group 'qinnan-software'
version '1.0-SNAPSHOT'

sourceCompatibility = 1.8

repositories {
    mavenCentral()
    mavenLocal()
}

dependencies {
    compile gradleApi()
    testCompile group: 'junit', name: 'junit', version: '4.12'
}
```
run `gradle build install`

#### Apply this plugin
1. To apply a 3rd party plugin, you should first instruct gradle to download the plugin. So
```
buildscript {
    repositories {
        mavenLocal()
    }
    dependencies {
        classpath 'qinnan-software:GradlePlugin:1.0-SNAPSHOT' // group:artifact(project):version
    }
}
```
2. And then you can apply the plugin with `apply plugin: "myplugin"`. It is not allowed to apply 3rd party plugin with `plugins {id xxxx}`.
3. You may need to change default extension setting
```
demoSetting { // extension instance name.
    repeat = 2
}
```
4. If you create a task instance in the plugin, then you invoke with `gradle demo`. Or you can create an enhanced task to inherit the Task class.
```
task mydemo(type: nqqin.app.gradleplugin.DemoTask){
    message = "qinnan" // override the task's property
}
```


### <a id="reference">V. References</a>
1. <a href="http://www.thinkcode.se/blog/2015/03/22/a-gradle-plugin-written-in-java" target="_blank">A Gradle plugin written in Java</a>