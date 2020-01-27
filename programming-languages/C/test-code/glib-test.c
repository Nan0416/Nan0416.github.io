#include <glib.h>
#include <gmodule.h>

static guint qn_str_hash_func(gconstpointer key){
    return 10;
}
static gboolean qn_str_equal (gconstpointer a, gconstpointer b){
    return FALSE;
}
int main()
{
    GHashTable * my_hash = g_hash_table_new (qn_str_hash_func, qn_str_equal);
}