#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <glib.h>
#include <gdk/gdkkeysyms-compat.h>
#include <gtk/gtk.h>

#include "err.h"
#include "read_line.h"
#include "gui1.h"

#define BUFFER_SIZE 2000

#define MAX_TOKENS 30

int gsock = -1;  //main socket
int area_width = 10, area_height = 10;

GtkWidget *colorseldlg = NULL;
GtkWidget *drawing_area = NULL;
GdkColor color;

KolGracz kolgracz[20];
int ilgracz = 0;

GtkWidget *player_box;

void init_colors () {
  char *colors[] = {"red", "green", "blue", "yellow", "brown", "cyan",
                    "magenta", NULL};
  GdkColor color;

  for (int i = 0; colors[i] != NULL; i++) {
    if (!gdk_color_parse(colors[i], &color))
      syserr("Invalid color spec");
    kolgracz[i].color = color;
  }
}

int find_player_index (char *player) {
  for (int i = 0; i < ilgracz; i++)
    if (strcmp(kolgracz[i].player, player) == 0)
      return i;
  fprintf(stderr, "Brak gracza w tablicy");
  return -1;
}

// Remove empty tokens

static int remove_empty (char *tokens[], char *result[]) {
  int i;
  int j = 0;

  for (i = 0; tokens[i] != NULL; i++) {
    if (strlen(tokens[i]) != 0)
      result[j++] = tokens[i];
    if (j > 30)
      syserr("Too many tokens");
  }
  result[j] = NULL;
  return j;
}

gboolean started = FALSE;

static gboolean idle_callback (gpointer data) {
  if (started) {
    char buffer[BUFFER_SIZE];
    ssize_t len;

    memset(buffer, 0, sizeof(buffer));
    len = readLine(gsock, buffer, sizeof(buffer));
    if (len < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        return G_SOURCE_CONTINUE;
      else
        syserr("reading error");
    }
    else if (len == 0) {
      printf("Pusty komunikat - koniec połączenia\n");
      close(gsock);
      exit(1);
    }
    else {
      char **raw_tokens, *tokens[MAX_TOKENS + 1];
      int numtok;

      printf("Command:%s\n", buffer);
      raw_tokens = g_strsplit_set(buffer, " \t\n\r", 0);
      numtok = remove_empty(raw_tokens, tokens);

      process_command(numtok, tokens);
    }
  }
  return G_SOURCE_CONTINUE;
}

// Create an Arrow widget with the specified parameters
// and pack it into a button

static GtkWidget *create_arrow_button (GtkArrowType arrow_type, 
                                       GtkShadowType shadow_type) {
  GtkWidget *button;
  GtkWidget *arrow;

  button = gtk_button_new();
  arrow = gtk_arrow_new(arrow_type, shadow_type);

  gtk_container_add(GTK_CONTAINER(button), arrow);
  
  gtk_widget_show(button);
  gtk_widget_show(arrow);

  return button;
}

// Arrow buttons callback functions

static void arrow_pressed (GtkButton *widget, gpointer data) {
  ssize_t len, snd_len;
  char answer[BUFFER_SIZE + 1];

  if (strcmp(data, "left") == 0)
    sprintf(answer, "LEFT_KEY_DOWN\n");
  else
    sprintf(answer, "RIGHT_KEY_DOWN\n");
  len = strlen(answer);
  snd_len = write(gsock, answer, len);
  printf("Wysyłam:%s\n", answer);  //tmp
  if (snd_len != len)
    syserr("writing to client socket");
  started = TRUE;  
}

static void arrow_released (GtkButton *widget, gpointer data) {
  ssize_t len, snd_len;
  char answer[BUFFER_SIZE + 1];

  if (strcmp(data, "left") == 0)
    sprintf(answer, "LEFT_KEY_UP\n");
  else
    sprintf(answer, "RIGHT_KEY_UP\n");
  len = strlen(answer);
  snd_len = write(gsock, answer, len);
  printf("Wysyłam:%s\n", answer);  //tmp
  if (snd_len != len)
    syserr("writing to client socket");
  started = TRUE;  
}

// Backing pixmap for drawing area

static GdkPixmap *pixmap = NULL;  //!!! gtk2

// Create a new backing pixmap of the appropriate size

static gboolean configure_event (GtkWidget *widget, GdkEventConfigure *event) {
  GtkAllocation allocation;

  if (pixmap)
    g_object_unref (pixmap);

  gtk_widget_get_allocation(widget, &allocation);
  pixmap = gdk_pixmap_new(widget->window,
                          allocation.width,
                          allocation.height,
                          -1);
  gdk_draw_rectangle(pixmap,
                     gtk_widget_get_style(widget)->white_gc,
                     TRUE,
                     0, 0,
                     allocation.width,
                     allocation.height);

  return TRUE;
}

// Redraw the screen from the backing pixmap

static gboolean expose_event (GtkWidget *widget, GdkEventExpose *event) {
  gdk_draw_drawable(widget->window,
                    gtk_widget_get_style(widget)->fg_gc[gtk_widget_get_state(widget)],
                    pixmap,
                    event->area.x, event->area.y,
                    event->area.x, event->area.y,
                    event->area.width, event->area.height);

  return FALSE;
}

// Draw a small rectangle on the drawing area

void draw_brush (GtkWidget *widget, gdouble x, gdouble y, char* player) {
  int index = find_player_index(player);

  if (index >= 0) {
    GdkColor color = kolgracz[index].color;
    GdkGC *gc = gtk_widget_get_style(widget)->black_gc;

    gdk_gc_set_rgb_fg_color(gc, &color);
    gdk_draw_point(pixmap,
                   gtk_widget_get_style(widget)->black_gc,
                   x, y);
    gtk_widget_queue_draw_area(widget, x, y, 1, 1);
  }
}

