Created: 2020-04-04
Modified: 2020-04-04

* [JUnit 4](#junit4)
* [Mockito](#mockito)
* [PowerMockito](#powermockito)
* [References](#reference)
***
### <a id="junit4">JUnit 4</a>
JUnit 4 is a Java unit test framework. 

#### Lifecycle
A test class con contain multiple test method. __Each test method will have a new instance of the test class.__
The lifecycle of a test instance is managed by annotation. 
1. The first running method is `@BeforeClass`, which is a static method and only run once for all test method. (This is not compile time error if the method is not static, but it throws runtime error.)
2. The next method is `@Before`, which runs every time before running a test method. So writing common initial logic here.
3. `@Test` the test method. (A test class at least should contain a test method, otherwise runtime exception.)
4. The next method is `@After`, which runs after every test method. It runs even if the test method failed.
5. The final method is `@AfterClass` and must be static, which runs after all test methods finish.

@BeforeClass, @Before, @After and @AfterClass are optional.

```java
import org.junit.BeforeClass;
import org.junit.Before;
import org.junit.Test;
import org.junit.AfterClass;
import org.junit.After;

public class MyTest {
    private static String message;
    @BeforeClass
    public static void classInit(){
        message = "expensive operation";
        System.out.println("Class Init, " + message);
    }

    @Before
    public void instanceInit(){ // runtime error if static
        System.out.println("instance Init");
    }

    @Test
    public void test1(){ System.out.println("Test 1");}

    @Test
    public void test2(){ Assert.fail("just fail");}

    @After
    public void instanceDestroy(){ System.out.println("instance end"); }

    @AfterClass
    public static void classDestroy(){ System.out.println("class end");}
}
``` 

#### Assertion
Assertion can statement (java function) that for testing certain condition. If the condition is not met, then the test method will be failed.

The org.junit.Assert defines a list of static methods for assert operations. They are usually import as static method. for example,
```java
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.failNotNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertNotSame;
import static org.junit.Assert.assertThat;
```

Each assert* method has different overload. 
0. the implementation of each assert method is to return on success and throw error on failure.
1. every assert* method has (string message) version and without message version. (the message is the first argument, which is moved to the last in Junit 5.)
2. assertEquals has different versions on primary types, and a version on Object type.
On object: return true if both are null or a.equals(b)

On double comparsion, it can also supply a delta to setup a threshold.
3. assertArrayEquals also have different overrload.
4. assertSame: compare reference.
5. assertThat(String reason, T actual, Matcher<? super T> matcher), the Matcher defines a function that user implement to check if the data is correct.

### <a id="mockito">Mockito</a>
Mockito is a mock framework, it can create stub on any objects (__not include static method__). The current (2020-04) version of Mockito is v2. These stub objects are usually the dependency of the target object that you want to test. To eliminate external influence and focus on the target object logic, you create stubs and explicitly control their behavior.

Junit4 provides a `Runner` interface that 3rd party library can extend its functionality. For example, the Mockito provides a
`MockitoJUnitRunner` implementation, which automatically create stub for property that annotated with `@Mock`.

A stub is an object of a specific type (class or interface). The object (stub) behavior can be configured and controlled. And you can also verify if certain operations 
are done on this stub.

#### Configuration
Using `when()` to configure the condition, it returns `<T> OngoingStubbing<T>` that defines certain outcome.
1. You can build a chain of the outcome, each call to `.then*` also return a new `OngoingStubbing`.
2. outcome: `.thenReturn(value ...)`. You can return a series of result that each call return a new one.
3. outcome: `.thenThrow(exception)`.
4. outcome: `.thenCallRealMethod()`.
5. condition are the input of the target method, there are two ways to define condition, explicit value or matcher. (matcher and value cannot be mixed)
6. condition (explicit value): given the explicit value.
7. condition (matcher): e.g. `any`, `anyString`, `eq` ... e.g. `when(weatherService.getWeather(anyString(), eq("CN"))). xxx`

**It will return null if non condition matched**
__it has no meaning to configure a method that return void__ The stub object just ignore the call on these methods.


```java
interface WeatherService{
    String getWeather(String location) throws Exception;
}
@RunWith(MockitoJUnitRunner.class)
public class TestClass{
    @Before
    public void init() throws Exception{
        when(weatherService.getWeather("xyz")) // setup with explicit value, when throws exceptions if the method throws.
                .thenThrow(new Exception("Unknown location"))
                .thenReturn("Sunny", "Rain", "Snow");
    }
    @Test
    public void test1() throws Exception{
        try{
            weatherService.getWeather("xyz");
        }catch (Exception e){
            System.out.println(e.getMessage());
            System.out.println(weatherService.getWeather("xyz")); // Sunny
            System.out.println(weatherService.getWeather("xyz")); // Rain
            System.out.println(weatherService.getWeather("xyz")); // Snow
            System.out.println(weatherService.getWeather("xyz")); // Snow
        }
    }
}
```

#### Verification
Verify certain methods with certain conditions have been invoked [certain times]. e.g. 

```java
verify(weatherService).getWeather(eq("Bothell"), anyString());
verify(weatherService, times(4)).getWeather(eq("xyz"), anyString());
verify(weatherService, never()).getWeather(eq("unknown"), anyString());
``` 

#### Injection
1. Mockit create stubs for all class member that annotated with `@Mock`. 
2. It also help create the target object annotated with `@InjectMocks` by injecting `@Mock` objects as dependencies. 
To enable this features, you need to call
```java
    @Before
    public void init() {
        MockitoAnnotations.initMocks(this);
    }
```


#### packages
```java
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;
import org.mockito.InjectMocks;

import static org.mockito.Mockito.when;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.nevers;

import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
```
### <a id="powermockito">PowerMockito</a>
Mockito only allows to create stub for objects, so there is no way to create stub for static method since they are controlled by class.

PowerMockito is built on top of Mockito by the same team to support static method stub. 

```java
// example static method
public class Configuration{
    public static String getAddress();
    public static String convert(String ip);
}

public class TargetClass{
    public String getName(){
        if(this.name == null){
            return Configuration.getAddress();
        }else{
            return this.name.toUpperCase();
        }
    }
}
```

```java
@RunWith(PowerMockRunner.class) // don't use MockitoJUnitRunner.class
@PrepareForTest(Configuration.class)
public class TestClass{
    @Before
    public void init() {
        PowerMockito.mockStatic(Configuration.class);
        Mockito.when(Configuration.getName()).thenReturn("0.0.0.0");

        PowerMockito.verifyStatic(Configuration.class); // called each time before verifying a method.
        Configuration.getName(); // verify it is invoked.

        PowerMockito.verifyStatic(Configuration.class, Mockito.times(2)); // 
        Static.convert(Mockito.any());
    }
}

```
### <a id="reference">References</a>
1. <a href="https://javadoc.io/static/org.mockito/mockito-core/3.3.3/org/mockito/InjectMocks.html" target="_blank">Annotation Type InjectMocks</a>
2. <a href="https://github.com/powermock/powermock/wiki/mockito" target="_blank">PowerMockito</a>