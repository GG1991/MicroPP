/*
 *  This source code is part of MicroPP: a finite element library
 *  to solve microstructural problems for composite materials.
 *
 *  Copyright (C) - 2018 - Guido Giuntoli <gagiuntoli@gmail.com>
 *						   Jimmy Aguilar Mena <kratsbinovish@gmail.com>
 *                         Judicaël Grasset <judicael.grasset@stfc.ac.uk>
 *                         Alejandro Figueroa <afiguer7@maisonlive.gmu.edu>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "common.hpp"
#include "micropp.hpp"

template <>
double micropp<3>::assembly_rhs(const double *u, const double *int_vars_old, double *b) {
  INST_START;

  memset(b, 0., nndim * sizeof(double));

  double be[dim * npe];
  int index[dim * npe];

  for (int ez = 0; ez < nez; ++ez) {
    for (int ey = 0; ey < ney; ++ey) {
      for (int ex = 0; ex < nex; ++ex) {
        int n[npe];
        get_elem_nodes(n, nx, ny, ex, ey, ez);

        for (int j = 0; j < npe; ++j)
          for (int d = 0; d < dim; ++d) index[j * dim + d] = n[j] * dim + d;

        get_elem_rhs(u, int_vars_old, be, ex, ey, ez);

        for (int i = 0; i < npe * dim; ++i) b[index[i]] += be[i];
      }
    }
  }

  // boundary conditions
  for (int i = 0; i < nx; ++i) {
    for (int j = 0; j < ny; ++j) {
      const int n = nod_index3D(i, j, 0);  // z = 0
      memset(&b[n * dim], 0., dim * sizeof(double));
    }
  }

  for (int i = 0; i < nx; ++i) {
    for (int j = 0; j < ny; ++j) {
      const int n = nod_index3D(i, j, nz - 1);  // z = lx
      memset(&b[n * dim], 0., dim * sizeof(double));
    }
  }

  for (int i = 0; i < nx; ++i) {
    for (int k = 1; k < nz - 1; ++k) {
      const int n = nod_index3D(i, 0, k);  // y = 0
      memset(&b[n * dim], 0., dim * sizeof(double));
    }
  }

  for (int i = 0; i < nx; ++i) {
    for (int k = 1; k < nz - 1; ++k) {
      const int n = nod_index3D(i, ny - 1, k);  // y = ly
      memset(&b[n * dim], 0., dim * sizeof(double));
    }
  }

  for (int j = 1; j < ny - 1; ++j) {
    for (int k = 1; k < nz - 1; ++k) {
      const int n = nod_index3D(0, j, k);  // x = 0
      memset(&b[n * dim], 0., dim * sizeof(double));
    }
  }

  for (int j = 1; j < ny - 1; j++) {
    for (int k = 1; k < nz - 1; ++k) {
      const int n = nod_index3D(nx - 1, j, k);  // x = lx
      memset(&b[n * dim], 0., dim * sizeof(double));
    }
  }

  // Common
  for (int i = 0; i < nndim; ++i) b[i] = -b[i];

  double norm = 0.0;
  for (int i = 0; i < nndim; ++i) norm += b[i] * b[i];
  norm = sqrt(norm);

  return norm;
}

template <>
void micropp<3>::assembly_mat(ell_matrix *A, const double *u, const double *int_vars_old) {
  INST_START;

  ell_set_zero_mat(A);

  double Ae[npe * dim * npe * dim];
  for (int ex = 0; ex < nex; ++ex) {
    for (int ey = 0; ey < ney; ++ey) {
      for (int ez = 0; ez < nez; ++ez) {
        get_elem_mat(u, int_vars_old, Ae, ex, ey, ez);
        ell_add_3D(A, ex, ey, ez, Ae);
      }
    }
  }
  ell_set_bc_3D(A);
}

template <int tdim>
void micropp<tdim>::get_elem_rhs(const double *u, const double *vars_old, double be[npe * dim], int ex, int ey,
                                 int ez) const {
  constexpr int npedim = npe * dim;
  double stress_gp[nvoi], strain_gp[nvoi];

  memset(be, 0, npedim * sizeof(double));

  for (int gp = 0; gp < npe; ++gp) {
    get_strain(u, gp, strain_gp, bmat, nx, ny, ex, ey, ez);
    get_stress(gp, strain_gp, vars_old, stress_gp, ex, ey, ez);

    for (int i = 0; i < npedim; ++i)
      for (int j = 0; j < nvoi; ++j) be[i] += bmat[gp][j][i] * stress_gp[j] * wg;
  }
}

template <int tdim>
void micropp<tdim>::get_elem_mat(const double *u, const double *vars_old, double Ae[npe * dim * npe * dim], int ex,
                                 int ey, int ez) const {
  const int e = glo_elem(ex, ey, ez);
  const material_t *material = get_material(e);

  double ctan[nvoi][nvoi];
  constexpr int npedim = npe * dim;
  constexpr int npedim2 = npedim * npedim;

  double TAe[npedim2] = {0.0};

  for (int gp = 0; gp < npe; ++gp) {
    double eps[6];
    get_strain(u, gp, eps, bmat, nx, ny, ex, ey, ez);

    const double *vars = (vars_old) ? &vars_old[intvar_ix(e, gp, 0)] : nullptr;
    material->get_ctan(eps, (double *)ctan, vars);

    double cxb[nvoi][npedim];

    for (int i = 0; i < nvoi; ++i) {
      for (int j = 0; j < npedim; ++j) {
        double tmp = 0.0;
        for (int k = 0; k < nvoi; ++k) tmp += ctan[i][k] * bmat[gp][k][j];
        cxb[i][j] = tmp * wg;
      }
    }

    for (int m = 0; m < nvoi; ++m) {
      for (int i = 0; i < npedim; ++i) {
        const int inpedim = i * npedim;
        const double bmatmi = bmat[gp][m][i];
        for (int j = 0; j < npedim; ++j) TAe[inpedim + j] += bmatmi * cxb[m][j];
      }
    }
  }
  memcpy(Ae, TAe, npedim2 * sizeof(double));
}
