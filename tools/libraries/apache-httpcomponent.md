Created: 2020-04-11
Modified: 2020-04-11

* [Introduction](#intro)
* [Examples](#examples)
* [References](#reference)
***
### <a id="intro">Introduction</a>
`The Apache HttpComponents™ project is responsible for creating and maintaining a toolset of low level Java components focused on HTTP and associated protocols`.
As 2020-04, it has 3 components
* HttpComponents Core: a low level HTTP transport component for both client-side and server-side. It provides both blocking I/O model and async/event driven I/O based on Java NIO.
* HttpClient: a http client support HTTP/1.1. It provides functions for (client-side) auth, http sessions, http cookie.
* AsyncClient: an async http client.

(This notes focus on HttpClient)

This project has two versions currently,
* 4.5: 
* 5.0: 

(This notes use 4.5)

Functions provided by HttpClient:
* The actual data sent/received are byte[], and this library helps convert Java String to/from byte[] using the proper encoding method. 
* https certificate verification.
* Send request through proxy
* support concurrency, an HTTP client can send requests to multiple domain at the same time, provide TCP reusability.


Functions not provided by HttpClient:
* It's content agnostic, meaning it doesn't support serialize/deserialize http content from/to Java object.

#### Concept
* Entity: (an entity control the standard used to encode/decode String to byte[])
* chunked encoding for this entity:?
* Expect: 100-Continue `RequestExpectContinue`

#### RequestConfig
* RequestConfig (Used in `DefaultRedirectStrategy`)
    * relativeRedirectsAllowed, true indicate allow relative redirection. (HTTP standard require absolute uri)
    * circularRedirectsAllowed, allow circle in the redirection.
* RequestConfig (Used in `RedirectExec`)
    * redirectsEnabled: allow redirect

#### Internal
1. HttpGet/Post are the request objects that contain headers, entity and RequestConfig. It also provides a method to abort this request.
2. HttpClient. A http client object is most likely obtained from `HttpClients.custom()`, which returns a `HttpClientBuilder` that you can configure the http client.
The type of the http client is `InternalHttpClient`.
3. The `InternalHttpClient` object can be reused to send mutliple requests. It has 
    * connection manager: manage TCP connection, reuse them.
    * cookie store/registry: manage cookie
    * auth scheme: authentication
    * a chain of `ClientExecChain` process the request, send request, receive response and process response.
4. The default chain is `RedirectExec` -> `RetryExec` ->  `ProtocolExec` -> `MainClientExec`, where `ProtocolExec` manage a list of request processors and response processor.
Request processors are used to process request before sending it, operation including: setup header (content length, origin, cookies from cookie store)
Response processor are used to process response after receving it, operation including: add cookies to the cookie store, decode byte[] entity to java string entity.

5. Each request will have its own `HttpClientContext` when execute, the context is basically a key-value store. 


### <a id="examples">Examples</a>

1. Register a response processor to print all the cookie returned.
```java
class PrintCookie implements HttpResponseInterceptor{
    @Override
    public void process(HttpResponse response, HttpContext context) throws HttpException, IOException {
        System.out.println("Print cookies");
        final HttpClientContext clientContext = HttpClientContext.adapt(context);
        CookieStore cs = clientContext.getCookieStore();
        for(Cookie c: cs.getCookies()){
            System.out.println(c);
        }
    }
}
public class Main{
    public static void main(String[] args) {
        HttpGet httpGet = new HttpGet("https://www.google.com/");
        CloseableHttpClient httpclient = HttpClients.custom()
                .setUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.163 Safari/537.36")
                .addInterceptorLast(new PrintCookie())
                .build();
        }
    }
}
```

### <a id="reference">References</a>
1. <a href="https://hc.apache.org/index.html" target="_blank">Apache HttpComponents (HC) homepage</a>
2. <a href="https://hc.apache.org/httpcomponents-client-ga/tutorial/html/" target="_blank">HttpClient Tutorial</a>
3. <a href="https://git-wip-us.apache.org/repos/asf" target="_blank">A List of Apache Software Foundation project source code.</a>
4. <a href="https://www.baeldung.com/httpclient-post-http-request" target="_blank">Posting with HttpClient</a>
5. <a href="https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers" target="_blank">General Header, Request Header, Response Header, Entity Header</a>


```java
// header can exist in both http request and http response
// each header may have multiple key-value pairs (set-cookies/accept-language), each one is called an element. Elements are splited by ,
// for example, accept-language: en-US,en;q=0,zh-CN;q=0.8,zh;q=0.7
// contains 4 elements: 
// 1. en-US
// 2. en;q=0
// 3. zh-CN;q=0.9
// 4. zh;q=0.7
// each element can futher contains multiple or 2? key-value pair
// in the form of name[=[value]][;...]
 
interface Header extends NameValuePair{ // NameValuePair {getName(), getValue() both return string}
    HeaderElement[] getElements() throws ParseException; // HeaderElement {getName, getValue, NameValuePair[] }
}
interface HttpMessage{
    ProtocolVersion getProtocolVersion(); // {String protocol, int major, int minor}
    boolean containsHeader(String name);
    Header[] getHeaders(String name);
    void addHeader(Header header); 
    void setHeader(Header header); // a header can have duplicated name, this set* will override the pervious one.
    void removeHeader(Header header);
}
abstract class AbstractHttpMessage implements HttpMessage{ // provide header operation
    protected HeaderGroup headergroup; // the real header to implement the operations on headers, e.g. addHeader, ...
}

interface HttpRequest extends HttpMessage{ // An HttpRequest contains the "first line of message", headers and optional request body.
    RequestLine getRequestLine(); // e.g. GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.1
}

interface HttpExecutionAware { // to be notified when blocking IO operations that is cancelled.
    boolean isAborted();
    void setCancellable(Cancellable cancellable); // Cancellable, an interface defined a cancel method.
}

abstract class AbstractExecutionAwareRequest extends AbstractHttpMessage implements
HttpExecutionAware, Cloneable, HttpRequest{ // provide a concurrent safe wrapper for setting and aborting the cancellable object.

}

interface HttpUriRequest extends HttpRequest{
    String getMethod();
    URI getURI();
    void abort() throws UnsupportedOperationException;
    boolean isAborted(); // test if the request has been aborted.
}
abstract class HttpRequestBase extends AbstractExecutionAwareRequest 
implements HttpUriRequest, Configurable { // basic implementation of HttpUriRequest.
// Configurable defines RequestConfig getConfig();
// the RequestConfig is an immutable class that contains configuration such as timeout, max-redirect, sender ip, ...
}

// =========================================================================================================================
// HttpEntity
// =========================================================================================================================
interface HttpEntity{
    boolean isRepeatable(); // stream data is not repeatable, in-memory usually repeatable
    InputStream getContent();
    void writeTo(OutputStream outStream); // writeTo and getContent are repeatable if isRepeatable is true.
    boolean isStreaming(); // indicate if the underly entity depends on a stream, for example, it may read directly from socket.
    Header getContentEncoding(); //
    boolean isChuncked(); 
    // for a http response, it is usually returned as {streaming: true, repeatable: false, chuncked: false}
}

abstract class AbstractHttpEntity implements HttpEntity{
    protected static final int OUTPUT_BUFFER_SIZE = 4096;

    protected Header contentType;
    protected Header contentEncoding;
    protected boolean chunked;
    public void setContentType(String ctString){
        this.contentType = new BasicHeader("Content-Type", ctString);
    }
    public void setContentEncoding(String ceString){
        this.contentEncoding = new BasicHeader("Content-Encoding", ceString);
    }
}
class ContentType{
    public static final ContentType APPLICATION_ATOM_XML = create(
            "application/atom+xml", Consts.ISO_8859_1);
    public static final ContentType APPLICATION_JSON = create(
            "application/json", Consts.UTF_8);
    // ...
}
class StringEntity extends AbstractHttpEntity{
    protected final byte[] content;
    public StringEntity(final String string, final ContentType contentType){
        this.content = string.getBytes(contentType.charset);
        setContentType(contentType.mimeType + ";" + contentType.charset);
    }
    public boolean isStreaming() {
        return false;
    }
    public InputStream getContent() throws IOException {
        return new ByteArrayInputStream(this.content);
    }
}

// =========================================================================================================================
// HttpContext
// =========================================================================================================================

interface HttpContext{} // a key-object store. (e.g. map<String, Object>)
class BasicHttpContext implements HttpContext{} // an implementation based on ConcurrentHashMap
class HttpCoreContext implements HttpContext{} // an adapter (wrapper around the real context), also defined a list key, .e.g HTTP_CONNECTION, HTTP_REQUEST, 
class HttpClientContext extends HttpCoreContext{ // defines more attributes for http specific task.
    /**
    1. RouteInfo getHttpRoute(); / set
    2. CookieStore getCookieStore()
    3. CookieSpec getCookieSpec()
    4. CookieOrigin getCookieOrigin()
    5. List<URI> getRedirectLocations()
    6. CredentialsProvider getCredentialsProvider()
    7. AuthCache getAuthCache()
    8. RequestConfig getRequestConfig()
    */
}

// =========================================================================================================================

interface RouteInfo{ // contains the route info for this request, including all proxy address and the target address.
    HttpHost getTargetHost(); // {hostname, port, shemeName, InetAddress}
    InetAddress getLocalAddress();  
    int getHopCount(); // 1 + #of proxy.
    HttpHost getProxyHost();
} 

final class HttpRoute implements RouteInfo{} // implementation of RouteInfo, this class primarily hold data.
// =========================================================================================================================

interface HttpConnection{
    void close() throws IOException;
    boolean isOpen();
    boolean isStale(); // test if the connection is still usable.
    void setSocketTimeout(int timeout);
    void shutdown() throws IOException;
    HttpConnectionMetrics getMetrics(); // interface {# HTTP request in this TCP connection, SentBytesCount}
}
interface HttpClientConnection extends HttpConnection{
    void sendRequestHeader(HttpRequest request)
    sendRequestEntity(HttpEntityEnclosingRequest request)
    boolean isResponseAvailable(int timeout) // Checks if response data is available from the connection
    //...
}
interface ConnectionRequest extends Cancellable{
    HttpClientConnection get(long timeout, TimeUnit timeUnit) // get a connection within the given time, otherwise throw exceptions.
}
interface HttpClientConnectionManager{ // the implementation may manage a pool of connection to make TCP connection reusable.
    ConnectionRequest requestConnection(HttpRoute route, Object state); // return a connection, the ConnectionRequest is a wrapper of HttpClientConnection.
    void releaseConnection(HttpClientConnection conn, Object newState, long validDuration, TimeUnit timeUnit); // release a TCP back to the pool.
    void connect(HttpClientConnection conn, HttpRoute route, int connectTimeout, HttpContext context) throws IOException; // open the TCP connection with a timeout.
    void upgrade(HttpClientConnection conn, HttpRoute route, HttpContext context) throws IOException; //upgrade http to http ssl after connected.
    // ...
}
class PoolingHttpClientConnectionManager implements HttpClientConnectionManager, ConnPoolControl<HttpRoute>{
    // a pool of TCP connections, each route (domain) is assign multiple connection. 
}

interface ConnectionSocketFactory{ // A factory for creating and connecting connection sockets.
    Socket createSocket(HttpContext context) throws IOException;
    Socket connectSocket(int connectTimeout, Socket sock, HttpHost host, InetSocketAddress remoteAddress,InetSocketAddress localAddress,HttpContext context) throws IOException;
    // timeout if cannot establish the connection within given time.
}

class PlainConnectionSocketFactory implements ConnectionSocketFactory{ // a concrete class for creating and opening socket.
    void sample(){
        Socket sock = new Socket();
        sock.bind(localAddress);
        sock.connect(remoteAddress, connectTimeout);
    }
}

interface LayeredConnectionSocketFactory extends ConnectionSocketFactory{ // create a layered socket (SSL) over an  existing socket.
    Socket createLayeredSocket(Socket socket,String target,int port,
        HttpContext context) throws IOException, UnknownHostException;
}

class SSLConnectionSocketFactory implements LayeredConnectionSocketFactory{

}

// =========================================================================================================================

class HttpRequestExecutor{
    HttpResponse execute(HttpRequest request, HttpClientConnection conn, HttpContext context); // send http request over the conn, and also set conn object to the conn,
    // the conn control timeout for receiving response after sending request. 3000ms by default.
}

// =========================================================================================================================

interface ClientExecChain { 
    CloseableHttpResponse execute(
            HttpRoute route,
            HttpRequestWrapper request,
            HttpClientContext clientContext,
            HttpExecutionAware execAware) throws IOException, HttpException;
}
// When you build the http client, it setup a chain of this interface, (Chain of Responsibility), 
// e.g. [BackoffStrategyExec] -> RedirectExec -> [ServiceUnavailableRetry] -> RetryExec ->  ProtocolExec -> MainClientExec

// =========================================================================================================================
// RedirectExec
// =========================================================================================================================
interface RedirectStrategy{ // determine if the request should be redirect to a new location according to the Http response.
    boolean isRedirected(HttpRequest request,HttpResponse response,HttpContext context); // return true if this request should be redirect after receiving the response.
    HttpUriRequest getRedirect(HttpRequest request,HttpResponse response, HttpContext context); // get the new direction.
}
interface HttpRoutePlanner{ 
    HttpRoute determineRoute(HttpHost target, HttpRequest request, HttpContext context) throws HttpException;
    // for example, returns new HttpRoute(target, local, proxy, secure);
}
// the implementation of this class takes consideration of target and proxy if used, and also set port if not set yet.
class RedirectExec implements ClientExecChain{
    public RedirectExec(ClientExecChain requestExecutor, HttpRoutePlanner routePlanner, RedirectStrategy redirectStrategy);
    // requestExecutor is the next exectutor in the chain
    // routePlanner returns a HttpRoute
    execute(){
        maxRedirects = config.getMaxRedirects() > 0 ? config.getMaxRedirects() : 50;
        redirectCount = 0;
        while(true){
            response = requestExecutor.execute(currentRoute, currentRequest, context, execAware); // first invoke next executor.
            if(config.isRedirectsEnabled() && redirectStrategy.isRedirected(...)){
                if(redirectCount >= maxRedirects){
                    throw error;
                }
                redirectCount++;
                HttpRequest redirect = this.redirectStrategy.getRedirect(response, ...);
                redirect.setHeaders(original.getAllHeaders());
                currentRequest = HttpRequestWrapper.wrap(redirect);
                // reset context.target Auth.
            }else{
                return response;
            }
        }
    }
}
// =========================================================================================================================
// RetryExec
// =========================================================================================================================
interface HttpRequestRetryHandler{
    boolean retryRequest(IOException exception, int executionCount, HttpContext context); // true indicate should retry.
}
class RetryExec implements ClientExecChain{
    execute(){
        for(int count = 0; count++;){
            try{
                return requestExecutor.execute(currentRoute, currentRequest, context, execAware); // try invoke next, if success, then return.
            }catch(IOException e){
                if(execAware != null && execAware.isAborted()){throw e; // already aborted}
                if(retryHandler.retryRequest(ex, count, context)){ // ask the handler, the default handler has a max 3 re-try.
                    // reset header.
                }else{
                    throw e;
                }
            }
        }
    }
}
// =========================================================================================================================
// ProtocolExec
// =========================================================================================================================
interface HttpRequestInterceptor {
    void process(HttpRequest request, HttpContext context); // process request before send
}
interface HttpResponseInterceptor{
    void process(HttpResponse response, HttpContext context); // process response before message body is evaluated.
}
interface HttpProcessor extends HttpRequestInterceptor, HttpResponseInterceptor{
}
class ImmutableHttpProcessor implements HttpProcessor{
    // this class can contains a list of HttpRequestInterceptor and HttpResponseInterceptor, and apply them at once.
    // the order is same as added order.
}
{

RequestDefaultHeaders(defaultHeaders) // add default header to request.
RequestContent() // for a request with entity, it adds content length to header. It also added content-type and content-encoding to the header (if known from entity and not set)
RequestTargetHost() // adding the target host target, e.g. Host: https://robinhood.com
RequestClientConnControl() // adding Connection:Keep-Alive or Proxy-Connection:Keep-Alive
RequestUserAgent(userAgentCopy) // adding user-agent
RequestExpectContinue(); // Expect: 100-Continue' handshake is enabled entity enclosing methods
RequestAddCookies(); // add all cookies from cookie stores that match the origin
RequestAcceptEncoding(); // add "Accept-Encoding" header, by default, it's "gzip,deflate"
                    b.add(new RequestAuthCache());
ResponseProcessCookies(); // add 'set-cookie' header to cookie store.
ResponseContentEncoding(); // DecompressingEntity based on response header's encoding.
}

ProtocolExec(final ClientExecChain requestExecutor, final HttpProcessor httpProcessor) // constructor
execute(){
    context.setAttribute(HttpCoreContext.HTTP_TARGET_HOST, target);
    context.setAttribute(HttpClientContext.HTTP_ROUTE, route);
    context.setAttribute(HttpCoreContext.HTTP_REQUEST, request);
    this.httpProcessor.process(request, context); // invokes all request processor.
    response = this.requestExecutor.execute(route, request, context, execAware); // invoke next executor.
    context.setAttribute(HttpCoreContext.HTTP_RESPONSE, response);
    this.httpProcessor.process(response, context); // invokes all response processor.
    return response;
}
// =========================================================================================================================
// MainClientExec
// =========================================================================================================================
class MainClientExec implements ClientExecChain{// the central point for sending requests. It has the connManager, requestExecutor.
    public MainClientExec(
        final HttpRequestExecutor requestExecutor,
        final HttpClientConnectionManager connManager,
        final ConnectionReuseStrategy reuseStrategy,
        final ConnectionKeepAliveStrategy keepAliveStrategy,
        final HttpProcessor proxyHttpProcessor,
        final AuthenticationStrategy targetAuthStrategy,
        final AuthenticationStrategy proxyAuthStrategy,
        final UserTokenHandler userTokenHandler){}
    execute(){
        // execAware = (HttpExecutionAware) request; this is done in the InternalHttpClient's doExecute;
        ConnectionRequest connRequest = connManager.requestConnection(route, userToken);
        execAware.setCancellable(connRequest);
        RequestConfig config = context.getRequestConfig();
        timeout = config.getConnectionRequestTimeout();
        HttpClientConnection managedConn = connRequest.get(timeout > 0 ? timeout : 0, TimeUnit.MILLISECONDS);
        context.setAttribute(HttpCoreContext.HTTP_CONNECTION, managedConn);
        ConnectionHolder connHolder = new ConnectionHolder(this.log, this.connManager, managedConn);
        execAware.setCancellable(connHolder);
        establishRoute(proxyAuthState, managedConn, route, request, context); // open connection
        response = requestExecutor.execute(request, managedConn, context); // set request.
    }


}

// InternalHttpClient -> a chain of executor -> MainClientExec -> requestexecutor -> connection.
// =========================================================================================================================
class InternalHttpClient extends CloseableHttpClient implements Configurable{

}

// when execute a request, the client will hold a list of executor to process the request.


// In order to send a request, you need 
// 1. a request
// 2. a execution-aware object for aborting
// 3. http (client) context 
// 4. request config
// 5. a list of ClientExecChain

// the context hold most configuration things
// e.g. cookieStore, credential provider, the request config

public InternalHttpClient{
    public InternalHttpClient(
            final ClientExecChain execChain, // a chain of executors, e.g redirect,
            final HttpClientConnectionManager connManager,
            final HttpRoutePlanner routePlanner,
            final Lookup<CookieSpecProvider> cookieSpecRegistry,
            final Lookup<AuthSchemeProvider> authSchemeRegistry,
            final CookieStore cookieStore,
            final CredentialsProvider credentialsProvider,
            final RequestConfig defaultConfig,
            final List<Closeable> closeables) {}
}

// =========================================================================================================================
// RequestAddCookies & ResponseProcessCookies
// =========================================================================================================================
interface Cookie{
    name, value, Date getExpiryDate();
    boolean isPersistent();
    String getDomain(); // https://www.example.com:8843
    String getPath();
    boolean isSecure(); // indicate if the cookie need a secure connection  

}
interface CookieStore{
    void addCookie(Cookie cookie);
    List<Cookie> getCookies();
    boolean clearExpired(Date date); // remove all cookie expired at this date.
    void clear(); // clear all
}
interface CookieOrigin{} // = domain.
interface CookieSpec{ // determine if a cookie belongs to a request, parse cookie from http header, convert to cookie to header
    List<Cookie> parse(Header header, CookieOrigin origin);
    List<Header> formatCookies(List<Cookie> cookies);
    void validate(Cookie cookie, CookieOrigin origin); 
    boolean match(Cookie cookie, CookieOrigin origin);
}
interface CookieSpecProvider{
    CookieSpec create(HttpContext context);// create cookieSpec from the context
}
class RequestAddCookies implements HttpRequestInterceptor{
    process(HttpRequest request, HttpContext context){     
        CookieStore cookieStore = clientContext.getCookieStore();
        Lookup<CookieSpecProvider> registry = clientContext.getCookieSpecRegistry();
        policy = config.getCookieSpec() or CookieSpecs.DEFAULT;
        CookieSpecProvider provider = registry.lookup(policy);
        CookieSpec cookieSpec = provider.create(clientContext);
        List<Cookie> cookies = cookieStore.getCookies(); // get all cookie
        CookieOrigin origin = context.getTargetHost ....
        for(cookie: cookies){
            if(cookie.isnotexpired && cookieSpec.match(cookie, origin)){
                request.addcookie
            }
        }
        // save cookie response process time.
        context.setAttribute(HttpClientContext.COOKIE_SPEC, cookieSpec);
        context.setAttribute(HttpClientContext.COOKIE_ORIGIN, cookieOrigin);
    }
}

class ResponseProcessCookies implements HttpResponseInterceptor{
    process(...){
        CookieSpec cookieSpec = clientContext.getCookieSpec();
        CookieStore cookieStore = clientContext.getCookieStore();
        CookieOrigin cookieOrigin = clientContext.getCookieOrigin();
        HeaderIterator it = response.headerIterator(SM.SET_COOKIE); // get all headers of "set-cookie"
        for(header: it){
            final List<Cookie> cookies = cookieSpec.parse(header, cookieOrigin);
            for(cookie: cookies){
                try{
                    cookieSpec.validate(cookie, cookieOrigin); // validate if the cookie has the cookie origin.
                    cookieStore.addCookie(cookie);
                }
            }
        }
    }
}

// =========================================================================================================================
// Auth
// =========================================================================================================================
// this interface automate the process of challenge-response oriented "Authentication" scheme.
// it should be able to understand (parse) challenge sent by target server.

interface AuthScheme{ 
    void processChallenge(final Header header);
    String getSchemeName();
    String getParameter(final String name);
    String getRealm();
    boolean isConnectionBased();
    boolean isComplete(); // Authentication process may involve a series of challenge-response exchanges
}
interface ContextAwareAuthScheme extends AuthScheme{ // provide the Context to this authscheme
    Header authenticate(Credentials credentials,HttpRequest request, HttpContext context); // generate the authentication header.
    // header based authentication, OAuth is body based?
}
abstract class AuthSchemeBase implements ContextAwareAuthScheme{

}
interface AuthSchemeProvider{
    AuthScheme create(HttpContext context);
}
interface AuthCache{ // cache for authscheme
    void put(HttpHost host, AuthScheme authScheme);
    AuthScheme get(HttpHost host);
    void remove(HttpHost host);
    void clear();   
}
class KerberosSchemeFactory implements AuthSchemeProvider{

}
// =========================================================================================================================
// HttpClientBuilder and InternalHttpClient
// =========================================================================================================================
/*
The HttpClientBuilder expose parameters for us to configure, if we don't specify these parameters, this builder will use default value.

And when HttpClientBuilder create the InternalHttpClient, it pass part of these parameters to it.
*/
public InternalHttpClient(
            final ClientExecChain execChain,
            final HttpClientConnectionManager connManager,
            final HttpRoutePlanner routePlanner,
            final Lookup<CookieSpecProvider> cookieSpecRegistry,
            final Lookup<AuthSchemeProvider> authSchemeRegistry,
            final CookieStore cookieStore,
            final CredentialsProvider credentialsProvider,
            final RequestConfig defaultConfig,
            final List<Closeable> closeables)
/*
When calling the client's execute, it will first create an empty context if not given, and then a few attribute to the context.
*** Context per request.
*/
execute(HttpHost target,HttpRequest request,HttpContext context){
    HttpRequestWrapper wrapper = HttpRequestWrapper.wrap(request, target);
    HttpClientContext localcontext = HttpClientContext.adapt(context != null ? context : new BasicHttpContext());
    RequestConfig config = ((Configurable) request).getConfig(); // if the request has the config, otherwise, use the default config from constructor.
    if (localcontext.getAttribute(HttpClientContext.REQUEST_CONFIG) == null) {
        localcontext.setAttribute(HttpClientContext.REQUEST_CONFIG, this.defaultConfig);
    }
    localcontext.setRequestConfig(config);
    localcontext.setAttribute(HttpClientContext.TARGET_AUTH_STATE, new AuthState());
    localcontext.setAttribute(HttpClientContext.PROXY_AUTH_STATE, new AuthState());
    localcontext.setAttribute(HttpClientContext.COOKIESPEC_REGISTRY, this.cookieSpecRegistry);
    localcontext.setAttribute(HttpClientContext.COOKIE_STORE, this.cookieStore); // reuse cookie store passed from Client. 
    localcontext.setAttribute(HttpClientContext.CREDS_PROVIDER, this.credentialsProvider);
    this.execChain.execute(route, wrapper, localcontext, execAware);
}


/**
Think about the Client is a browser, it has a cookie store and manage a TCP connection pool.

*/
```