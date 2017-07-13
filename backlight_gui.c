#include <gtk/gtk.h> 
#include <stdlib.h>
#include <stdio.h>

GtkAdjustment *adj; 

int get_brightness(void) { 
    FILE *fp;
    int val;
    fp = fopen("/sys/class/backlight/intel_backlight/brightness", "r");
    fscanf(fp, "%d", &val); 
    fclose(fp); 
    return val;
}

int get_max_brightness(void) { 
    FILE *fp;
    int val;
    fp = fopen("/sys/class/backlight/intel_backlight/max_brightness", "r");
    fscanf(fp, "%d", &val);
    fclose(fp); 
    return val;
}

void set_brightness(int val) { 
    FILE *fp; 
    fp = fopen("/sys/class/backlight/intel_backlight/brightness", "w");
    float actual = val/100.f*get_max_brightness(); 
    fprintf(fp, "%d", (int)actual); 
    fclose(fp); 
}

static void range_callback(GtkObject *widget, gpointer data) {
    (void)widget;
    (void)data; 
    int val = gtk_adjustment_get_value(adj); 
    set_brightness(val);
}

static void scale_set_default_values(GtkScale *scale) {
    gtk_range_set_update_policy(GTK_RANGE(scale), GTK_UPDATE_CONTINUOUS);
    gtk_scale_set_draw_value(scale, FALSE);
} 

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) { 
    (void)widget;
    (void)event;
    (void)data;
    return FALSE;
}

static void destroy(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    gtk_main_quit();
} 

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *hscale; 

    gtk_init(&argc, &argv);

    /* Window */
    window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_set_title(GTK_WINDOW(window), "Screen brightness");
    gtk_container_set_border_width(GTK_CONTAINER(window), 0); 
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL); 
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL); 
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE); 

    /* Box */
    box = gtk_hbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_container_set_border_width(GTK_CONTAINER(box), 0); 

    /* Get current brightness in percent */
    float percent = (get_brightness()*100.0f)/get_max_brightness();
    /* Scale */ 
    adj = (GtkAdjustment *) gtk_adjustment_new(percent, 1.0, 101.0, 0.1, 1.0, 1.0); 
    hscale = gtk_hscale_new(GTK_ADJUSTMENT(adj));
    gtk_widget_set_size_request(GTK_WIDGET(hscale), 200, 50);
    scale_set_default_values(GTK_SCALE(hscale)); //Initialize
    gtk_box_pack_start(GTK_BOX(box), hscale, TRUE, TRUE, 0); 
    g_signal_connect(hscale, "value-changed", G_CALLBACK(range_callback), NULL); 
    g_signal_connect(hscale, "button_release_event", G_CALLBACK(destroy), NULL); 


    /* Present */
    gtk_widget_show(hscale);
    gtk_widget_show(box);
    gtk_widget_show(window);

    gtk_main();
    return 0;
}