int area_clear (GtkWidget *widget, gpointer data) {
  return TRUE;
}

// Drawing area event handler

void send_message (char* message) {
  ssize_t len = strlen(message);
  ssize_t snd_len = write(gsock, message, len);

  printf("Wysyłam:%s\n", message);  //tmp
  if (snd_len != len)
    syserr("writing to client socket");
}

gint area_event (GtkWidget *widget, GdkEventKey *event, gpointer data) {
  if (event->keyval == GDK_Left) {
    if (event->type == GDK_KEY_PRESS)
      send_message("LEFT_KEY_DOWN\n");
    else if (event->type == GDK_KEY_RELEASE)
      send_message("LEFT_KEY_UP\n");
    else
      syserr("Cud!");
  }
  else if (event->keyval == GDK_Right) {
    if (event->type == GDK_KEY_PRESS)
      send_message("RIGHT_KEY_DOWN\n");
    else if (event->type == GDK_KEY_RELEASE)
      send_message("RIGHT_KEY_UP\n");
    else
      syserr("Cud!");
  }

  started = TRUE;  
  return TRUE;
}

// Exit handler
gint destroy_window (GtkWidget *widget, GdkEvent *event, 
                     gpointer client_data) {
  close(gsock);
  gtk_main_quit();
  return TRUE;
}

// Zaczynamy

gint main (gint argc, gchar *argv[]) {
  GtkWidget *window, *frame;
  GtkWidget *box1, *box2, *box3;
  GtkWidget *event_box;
  GtkWidget *button;
  int idle_id;

  unsigned short port = 12346;  //default

  // Opcjonalny port
  if (argc > 1)
    port = atoi(argv[1]);

  init_net(port);

  // Initialize the toolkit, remove gtk-related commandline stuff
  
  gtk_init(&argc, &argv);

  init_colors();

  idle_id = g_idle_add(idle_callback, &started);
  
  // Create toplevel window, set title and policies/

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW (window), "Netacka GUI");
  gtk_window_set_resizable(GTK_WINDOW (window), TRUE);  /***/

  // Attach to the "delete" and "destroy" events so we can exit

  g_signal_connect(window, "delete-event",
                   G_CALLBACK(destroy_window), (gpointer)window);
    
  // Create main vertical box to pack horizontal boxes into it.
  box1 = gtk_vbox_new(FALSE, 0);
  
  // Create horizontal box for control buttons.
  box2 = gtk_hbox_new(FALSE, 0);

  button = create_arrow_button(GTK_ARROW_LEFT, GTK_SHADOW_ETCHED_IN);
  gtk_box_pack_start(GTK_BOX(box2), button, FALSE, FALSE, 3);
  g_signal_connect(button, "pressed",
                   G_CALLBACK(arrow_pressed), (gpointer)"left");
  g_signal_connect(button, "released",
                   G_CALLBACK(arrow_released), (gpointer)"left");
  
  button = create_arrow_button(GTK_ARROW_RIGHT, GTK_SHADOW_ETCHED_OUT);
  gtk_box_pack_start(GTK_BOX(box2), button, FALSE, FALSE, 3);
  g_signal_connect(button, "pressed",
                   G_CALLBACK(arrow_pressed), (gpointer)"right");
  g_signal_connect(button, "released",
                   G_CALLBACK(arrow_released), (gpointer)"right");

  button = gtk_button_new_with_label("Clear");
  gtk_box_pack_end(GTK_BOX(box2), button, FALSE, FALSE, 3);
  g_signal_connect(button, "clicked",
                   G_CALLBACK(area_clear), (gpointer)drawing_area);

  // Create horizontal box for drawing area and player info.
  box3 = gtk_hbox_new(FALSE, 0);

  // Create drawing area, set size and catch button events

  frame = gtk_frame_new(NULL);
  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(GTK_WIDGET(drawing_area),
                              area_width, area_height);

  color.red = 0;
  color.blue = 65535;
  color.green = 0;
  gtk_widget_modify_bg(drawing_area, GTK_STATE_NORMAL, &color);       

  gtk_widget_set_events(drawing_area,
                        GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

  g_signal_connect(drawing_area, "key-press-event",
		   G_CALLBACK(area_event), "press");
  g_signal_connect(drawing_area, "key-release-event", 
                   G_CALLBACK(area_event), "release");

  // Signals used to handle backing pixmap
  g_signal_connect(drawing_area, "expose-event",
                   G_CALLBACK(expose_event), NULL);
  g_signal_connect(drawing_area, "configure-event",
                   G_CALLBACK(configure_event), NULL);
    
  // Create vertical box to pack dynamic player info labels into it.
  event_box = gtk_event_box_new();
  player_box = gtk_vbox_new(FALSE, 0);
  gtk_widget_set_size_request(player_box, 100, -1);
  {
    GdkColor color;

    if (gdk_color_parse("white", &color))
      gtk_widget_modify_bg(event_box, GTK_STATE_NORMAL, &color);
    else
      syserr("player color");
  }
  gtk_container_add(GTK_CONTAINER(event_box), player_box);
  
  // Pack everything together.

  gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box1), box3, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(frame), drawing_area);
  gtk_box_pack_start(GTK_BOX(box3), frame, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box3), event_box, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), box1);

  gtk_widget_show_all(window);

  gtk_widget_set_can_focus(drawing_area, TRUE);
  gtk_widget_grab_focus(drawing_area);
  
  // Enter the gtk main loop
  gtk_main();

  // Satisfy grumpy compilers
  return 0;
}

/*EOF*/
