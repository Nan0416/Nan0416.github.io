Created: 2020-04-16
Modified: 2020-04-16

* [java.util.logging](#java.util.logging)
* [log4j](#log4j)
* [References](#reference)
***
### <a id="java.util.logging">I. java.util.logging</a>
java.util.logging (Java Logging API) is a JDK provided java logging library (released with JDK 1.4, 2002). It composes a 
* LogManager: A application wide singleton object that manages a set of logger instance in a hierachical structure.
* Logger: a logger is a container of filters and handlers. When a use doing log.info("msg"), the message will go through each filters, and then decide if it show be logged according to logging level, if yes, the message will go through each handler, and may also pass to the parent logger.
* Filter: determine if we should log the message. Both logger and handler can have at most one filter.
* Handler: a handler is an output location (e.g. console, or file), it has a formatter, a log level and an optional filter;
* Formatter: formatter.
* log level: OFF, SEVERE, WARNING, INFO, CONFIG, FINE, FINER, FINEST, ALL. Each log message is associated with log level, each logger and hanlder also have a log level.
When the log message level log (WARNING) is greater than or equal to logger & handler log level (INFO), the message is logged.

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


### <a id="log4j">II. Log4j</a>

Log4j came before java.util.logging api, it was release in 2001 by Apache Software Foundation. java.util.logging has the same logging hierarchy as Log4j.

Log4j was not supported since 2015, the last version of Log4j is 1.2. Its successor is Log4j 2.

* Logger, Appenders (Handlers) and Layouts (Formatter) <span style="color:red">It doesn't have filters</span>
* LogManager: has a repository that maintain the structures of loggers. The default implementation is `Hierarchy`.
* A child logger, by default, inherits its parent's appenders. (Achieved by the hierarcial structure.)
* Different from java.util.logging's handlers, appenders don't have log level control.

#### Create logger precedure.
```java
LogManager{
    preform initialization and configuration // there is no configuration by default. And configuration is against the repository.
    {
        repositorySelector = new DefaultRepositorySelector(new Hierarchy(new RootLogger((Level) Level.DEBUG)););
        String override = System.getProperty("log4j.defaultInitOverride");
        if(override != null && !"false".equalsIgnoreCase(override)){
            System.out.println("Override")
        }else{
            String configurationOptionStr = System.getProperty("log4j.configuration");
            String configuratorClassName = System.getProperty("log4j.configuratorClass");
            URL url = null;
            if(configurationOptionStr == null) {	
	            url = Loader.getResource("log4j.xml") or Loader.getResource("log4j.properties"); // if no option string, then one of this two file.
            } else {
                url = new URL(configurationOptionStr) or Loader.getResource(configurationOptionStr);  // loading from option string
            }
            if(url == null){
                System.out.println("could not find resource");
            }
            OptionConverter.selectAndConfigure(url, configuratorClassName, LogManager.getLoggerRepository()){
                // use a configurator, org.apache.log4j.xml.DOMConfigurator, org.apache.log4j.PropertyConfigurator, or the input argument.
                // to do
                configurator.doConfigure(url, repository);
            }
        }
    }
    RepositorySelector{
        LoggerRepository (interface){
            // a repository has a set of logger and manage their relationship.
            
        }   
    }
}
class Hierarchy implements LoggerRepository{
    //loggers are arranged in a named hierarchy.
    //children link to their parent but parents do not have any pointers to their children.

    LoggerFactory defaultFactory = new DefaultCategoryFactory(); //  Logger makeNewLoggerInstance(String name) { return new Logger(name); } 
    Hashtable ht = new Hashtable(); // name - logger

    public Logger getLogger(String name, LoggerFactory factory){
        Object o = ht.get(name);
        if(o != null && o instanceof Logger){
            return (Logger) o;
        }else if(o == null){
            Logger logger = factory.makeNewLoggerInstance(name);
            logger.setHierarchy(this);
            ht.put(name, logger);
            updateParents(logger); // maintain hierarchy
            return logger;
        }else{
            // ...
        }
    }
}
public class Logger extends Category{
    public class Category implements AppenderAttachable{ // Category is deprecated.
        public interface AppenderAttachable{

        }
    }
}
```

#### logging precedure.
A logger 
```java
logger.info(message){
    if(repo.level > INFO || this.level > INFO){ // if the (global) repo or the logger's level > INFO, then ignore.
        return;
    }
    logger = this;
    while(logger != null){
        logger.append(message);
        if(!logger.additive) { // additive control inherit.
            break;
        }
        logger = logger.parent;
    }
}
```

#### Configuration
By default, there is not configuration. You have a root logger but without any appenders. The easiest way is calling `BasicConfigurator.configure();` which does
```java
Logger.getRootLogger()
    .addAppender(new ConsoleAppender(
           new PatternLayout(PatternLayout.TTCC_CONVERSION_PATTERN)));
```

log4j looks the `-Dlog4j.configuration` system property, if set, loading its from URL or classpath,
otherwise try to loading `log4j.xml` and `log4j.properties` files from __classpath__.

* For .properties file, it uses `org.apache.log4j.PropertyConfigurator`
* For .xml file, it uses `org.apache.log4j.xml.DOMConfigurator`

##### .properties (PropertyConfigurator)

loggerName = LEVEL, appender-name-1, appender-name-2
log4j.appender.appender-name-1= org.apache.log4j.ConsoleAppender # class
...

### <a id="reference">References</a>
1. <a href="http://tutorials.jenkov.com/java-logging/index.html" target="_blank">Java Logging</a>
2. <a href="https://logging.apache.org/log4j/1.2/manual.html" target="_blank">Log4j 1.2</a>