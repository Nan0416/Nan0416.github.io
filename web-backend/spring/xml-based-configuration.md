Created: 2020-04-04
Modified: 2020-04-04

* [XML based configuration](#intro)
* [Context namespace](#context-namespace)
* [MVC namespace](#mvc-namespace)
* [References](#reference)
***
### <a id="intro">I. XML based configuration</a>
Spring framework support XML as the configuration file. In a standalone application, you can use the `ClassPathXmlApplicationContext` or the `FileSystemXmlApplicationContext` to create a application context based on a XML configuration file. The only different between these two classes is how to get the XML resource, as the name suggested. In a servlet application, the `FrameworkServlet` by default uses `XmlWebApplicationContext` as the application context.

*The `XmlWebApplicationContext` has the same `loadBeanDefinitions` method as the `X-XMLApplicationContext`, it doesn't register extra beans for servlet specific usage. For example, `HandlerMapping` is not registered here. But `WebApplicationContext` does register "request", "session" and "application" scope to the bean factory.*

XML based app context essentially uses a `XmlBeanDefinitionReader` object for processing XML file and register beans. Inside the `XmlBeanDefinitionReader` object, it converts the XML resource to a `Document` object and delegate the register operation to a `BeanDefinitionDocumentReader` object. The default implementation is `DefaultBeanDefinitionDocumentReader`. The XML reader has a `NamespaceHandlerResolver` object, which basically a group of namespace handlder. XML reader will delegate each namespace to its corresponding handler. Each handler also contains a group of parser to parse different XML elements.

#### NamespaceHandlerResolver
`NamespaceHandlerResolver` is created by `XmlBeanDefinitionReader` 
```java
public class XmlBeanDefinitionReader extends AbstractBeanDefinitionReader {
    protected NamespaceHandlerResolver createDefaultNamespaceHandlerResolver() {
		ClassLoader cl = (getResourceLoader() != null ? getResourceLoader().getClassLoader() : getBeanClassLoader());
		return new DefaultNamespaceHandlerResolver(cl);
	}
}
```

The default namespace-handler resolver will try to gather handlers from `META-INF/spring.handlers` file in the classpath. For example, some spring jars provide their spring.handlers files e.g. spring-webmvc-5.2.5.RELEASE.jar has

```
http\://www.springframework.org/schema/mvc=org.springframework.web.servlet.config.MvcNamespaceHandler
```

which will delegate operations on the namespace's uri of http://www.springframework.org/schema/mvc to the `MvcNamespaceHandler`.

It also means you can create your own XSD and XML namespace and implement your own handler and parsers for your customized functions.

A parser should implements `BeanDefinitionParser`, which takes the XML element and a ParserContext (`XmlReaderContext` and `BeanDefinitionParserDelegate`)
```java
public interface BeanDefinitionParser {
    BeanDefinition parse(Element element, ParserContext parserContext);
}
```



The `DefaultBeanDefinitionDocumentReader` only support parse element from default namesapce, all other namespace will be delegate to other handlers through the `delegate` object. In addition, the delegate has some basic methods, such as `parseBeanDefinitionElement`

```java
public class DefaultBeanDefinitionDocumentReader implements BeanDefinitionDocumentReader {
    // readerContext contains NamespaceHandlerResolver.
    private XmlReaderContext readerContext;

    // the delegate contains the core operation, it's created as 
    // new BeanDefinitionParserDelegate(this.readerContext);
    private BeanDefinitionParserDelegate delegate;
    protected void parseBeanDefinitions(Element root, BeanDefinitionParserDelegate delegate) {
        if (delegate.isDefaultNamespace(root)) {
			NodeList nl = root.getChildNodes();
			for (int i = 0; i < nl.getLength(); i++) {
				Node node = nl.item(i);
				if (node instanceof Element) {
					Element ele = (Element) node;
					if (delegate.isDefaultNamespace(ele)) {
						parseDefaultElement(ele, delegate);
					}
					else {
						delegate.parseCustomElement(ele);
					}
				}
			}
		}
		else {
			delegate.parseCustomElement(root);
		}
    }
    private void parseDefaultElement(Element ele, BeanDefinitionParserDelegate delegate) {
		if (delegate.nodeNameEquals(ele, IMPORT_ELEMENT)) {
			importBeanDefinitionResource(ele);
		}
		else if (delegate.nodeNameEquals(ele, ALIAS_ELEMENT)) {
			processAliasRegistration(ele);
		}
		else if (delegate.nodeNameEquals(ele, BEAN_ELEMENT)) {
			processBeanDefinition(ele, delegate);
		}
		else if (delegate.nodeNameEquals(ele, NESTED_BEANS_ELEMENT)) {
			// recurse
			doRegisterBeanDefinitions(ele);
		}
	}
}
```

### <a id="context-namespace">II. Context namespace</a>
Context namespace `http://www.springframework.org/schema/context` by default, maps to `org.springframework.context.config.ContextNamespaceHandler`, which is provided in spring-context-xxx.jar

1. component-scan

```xml
<beans xmlns = "http://www.springframework.org/schema/beans"
       xmlns:context = "http://www.springframework.org/schema/context">
<!-- http://www.springframework.org/schema/context/spring-context-3.0.xsd -->
<context:component-scan base-package = "nqqin.app.springframeworkweb" />
</beans>
```

`component-scan` element is mapped to `ComponentScanBeanDefinitionParser`

```java
public class ComponentScanBeanDefinitionParser implements BeanDefinitionParser {
    @Override
	@Nullable
	public BeanDefinition parse(Element element, ParserContext parserContext) {
		String basePackage = element.getAttribute(BASE_PACKAGE_ATTRIBUTE);
        // convert placeholder
		basePackage = parserContext.getReaderContext().getEnvironment().resolvePlaceholders(basePackage);
        // you can register a list of package, splited by ,
		String[] basePackages = StringUtils.tokenizeToStringArray(basePackage,
				ConfigurableApplicationContext.CONFIG_LOCATION_DELIMITERS);

		// The scanner also support other functions, such as filter, name-generator...
		ClassPathBeanDefinitionScanner scanner = configureScanner(parserContext, element);
		Set<BeanDefinitionHolder> beanDefinitions = scanner.doScan(basePackages);
		registerComponents(parserContext.getReaderContext(), beanDefinitions, element);
		return null;
	}
}
```


2. property-placeholder
```xml
<beans xmlns = "http://www.springframework.org/schema/beans"
       xmlns:context = "http://www.springframework.org/schema/context">
<!-- add a properties file for place holder -->
<context:property-placeholder location="classpath:my.properties"/>
</beans>
```

The functions of placeholder replacement is actually implemented by a bean factory post processor `PropertySourcesPlaceholderConfigurer` class, which extends `BeanFactoryPostProcessor` method. When abstract app context refresh, the post process will be applied to replace placeholder. So essentially, the `<context:property-placeholder location="classpath:my.properties"/>` is to register this bean.

#### The Parser Architecture

```java
// return a BeanDefinition to register with the registry, a bean definition usually contains the bean's class, 
// id, alias, lazy?, parent, scope
public interface BeanDefinitionParser {
    BeanDefinition parse(Element element, ParserContext parserContext);
}

// support id and name fields
public abstract class AbstractBeanDefinitionParser implements BeanDefinitionParser {
    public final BeanDefinition parse(Element element, ParserContext parserContext) {
        AbstractBeanDefinition definition = parseInternal(element, parserContext);
        String id = element.getAttribute("id");
        String[] aliases = split(element.getAttribute("name", ","));
        BeanDefinitionHolder holder = new BeanDefinitionHolder(definition, id, aliases);
        registerBeanDefinition(holder, parserContext.getRegistry());
        return definition;
    }
}

// abstract class for these parsers that only register a single bean.
public abstract class AbstractSingleBeanDefinitionParser extends AbstractBeanDefinitionParser{
    protected abstract String getParentName(Element element);
    protected abstract Class<?> getBeanClass(Element element); // another one is getBeanClassName
    protected final AbstractBeanDefinition parseInternal(Element element, ParserContext parserContext) {
        BeanDefinitionBuilder builder = BeanDefinitionBuilder.genericBeanDefinition();
        builder.getRawBeanDefinition().setParentName(getParent(element));
        builder.getRawBeanDefinition().setBeanClass(getBeanClass(element));
        doParse(element, parserContext, builder); // for subclass
        return builder.getBeanDefinition();
    }
}

// set bean's properties from the XML Doc element
abstract class AbstractPropertyLoadingBeanDefinitionParser extends AbstractSingleBeanDefinitionParser {
    protected void doParse(Element element, ParserContext parserContext, BeanDefinitionBuilder builder) {
        builder.addPropertyValue("locations", element.getAttribute("location").split(","));
        builder.addPropertyReference("properties", properelement.getAttribute("properties-ref"));
        builder.addPropertyValue("fileEncoding", lement.getAttribute("file-encoding"));
        builder.addPropertyValue("order", Integer.valueOf(element.getAttribute("order")));
        builder.addPropertyValue("localOverride", Boolean.valueOf(element.getAttribute("local-override")));
        builder.addPropertyValue("ignoreResourceNotFound",
				Boolean.valueOf(element.getAttribute("ignore-resource-not-found")));
        builder.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);
    }
}

// return the bean class
package org.springframework.context.config;
class PropertyPlaceholderBeanDefinitionParser extends AbstractPropertyLoadingBeanDefinitionParser {
    protected Class<?> getBeanClass(Element element) {
        return PropertySourcesPlaceholderConfigurer.class;
    }
}
```
### <a id="mvc-namespace">III. MVC namespace</a>
MVC namespace is handled by `org.springframework.web.servlet.config.MvcNamespaceHandler` provided in spring-webmvc-xxx.jar.

```xml
<!-- http://www.springframework.org/schema/mvc=org.springframework.web.servlet.config.MvcNamespaceHandler -->
<mvc:annotation-driven>
    <mvc:message-converters>
        <bean class="org.springframework.http.converter.StringHttpMessageConverter"/>
        <bean class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter"/>
    </mvc:message-converters>
</mvc:annotation-driven>
```
The `annotation-driven` parser 
1. registers the `RequestMappingHandlerMapping` (handler-mapping) to bean registry as singleton. It also finds if the element provides a `content-negotiation-manager` attribute, if not, it uses `ContentNegotiationManagerFactoryBean` by default, and set it as property reference to the handler-mapping.
handler-mapping: 

2. register `ConfigurableWebBindingInitializer` (web-binding-initilizer), and set associate property references.

3. register `RequestMappingHandlerAdapter`, and setup message converters.

### <a id="reference">References</a>
