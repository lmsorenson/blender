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
 * along with this program; if not, write to the Free Software  Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2005 by the Blender Foundation.
 * All rights reserved.
 *
 * Contributor(s): Antonio Vazquez
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 */

/** \file blender/modifiers/intern/MOD_gpencilthick.c
 *  \ingroup modifiers
 */

#include <stdio.h>

#include "DNA_scene_types.h"
#include "DNA_object_types.h"
#include "DNA_gpencil_types.h"

#include "BLI_utildefines.h"

#include "BKE_DerivedMesh.h"
#include "BKE_gpencil.h"
#include "BKE_colortools.h"

#include "MOD_modifiertypes.h"

static void initData(ModifierData *md)
{
	GpencilThickModifierData *gpmd = (GpencilThickModifierData *)md;
	gpmd->pass_index = 0;
	gpmd->thickness = 0;
	gpmd->layername[0] = '\0';
	gpmd->vgname[0] = '\0';
	gpmd->cur_thickness = curvemapping_add(1, 0.0f, 0.0f, 1.0f, 1.0f);
	if (gpmd->cur_thickness) {
		curvemapping_initialize(gpmd->cur_thickness);
	}
}

static void freeData(ModifierData *md)
{
	GpencilThickModifierData *gpmd = (GpencilThickModifierData *)md;

	if (gpmd->cur_thickness) {
		curvemapping_free(gpmd->cur_thickness);
	}
}

static void copyData(ModifierData *md, ModifierData *target)
{
	GpencilThickModifierData *gmd = (GpencilThickModifierData *)md;
	GpencilThickModifierData *tgmd = (GpencilThickModifierData *)target;

	if (tgmd->cur_thickness != NULL) {
		curvemapping_free(tgmd->cur_thickness);
	}

	modifier_copyData_generic(md, target);

	tgmd->cur_thickness = curvemapping_copy(gmd->cur_thickness);
}

static DerivedMesh *applyModifier(
        ModifierData *md, const struct EvaluationContext *UNUSED(eval_ctx), Object *ob,
        DerivedMesh *UNUSED(dm),
        ModifierApplyFlag UNUSED(flag))
{
	bGPdata *gpd;
	if ((!ob) || (!ob->gpd)) {
		return NULL;
	}
	gpd = ob->gpd;

	for (bGPDlayer *gpl = gpd->layers.first; gpl; gpl = gpl->next) {
		for (bGPDframe *gpf = gpl->frames.first; gpf; gpf = gpf->next) {
			for (bGPDstroke *gps = gpf->strokes.first; gps; gps = gps->next) {
				BKE_gpencil_thick_modifier(-1, (GpencilThickModifierData *)md, ob, gpl, gps);
			}
		}
	}

	return NULL;
}

ModifierTypeInfo modifierType_GpencilThick = {
	/* name */              "Thickness",
	/* structName */        "GpencilThickModifierData",
	/* structSize */        sizeof(GpencilThickModifierData),
	/* type */              eModifierTypeType_Gpencil,
	/* flags */             eModifierTypeFlag_GpencilMod | eModifierTypeFlag_SupportsEditmode,

	/* copyData */          copyData,
	/* deformVerts */       NULL,
	/* deformMatrices */    NULL,
	/* deformVertsEM */     NULL,
	/* deformMatricesEM */  NULL,
	/* applyModifier */     applyModifier,
	/* applyModifierEM */   NULL,
	/* initData */          initData,
	/* requiredDataMask */  NULL,
	/* freeData */          freeData,
	/* isDisabled */        NULL,
	/* updateDepsgraph */   NULL,
	/* dependsOnTime */     NULL,
	/* dependsOnNormals */	NULL,
	/* foreachObjectLink */ NULL,
	/* foreachIDLink */     NULL,
	/* foreachTexLink */    NULL,
};
