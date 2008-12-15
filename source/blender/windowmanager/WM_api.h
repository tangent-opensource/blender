/**
 * $Id:
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007 Blender Foundation.
 * All rights reserved.
 *
 * 
 * Contributor(s): Blender Foundation
 *
 * ***** END GPL LICENSE BLOCK *****
 */
#ifndef WM_API_H
#define WM_API_H

/* dna-savable wmStructs here */
#include "DNA_windowmanager_types.h"

struct bContext;
struct wmEvent;
struct wmEventHandler;
struct wmGesture;
struct rcti;

			/* general API */
void		WM_setprefsize		(int stax, int stay, int sizx, int sizy);

void		WM_init				(struct bContext *C);
void		WM_exit				(struct bContext *C);
void		WM_main				(struct bContext *C);

wmWindow	*WM_window_open		(struct bContext *C, struct rcti *rect);

			/* files */
int			WM_read_homefile	(struct bContext *C, int from_memory);
int			WM_write_homefile	(struct bContext *C, struct wmOperator *op);
void		WM_read_file		(struct bContext *C, char *name);
void		WM_write_file		(struct bContext *C, char *target);
void		WM_read_autosavefile(struct bContext *C);
void		WM_write_autosave	(struct bContext *C);

			/* mouse cursors */
void		WM_cursor_set		(struct wmWindow *win, int curs);
void		WM_cursor_modal		(struct wmWindow *win, int curs);
void		WM_cursor_restore	(struct wmWindow *win);
void		WM_cursor_wait		(struct wmWindow *win, int val);
void		WM_timecursor		(struct wmWindow *win, int nr);

			/* keymap and handlers */
wmKeymapItem *WM_keymap_set_item	(ListBase *lb, char *idname, short type, 
								 short val, int modifier, short keymodifier);
wmKeymapItem *WM_keymap_verify_item(ListBase *lb, char *idname, short type, 
								 short val, int modifier, short keymodifier);
wmKeymapItem *WM_keymap_add_item	(ListBase *lb, char *idname, short type, 
								 short val, int modifier, short keymodifier);
ListBase	*WM_keymap_listbase	(wmWindowManager *wm, const char *nameid, 
								 int spaceid, int regionid);


struct wmEventHandler *WM_event_add_keymap_handler(ListBase *handlers, ListBase *keymap);
						/* boundbox, optional subwindow boundbox for offset */
struct wmEventHandler *WM_event_add_keymap_handler_bb(ListBase *handlers, ListBase *keymap, rcti *bb, rcti *swinbb);

void		WM_event_remove_keymap_handler(ListBase *handlers, ListBase *keymap);

struct wmEventHandler *WM_event_add_ui_handler(bContext *C, ListBase *handlers,
			int (*func)(bContext *C, struct wmEvent *event, void *userdata),
			void (*remove)(bContext *C, void *userdata), void *userdata);
void		WM_event_remove_ui_handler(ListBase *handlers,
			int (*func)(bContext *C, struct wmEvent *event, void *userdata),
			void (*remove)(bContext *C, void *userdata), void *userdata);

struct wmEventHandler *WM_event_add_modal_handler(bContext *C, ListBase *handlers, wmOperator *op);
void		WM_event_remove_handlers(bContext *C, ListBase *handlers);

void		WM_event_add_mousemove(bContext *C);

void		WM_event_add_notifier(bContext *C, int type, int value, void *data);

void		wm_event_add		(wmWindow *win, struct wmEvent *event_to_add); /* XXX only for warning */

			/* one-shot timer, returns wmTimerData.handle */
struct wmTimerHandle *WM_event_add_window_timer(wmWindow *win, int delay_ms, int interval_ms);
void		WM_event_remove_window_timer(wmWindow *wm, struct wmTimerHandle *handle);

			/* operator api, default callbacks */
			/* confirm menu + exec */
int			WM_operator_confirm		(struct bContext *C, struct wmOperator *op, struct wmEvent *event);
			/* context checks */
int			WM_operator_winactive	(struct bContext *C);

			/* operator api */
wmOperatorType *WM_operatortype_find(const char *idname);
void		WM_operatortype_append	(void (*opfunc)(wmOperatorType*));

int         WM_operator_call		(struct bContext *C, const char *opstring);
int         WM_operator_call_rwin	(struct bContext *C, const char *opstring);

			/* default operator callbacks for border/lasso */
int			WM_border_select_invoke	(struct bContext *C, wmOperator *op, struct wmEvent *event);
int			WM_border_select_modal	(struct bContext *C, wmOperator *op, struct wmEvent *event);

			/* default operator for arearegions, generates event */
void		WM_OT_tweak_gesture(wmOperatorType *ot);

			/* Gesture manager API */
struct wmGesture *WM_gesture_new(struct bContext *C, struct wmEvent *event, int type);
void		WM_gesture_end(struct bContext *C, struct wmGesture *gesture);

			/* Reporting information and errors */
void		WM_report(struct bContext *C, int type, const char *message);
void		WM_reportf(struct bContext *C, int type, const char *format, ...);

			/* OpenGL wrappers, mimicing opengl syntax */
void		wmLoadMatrix		(wmWindow *win, float mat[][4]);
void		wmGetMatrix			(wmWindow *win, float mat[][4]);
void		wmMultMatrix		(wmWindow *win, float mat[][4]);
void		wmGetSingleMatrix	(wmWindow *win, float mat[][4]);
void		wmScale				(wmWindow *win, float x, float y, float z);
void		wmLoadIdentity		(wmWindow *win);	/* note: old name clear_view_mat */

void		wmFrustum			(wmWindow *win, float x1, float x2, float y1, float y2, float n, float f);
void		wmOrtho				(wmWindow *win, float x1, float x2, float y1, float y2, float n, float f);
void		wmOrtho2			(wmWindow *win, float x1, float x2, float y1, float y2);


#endif /* WM_API_H */

