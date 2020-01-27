Created: 2020-01-22
Modified: 2020-01-22

* [Introduction](#intro)
* [Types](#types)
* [References](#reference)
***
### <a id="intro">Introduction</a>

### <a id="types">Types</a>

```C
typedef int     gint;
typedef gint   gboolean;
typedef unsigned int    guint;
typedef short  gshort;
typedef char   gchar;
typedef signed long gint64;

typedef void* gpointer;
typedef const void *gconstpointer; // 
typedef signed long gintptr;

typedef unsigned long gsize;
```


#### HashTable

```C
#define _GLIB_EXTERN extern    // glib/gmacros.h
#define GLIB_AVAILABLE_IN_ALL                   _GLIB_EXTERN // glib/gversionmacros.h
GLIB_AVAILABLE_IN_ALL gboolean g_str_equal(gconstpointer  v1,gconstpointer  v2); // glib/ghash.h

gboolean g_str_equal (gconstpointer v1, gconstpointer v2) // glib/ghash.c
{
  const gchar *string1 = v1;
  const gchar *string2 = v2;
  return strcmp (string1, string2) == 0;
}
```

glib/ghash.h
guint (*GHashFunc) (gconstpointer key);


### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>