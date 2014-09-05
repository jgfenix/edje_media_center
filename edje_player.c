/* 
 * just type make and then run with ./edje_player 
 *
 */

#include <Eina.h>
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Edje.h>

#ifndef PACKAGE_DATA_DIR
#define PACKAGE_DATA_DIR "."
#endif

#define WIDTH 800
#define HEIGHT 600

static const char commands[] = \
  "commands are:\n"
  "\tEsc \n"
  "\tLeft \n"
  "\tRight \n"
  "\tEnter \n"
  "\tMouse (in / click)\n\n";


static void
_on_mouse_over(void        *data EINA_UNUSED,
               Evas_Object *edje_obj,
               const char  *emission,
               const char  *source)
{
   fprintf(stdout, "Signal '%s' coming from part '%s'\n\n", emission, source);

   if (!strcmp(emission, "yes_bt,clicked")) 
        ecore_main_loop_quit();
 }

static void
_on_keydown(void        *data,
            Evas        *evas EINA_UNUSED,
            Evas_Object *o EINA_UNUSED,
            void        *einfo)
{
   Evas_Event_Key_Down *ev;
   Evas_Object         *edje_obj;

   ev = (Evas_Event_Key_Down *)einfo;
   edje_obj = (Evas_Object *)data;

   if (!strcmp(ev->key, "Escape")) 
     {
        printf("%s pressed\n", ev->key);
        edje_object_signal_emit(edje_obj,"escape", ""); 
        //ecore_main_loop_quit();
     }
   else if (!strcmp(ev->key, "Right")) 
     {
        printf("'%s' pressed\nsignal: 'main.show.next'\n\n", ev->key);
        edje_object_signal_emit(edje_obj, "main.show.next", "");
        return;
    }
   else if (!strcmp(ev->key, "Left")) 
     {
        printf("%s pressed\nsignal: 'main.show.prev'\n\n", ev->key);
        edje_object_signal_emit(edje_obj, "main.show.prev", "");
        return;
   }
   else if (!strcmp(ev->key, "Return")) 
     {
        printf("%s pressed\nsignal: 'main.show.enter'\n\n", ev->key);
        edje_object_signal_emit(edje_obj, "main.show.enter", "");
        return;
   }
   else
   	printf("Signal '%s' not used\n\n",ev->key );

}

static Evas_Object *create_my_group(Evas *canvas)
{
   Evas_Object *edje;
   edje = edje_object_add(canvas);
   const char  *edje_file = PACKAGE_DATA_DIR"/edje_player.edj";
   if (!edje)
     {
        EINA_LOG_CRIT("could not create edje object!");
        return NULL;
    }

    if (!edje_object_file_set(edje, edje_file , "my_group"))
     {
       int err = edje_object_load_error_get(edje);
       const char *errmsg = edje_load_error_str(err);
       fprintf(stderr, "could not load 'group_name' from theme.edj: %s",
       	errmsg);
 
       evas_object_del(edje);
       return NULL;
    }

   evas_object_move(edje, 0, 0);
   evas_object_resize(edje, WIDTH, HEIGHT);
   evas_object_show(edje);
   return edje;
}

int main(int argc, char *argv[])
{ 
   Ecore_Evas *window;
   Evas *canvas;
   Evas_Object *edje;
      
   ecore_evas_init();
   edje_init();

   window = ecore_evas_new(NULL, 0, 0, WIDTH, HEIGHT, NULL);
   if (!window)
     {
        EINA_LOG_CRIT("could not create window.");
        return -1;
   }
   canvas = ecore_evas_get(window);

   edje = create_my_group(canvas);
   
   if (!edje)
     return -2;
     
   fprintf(stdout, commands);

   evas_object_focus_set(edje, EINA_TRUE);

   evas_object_event_callback_add(edje, EVAS_CALLBACK_KEY_DOWN, _on_keydown, edje);


   edje_object_signal_callback_add(edje, "main.show.audio", "audio_bt",
                                   _on_mouse_over, NULL);

   edje_object_signal_callback_add(edje, "main.selected.audio", "audio_bt",
                                   _on_mouse_over, NULL);


   edje_object_signal_callback_add(edje, "main.show.video", "video_bt",
                                   _on_mouse_over, NULL);

   edje_object_signal_callback_add(edje, "main.selected.video", "video_bt",
                                   _on_mouse_over, NULL);


   edje_object_signal_callback_add(edje, "main.show.configure", "configure_bt",
                                   _on_mouse_over, NULL);

   edje_object_signal_callback_add(edje, "main.selected.configure", "configure_bt",
                                   _on_mouse_over, NULL);


   edje_object_signal_callback_add(edje, "main.show.exit", "exit_bt",
                                   _on_mouse_over, NULL);

   edje_object_signal_callback_add(edje, "main.selected.exit", "exit_bt",
                                   _on_mouse_over, NULL);
   
   edje_object_signal_callback_add(edje, "yes_bt,clicked", "yes_bt",
                                   _on_mouse_over, NULL);
   
   ecore_evas_show(window);
   ecore_main_loop_begin();
   
   edje_shutdown();
   ecore_evas_shutdown();
 
   return 0;
}
