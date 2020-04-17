Created: 2020-04-16
Modified: 2020-04-16

* [java.util.logging](#java.util.logging)
* [References](#reference)
***
### <a id="java.util.logging">java.util.logging</a>
java.util.logging (Java Logging API) is a JDK provided java logging library. It composes a 
* LogManager: A application wide singleton object that manages a set of logger instance in a hierachical structure.
* Logger: a logger is a container of filters and handlers. When a use doing log.info("msg"), the message will go through each filters, and then decide if it show be logged according to logging level, if yes, the message will go through each handler, and may also pass to the parent logger.
* Filter: determine if we should log the message. Both logger and handler can have at most one filter.
* Handler: a handler is an output location (e.g. console, or file), it has a formatter, a log level and an optional filter;
* Formatter: formatter.


When you do `Logger.getLogger("nqqin.app")`, the getLogger will first to check if the LogManager is initialized. If not, the LogManager will be initialized first.
You can also execute `LogManager manager = LogManager.getLogManager();` to explicitly initialize the LogManager.
* Initialize LogManager: LogManager instance is app wide (JVM-wide) singleton object.
    1. Initalize the LogManager as following
    ```java
    public class LogManager{
        static {
            cname = System.getProperty("java.util.logging.manager");
            if(cname != null){
                this.manager = (LogManager)ClassLoader.getSystemClassLoader().loadClass(cname);
            } else {
                this.manager = new LogManager();   
            }
        }
    }
    ```
    , which means you can have your own `LogManager` by supplying the system property.

    2. Read configuration, Update existing logger's log level, notify listeners.
    ```java
    public void readConfiguration(){
        String cname = System.getProperty("java.util.logging.config.class");
        if(cname != null){
            Class<?> clz = ClassLoader.getSystemClassLoader().loadClass(cname);
            clz.newInstance(); // the constructor of the class will initialize the LogManager.
            return;
        }
        String fname = System.getProperty("java.util.logging.config.file");
        if(fname == null){
            fname = System.getProperty("java.home"); // read from $JAVA_HOME/jre/lib/logging.properties, a default configuration provided by JRE.
            File f = new File(fname, "lib");
            f = new File(f, "logging.properties");
            fname = f.getCanonicalPath();
        }
        readConfiguration(new BufferedInputStream(new FileInputStream(fname)));
    }
    ```

    Example of Configuration Class (does the same thing) (Configuration class is not very useful, since you sill need to invoke `.readConfiguration(InputStream in)`, but it may be useful when you do configuration over network since you can put the network input stream to the function);
    ```java
    public class LogConfiguration {
        public LogConfiguration() throws IOException {
            // does the same thing, read default configuration.
            LogManager logManager = LogManager.getLogManager();
            String fname = System.getProperty("java.home");
            File f = new File(fname, "lib");
            f = new File(f, "logging.properties");
            fname = f.getCanonicalPath();
            logManager.readConfiguration(new BufferedInputStream(new FileInputStream(fname)));
        }
    }
    ```
    * Configuration properties. The `readConfiguration(InputStream in)` convert input bytes to key-value pairs (as shown in the properties files)
        * the readConfiguration will update __existing__ logger to the new log level `loggername.level = INFO`,
        * notify event listen of this configuration update event, listener should implement `java.beans.PropertyChangeListener`
    
    3. Create the root logger. The RootLogger is a private subclass of Logger class, the only difference is RootLogger try to initalize its handler from the 
    configuration's handlers and find the handler's WARNING. (Where sets the formatter? The formatter is set by the handler's constructor)

    4. Add the root logger to the hierarchy (log manager's context)

#### Configuration
```bash
# the handlers field is only used by root logger, root logger will have the following handler.
handlers =  java.util.logging.ConsoleHandler, otherHandlerClass

# config each logger's handlers.
loggername.handlers = handler-class, handler-class, ...

# handler's level and format
java.util.logging.ConsoleHandler.level = INFO
java.util.logging.ConsoleHandler.formatter = java.util.logging.XMLFormatter
```

__Logger__ is not aware of configuration, the handler setting is configured during `LogManager.addLogger` (so if you directly new a Logger, the configuration file won't be applied). But __Handler__ is aware of configuration's formatter, level and filter, and they also have default value (e.g. ConsoleHandler's default level is INFO, without filter, with SimpleFormatter)

#### Hierachical structure

Suppose we have 3 loggers, "" root logger, "nqqin.app", and "nqqin.app.log", then "" is the direct parent of "nqqin.app" and "nqqin.app" is the direct parent of "nqqin.app.log".
```
              "" - [verify log level] --> [filter] --> [ConsoleHandler <SimpleFormatter>]
                                                            |
        "nqqin" --> [verify log level] --> [filter] --> [handlers [log level + filter + formatter]]
                                                            |
"nqqin.app.log" --> [verify log level] --> [filter] --> [handlers [log level + filter + formatter]]
```

When you do `Logger.getLogger("nqqin.app.log").log("message")`, the message may also be passed into the parent logger.
```java
logger.log(level, msg){
    if(this.level > level){
        return;
    }
    if(this.filter != null && !this.filter.isLoggable(leve, msg)){
        return;
    }
    Logger logger = this;
    while(logger != null){
        for(Handler handler: logger.getHandlers()){
            handler.publish(level, msg);
        }
        if(!logger.getUserParentHandlers()){
            break;
        }
        logger = logger.getParent();
    }
}
```

```java
handler.publish(level, msg){
    if(this.level > level) {
        return;
    } 
    if(this.filter != null && !this.filter.isLoggable(leve, msg)){
        return;
    }
    msg = getFormatter().format(msg);
    write(msg);
}
```


#### Example
Let only the root logger have a file handler, all loggers can send data to a single place.

```java
// logging.properties
// handlers = java.util.logging.FileHandler
// java.util.logging.FileHandler.formatter = java.util.logging.SimpleFormatter 
public class JavaLog{
    private static final Logger logger = Logger.getLogger(JavaLogger.class.getName());
    public static void main(String[] args) {
        logger.info("Message");
    }
}
```
The FileHandler has a output can be configured by a pattern defined in the configuration. e.g. %h/java%u.log to ${userhome}/java0.log

### <a id="reference">References</a>
1. <a href="http://tutorials.jenkov.com/java-logging/index.html" target="_blank">Java Logging</a>