/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
    Gpredict: Real-time satellite tracking and orbit prediction program

    Copyright (C)  2001-2009  Alexandru Csete, OZ9AEC.

    Authors: Alexandru Csete <oz9aec@gmail.com>

    Comments, questions and bugreports should be submitted via
    http://sourceforge.net/projects/gpredict/
    More details can be found at the project home page:

            http://gpredict.oz9aec.net/

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, visit http://www.fsf.org/
*/
#ifndef __GTK_SAT_MODULE_H__
#define __GTK_SAT_MODULE_H__ 1

#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>
#include "gtk-sat-data.h"
#include "qth-data.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** \brief The state of a module */
typedef enum {
    GTK_SAT_MOD_STATE_DOCKED = 0,   /*!< The module is docked into the notebook. */
    GTK_SAT_MOD_STATE_WINDOW,       /*!< The module is in it's own window. */
    GTK_SAT_MOD_STATE_FULLSCREEN    /*!< The module is in FULLSCREEN mode :-) */
} gtk_sat_mod_state_t;


/** \brief Module layout */
typedef enum {
    GTK_SAT_MOD_LAYOUT_1 = 0, /*!< one view */
    GTK_SAT_MOD_LAYOUT_2,     /*!< Two views, one above the other */
    GTK_SAT_MOD_LAYOUT_3,     /*!< Three views, big one on top, two small one at bottom */
    GTK_SAT_MOD_LAYOUT_4      /*!< Three views, big one on bottom, small ones on top */
} gtk_sat_mod_layout_t;


/** \brief View types */
typedef enum {
    GTK_SAT_MOD_VIEW_LIST = 0,  /*!< GtkSatList */
    GTK_SAT_MOD_VIEW_MAP,       /*!< GtkSatMap */
    GTK_SAT_MOD_VIEW_POLAR,     /*!< GtkPolarView */
    GTK_SAT_MOD_VIEW_SINGLE,    /*!< GtkSingleSat */
    GTK_SAT_MOD_VIEW_NUM,       /*!< Number of modules */
} gtk_sat_mod_view_t;



#define GTK_TYPE_SAT_MODULE         (gtk_sat_module_get_type ())
#define GTK_SAT_MODULE(obj)         GTK_CHECK_CAST (obj,\
                                    gtk_sat_module_get_type (),	\
                                    GtkSatModule)

#define GTK_SAT_MODULE_CLASS(klass) GTK_CHECK_CLASS_CAST (klass,\
gtk_sat_module_get_type (),\
GtkSatModuleClass)

#define IS_GTK_SAT_MODULE(obj)      GTK_CHECK_TYPE (obj, gtk_sat_module_get_type ())


typedef struct _gtk_sat_module      GtkSatModule;
typedef struct _GtkSatModuleClass   GtkSatModuleClass;

struct _gtk_sat_module
{
    GtkVBox vbox;

    gchar         *name;         /*!< The module name */

    GtkWidget     *popup_button; /*!< popup menu button. */
    GtkWidget     *close_button; /*!< The close button */

    GtkWidget     *win;       /*!< Window when module is not docked */

    GtkWidget     *rotctrlwin;  /*!< Rotator controller window */
    GtkWidget     *rotctrl;
    GtkWidget     *rigctrlwin;  /*!< Radio controller window */
    GtkWidget     *rigctrl;

    GtkWidget     *header;
    guint          head_count;
    guint          head_timeout;
    guint          event_count;
    guint          event_timeout;

    /* layout and children */
    gint         *grid;         /*!< The grid layout array [(type,left,right,top,bottom),...] */
    guint         nviews;       /*!< The number of views */
    GSList       *views;        /*!< Pointers to the views */

    GKeyFile      *cfgdata;      /*!< Configuration data. */
    qth_t         *qth;          /*!< QTH information. */

    GHashTable    *satellites;   /*!< Satellites. */

    guint32        timeout;      /*!< Timeout value [msec] */

    gtk_sat_mod_state_t  state;   /*!< The state of the module. */

    guint          timerid;      /*!< The timeout ID (FIXME: REMOVE) */
    GMutex       *busy;          /*!< Flag indicating whether timeout has
                                      finished or not. Also used for blocking
                                      the module during TLE update. */

    /* time keeping */
    gdouble        rtNow;        /*!< Real-time in this cycle */
    gdouble        rtPrev;       /*!< Real-time in previous cycle */

    gint           throttle;     /*!< Time throttle. */
    gdouble        tmgPdnum;     /*!< Daynum at previous update. */
    gdouble        tmgCdnum;     /*!< Daynum at current update. */
    gboolean       tmgActive;    /*!< Flag indicating whether time mgr is active */
    GtkWidget     *tmgFactor;    /*!< Spin button for throttle value selection 2..10 */
    GtkWidget     *tmgCal;       /*!< Calendar widget for selecting date */
    GtkWidget     *tmgHour;      /*!< Spin button for setting the hour */
    GtkWidget     *tmgMin;       /*!< Spin button for setting the minutes */
    GtkWidget     *tmgSec;       /*!< Spin button for setting the seconds */
    GtkWidget     *tmgMsec;      /*!< Spin button for setting the milliseconds */
    GtkWidget     *tmgSlider;    /*!< Slider for manual time "dragging" */
    GtkWidget     *tmgStop;      /*!< Stop button; throttle = 0 */
    GtkWidget     *tmgFwd;       /*!< Forward */
    GtkWidget     *tmgBwd;       /*!< Backward */
    GtkWidget     *tmgReset;     /*!< Reset button */
    GtkWidget     *tmgWin;       /*!< Window containing the widgets. */
    GtkWidget     *tmgState;     /*!< Status label indicating RT/SRT/MAN */

    gboolean       reset;     /*!< Flag indicating whether time reset is in progress */
};

struct _GtkSatModuleClass
{
    GtkVBoxClass parent_class;
};


GtkType        gtk_sat_module_get_type        (void);
GtkWidget*     gtk_sat_module_new             (const gchar *cfgfile);


void     gtk_sat_module_close_cb       (GtkWidget *button, gpointer data);
void     gtk_sat_module_config_cb      (GtkWidget *button, gpointer data);

void     gtk_sat_module_reload_sats    (GtkSatModule *module);
void     gtk_sat_module_reconf         (GtkSatModule *module, gboolean local);

void     gtk_sat_module_fix_size (GtkWidget *module);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTK_SAT_MODULE_H__ */
