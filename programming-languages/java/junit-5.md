Created: 2020-04-04
Modified: 2020-04-04

* [Introduction](#intro)
* [Features & Examples](#example)
* [References](#reference)
***
### <a id="intro">Introduction</a>
JUnit 5 is a Java unit test framework. 

#### How does JUnit5 work?

JUnit 5 provides a runtime engine, this engine can launch and execute the target test methods, and then verify the result.
JUnit 5 is composed of JUnit Platform + JUnit Jupiter + JUnit Vintage
1. JUnit Platform: defines
    * Launcher API: discover, filter, and sumbit execute tests request
    * TestEngine: manage test instance's lifecycle, engine test methods.
    * Junit 4 support engine
    * ConsoleLauncher: a jar that utilizes Launcher API to implement a cli so that we can run tests from commandline.

2. JUnit Jupiter: programming model and extension model (providing @anntotations, e.g. parse anntotation to metadata objects that can be understand by the test engine).
3. Junit Vintage: a TestEngine support JUnit 3 and JUnit4.

The launcher API is also utilized by Gradle and IDE so that you can run unittest from gradle (as a task) and by clicking IDE buttons.

#### Using ConsoleLauncher


#### Using Gradle


#### Using Intellij



### <a id="reference">References</a>
1. <a href="https://junit.org/junit5/docs/current/user-guide/#overview" target="_blank">Junit 5 Overview</a>