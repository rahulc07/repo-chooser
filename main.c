#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gtk/gtk.h>

typedef struct Transfer {
  // This is to conjoin the repobuffer and the next button together so they can be sent as 1
  GtkEntryBuffer *repobuffer;
  GtkWidget *next_button;
  GtkWidget *status_check;

} transfer;

typedef struct Window_And_Repo {
  GtkEntryBuffer *repobuffer;
  GtkWidget *window;

} window_and_repo;


void test_connection(GtkWidget *button, transfer * magic) {
  char buffer[1000];
  sprintf(buffer, "cd /tmp && wget %s/available-packages/sha256 &> /dev/null\0", gtk_entry_buffer_get_text(magic->repobuffer));
  int exitcode = system(buffer);
  if (exitcode == 0) {
    gtk_widget_set_sensitive(magic->next_button, TRUE);
    gtk_check_button_set_active(magic->status_check, TRUE);
   }
  else {
    gtk_widget_set_sensitive(magic->next_button, FALSE);
    gtk_check_button_set_active(magic->status_check, FALSE);
   }
 
}
void dump(GtkWidget *next, window_and_repo * transfer_next) {
	g_print("%s\n", gtk_entry_buffer_get_text(transfer_next->repobuffer));
	gtk_window_destroy(transfer_next->window);

}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  // Vars
  GtkWidget *window;
  GtkWidget *center_box_v; // Vertical Center box
  GtkWidget *center_box_h; // Horizontal Center Box in the center
  GtkWidget *center_box_h_b; // Horizontal Center Box at the bottom
  GtkWidget *center_box_h_v; // Horizontal Center Box at the top
  GtkWidget *label;
  GtkWidget *box;
  GtkWidget *status_check;
  GtkWidget *grid_center; 
  GtkWidget *entry;
  GtkWidget *test_button;
  GtkWidget *next_button;
  GtkEntryBuffer *repobuffer;
  

  // Window Setup
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Tucana Repo Selector");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
  
 
  // Setup inital grids
  center_box_v = gtk_center_box_new();
   // Margins
  gtk_widget_set_margin_start(center_box_v, 10);
  gtk_widget_set_margin_top(center_box_v, 10);
  gtk_widget_set_margin_end(center_box_v, 10);
  gtk_widget_set_margin_bottom(center_box_v, 10);
  gtk_orientable_set_orientation(center_box_v, GTK_ORIENTATION_VERTICAL);

    // The Horizontal box in the center of the vertical box
  center_box_h = gtk_center_box_new();
  gtk_center_box_set_center_widget(center_box_v, center_box_h);
    // The Horizontal box at the bottom of the vertical box
  center_box_h_b = gtk_center_box_new();
  gtk_center_box_set_end_widget(center_box_v, center_box_h_b);
    // The Horizontal box at the top of the vertical box
  center_box_h_v = gtk_center_box_new();
  gtk_center_box_set_start_widget(center_box_v, center_box_h_v);

  // Window Label
  label = gtk_label_new("Tucana Repo Connection Tester");
  gtk_center_box_set_center_widget(center_box_h_v, label);
  /* Declare a transfer struct used to get the repo contents and 
  close the window */
  window_and_repo * transfer_next = malloc(sizeof(window_and_repo));
  transfer_next->window = window;
  
  // Next Button
  next_button = gtk_button_new_with_label("Next");
  g_signal_connect(next_button, "clicked", dump, transfer_next);
   // Gray it out by default
  gtk_widget_set_sensitive(next_button, FALSE);
  gtk_center_box_set_end_widget(center_box_h_b, next_button);

  // Entry Setup
  repobuffer = gtk_entry_buffer_new(NULL, -1);
  transfer_next->repobuffer = repobuffer;
  entry = gtk_entry_new_with_buffer(repobuffer);
  gtk_center_box_set_start_widget(center_box_h, entry);

  // Checkbox 
  status_check = gtk_check_button_new_with_label("Status Check");
  gtk_widget_set_sensitive(status_check, FALSE);
  gtk_center_box_set_start_widget(center_box_h_b, status_check);
  
  // Initalize the struct with the next button and the repo
  transfer *magic = malloc(sizeof(transfer));
  magic->repobuffer = repobuffer;
  magic->next_button = next_button;
  magic->status_check = status_check;

  // Setup the test button
  test_button = gtk_button_new_with_label("Test Connection");
  g_signal_connect(test_button, "clicked", test_connection, magic);
  gtk_center_box_set_end_widget(center_box_h, test_button);

  // Boiler plate nonsense
  gtk_window_set_child (GTK_WINDOW (window), center_box_v);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.tucana.reposelect", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
