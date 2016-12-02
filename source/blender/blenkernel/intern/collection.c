/*
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
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Contributor(s): Dalai Felinto
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/blenkernel/intern/collection.c
 *  \ingroup bke
 */

#include "BLI_blenlib.h"
#include "BLI_listbase.h"
#include "BLT_translation.h"

#include "BKE_collection.h"
#include "BKE_layer.h"
#include "BKE_library.h"

#include "DNA_ID.h"
#include "DNA_layer_types.h"
#include "DNA_scene_types.h"

#include "MEM_guardedalloc.h"

/*
 * Add a collection to a collection ListBase and syncronize all render layers
 * The ListBase is NULL when the collection is to be added to the master collection
 */
SceneCollection *BKE_collection_add(Scene *scene, SceneCollection *sc_parent, const char *name)
{
	SceneCollection *sc = MEM_callocN(sizeof(SceneCollection), "New Collection");
	SceneCollection *sc_master = BKE_collection_master(scene);

	BLI_strncpy(sc->name, name, sizeof(sc->name));
	BLI_uniquename(&sc_master->collections, sc, DATA_("Collection"), '.', offsetof(SceneCollection, name), sizeof(sc->name));

	BLI_addtail(&sc_parent->collections, sc);

	TODO_LAYER_SYNC;
	return sc;
}

/* free the collection items recursively */
static void collection_free(SceneCollection *sc)
{
	for (LinkData *link = sc->objects.first; link; link = link->next) {
		id_us_min(link->data);
	}

	for (LinkData *link = sc->filter_objects.first; link; link = link->next) {
		id_us_min(link->data);
	}

	for (SceneCollection *nsc = sc->collections.first; nsc; nsc = nsc->next) {
		collection_free(nsc);
	}
	BLI_freelistN(&sc->collections);
}

/* unlink the collection recursively
 * return true if unlinked */
static bool collection_remlink(SceneCollection *sc_parent, SceneCollection *sc_gone)
{
	for (SceneCollection *sc = sc_parent->collections.first; sc; sc = sc->next)
	{
		if (sc == sc_gone) {
			BLI_remlink(&sc_parent->collections, sc_gone);
			return true;
		}

		if (collection_remlink(sc, sc_gone)) {
			return true;
		}
	}
	return false;
}

/*
 * Remove a collection from the scene, and syncronize all render layers
 */
bool BKE_collection_remove(Scene *scene, SceneCollection *sc)
{
	SceneCollection *sc_master = BKE_collection_master(scene);

	/* the master collection cannot be removed */
	if (sc == sc_master) {
		return false;
	}

	/* unlink from the respective collection tree */
	if (!collection_remlink(sc_master, sc)) {
		BLI_assert(false);
	}

	/* clear the collection items */
	collection_free(sc);

	/* TODO: check all layers that use this collection and clear them */
	TODO_LAYER_SYNC;

	MEM_freeN(sc);

	return true;
}

/*
 * Returns the master collection
 */
SceneCollection *BKE_collection_master(Scene *scene)
{
	return &scene->collection;
}

/*
 * Add object to collection
 */
void BKE_collection_object_add(struct Scene *UNUSED(scene), struct SceneCollection *sc, struct Object *ob)
{
	BLI_addtail(&sc->objects, BLI_genericNodeN(ob));
	id_us_plus((ID *)ob);
	TODO_LAYER_SYNC;
}
