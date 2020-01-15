Created: 2020-01-14
Modified: 2020-01-14

* [Introduction](#intro)
* [ThreadPool](#threadpool)
* [Message Loop on a Thread](#messageloop)
* [IntentService]
* [AsyncTask](#asyntask)
* [References](#reference)
***
### <a id="intro">Introduction</a>
A Android application runs in a process. A process, by default, only has a single thread, which handles all tasks, such as UI and event handling. So the main thread is also called UI thread.

In order to employ multithreading, Android platform utilizes Java concurrency library for multithreading. The basic classes are still `Thread` and `Runnable`. And more advanced classes for multithreading are `ThreadPoolExecutor` and `ExectuorServices`, which manages and run task.

#### Thread Priority

#### Thread group


### <a id="threadpool">ExecutorService & ThreadPoolExecutor</a>

`ExecutorService` defines a interface allows developers to sumbit tasks and shutdown. How to manage and run these tasks belongs to the implemenetation.

The `ThreadPoolExecutor` is an implementation of `ExecutorService`. This implementation creates a thread pool to run these tasks.

* Manages a set of threads to run tasks. 
* API for setting thread factory to control how to create thread, such as priority, name, group.
* Tasks can be submitted at runtime.
* It also has a task queue `BlockingQueue` to pend tasks.
* Callback method on different events, such as `beforeExecute` and `afterExecute`.

#### class structures.
```Java
// how to execute a sumbit task? in the current thread, or in a new thread
// execute in serial order or parallel. A executor may be able to handle multiple runnable.
public interface Executor{
    abstract void execute(Runnable command)
}
 
public interface ExecutorService implements Executor{
    public abstract Future<T> submit (Callable<T> task);
    public abstract void shutdown ();
    public abstract boolean isTerminated (); // a pool is never terminated unless the pool is shutdown.
}

public interface Callable<V>{
    abstract V call()
}
public interface Future<V>{ // represents the result of an asynchronous computation, a wrapper of thread.
    abstract boolean cancel(boolean mayInterruptIfRunning);
    abstract boolean isDone();
    abstract boolean isCancelled();
    abstract V get(); //  wait to get result.
    abstract V get (long timeout, TimeUnit unit) // wait to get result with a max time.
}

/*Object
 * -> AbstractExecutorService implements ExecutorServices.
 * - - -> ThreadPoolExecutor extends AbstractExecutorService.
 */
```


### <a id="messageloop">MessageLoop</a>

A message loop is kind like a event loop or pubsub that run in a single thread. Android message loop is an important mechanism used within the UI thread to handle different event, such as clicking a button.

* A message loop `Looper` should associate with a single thread. (The message loop actually run on the thread)
* A handler object `Handler` is bound to a single message loop, but a message loop can have serveral handler. (a handler is like a subscriber). However, each handler can also add message to the loop's message queue.
* A handler can bind to a looper at runtime.
* A message loop also has a message queue `MessageQueue` object. However, message is not directly add to the message queue, instead, using handler to add message.
* A message `Message` is an implementation of `Parcelable`. It can not only have key-value pair, but also a `Runnable` object.
* <span style="color:red">Accroding to the above, a handler is also bound to thread.</span>The bound thread is the thread for handling the message. In addition, a hanlder can be used by other threads to send message and handle in its bound thread.
* <span style="color:red">A handler will handle the message sent by itself.</span>
```Java
// Handler.enqueueMessage
msg.target = this;
// Looper.loop
msg.target.dispatchMessage(msg);
```

#### Example
You cannot create a loop by given its a thread object (because a loop can only be created on a running thread?). Therefore, creating a loop inside a thread.
```Java
public class LooperThread extends Thread{
    private Looper looper;
    public Looper getLooper(){
        return looper;
    }
    public boolean isReady(){
        return looper != null;
    }
    @Override
    public void run(){
        Looper.prepare(); // initialize a looper on the current thread.
        looper = Looper.myLooper(); // get the current thread's looper.
        looper.loop();
    }
}
public void testLoop(){
    LooperThread lt = new LooperThread();
    lt.start();
    while(!lt.isReady());
    Handler h = new Handler(lt.getLooper()){
        @Override
        public void handleMessage(Message msg){
            Log.d("Threading: " +  Thread.currentThread().toString(), "secret: " + msg.getData().getString("secret"));
        }
    };
    Log.d("Threading: " +  Thread.currentThread().toString(), "sendMessage");
    Message msg = Message.obtain();
    Bundle b = new Bundle();
    b.putString("secret", "231231ewjior, threading" + Thread.currentThread().toString());
    msg.setData(b);
    h.sendMessage(msg);
}
```

#### Source code
1. Looper.prepare is a factory method to create a Looper for the current thread and guarantee a thread only prepare once.
Looper.prepare utilize java's `ThreadLocal` to associate the looper with the current thread.
```Java
private Looper(boolean quitAllowed) {
    mQueue = new MessageQueue(quitAllowed);
    mThread = Thread.currentThread();
}
private static void prepare(boolean quitAllowed) {
    if (sThreadLocal.get() != null)
        throw new RuntimeException("Only one Looper may be created per thread");        }
    sThreadLocal.set(new Looper(quitAllowed));
}
public static void prepare() {
    prepare(true);
}

public static @Nullable Looper myLooper() {
    return sThreadLocal.get();
}

public void loop(){
    for (;;) {
        Message msg =  myLooper().mQueue.next();
        if(msg == null){return;}
        final long dispatchStart = needStartTime ? SystemClock.uptimeMillis() : 0;
        msg.target.dispatchMessage(msg); // a message handler should the message by itself.
        dispatchEnd = needEndTime ? SystemClock.uptimeMillis() : 0;
        slowDeliveryDetected = (dispatchStart - msg.when) <= 10;
    }
}
```

2. MessageQueue & Message
MessageQueue manages `Message` as a doubly LinkedList. Part of `MessageQueue` is developed with native code. Important fields and methods
* enqueueMessage(Message msg, long when); 
```Java
// the enqueue provides a absolute time for which the message should be dequeue.
if(msg.target == null){throw exception}
if(msg.isInUse()){throw exception}
msg.markInUse();
msg.when = when; 
// insert message into the linkedlist.
```

* next(); // return next message.
* mQuitAllowed and quit(); // terminate the queue.


3. A handler should have
* a Looper, by default, it's from `Looper.myLooper()`
* a message queue from looper.mQueue.
* a flag indicate if this handler is async or sync?
* a callback. 
    * a handler can has two place to setup message handler.
    * 1 via the callback of `Handler.Callback`
    * 2 via override `handleMessage` method.

```Java
// dispatchMessage function is invoked by the Looper.
// msg.target.dispatchMessage(msg);
public void dispatchMessage(@NonNull Message msg) {
    if (msg.callback != null) {
        msg.callback.run();
    } else {
        if (mCallback != null) {
            if (mCallback.handleMessage(msg)) {
                return;
            }
        }
        handleMessage(msg);
    }
}

```

sendmessage{
    msg.target = this;
    if (mAsynchronous) {
        msg.setAsynchronous(true);
    }
    queue.enqueueMessage(msg, uptimeMillis);
}




### <a id="reference">References</a>
1. <a href="https://developer.android.com/training/multiple-threads" target="_blank">Android Multithreading</a>