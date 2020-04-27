Created: 2020-04-04
Modified: 2020-04-04

* [Bean Related Classes & Interfaces](#bean)
* [Application Context](#ac)
* [Annotation based App Context](#aac)
* [XML based App Context](#xac)
* [Web Application Context](#wac)
* [References](#reference)
***

### <a id="bean">I. Bean Related Classes & Interfaces</a>

#### Get Beans
Get Beans operation is through the BeanFactory object, 

* BeanFactory: defines a series of getBean(String name) methods.
```java
public interface BeanFactory{
    boolean containsBean(String name);
    <T> T getBean(Class<T> requiredType, Object... args) throws BeansException;
    Object getBean(String name) throws BeansException;
    boolean isPrototype(String name); // lifecycle
    <T> ObjectProvider<T> getBeanProvider(ResolvableType requiredType);
}
```
* ListableBeanFactory: extends BeanFactory, which can iterated beans
```java
public interface ListableBeanFactory extends BeanFactory{
    boolean containsBeanDefinition(String beanName);
    String[] getBeanDefinitionNames();
    String[] getBeanNamesForType(ResolvableType type, boolean includeNonSingletons, boolean allowEagerInit);
    Map<String, Object> getBeansWithAnnotation(Class<? extends Annotation> annotationType) throws BeansException;
}
```
* HierarchicalBeanFactory: extends BeanFactory, which allows to setup a parent BeanFactory.
```java
public interface HierarchicalBeanFactory extends BeanFactory{
    BeanFactory getParentBeanFactory();
    boolean containsLocalBean(String name);
}
```
* AutowireCapableBeanFactory: extend BeanFactory, which provide @Autowire futher
```java
public interface AutowireCapableBeanFactory extends BeanFactory {
    <T> T createBean(Class<T> beanClass); // fully initialize a bean
    void autowireBean(Object existingBean); // Populate the given bean instance through applying after-instantiation
}
```

* SingletonBeanRegistry: register objects directly
```java
public interface SingletonBeanRegistry{
    void registerSingleton(String beanName, Object singletonObject);
    Object getSingleton(String beanName);
    //...
}
```

* ConfigurableBeanFactory: setup parentBeanFactory, setBeanClassLoader, addPropertyEditorRegistrar
```java
public interface ConfigurableBeanFactory extends HierarchicalBeanFactory, SingletonBeanRegistry{
    // BeanPostProcessor is invoked after a bean is created. 
    // For example, ApplicationContextAwareProcessor will setup application context to the bean if the bean implemenet *Aware interface.
    void addBeanPostProcessor(BeanPostProcessor beanPostProcessor);
    void setBeanClassLoader(@Nullable ClassLoader beanClassLoader);
}
```

* ConfigurableListableBeanFactory
```java
public interface ConfigurableListableBeanFactory extends ListableBeanFactory, AutowireCapableBeanFactory, ConfigurableBeanFactory{
    BeanDefinition getBeanDefinition(String beanName);
    boolean isAutowireCandidate(String beanName, DependencyDescriptor descriptor);
    // blacklist the type, which means you can getBean(ifc). Usually we have a set of beans that implement the *Aware interfaces (e.g. MessageSourceAware, ApplicationContextAware) to acquire MessageSource, 
    // ApplicationContext and so on. So these interface is kind of a Marker interface, it doesn't perform certain functions. Therefore, we want to blacklist these *Aware interface.
    void ignoreDependencyInterface(Class<?> ifc);

    // bind a bean instance to a type.
    void registerResolvableDependency(Class<?> dependencyType, @Nullable Object autowiredValue);
}
```

#### Register Bean definitions
* AliasRegistry: support bean name with alias
```java
public interface AliasRegistry{
    void registerAlias(String name, String alias);
    void removeAlias(String alias);
    boolean isAlias(String name);
    String[] getAliases(String name);
}
```
* BeanDefinitionRegistry: register bean definition
```java
public interface BeanDefinitionRegistry extends AliasRegistry{
    void registerBeanDefinition(String beanName, BeanDefinition beanDefinition);
    void removeBeanDefinition(String beanName);
    BeanDefinition getBeanDefinition(String beanName);
    boolean containsBeanDefinition(String beanName);
    String[] getBeanDefinitionNames();
}
```
#### BeanDefinition
* AttributeAccessor: an attribute store
```java
public interface AttributeAccessor{
    void setAttribute(String name, @Nullable Object value);
    Object getAttribute(String name);
    Object removeAttribute(String name);
    // ...
}
```
* BeanMetadataElement: carry a configuration source object.
```java
public interface BeanMetadataElement{
    default Object getSource(); // return configuration source.
}
```
* BeanDefinition: contain information about the bean definition, e.g. scope, lazy?, depends, primary?, init method, bean classname...
```java
public interface BeanDefinition extends AttributeAccessor, BeanMetadataElement{
    void setInitMethodName(@Nullable String initMethodName);
    boolean isPrototype();
    String getFactoryBeanName();
    String getBeanClassName();
    void setParentName(@Nullable String parentName);
}
```

### <a id="ac">II. Application Context</a>

```java
public interface EnvironmentCapable {
    Environment getEnvironment(); // implementation usually setup environment from container's ServletConfig and ServletContext.
}
public interface ResourceLoader{
    Resource getResource(String location);
    ClassLoader getClassLoader();
}

public class DefaultResourceLoader implements ResourceLoader{
    private ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
    public void addProtocolResolver(ProtocolResolver resolver) { // a resolver is kind of handler that resolve the resource.
		this.protocolResolvers.add(resolver);
	}
    public Resource getResource(String location) {
        for (ProtocolResolver protocolResolver : getProtocolResolvers()) {
			Resource resource = protocolResolver.resolve(location, this);
			if (resource != null) {
				return resource;
			}
		}
        // ...
    }
}
public interface ResourcePatternResolver extends ResourceLoader{
    Resource[] getResources(String locationPattern) throws IOException;
}
public class PathMatchingResourcePatternResolver implements ResourcePatternResolver{

}
```
ApplicationContext is basic the IoC container, it extends the BeanFactory interface (but usually compose a beanfactory object) and others

```java
public interface ApplicationContext extends EnvironmentCapable, ListableBeanFactory, HierarchicalBeanFactory,
		MessageSource, // getMessage()
        ApplicationEventPublisher, // publishEvent()
        ResourcePatternResolver {

    ApplicationContext getParent(); // in most case, we don't have a application context parent;
    String getApplicationName();
    String getId();
    AutowireCapableBeanFactory getAutowireCapableBeanFactory()
}
```
* ConfigurableApplicationContext: support set* method so that you can modify (configure) the application context. It also delegate promised functions to different objects.

```java
interface ConfigurableApplicationContext extends ApplicationContext, 
Lifecycle // start(), stop(), isRunning()
{
    void setId(String id);
    void setParent(@Nullable ApplicationContext parent);
    void addBeanFactoryPostProcessor(BeanFactoryPostProcessor postProcessor); // void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory)
    void addApplicationListener(ApplicationListener<?> listener);
    void addProtocolResolver(ProtocolResolver resolver); // for 
    void refresh();
    ConfigurableListableBeanFactory getBeanFactory();
    
}
```
#### 1). AbstractApplicationContext
AbstractApplicationContext is the abstract implementation of ApplicationContext, it supports ResourceLoader, ApplicationEventPubliser
It implements the `refresh()` method to initialize these functions.

1. __Implementation of ApplicationEventPublisher__

Inside the `refresh()`, it has a `initApplicationEventMulticaster()` function, which creates the `ApplicationEventMulticaster` properties either from bean factory or use a default implementation.

It also caches the listeners registered before the multicaster is created and events pulished before the multicast is created.

```java
public abstract class AbstractApplicationContext extends DefaultResourceLoader implements ConfigurableApplicationContext{
    private ApplicationEventMulticaster applicationEventMulticaster;
    private final Set<ApplicationListener<?>> applicationListeners = new LinkedHashSet<>();
    private Set<ApplicationListener<?>> earlyApplicationListeners;
    private Set<ApplicationEvent> earlyApplicationEvents;

    @Override
	public void refresh(){
        prepareRefresh();
        // ...
        initApplicationEventMulticaster();
        // ...
        registerListeners();
        //...

    } 
    public void prepareRefresh(){
        // setup application listeners that created before the mulitcaster created.
        if (this.earlyApplicationListeners == null) {
			this.earlyApplicationListeners = new LinkedHashSet<>(this.applicationListeners);
		} else {
			// Reset local application listeners to pre-refresh state.
			this.applicationListeners.clear();
			this.applicationListeners.addAll(this.earlyApplicationListeners);
		}
        this.earlyApplicationEvents = new LinkedHashSet<>();
    }
    protected void initApplicationEventMulticaster(){
        ConfigurableListableBeanFactory beanFactory = getBeanFactory();
        if (beanFactory.containsLocalBean("applicationEventMulticaster")) { // check if the local (without asking parent) bean factory has the mutlicaster.
			this.applicationEventMulticaster = beanFactory.getBean("applicationEventMulticaster", ApplicationEventMulticaster.class);
        }else{
            this.applicationEventMulticaster = new SimpleApplicationEventMulticaster(beanFactory);
            beanFactory.registerSingleton("applicationEventMulticaster", this.applicationEventMulticaster);
        }
    }
    // parent to the current context also to the parent context.
    protected void publishEvent(Object event, @Nullable ResolvableType eventType){
        if (this.earlyApplicationEvents != null) { // if event published before the multicaster is created, these event will be cached here.
			this.earlyApplicationEvents.add(applicationEvent);
		} else {
			this.applicationEventMulticaster.multicastEvent(applicationEvent, eventType);
		}
        if (this.parent != null) {
            this.parent.publishEvent(event);
        }
    }
    // register event listeners, cache them if the multicaster is not created yet.
    public void addApplicationListener(ApplicationListener<?> listener) {
		if (this.applicationEventMulticaster != null) {
			this.applicationEventMulticaster.addApplicationListener(listener);
		}
		this.applicationListeners.add(listener);
	}

    // register all cached listeners and flush cached event that published before multicaster is created.
    protected void registerListeners(){
        // register cached listeners
        for (ApplicationListener<?> listener : this.applicationListeners) {
			this.applicationEventMulticaster.addApplicationListener(listener);
		}
        // get all registered listeners from bean factory (singleton/prototype) not included lazy, register these listeners to multicaster.
        // lazy listener is registered by ApplicationListenerDetector post processor.
        String[] listenerBeanNames = getBeanNamesForType(ApplicationListener.class, true, false);
        for (String listenerBeanName : listenerBeanNames) {
			this.applicationEventMulticaster.addApplicationListenerBean(listenerBeanName);
		}
        // flush all cached event.
        Set<ApplicationEvent> earlyEventsToProcess = this.earlyApplicationEvents;
		this.earlyApplicationEvents = null;
		if (earlyEventsToProcess != null) {
			for (ApplicationEvent earlyEvent : earlyEventsToProcess) {
				this.applicationEventMulticaster.multicastEvent(earlyEvent);
			}
		}
    }
}

```

2. __ApplicationContext initialize BeanFactory__

The `AbstractApplicationContext` doesn't create BeanFactory, but it will do some settings on it.

```java
public abstract class AbstractApplicationContext{
    public void refresh(){
        ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();
        prepareBeanFactory(beanFactory); // blacklist *Aware interface, add ApplicationContextAwareProcessor & ApplicationListenerDetector post processor
        postProcessBeanFactory(beanFactory); // abstract method.
        // apply a factory post-processor and registry post-processor to the beanfactory.
        // the actual operation is delegated to `PostProcessorRegistrationDelegate.invokeBeanFactoryPostProcessors`
        // the post-processor comes from an input argument and also 
        //     beanFactory.getBeanNamesForType(BeanDefinitionRegistryPostProcessor.class), and 
        //     beanFactory.getBeanNamesForType(BeanFactoryPostProcessor.class)
        invokeBeanFactoryPostProcessors(beanFactory); 

        // operation is delegated to PostProcessorRegistrationDelegate.registerBeanPostProcessors. Find bean post-processor from BeanFactory.
        //      String[] postProcessorNames = beanFactory.getBeanNamesForType(BeanPostProcessor.class)
        // re-register ApplicationListenerDetector ? 
        //      beanFactory.addBeanPostProcessor(new ApplicationListenerDetector(applicationContext)); 
        registerBeanPostProcessors(beanFactory);
    }
    protected ConfigurableListableBeanFactory obtainFreshBeanFactory() {
		refreshBeanFactory(); // abstract
		return getBeanFactory(); // abstract
	}
	protected void prepareBeanFactory(ConfigurableListableBeanFactory beanFactory) {

		beanFactory.setBeanClassLoader(getClassLoader()); // Launcher$AppClassLoader, the system default loader.
		beanFactory.setBeanExpressionResolver(new StandardBeanExpressionResolver(beanFactory.getBeanClassLoader()));
		beanFactory.addPropertyEditorRegistrar(new ResourceEditorRegistrar(this, getEnvironment()));

		// setup beans implements *Aware inteface.
		beanFactory.addBeanPostProcessor(new ApplicationContextAwareProcessor(this));

        // blacklist the *Aware interface.
		beanFactory.ignoreDependencyInterface(EnvironmentAware.class);
		beanFactory.ignoreDependencyInterface(EmbeddedValueResolverAware.class);
		beanFactory.ignoreDependencyInterface(ResourceLoaderAware.class);
		beanFactory.ignoreDependencyInterface(ApplicationEventPublisherAware.class);
		beanFactory.ignoreDependencyInterface(MessageSourceAware.class);
		beanFactory.ignoreDependencyInterface(ApplicationContextAware.class);

        // register Type - Instance
		beanFactory.registerResolvableDependency(BeanFactory.class, beanFactory);
		beanFactory.registerResolvableDependency(ResourceLoader.class, this);
		beanFactory.registerResolvableDependency(ApplicationEventPublisher.class, this);
		beanFactory.registerResolvableDependency(ApplicationContext.class, this);

		// Register lazy beans that implements ApplicationListener.
		beanFactory.addBeanPostProcessor(new ApplicationListenerDetector(this));

		// Detect a LoadTimeWeaver and prepare for weaving, if found.
		if (beanFactory.containsBean("loadTimeWeaver")) {
			beanFactory.addBeanPostProcessor(new LoadTimeWeaverAwareProcessor(beanFactory));
			// Set a temporary ClassLoader for type matching.
			beanFactory.setTempClassLoader(new ContextTypeMatchClassLoader(beanFactory.getBeanClassLoader()));
		}

		// Register default environment beans.
		if (!beanFactory.containsLocalBean("environment")) {
			beanFactory.registerSingleton("environment", getEnvironment());
		}
		if (!beanFactory.containsLocalBean("systemProperties")) {
			beanFactory.registerSingleton("systemProperties", getEnvironment().getSystemProperties());
		}
		if (!beanFactory.containsLocalBean("systemEnvironment")) {
			beanFactory.registerSingleton("systemEnvironment", getEnvironment().getSystemEnvironment());
		}
	}
}
```


3. __implementation of ResourceLoader__

```java
public abstract class AbstractApplicationContext extends DefaultResourceLoader
		implements ConfigurableApplicationContext {
}
```

#### 2). GenericApplicationContext

`GenericApplicationContext` extends `AbstractApplicationContext` and implements `BeanDefinitionRegistry`.

The `GenericApplicationContext` has a default BeanFactory `DefaultListableBeanFactory`, which also implements the `BeanDefinitionRegistry`. So the GenericAC implements the `BeanDefinitionRegistry` by delegating operations to the default BeanFactory.

```java
public class GenericApplicationContext extends AbstractApplicationContext implements BeanDefinitionRegistry {
    public GenericApplicationContext() {
		this.beanFactory = new DefaultListableBeanFactory();
	}
    @Override // delegate operations
    public void registerBeanDefinition(String beanName, BeanDefinition beanDefinition)
			throws BeanDefinitionStoreException {
		this.beanFactory.registerBeanDefinition(beanName, beanDefinition);
	}
```

The `GenericAC` class provides method to register bean definition, but on one invokes this method to register beans. This is done by subclass, e.g. `AnnotationConfigApplicationContext` that reader bean definition from configuration class and then register them with `GenericAC.registerBeanDefinition` method.

### <a id="acc">III. Annotation based App Context (AnnotationConfigApplicationContext)</a>

`AnnotationConfigApplicationContext` read bean definition from Configuration classes and register them with `GenericAC`'s `registerBeanDefinition` method. The operation is delegated to `AnnotatedBeanDefinitionReader`

```java
public class AnnotationConfigApplicationContext extends GenericApplicationContext implements AnnotationConfigRegistry {
    private final AnnotatedBeanDefinitionReader reader;
	private final ClassPathBeanDefinitionScanner scanner;

    public AnnotationConfigApplicationContext() {
		this.reader = new AnnotatedBeanDefinitionReader(this);
		this.scanner = new ClassPathBeanDefinitionScanner(this);
	}
    public AnnotationConfigApplicationContext(Class<?>... componentClasses) {
		this();
		register(componentClasses); // use AnnotatedBeanDefinitionReader to invoke GenericAC's registerBeanDefinition.
		refresh(); // AbstractApplicationContext.
	}
    void register(Class<?>... componentClasses) {// from AnnotationConfigRegistry interface
        this.reader.register(componentClasses)
        {
            AnnotatedGenericBeanDefinition abd = new AnnotatedGenericBeanDefinition(beanClass);
            if (this.conditionEvaluator.shouldSkip(abd.getMetadata())) { // evaluate @Conditional annotation.
			    return;
		    }
        }
    }
    void scan(String... basePackages){ // from AnnotationConfigRegistry interface.
        this.scanner.scan(basePackages);
    }
}
```
You can register a list of `@Configuration` decorated configuration classes. A configuration class can have a `@Conditional` (or `@Profile`) annotation to do a 
conditional registration. The `AnnotatedBeanDefinitionReader.register()`, however, only registers the configuration class itself. It doesn't register the beans declared inside the configuration class.
Internal `@Bean` is registered during `refresh().invokeBeanFactoryPostProcessors()` by the `ConfigurationClassPostProcessor` class (this class comes from `AnnotatedBeanDefinitionReader` constructor). 
The `ConfigurationClassPostProcessor` asks `ConfigurationClassParser` to find all other @Configuration classes via @import, @ComponentScan, nest class, and then register the beans defined.

##### Handler of parsing a @Configuration class

```java
class ConfigurationClassParser{
// find all @configuration class from the source-class.
// 
    protected final SourceClass doProcessConfigurationClass(ConfigurationClass configClass, SourceClass sourceClass, Predicate<String> filter){
        // this method parse @Component, @PropertySources, @ComponentScan, @Import, @ImportResource
        // @Component (@Configuration is @Component) processes nested @Configuration class.
        for(nestClass: sourceClass){
            if(isConfigurationCandidate(nestClass)){ // a class is Configuration class if it has {@Component, @ComponentScan, @Import, @ImportResource, or has @Bean inside}
                recurisve ...
            }
        }
        // @PropertySources

        // @ComponentScan
        // @ComponentScan will scan the current package and all sub-packages, because @Configuration is also @Component, so this @ComponentScan will also include @Configuration class within the packages.

        // @Import
        for(importConfig: imports){ // recursive.
            doProcessConfigurationClass(importConfig, configClass, filter);
        }
        // @ImportResource

    }
}
```

##### enhancer of Configuration.

A `@Configuration` class (config class) has two modes, `@Configuration(proxyBeanMethods = false)` lite-mode or `@Configuration(proxyBeanMethods = true) (default)` full-mode. The different is that the 
`ConfigurationClassPostProcessor` will create a proxy wraps the config instance. If you print the bean classname, it usually looks like `MyConfiguration$$EnhancerBySpringCGLIB$$a2ca5fbb`.

Think about the following example
```java
@Configuration
public class MyConfiguration {
    @Bean
    public X getX(){
        return new X();
    }
    @Bean
    public Y getY(){
        return new Y(getX());
    }
}
public class Driver {
    public static void main(String [] args){
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(MyConfiguration.class);
        X x = ctx.getBean(X.class);
        Y y = ctx.getBean(Y.class);
    }
```
X is expected to be a singleton instance. However, when you do `ctx.getBean(Y.class)` the `getY()` will be invoked, it also invokes `getX()`, which means the first time you `getBean(Y.class)` also creates a X. And when you do `ctx.getBean(X.class)`, it just create another X. With full-mode, a proxy object is created to wrap the real config object. And the `return new Y(getX())`'s `getX()` is also proxied to call the proxy's `getX()`, which prevent from creating a new X.

This step happened after parsing @Configuration classes.

*Without the proxy, you have to do method parameter injection if you still want X to be singleton.*

#####  Example of Configuration class
```java
@Configuration
@ComponentScan // import all class annotated with @Component in this package and subpackages.
@Import({AnotherConfiguration.class}) // explicitly import a configuration class.
@Profile("dev") // conditional import.
public class MyConfiguration {
    @Bean
    public X getX(){
        return new X();
    }
    @Bean
    public Y getY(){
        return new Y(getX());
    }

    // You can have a nested configuration class inside.
    public class NestConfiguration{
        @Bean
        public Z2 getZ2(){
            return new Z2(getX());
        }
    }
}
```

### <a id="reference">XML based app context</a>
There are two XML based App Context, `ClassPathXmlApplicationContext` and `FileSystemXmlApplicationContext`

#### Class structure
```bash
AbstractApplicationContext
AbstractRefreshableApplicationContext
AbstractRefreshableConfigApplicationContext
AbstractXmlApplicationContext
ClassPath/FileSystemXmlApplicationContext
```

The common ancestor of a XML application context and annotation based app context is `AbstractApplicationContext`.
* AbstractRefreshableApplicationContext: provide a refresh skeleton invoked by parent, use `DefaultListableBeanFactory` as default BeanFactory and registry.
```java
protected final void refreshBeanFactory(){
    if(this.beanFactory != null){
        this.beanFactory.destroySingletons();
    }
    this.beanFactory = new DefaultListableBeanFactory(getInternalParentBeanFactory());
    customizeBeanFactory(beanFactory)
    {
        beanFactory.setAllowBeanDefinitionOverriding(this.allowBeanDefinitionOverriding);
        beanFactory.setAllowCircularReferences(this.allowCircularReferences);
    }
    loadBeanDefinitions(beanFactory); // abstract
}
```

* AbstractRefreshableConfigApplicationContext: provide method to set/get configuration locations and resolve configuration's real locations in case it contains placeholder.
```java
public void setConfigLocations(@Nullable String... locations) {	
	this.configLocations = new String[locations.length];
	for (int i = 0; i < locations.length; i++) {
		this.configLocations[i] = this.environment.resolveRequiredPlaceholders(locations[i]).trim();
	}
}
```

* AbstractXmlApplicationContext: provide a skeleton for loading XML based configurations.
```java
protected void loadBeanDefinitions(DefaultListableBeanFactory beanFactory) throws BeansException, IOException {	
	XmlBeanDefinitionReader beanDefinitionReader = new XmlBeanDefinitionReader(beanFactory); // the actual worker

	beanDefinitionReader.setEnvironment(this.getEnvironment());
	beanDefinitionReader.setResourceLoader(this);
	beanDefinitionReader.setEntityResolver(new ResourceEntityResolver(this));
    beanDefinitionReader.setValidating(this.validating); // indicate if allowing invalid .xml
	loadBeanDefinitions(beanDefinitionReader);{
        Resource[] configResources = getConfigResources(); // leave to subclass to provide default configurations.
		if (configResources != null) {
			reader.loadBeanDefinitions(configResources);
		}
		String[] configLocations = getConfigLocations(); // ClasspathXML override this class to provide XML resource.
		if (configLocations != null) {
			reader.loadBeanDefinitions(configLocations);
		}
    }
}
```

1. <a href="http://tomcat.apache.org/tomcat-8.0-doc/class-loader-howto.html" target="_blank">Tomcat classloader</a>