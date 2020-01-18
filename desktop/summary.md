Created: 2020-01-15
Modified: 2020-01-15

* [How is a GUI made?](#intro)
* [References](#reference)
***
### <a id="intro">Introduction</a>

1. bitmap (raster graphics): a dot matrix data structure used to store and display image. The data structure can be placed inside display's memory to display images.

2. Windowing System (e.g. X window system): the software component used to draw elements and print text on display. A layer on the top of kernel.

3. X Window System （X11) and X.Org: X11 is a windowing system which defines standard, X.Org is one of the implementations. 
* X11 defines a client-server model. Each application is a client to ask the X server draw something. 
* X11 can monitor keyboard, mouse and send events to the corresponding client.

4. Window Manager: create window objects on top of window system. (e.g. ask window system to draw windows). It also control the appearance of windows.

5. Desktop environment: including a window manager but also system menus, starter and taskbar of Microsoft Windows, Dock and Launchpad of MacOS. Each desktop environment would has its own specials. 

6. GUI toolkit/Widget toolkit: a library or a framework that has a few predefined widgets, such as button, text input. Example of GUI toolkit includes GTK, Qt.

```
Desktop Application
GUI toolkit (GTK, QT), Window Manager (Mutter, KWin, Compiz)
Windowing System (X.Org)
Kernel
Hardware
```
<span style="color:red">
Input of windowing system is a bitmap, the computation/creation of bitmap is done at top layel. e.g. Window Manager, GUI toolkit, or desktop application.
</span>

#### X Server and X window manager
X Server uses a client-server architecture, where a client utilize client library (xlib) to send request to X server.
The request can be "creating a window" request that should be handled by window manager. In this case, the request will be forward to a window manager program from the X server. And then the window manager will than utilize X server to draw windows.
```
x client (xlib, xcb)
  | (ICCCM)
X server <-> X Window Manager
```

### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>