Created: 2020-04-04
Modified: 2020-04-04

* [ResourceLoader](#resource-loader)
* [Environment](#environment)
* [Spring Logging](#spring-logging)
* [Spring BeanFactory](#bean-factory)
* [References](#reference)
***
### <a id="resource-loader">I. ResourceLoader</a>
Application context provides the functionalities to load resources. For example, get the configuration files for ClassPathXML. The functions are defined in 
`ResourceLoader` interface.
```java
public interface ResourceLoader {
    String CLASSPATH_URL_PREFIX = "classpath:";
    Resource getResource(String location);
    ClassLoader getClassLoader();
}
```
A `Resource` is essentially a URL to that resource, and the URL can open connection to it and return a `InputStream` to get the resource data. A resource is usually loaded by a classloader,
which means resources should be available on its classpath. There exists different kind of resource classes. 
* AbstractFileResolvingResource: an abstract resource class that indicates the resource is a file system based resources (able to convert the url to a file)
* UrlResource: 
* FileUrlResource:
* ClassPathResource:
* ClassPathContextResource:

The default implementation of `ResourceLoader` is `DefaultResourceLoader`, which get the default classloader from its current thread. Normally, it's the `Launcher$AppClassLoader` for a standalone application, or the container provided classloader in a servlet application.

```java
public class DefaultResourceLoader implements ResourceLoader {
    private ClassLoader classLoader; // default set to the current thread associated classloader.
    private final Set<ProtocolResolver> protocolResolvers = new LinkedHashSet<>(4); // consumer provided custom resource loader.

    // first try to load from the protocol resolvers
    // return a ClassPath resource if the location prefix "classpath:"
    // try to convert the location to a url, if so, return a url resource.
    public Resource getResource(String location) {
        for (ProtocolResolver protocolResolver : getProtocolResolvers()) {
			Resource resource = protocolResolver.resolve(location, this);
			if (resource != null) {
				return resource;
			}
		}
        if (location.startsWith(CLASSPATH_URL_PREFIX)) {
			return new ClassPathResource(location.substring(CLASSPATH_URL_PREFIX.length()), getClassLoader());
		})
        try{
            URL url = new URL(location);
			return (ResourceUtils.isFileURL(url) ? new FileUrlResource(url) : new UrlResource(url));
        }catch(MalformedURLException ex) {
				// No URL -> resolve as resource path.
				return getResourceByPath(location);
		}
    }
    protected Resource getResourceByPath(String path) {
		return new ClassPathContextResource(path, this.classLoader);
	}
}
```

When loading a resource, the default behavior is to load from the classloader's classpath. In addition the `DefaultResourceLoader` also provide a mechanism to customized the behavior when load a resource. For example, when you ask "myConfiguration.xml" resource, if no protocol resolver takes care of it, and since it also doesn't start with "classpath:", and cannot form a URL, it will be
passed to the `getResourceByPath()`, the default implementation is to find from classpath.

The Application context acquires the function of Resource loader by <span style="color:red">inheriting from "DefaultResourceLoader".</span>. Inheritance rather than composition makes it hard to provide your customized resource loader.

### <a id="environment">II. Environment</a>
An environment a container of a group of property sources (configurations), and has the names of active/default profile names. It also provides functions such as resolving path according its property sources. For example, resolving "${location}-1.0.jar" to "/bin/sample-1.0.jar" if we have a property of location=/bin/sample.

The Application Context extends `EnvironmentCapable`, which defines a method `Environment getEnvironment();` There are two default implementations of Environment
* `StandardEnvironment`: used in a stonealone application that has property sources from "JVM system properties" and "environment variables"
* `StandardServletEnvironment`: used in a servlet environment. It extends `StandardEnvironment`, but also add the web.xml's context-param and init-param as property sources to the environment.

Inside the `StandardEnvironment`, it composes a property sources registry `MutablePropertySources`, which allow you to add new property sources. It also delegate the path resolution operations to 
`PropertyResolver`. It also provides a `merge` method to merge another environment to it. The operations include add all property sources to the end, add all active/default profiles. 

The `AbstractEnvironment` implements all the functions but doesn't register any property sources, which leaves to subclass. 
```java
public abstract class AbstractEnvironment implements ConfigurableEnvironment{
    // profile name holder.
    private final Set<String> activeProfiles = new LinkedHashSet<>();
	private final Set<String> defaultProfiles = new LinkedHashSet<>(Collections.singleton("default"));

    // manage a group of property sources.
    private final MutablePropertySources propertySources = new MutablePropertySources();
    // resolve placeholders.
    private final ConfigurablePropertyResolver propertyResolver =
			new PropertySourcesPropertyResolver(this.propertySources);

    // abstract function invoked for subclass to registry property sources.
    protected void customizePropertySources(MutablePropertySources propertySources) {
	}
}
```

#### PropertySource
A property source is essentially a container of key-object pair. 
```java
public abstract class PropertySource<T> {
    protected final String name;
	protected final T source;
    public abstract Object getProperty(String name);
}
```
Spring provides a few implementations, 
* `MapPropertySource`: convert a `Map<String, Object>` to a property source.
* `PropertiesPropertySource`: subclass of `MapPropertySource`, convert a java defined `Properties` to Map and then to a property source.
* `ResourcePropertySource`: subclass of `PropertiesPropertySource`, convert a resource to java `Properties`, and then to a Map, and then to a property source.
* `SystemEnvironmentPropertySource`

#### Utilize Environment
The following example shows the internal logic of a Environment.
```java
Map<String, Object> prop = new HashMap<>();
prop.put("date", "2020-01-02");
MutablePropertySources propertySources = new MutablePropertySources();
PropertyResolver pr = new PropertySourcesPropertyResolver(propertySources);
propertySources.addFirst(new MapPropertySource("date-prop", prop));
pr.resolvePlaceholders("${hi}-${PATH}")
```

*header: org.springframework.core.env.Environment*

You can get the Environment object from context either by calling `.getEnvironment()` method or by using the `.getBean("environment")` 
The environment is register to bean registry when
```java
AbstractApplicationContext.refresh(){
    prepareBeanFactory(beanFactory){
        // ...
        if (!beanFactory.containsLocalBean(ENVIRONMENT_BEAN_NAME)) {
			beanFactory.registerSingleton(ENVIRONMENT_BEAN_NAME, getEnvironment());
		}
        if (!beanFactory.containsLocalBean(SYSTEM_PROPERTIES_BEAN_NAME)) {
			beanFactory.registerSingleton(SYSTEM_PROPERTIES_BEAN_NAME, getEnvironment().getSystemProperties());
		}
		if (!beanFactory.containsLocalBean(SYSTEM_ENVIRONMENT_BEAN_NAME)) {
			beanFactory.registerSingleton(SYSTEM_ENVIRONMENT_BEAN_NAME, getEnvironment().getSystemEnvironment());
		}
        // ...
    }
}
```

### <a id="spring-logging">III. Spring Logging</a>
Spring utilize the apache common logging library.

<p class="reference-box">
A library cannot impose the use of a particular one on the overall application that the library is a part of.
</p>

A library or a framework (spring) should not determine the runtime library for the application. In other worlds, the logging implementation should be choose by the application developer.
And Apache Common Logging library is a layer on top of the implementation. It acquires loggers by detecting the logger framework at runtime. The work is done by an internal class `LogAdapter`.

The `LogAdapter` can detect log4j and slf4j, if none of them is found, it will use the JUL (java.util.logging) api.

The Apache Common Logging's Log interface defines six logging log: trace, debug, info, warn, error, fatal. They are mapped to underling logging framework.

### <a id="bean-factory">IV. Spring BeanFactory</a>

Spring Bean Factory is the core component of the Inversion of Control container. It resolves dependency and returns bean definition.
The default `DefaultListableBeanFactory` also implements `BeanDefinitionRegistry` function so that it provides method to register `BeanDefinition`. 

Application Context classes also integrate a mechanism to register bean definition from different sources, such as `AbstractXmlApplicationContext` has a `XmlBeanDefinitionReader` to register bean definitions from XML files, and `AnnotationConfigApplicationContext` has a `AnnotatedBeanDefinitionReader` to register bean definitions from @Configuration classes.

If you want to use the BeanFactory object standalone, you have to figure out a way to register bean definition. 

#### BeanDefinition

Architecture
```java
public interface BeanMetadataElement { // a bean definition may carry a Configuration source.
    Object getSource();
}
public interface AttributeAccessor { // a key-value store for attributes.
    void setAttribute(String name, @Nullable Object value);
    Object getAttribute(String name);
    // ...
}
public abstract class AttributeAccessorSupport implements AttributeAccessor, Serializable {
    // a LinkedHashMap<> implementation
}
public class BeanMetadataAttributeAccessor extends AttributeAccessorSupport implements BeanMetadataElement {
    // a simple implementation
    private Object source;
    public void addMetadataAttribute(BeanMetadataAttribute attribute) { // name, value, source
		super.setAttribute(attribute.getName(), attribute);
	}
}
public interface BeanDefinition extends AttributeAccessor, BeanMetadataElement{
    String SCOPE_SINGLETON = ConfigurableBeanFactory.SCOPE_SINGLETON;
    String SCOPE_PROTOTYPE = ConfigurableBeanFactory.SCOPE_PROTOTYPE;
    void setParentName(@Nullable String parentName); // a bean definition can have a parent, this bean definition will inherit parent's bean definitions, such as property settings, lazy?
    void setBeanClassName(@Nullable String beanClassName); // the core component.
    void setScope(@Nullable String scope);
    void setLazyInit(boolean lazyInit);
    void setDependsOn(@Nullable String... dependsOn);
    void setAutowireCandidate(boolean autowireCandidate); //  Set whether this bean is a candidate for getting autowired into some other bean. Autowire by name, type, and constructor. This field only affect by type.
    void setPrimary(boolean primary); // if mutliple beans satisfy the required bean type.
    void setFactoryBeanName(@Nullable String factoryBeanName);
    void setFactoryMethodName(@Nullable String factoryMethodName);
    MutablePropertyValues getPropertyValues(); // beanwrapper used to mount the property to corresponding bean's member.
    void setInitMethodName(@Nullable String initMethodName); // bean's lifecycle method
    void setDestroyMethodName(@Nullable String destroyMethodName); // bean's lifecycle method
    void setRole(int role); // this is a hint indicate if the bean definition is created by internal spring or application.
    ResolvableType getResolvableType(); // ?
}

public abstract class AbstractBeanDefinition extends BeanMetadataAttributeAccessor
		implements BeanDefinition, Cloneable{
    // an abstract implementation which has default values.
    private String scope = "";
    private int autowireMode = AUTOWIRE_NO;
    private boolean autowireCandidate = true;
    private int role = BeanDefinition.ROLE_APPLICATION;
	private boolean primary = false;
    private final Map<String, AutowireCandidateQualifier> qualifiers = new LinkedHashMap<>(); // Qualifier for resolving autowire candidates
    private MutablePropertyValues propertyValues;
	private MethodOverrides methodOverrides = new MethodOverrides();
    // provide method to copy, override bean definition
}
```

`GenericBeanDefinition` is the commonly used bean definition when you programmatically register a bean definition. It's almost same as the `AbstractBeanDefinition`.

Another way to create a bean definition is to use `BeanDefinitionBuilder` which is a builder pattern around a `GenericBeanDefinition` or a `RootBeanDefinition`.

##### Properties
You can set a bean's property member by adding it to the bean definition's property value. It can also be a __reference__ property that the bean factory will resolve it for you.

For example,
```java
class X{
    private Y y;
    private String name;
    public void setName(String name){this.name = name;}
    public void setY(Y y){this.y = y}; // you need a setter method.
}
class Y{}
public class Main{
    public static void main(String [] args){
        DefaultListableBeanFactory myBeanFactory = new DefaultListableBeanFactory();

        BeanDefinition yBb = new GenericBeanDefinition();
        yBb.setBeanClassName(Y.class.getName());
        BeanDefinition xBb = new GenericBeanDefinition();
        xBb.setBeanClassName(X.class.getName());
        xBb.getPropertyValues().add("y", new RuntimeBeanReference("beanY")); // a reference to another bean.
        xBb.getPropertyValues().add("name", "X class");

        myBeanFactory.registerBeanDefinition("beanX", xBb);
        myBeanFactory.registerBeanDefinition("beanY", yBb);

        myBeanFactory.getBean(X.class);
    }
}
```


#### BeanDefinitionRegistry

BeanDefinitionRegistry is an interface for register beandefinitions, it also support alias. The `DefaultListableBeanFactory` implements the registry with a concurrent hashmap. Alias interface is implemented directly by a `SimpleAliasRegistry`, which then indirectly inherited by the `DefaultListableBeanFactory`.


```java
public interface AliasRegistry {
    void registerAlias(String name, String alias);
    void removeAlias(String alias);
    String[] getAliases(String name);
}
public interface BeanDefinitionRegistry extends AliasRegistry {
    void registerBeanDefinition(String beanName, BeanDefinition beanDefinition);
    BeanDefinition getBeanDefinition(String beanName);
    int getBeanDefinitionCount();
    String[] getBeanDefinitionNames();
}
public class SimpleAliasRegistry implements AliasRegistry {
    private final Map<String, String> aliasMap = new ConcurrentHashMap<>(16); // alias -> canonical name.
    public String canonicalName(String name) { // find the canonical name.
        do {
			resolvedName = this.aliasMap.get(canonicalName);
			if (resolvedName != null) {
				canonicalName = resolvedName;
			}
		}
    }
}
public class DefaultListableBeanFactory extends AbstractAutowireCapableBeanFactory
		implements ConfigurableListableBeanFactory, BeanDefinitionRegistry, Serializable {
    // bean definition registry part
    private final Map<String, BeanDefinition> beanDefinitionMap = new ConcurrentHashMap<>(256);
}
```

#### SingletonBeanRegistry
A special registry for shared (singleton) bean instances. The default implementation support alias, register beans or bean factorys,
dependent bean relationship, in-creation logic. It has two ways to get singleton. 
1. getSingleton by bean name, if the bean doesn't existed, return null.
2. getSingleton by bean name with object factory (a lambda that tell how to create the bean), if the bean doesn't exist, invoke the lambda.

The singleton bean registry can be used by application directly or by the spring bean factory to register a created singleton bean.

```java
public interface SingletonBeanRegistry {
    void registerSingleton(String beanName, Object singletonObject);
    // get all names, get count, get singleton, contains?
    // no remove method
    Object getSingletonMutex(); // used as a lock target
}

public class DefaultSingletonBeanRegistry extends SimpleAliasRegistry implements SingletonBeanRegistry {
    private final Map<String, Object> singletonObjects = new ConcurrentHashMap<>(256);
    private final Map<String, ObjectFactory<?>> singletonFactories = new HashMap<>(16);
    private final Set<String> registeredSingletons = new LinkedHashSet<>(256);
    private final Map<String, Set<String>> dependentBeanMap = new ConcurrentHashMap<>(64);
    protected Object getSingleton(String beanName, boolean allowEarlyReference) {
        Object singletonObject = this.singletonObjects.get(beanName);
        if(singletonObject == null){
            //... early reference
        }
        return singletonObject;
    }
    public Object getSingleton(String beanName, ObjectFactory<?> singletonFactory) { // ObjectFactory is an lambda to tell how to create the singleton.
        Object singletonObject = this.singletonObjects.get(beanName);
        if(singeltonObject == null){
            beforeSingletonCreation(beanName);
            singletonObject = singletonFactory.getObject();
            afterSingletonCreation(beanName);
            addSingleton(beanName, singletonObject);
        }
        return singletonObject;
    }
}
public abstract class FactoryBeanRegistrySupport extends DefaultSingletonBeanRegistry{
    // a cache for factory created bean.
    private final Map<String, Object> factoryBeanObjectCache = new ConcurrentHashMap<>(16);
}
```

#### BeanFactory

A beanfactory is responsible for creating beans. A sub interface `HierarchicalBeanFactory` proposed a hierachical structure, meaning a factory bean can have a parent factory bean.

When you require a bean that should be created by a factory bean, it will return a the factory bean itself if the bean names start with '&'. For example, bean `X` should be created by `FactoryX`, `.getBean("X")` return "X", `getBean("&X")` return "FactoryX".


```java
public interface BeanFactory {
    String FACTORY_BEAN_PREFIX = "&"; //get &myBean will return the factory for creating myBean if available.
    Object getBean(String name);
    <T> T getBean(Class<T> requiredType);
    <T> T getBean(String name, Class<T> requiredType);
    Class<?> getType(String name); // return type of the given bean name.
    boolean isSingleton(String name);
    String[] getAliases(String name);
}
public interface HierarchicalBeanFactory extends BeanFactory {
    @Nullable
	BeanFactory getParentBeanFactory();
    boolean containsLocalBean(String name);
}
public interface ConfigurableBeanFactory extends HierarchicalBeanFactory, SingletonBeanRegistry {
    void registerScope(String scopeName, Scope scope); // allow register addition scope. used by web based application context to register request and session scope.
    void addBeanPostProcessor(BeanPostProcessor beanPostProcessor);
    void addPropertyEditorRegistrar(PropertyEditorRegistrar registrar); // for a certain bean type, add a customize property editor.
    void registerCustomEditor(Class<?> requiredType, Class<? extends PropertyEditor> propertyEditorClass); // 
    void setParentBeanFactory(BeanFactory parentBeanFactory);
    void setBeanClassLoader(@Nullable ClassLoader beanClassLoader);
}
public abstract class AbstractBeanFactory extends FactoryBeanRegistrySupport implements ConfigurableBeanFactory {
    private BeanFactory parentBeanFactory;
    private ClassLoader beanClassLoader = Thread.currentThread().getContextClassLoader();
    private ClassLoader tempClassLoader;
    private final List<BeanPostProcessor> beanPostProcessors = new CopyOnWriteArrayList<>();
    private final List<StringValueResolver> embeddedValueResolvers = new CopyOnWriteArrayList<>(); // resolve string value e.g. from annotation
    private final Map<String, Scope> scopes = new LinkedHashMap<>(8);

    protected RootBeanDefinition getMergedBeanDefinition(String beanName, BeanDefinition bd, @Nullable BeanDefinition containingBd){
        // merge two bean definition.
    }
}
```

The foundamental method for getting a bean is `AbstractBeanFactory.doGetBean` defines a skeleton for get different scope's beans. 
* If it's singleton, delegate the operation to getSingleton of `DefaultSingetonBeanRegistry` (inherited)
* If it's prototype, create the bean.
* If it's customized scope, delegate the operation to the Scope object.

In addition, any scoped beans can be obtained directly or from a factory bean. So the `AbstractBeanFactory.getObjectForBeanInstance` is used to distinguish if it's factory bean after you get an instance.

```java
public abstract class AbstractBeanFactory extends FactoryBeanRegistrySupport implements ConfigurableBeanFactory {
    // Different get logical for different scopes. 
    protected <T> T doGetBean(final String name, @Nullable final Class<T> requiredType,
			@Nullable final Object[] args, boolean typeCheckOnly) throws BeansException {
        String beanName = canonicalName(name); // get the real name;
        Object sharedInstance = getSingleton(beanName); // from the singleton registry
        if (sharedInstance != null && args == null) {
            return getObjectForBeanInstance(sharedInstance, name, beanName, null);

        }else if(this.parentBeanFactory != null && !this.beanDefinitionRegistry.containsBeanDefinition(beanName)){
            return parentBeanFactory.getBean(beanName);

        }else{
            // get merged beans and setup dependency.
            RootBeanDefinition mbd = getMergedLocalBeanDefinition(beanName);
            String[] dependsOn = mbd.getDependsOn();
            if(mbd.isAbstract()){
                throw new BeanIsAbstractException(beanName);
            }
            String[] dependsOn = mbd.getDependsOn();
            for(String dep : dependsOn){
                if (isDependent(beanName, dep)) throw new BeanCreationException("circular depends on");
                String canonicalDepName = canonicalName(dep);
                singletonRegistry.dependentBeanMap.get(canonicalDepName).add(beanName);
                singletonRegistry.dependenciesForBeanMap.get(beanName).add(canonicalDepName);
                getBean(dep);
            }

            if (mbd.isSingleton()) {
                sharedInstance = getSingleton(String beanName, ()->{
                    // abstract method for subclass returns either the bean or the factory bean
                    return createBean(beanName, mbd, args); 
                });
                bean = getObjectForBeanInstance(sharedInstance, name, beanName, mbd);

            }else if(mbd.isPrototype()){
                beforePrototypeCreation(beanName);
                prototypeInstance = createBean(beanName, mbd, args);
                afterPrototypeCreation(beanName);
                bean = getObjectForBeanInstance(prototypeInstance, name, beanName, mbd);

            }else{ // for a customized scope. The scope should be first registered.
                String scopeName = mbd.getScope();
                final Scope scope = this.scopes.get(scopeName);
                Object scopedInstance = scope.get(beanName, ()->{
                    beforePrototypeCreation(beanName);
                    bean = createBean(beanName, mbd, args);
                    afterPrototypeCreation(beanName);
                    return bean;
                }
                bean = getObjectForBeanInstance(scopedInstance, name, beanName, mbd);
            }
        }
        return bean;
    }
    // in case it returns a factory bean, you need to invoke the factory bean to create the bean.
    protected Object getObjectForBeanInstance(Object beanInstance, String name, String beanName, @Nullable RootBeanDefinition mbd) {
        if(name.startwith("&")){ // indicate you do ask the factory bean.
            if (!(beanInstance instanceof FactoryBean)) {
				throw new BeanIsNotAFactoryException(beanName, beanInstance.getClass());
			}
            return beanInstance;
        }
        // if it's a bean, then return.
        if (!(beanInstance instanceof FactoryBean)) {
			return beanInstance;
		}
        return (FactoryBean<?>) beanInstance).getObject(); // the real implementation use `FactoryBeanRegistrySupport` to add middle layer of cache for singleton object.
    }
}
```

AbstractBeanFactory doesn't define `createBean` method, which leaves to subclass. *`createBean` method is not aware scope, it just create beans.* The `AbstractAutowireCapableBeanFactory` is the next layer class that implements `createBean` method.

__Autowire__: autowire means the ability to inject bean's properties with corresponding values, or references to other beans. The autowires are indicated by `BeanDefinition.property`

```java
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory
		implements AutowireCapableBeanFactory{
    
    protected Object createBean(String beanName, RootBeanDefinition mbd, @Nullable Object[] args){
        if(mbd.getBeanClass() == null){
            mbd.setBeanClass(resolveBeanClass(mbd, beanName);)
        }
        // apply registered BeanPostProcessors, here, you return a proxy object instead of the real one.
        Object bean = resolveBeforeInstantiation(beanName, mbdToUse);
		if (bean != null) {return bean;}
        return doCreateBean(beanName, mbdToUse, args);
    }
    protected Object doCreateBean(final String beanName, final RootBeanDefinition mbd, final @Nullable Object[] args){
        BeanWrapper instanceWrapper = createBeanInstance(beanName, mbd, args); // with wrapper.
        populateBean(beanName, mbd, instanceWrapper); // populate properties. setup reference/value.
        return instanceWrapper.getWrappedInstance();
    }
}
```

#### How does scope 
ToDo
### <a id="reference">V. References</a>
* <a id="http://commons.apache.org/proper/commons-logging/" target="_blank">Apache Commons Logging</a>