/*
 * This source code is part of MicroPP: a finite element library
 * to solve microstructural problems for composite materials.
 *
 * Copyright (C) - 2018 - Jimmy Aguilar Mena <kratsbinovish@gmail.com>
 *                        Guido Giuntoli <gagiuntoli@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MICROPP3_WRAPPER_H
#define MICROPP3_WRAPPER_H

#include "material_base.h"

#ifdef __cplusplus
extern "C" {
	#endif

	struct micropp3 {
		void *ptr;
	};

	void micropp3_new(struct micropp3 *,int ngp, const int size[3],
	                  const int micro_type, const double *micro_params,
	                  const struct material_base *materials);

	void micropp3_free(struct micropp3 *in);
	void micropp3_set_macro_strain(struct micropp3 *self, const int gp_id,
	                               const double *macro_strain);

	void micropp3_homogenize(struct micropp3 *self);
	bool micropp3_get_nl_flag(const struct micropp3 *self, const int gp_id);
	int micropp3_get_cost(const struct micropp3 *self, int gp_id);
	void micropp3_get_macro_stress(const struct micropp3 *self, const int gp_id,
	                               double *macro_stress);

	void micropp3_get_macro_ctan(const struct micropp3 *self, const int gp_id,
	                             double *macro_ctan);

	void micropp3_update_vars(struct micropp3 *self);

	void micropp3_output(struct micropp3 *self, const int gp_id,
	                     const char *filename);

	void micropp3_print_info(struct micropp3 *self);

	int micropp3_get_non_linear_gps(const struct micropp3 *self);

	double micropp3_get_f_trial_max(const struct micropp3 *self);

	#ifdef __cplusplus
}
#endif
#endif
