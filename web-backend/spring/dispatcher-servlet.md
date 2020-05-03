Created: 2020-04-25
Modified: 2020-04-26

* [Web Application Context](#intro)
* [DispatcherServlet](#dispatcher-servlet)
* [References](#reference)
***
### <a id="intro">I. Web Application Context</a>
The FrameworkServlet provides a `XmlWebApplicationContext` as the default the app context if not specifies in the init-param. Any Web enabled application context actually doesn't register extra beans by default. The extra things they do are to support `getServletContext/Config` and `namespace`, override environment, and also extra bean factory scopes. The extra beans are registered in other forms.

#### Context for Web App
Spring defines an interface `WebApplicationContext` that defines the extra functions should be included with a web app enabled context, such as knowing `ServletContext`. `ConfigurableWebApplicationContext` futher extends its functions by adding `namespace`, `servlet-config`, and `config location`.

```java
public interface WebApplicationContext extends ApplicationContext{
    String ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE = WebApplicationContext.class.getName() + ".ROOT";
    String SCOPE_REQUEST = "request"; // the IoC container should support extra scope
    String SCOPE_SESSION = "session";
    String SCOPE_APPLICATION = "application";
    String SERVLET_CONTEXT_BEAN_NAME = "servletContext"; // the beanName of the servletContext.
    String CONTEXT_PARAMETERS_BEAN_NAME = "contextParameters"; // the beanName of the context-init parameters
    String CONTEXT_ATTRIBUTES_BEAN_NAME = "contextAttributes";
    ServletContext getServletContext();
}
public interface ConfigurableWebApplicationContext extends WebApplicationContext, ConfigurableApplicationContext {
    void setConfigLocations(String... configLocations);
    void setServletConfig(@Nullable ServletConfig servletConfig);
    void setServletContext(@Nullable ServletContext servletContext);
    void setNamespace(@Nullable String namespace);
}
```

`AbstractRefreshableWebApplicationContext` is a reusable abstract web application context, the only thing missing is it doesn't register any beans. It leaves to subclass to implement `loadBeanDefinitions`

It also 
1. overrides the environment by using `StandardServletEnvironment`, which add context-param and init-param to the environment.
2. implement `AbstractApplicationContext` function hook `postProcessBeanFactory`

```java
	protected void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) {
		// support ServletContextAware or ServletConfigAware interface, the context/config will be set.
		beanFactory.addBeanPostProcessor(new ServletContextAwareProcessor(this.servletContext, this.servletConfig));
		beanFactory.ignoreDependencyInterface(ServletContextAware.class);
		beanFactory.ignoreDependencyInterface(ServletConfigAware.class);

		// register the request & session scope.
		WebApplicationContextUtils.registerWebApplicationScopes(beanFactory, this.servletContext);
		WebApplicationContextUtils.registerEnvironmentBeans(beanFactory, this.servletContext, this.servletConfig);
	}
```

`XmlWebApplicationContext` has a default configuration locations if you don't explicitly give one. The default configuration is `/WEB-INF/${namespace}.xml` or `/WEB-INF/applicationContext.xml` if you don't have a namespace. But the `FrameworkServlet` always setup its namespace to the app context. If the namespace of the `FrameworkServlet` is not set in the web.xml, by default, namespace = `{servlet-name}-servlet`. Therefore, if you set nothing, the `XmlWebApplicationContext` will look for a file named as `/WEB-INF/${servlet-name}-servlet.xml`.

#### Where does web/http related beans registered?

Different configuration sources have different forms to register web/http related beans.
For example, a Xml based web application's xml configuration usually contains

```xml
<mvc:annotation-driven>
    <mvc:message-converters>
        <bean class="org.springframework.http.converter.StringHttpMessageConverter"/>
        <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter"/>
    </mvc:message-converters>
</mvc:annotation-driven>
```

And the `mvc:annotation-driven` element is handler by MvcNamespaceHandler's AnnotationDrivenParser, which does register a group of beans for handling Http specific requests. For example, 
`RequestMappingHandlerMapping` helps finding the right controller method by reading `@RequestMapping` annotation.


### <a id="dispatcher-servlet">II. DispatcherServlet</a>

DispatcherServlet is on top of FrameworkServlet, which means it already have the default application context (IoC). The main purpose of `DispatcherServlet` is to handle HttpRequests.

DispatcherServlet (DS) handles http requests by delegating them to different Controller method, the binding between request and controller's method is done with `HandlerAdapter`. The `HanlderAdapter` can convert http message to controller method's input, and convert method returned object to http response (e.g. json).

In addition, DS also needs `HandlerMapping`s to mapping request to proper adapters.

#### How does DS find these helper (handler mapping/adapter) objects.
DS has a `initStrategies()` methods that resolve different helpers from the bean factory.

```java
public class DispatcherServlet extends FrameworkServlet {
    private static final String DEFAULT_STRATEGIES_PATH = "DispatcherServlet.properties";
    private static final Properties defaultStrategies; // parsed from the above file, used when default strategies are empty.

    private List<HandlerMapping> handlerMappings;
    private boolean detectAllHandlerMappings = true; // include HandlerMapping from parent's bean factory.
    
    static {
		ClassPathResource resource = new ClassPathResource(DEFAULT_STRATEGIES_PATH, DispatcherServlet.class);
		defaultStrategies = PropertiesLoaderUtils.loadProperties(resource);
	}
    protected void initStrategies(ApplicationContext context) {
		initMultipartResolver(context);
		initLocaleResolver(context);
		initThemeResolver(context);
		initHandlerMappings(context); // get all `HandlerMapping` types from the bean factory, you can control if include the Handler mapping from parent.
		initHandlerAdapters(context);
		initHandlerExceptionResolvers(context);
		initRequestToViewNameTranslator(context);
		initViewResolvers(context);
		initFlashMapManager(context);
	}
```

The default strategy file is in spring-webmvc-xxx.jar's org.springframework.web.servlet package.

```bash
org.springframework.web.servlet.LocaleResolver=org.springframework.web.servlet.i18n.AcceptHeaderLocaleResolver
org.springframework.web.servlet.ThemeResolver=org.springframework.web.servlet.theme.FixedThemeResolver

org.springframework.web.servlet.HandlerMapping=org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping,\
	org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping,\
	org.springframework.web.servlet.function.support.RouterFunctionMapping

org.springframework.web.servlet.HandlerAdapter=org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter,\
	org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter,\
	org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter,\
	org.springframework.web.servlet.function.support.HandlerFunctionAdapter

org.springframework.web.servlet.HandlerExceptionResolver=org.springframework.web.servlet.mvc.method.annotation.ExceptionHandlerExceptionResolver,\
	org.springframework.web.servlet.mvc.annotation.ResponseStatusExceptionResolver,\
	org.springframework.web.servlet.mvc.support.DefaultHandlerExceptionResolver

org.springframework.web.servlet.RequestToViewNameTranslator=org.springframework.web.servlet.view.DefaultRequestToViewNameTranslator
org.springframework.web.servlet.ViewResolver=org.springframework.web.servlet.view.InternalResourceViewResolver
org.springframework.web.servlet.FlashMapManager=org.springframework.web.servlet.support.SessionFlashMapManager
```



#### How does this initStrategy invoked?
`FrameworkServlet` add a listener to the Web Application Context.

```java
public class FrameworkServlet{
    protected void configureAndRefreshWebApplicationContext(ConfigurableWebApplicationContext wac) {
        //...
        wac.addApplicationListener(new SourceFilteringListener(wac, new ContextRefreshListener()));
        //...
    }
    private class ContextRefreshListener implements ApplicationListener<ContextRefreshedEvent> {
		@Override
		public void onApplicationEvent(ContextRefreshedEvent event) {
			FrameworkServlet.this.onApplicationEvent(event);
		}
	}
    public void onApplicationEvent(ContextRefreshedEvent event) {
		this.refreshEventReceived = true;
		synchronized (this.onRefreshMonitor) {
			onRefresh(event.getApplicationContext());
		}
	}
    protected void onRefresh(ApplicationContext context) {
		// For subclasses: do nothing by default.
	}
}
```

DS override the onRefresh listener, and calls the `initStrategies` method.
The `AbstractApplicationContext` publish a refresh event at the end of refresh().


### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>




doFilter

service

FrameworkServlet.processRequest(HttpServletRequest request, HttpServletResponse response){

}

DispatcherServlet.doService((HttpServletRequest request, HttpServletResponse response) throws Exception{
    logRequest(request);
    request.setAttribute(WEB_APPLICATION_CONTEXT_ATTRIBUTE, getWebApplicationContext());
	request.setAttribute(LOCALE_RESOLVER_ATTRIBUTE, this.localeResolver);
	request.setAttribute(THEME_RESOLVER_ATTRIBUTE, this.themeResolver);
	request.setAttribute(THEME_SOURCE_ATTRIBUTE, getThemeSource());
    doDispatch(request, response){

    }
}

public interface ServletRequest {
    public Object getAttribute(String name);
    public Enumeration<String> getAttributeNames();
    public Enumeration<String> getParameterNames();
    public String getParameter(String name);
    public String getProtocol();
    public Cookie[] getCookies();
}



How does ... parse XML
Each namespace is solved by a different Handler, each hanlder is aware/parse a few element.

It has a XMLReaderContext, which 

```java
public class XmlWebApplicationContext extends AbstractRefreshableWebApplicationContext{
    protected void loadBeanDefinitions(DefaultListableBeanFactory beanFactory) throws BeansException, IOException {
		// Create a new XmlBeanDefinitionReader for the given BeanFactory.
		XmlBeanDefinitionReader beanDefinitionReader = new XmlBeanDefinitionReader(beanFactory);

		// Configure the bean definition reader with this context's
		// resource loading environment.
		beanDefinitionReader.setEnvironment(getEnvironment());
		beanDefinitionReader.setResourceLoader(this);
		beanDefinitionReader.setEntityResolver(new ResourceEntityResolver(this));

		// Allow a subclass to provide custom initialization of the reader,
		// then proceed with actually loading the bean definitions.
		initBeanDefinitionReader(beanDefinitionReader);
		loadBeanDefinitions(beanDefinitionReader);
	}
    protected void loadBeanDefinitions(XmlBeanDefinitionReader reader) throws IOException {
		String[] configLocations = getConfigLocations();
		if (configLocations != null) {
			for (String configLocation : configLocations) {
				reader.loadBeanDefinitions(configLocation);
			}
		}
	}
}
// Read beans definition from xml and register
/**
* The XmlBeanDefinitionReader convert a XML source to a Document objects, and use a DocumentReader to parse the document.
The documen reader has a namespace handler resolver that has different hanvler for different XML namespace.
The NamespaceHandlerResolver reads configuration from all jar's /META-INF/spring.handlers, which are define a cross a list of jar.
e.g. spring-webmvc-5.2.5.RELEASE.jar defines 
http://www.springframework.org/schema/mvc=org.springframework.web.servlet.config.MvcNamespaceHandler
which means when you have a namespace's uri is http://www.springframework.org/schema/mvc, then the handler used is 
org.springframework.web.servlet.config.MvcNamespaceHandler

Each handler has a group of parser, then support parse a list of elements, for example, MvcNamespaceHandler supports
"annotation-driven", "resources", "view-controller" and so on.
*/
public class XmlBeanDefinitionReader extends AbstractBeanDefinitionReader{
    protected int doLoadBeanDefinitions(InputSource inputSource, Resource resource){
        // read as a XML Document and register        
        Document doc = doLoadDocument(inputSource, resource);
		BeanDefinitionDocumentReader documentReader = new DefaultBeanDefinitionDocumentReader();
        XmlReaderContext xmlReaderContext = new XmlReaderContext(resource, 
            this.problemReporter, 
            this.eventListener,
			this.sourceExtractor, 
            this, 
            new DefaultNamespaceHandlerResolver());
        documentReader.registerBeanDefinitions(doc, xmlReaderContext);
    }
}
```