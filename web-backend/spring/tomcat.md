Created: 2020-04-22
Modified: 2020-04-22

* [ClassLoader](#classloader)
* [Logging](#logging)
* [System Property Replacement](#system-property-replacement)
* [References](#reference)
***
### <a id="classloader">I. ClassLoader</a>
Tomcat is a servlet container, it can run multiple servlets at the same time. However, a Tomcat instance only owns a JVM process, which means all the servlets run within the same JVM. 
How does tomcat make each servlet to have its runtime dependency? For example, servlet A has a runtime dependencies of class X, but servlet B cannot access X. The underlying reason is that each servlet has its own classloader. 

Classloader is the most fundamental block that JVM used to manage (load) runtime dependency. Each classloader is bound to certain `classpath`, and it can only load the dependencies in that `classpath`. The default JVM classloader is a hierachical structure, which contains three layers `bootstrap classloader`, `extension loader`, and `AppClassLoader`. 

Checkout [Java Classloader](#http://localhost:9001/programming-languages/java/system.html#classloader)

Tomcat has its own classloader implementation, by default, the structure is 

```bash
bootstrap classloader
Launcher$ExtClassloader
Launcher$AppClassloader  
# the above three are JVM vendor defined classloader. 
# Tomcat scripts, .e.g ./catalina.sh or ./daemon.sh add /bin/bootstrap.jar and /bin/tomcat-juli.jar to AppClassLoader.

URLClassloader # common
ParallelWebappClassLoader # webappX
```

When you run `./catalina.sh` and `./daemon.sh`, these scripts will add `${catalina_home}/bin/bootstrap.jar` and `${catalina_home}/bin/tomcat-juli.jar` to the JVM's AppClassLoader's classpath by setting up the environment variables `$CLASSPATH`.

The bottom two classloader
* `URLClassloader` is created by the initial tomcat code, which add the path defined as `common.loader` in the `/conf/catalina.properties`. By default, it's `${catalina_base}/lib/*`. Example of the libraries are Java EE defined `servlet-api.jar — Servlet 3.1 API`.
* `ParallelWebappClassLoader` (WebappX)
<p class="reference-box">
A class loader is created for each web application that is deployed in a single Tomcat instance. All unpacked classes and resources in the /WEB-INF/classes directory of your web application, plus classes and resources in JAR files under the /WEB-INF/lib directory of your web application, are made visible to this web application, but not to other ones.
</p>

#### The process of launching a servlet.
1. create the classloader for that servlet, set classpath to `${context-path}/WEB-INF/lib`.
2. load the servlet class defined in `web.xml` from that classloader. When the servlet.class load other classes, it will use the classloder that loads itself by default.
3. init on the servlet, and then handle requests.

#### ParallelWebappClassLoader

`org.apache.catalina.loader.ParallelWebappClassLoader` is the default implementation. It extends `WebappClassLoaderBase`, which loads "/WEB-INF/classes" and "/WEB-INF/lib".

You can control the classloader used by specifing a `<loader>` within `<context>`
```xml
<Loader className="nqqin.app.MyWebappLoader" delegate="false" />
```

### <a id="logging">II. Logging</a>
*System.out/err are redirected to `catalina.out` by the running script `catalina.sh` or `daemon.sh`.* Controlled by `$CATALINA_OUT` env variable.

The internal logging framework used by Tomcat is JULI. When you invokes `servletConfig.getServletContext().log("message");`, you actually use the Tomcat's logging.

JULI is based on `java.util.logging` but with a different log manager and provides more functionality. For example, you can 
1. have different formatters for a handler type.
2. per servlet logging and configuration.
3. By default, if the logger has its own handlers, it won't invoke parent's handler.
4. When you doing logging `servletConfig.getServletContext().log("message");`, the name of the logger used is `org.apache.catalina.core.ContainerBase.[${engine}].[${host}].[${context}]`. For example, `org.apache.catalina.core.ContainerBase.[Catalina].[localhost].[/sample]`.

The java.util.logging properties file is a subset of JULI configuration property. For example, JULI allows 
<p class="reference-box">
A prefix may be added to handler names, so that multiple handlers of a single class may be instantiated. A prefix is a String which starts with a digit, and ends with '.'. For example, 22foobar. is a valid prefix.
</p>

#### Configuration properties
`catalina.sh` or `daemon.sh` prefer to use `${catalina_base}/conf/logging.properties` as the default configuration for the global logging, if this file is not found, it will look
at the file provided by java.util.logging library.

```bash
LOGGING_CONFIG="-Djava.util.logging.config.file=$CATALINA_BASE/conf/logging.properties"
LOGGING_MANAGER="-Djava.util.logging.manager=org.apache.juli.ClassLoaderLogManager"
```


```bash
# define all the handlder will be used.
handlers = 5my.org.apache.juli.AsyncFileHandler, java.util.logging.ConsoleHandler
# configure loggers
# root logger
.handlers = java.util.logging.ConsoleHandler
# my servlet logger
org.apache.catalina.core.ContainerBase.[Catalina].[localhost].[/sample].level = INFO
org.apache.catalina.core.ContainerBase.[Catalina].[localhost].[/sample].handlers = 5sample.org.apache.juli.AsyncFileHandler

# configure the my hanlder
5sample.org.apache.juli.AsyncFileHandler.level = FINE
5sample.org.apache.juli.AsyncFileHandler.directory = ${catalina.base}/logs
5sample.org.apache.juli.AsyncFileHandler.prefix = my-project.
5sample.org.apache.juli.AsyncFileHandler.maxDays = 90
5sample.org.apache.juli.AsyncFileHandler.encoding = UTF-8
```



### <a id="reference">References</a>
1. <a href="https://tomcat.apache.org/tomcat-9.0-doc/config/loader.html" target="_blank">loader configuration</a>
2. <a href="https://tomcat.apache.org/tomcat-9.0-doc/class-loader-howto.html" target="_blank">Tomcat ClassLoader How-to</a>
3. <a href="https://tomcat.apache.org/tomcat-9.0-doc/logging.html" target="_blank">Tomcat Logging</a>
4. <a href="https://tomcat.apache.org/tomcat-9.0-doc/config/index.html" target="_blank">Apache Tomcat 9 Configuration Reference</a>