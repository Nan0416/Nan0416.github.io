Created: 2020-04-19
Modified: 2020-04-19

* [URI, URL, URN](#uri)
* [File](#file)
* [References](#reference)
***
### <a id="intro">I. URI, URL, URN</a>
* URI (uniform resource identifier): a way for identifying a resources, it has two specifications, URL and URN.
* URL (uniform resource locator): a specific URI that identifies a resource by the path to the resource.
* URN (uniform resource name): a specific URI that identifies a resource by its unique name.

With URL, you can not only identify a resource but also find the resource. With URN, you only know the "key" of the resource, you need somehow (e.g. a database) to find the location of the resource.

#### Java Implementation
Java has two classes `java.net.URI` and `java.net.URL`. It doesn't have URN class. URN can be represented by URI in java.

##### java.net.URI
The syntax for a URI is `[scheme:]scheme-specific-part[#fragment]`, you can define your own scheme, e.g. `nqqin`.
* URI can be categorized as absolute or relative, opaque or hierachical.
* An opaque URI must be an absolute URI whose `scheme-specific-part` doesn't start with `/`.
```
mailto:java-net@java.sun.com
urn:isbn:096139210x #ISBN
arn:partition:service:region:account-id:resource-id # Amazon Resource Names
```

* A hierarchical URI is either
    * an absolute URI whose scheme-specific part begins with a slash character, or
    * a relative URI, that is, a URI that does not specify a scheme

```
http://java.sun.com/j2se/1.3/
docs/guide/collections/designfaq.html#28
../../../demo/jfc/SwingSet2/src/SwingSet2.java
```
The format of hierarchical URI is `[scheme:][//authority][path][?query][#fragment]`. There are two types of authority, server-based (widely used) or registry-based.
Server-based authority: `[user-info@]host[:port]`

If the `path` starts with `/` then it's absolute, otherwise, relative.

Java `URI` class doesn't parse the scheme-specific-part of a opaque URI, but parse hierarchical URI.

##### URI
URI is a container for a parsed URI. It contains

* scheme
* scheme-specific-part
* authority
* user-info
* host
* port
* path
* query
* fragment

A opaque URI will then only has scheme, scheme-specific-part and optional fragment, but a hierarchical scheme has scheme, scheme-specific-part, path, optional host, user-info, and port...

1. example http url
```
https://docs.oracle.com/javase/7/docs/api/java/net/URI.html
scheme = https
authority = docs.oracle.com
host = docs.oracle.com
path = /javase/7/docs/api/java/net/URI.html
```

2. example ftp url
```
ftp://nan:12345@localhost:6011/home/docs.txt
scheme = ftp
authority = nan:12345@localhost:6011
userInfo = nan:12345
host = localhost
path = /home/docs.txt
port = 6011
scheme-specific-part = //nan:12345@localhost:6011/home/docs.txt
```

3. local file system
```
file:/home/docs.txt
scheme = file
authority = null
path /home/docs.txt
```

URI support three operations: normalization (removing . and .. from path), resolution (convert relative path to absolute path based on another URI), and relativization.

##### java.net.URL
Even though URL is one type of URI, but the URL class is not implemented as a subclass of URI. URI has a method called toURL(), which convert the URI to a URL.
To successfully convert a URI to an URL, it must be
* absolute hierachical URI. (e.g. scheme != null)

In URL world, 
* scheme is called protocol. Each protocol has a handler to __parse__ the URI and set the URL and __open URLConnection__. For example, `sun.net.www.protocol.{protocol}.Handler` 
is dynamic loaded. __You can supply you implement by given -Djava.protocol.handler.pkgs=you.package.name__ .e.g defining you.package.name.nqqin.Handler will handle nqqin protocol.
* fragment is called anchor (reference)

1. local file system
```
file:/home/docs.txt
protocol=file
path=/home/docs.txt
host=""
file=/home/docs.txt
```

The handler has a open connection method to open the URL.
```java
URI fileUri = new URI("file:/Users/nqqin/simple");
URL url = fileUri.toURL();
URLConnection urlConnection = url.openConnection();
urlConnection.connect();
urlConnection.getInputStream().close();
```


### <a id="file>2. File</a>

### <a id="reference">References</a>
1. <a href="https://docs.oracle.com/javase/7/docs/api/java/net/URI.html" target="_blank">Class URI</a>