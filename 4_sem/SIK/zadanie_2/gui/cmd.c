#include <string.h>
#include <stdio.h>

#include "gui1.h"

int process_command (int numtok, char *tokens[]) {
  if (strncmp(tokens[0], "NEW_GAME", 8) == 0) {
    // Clear old players labels
    if (ilgracz > 0)
      for (int i = 0; i < ilgracz; i++) {
        gtk_widget_destroy(kolgracz[i].label);
        kolgracz[i].label = NULL;
      }

    area_width = atoi(tokens[1]);
    area_height = atoi(tokens[2]);
    gtk_widget_set_size_request(drawing_area, area_width - 2, area_height - 2);
    gtk_widget_set_size_request(drawing_area, area_width, area_height);
    ilgracz = numtok - 3;
    for (int i = 0; i < ilgracz; i++) {
      GtkWidget *label;

      strcpy(kolgracz[i].player, tokens[i + 3]);
      label = gtk_label_new(tokens[i + 3]);
      kolgracz[i].label = label;
      gtk_widget_modify_fg(label, GTK_STATE_NORMAL, &(kolgracz[i].color));
      gtk_box_pack_start(GTK_BOX(player_box), label, FALSE, FALSE, 3);
      gtk_widget_show(label);
    }
    printf("NEW_GAME command accepted\n");
    return 1;
  }
  else if (strncmp(tokens[0], "PIXEL", 5) == 0) {
    if (numtok == 4) {
      draw_brush(drawing_area, atoi(tokens[1]), atoi(tokens[2]), tokens[3]);
      printf("PIXEL command accepted\n");
      return 1;
    }
    else return 0;
  }
  else if (strcmp(tokens[0], "PLAYER_ELIMINATED") == 0) {
    if (numtok == 2) {
      int index = find_player_index(tokens[1]);
      char buf[66];

      memset(buf, 0, sizeof(buf));
      strcpy(buf, tokens[1]);
      strcat(buf, " X");
      gtk_label_set_text(GTK_LABEL(kolgracz[index].label), buf);
      printf("PLAYER_ELIMINATED command accepted\n");
      return 1;
    }
    else return 0; 
  }
  else
    printf("Wrong command\n");
  return 0;
}

/*EOF*/
