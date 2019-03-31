/*
 *  This is a test example for MicroPP: a finite element library
 *  to solve microstructural problems for composite materials.
 *
 *  Copyright (C) - 2018 - Jimmy Aguilar Mena <kratsbinovish@gmail.com>
 *                         Guido Giuntoli <gagiuntoli@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <iomanip>

#include <cstring>
#include <ctime>
#include <cassert>
#include <bits/stdc++.h>

#include "micro.hpp"

using namespace std;

#define dim 3
#define D_EPS 0.01

int main(int argc, char **argv)
{

	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " n [ngp] [steps]" << endl;
		return(1);
	}

	const int nvoi = 6;
	const int n = atoi(argv[1]);
	const int ngp = (argc > 2 ? atoi(argv[2]) : 2);
	const int time_steps = (argc > 3 ? atoi(argv[3]) : 10);  // Optional value

	assert(n > 1 && ngp > 1 && time_steps > 0);

	const int size[dim] = { n, n, n };
	const int micro_type = 1;	// 2 materiales matriz y fibra (3D esfera en matriz)
	const double micro_params[4] = { 1.0, 1.0, 1.0, 0.1 };

	material_base mat_params[2];
	material_set(&mat_params[0], 0, 1.0e6, 0.3, 5.0e4, 2.0e4, 0.0);
	material_set(&mat_params[1], 1, 1.0e3, 0.3, 5.0e4, 1.0e3, 0.0);

	int dir = 2;
	double eps[nvoi] = { 0.0 };
	double sig[nvoi], (*sig_test)[nvoi];
	double ctan[nvoi * nvoi], (*ctan_test)[nvoi * nvoi];

	sig_test = (double (*)[nvoi]) malloc(nvoi * ngp * sizeof(double));
	ctan_test = (double (*)[nvoi * nvoi]) malloc(nvoi * nvoi * ngp * sizeof(double));

	micropp<dim> micro(ngp, size, micro_type, micro_params, mat_params);

	double time = clock();

	cout << scientific;
	for (int t = 0; t < time_steps; ++t) {

		cout << "Time step = " << t << endl;

		if (t < 20)
			eps[dir] += D_EPS;
		else if (t < 40)
			eps[dir] -= D_EPS;
		else
			eps[dir] += D_EPS;

		cout << "setting strains ..." << endl;
		cout << scientific;
		for (int gp = 0; gp < ngp; ++gp) {
			micro.set_strain(gp, eps);
		}
		cout << " eps = ";
		for (int i = 0; i < 6; ++i)
			cout << setw(14) << eps[i] << " ";
		cout << endl;

		cout << "Homogenizing ..." << endl;
		micro.homogenize();

		cout << "Getting stresses ..." << endl;
		for (int gp = 0; gp < ngp; ++gp) {
			micro.get_stress(gp, sig);
			memcpy(sig_test[gp], sig, 3 * sizeof(double));
		}
		cout << " sig  = ";
		for (int i = 0; i < 6; ++i)
			cout << setw(14) << sig[i] << "\t";
		cout << endl;

		for (int gp = 0; gp < ngp; ++gp) {
			micro.get_ctan(gp, ctan);
			memcpy(ctan_test[gp], ctan, 3 * sizeof(double));
		}
		cout << " ctan = ";
		for (int i = 0; i < 6; ++i)
			cout << setw(14) << ctan[i] << "\t";
		cout << endl;

		double diff_sum_sig = 0.0, diff_sum_ctan = 0.0;
		for (int gp = 1; gp < ngp; ++gp) {
			for (int i = 0; i < 6; ++i) {
				const double tmp = fabs(sig_test[gp][i] - sig_test[0][i]);
				diff_sum_sig += tmp;
				assert(tmp < 1.0e-8);
			}
			for (int i = 0; i < 6; ++i) {
				const double tmp = fabs(ctan_test[gp][i] - ctan_test[0][i]);
				diff_sum_ctan += tmp;
				assert(tmp < 1.0e-8);
			}
		}
		cout << "Diff sig:\t" << diff_sum_sig << endl;
		cout << "Diff ctan:\t" << diff_sum_ctan << endl;
		cout << endl;

		micro.update_vars();

	}

	time = clock() - time;
	printf("time = %lf\n", time);

	return 0;
}
