/**
 * vrms-rpm - list non-free packages on an rpm-based Linux distribution
 * Copyright (C) 2018 Artur "suve" Iwicki
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program (LICENCE.txt). If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>

#include "fileutils.h"
#include "licences.h"
#include "options.h"
#include "packages.h"
#include "pipes.h"

static void easteregg(void) {
	int free, nonfree;
	packages_getcount(&free, &nonfree);
	
	if(nonfree == 0) {
		putc('\n', stdout);
		rms_happy();
		puts("Only free packages - rms would be proud!");
	} else {
		const int total_packages = free + nonfree;
		if(nonfree > (total_packages / 10)) {
			putc('\n', stdout);
			rms_disappointed();
			puts("Over 10% non-free packages. Don't you appreciate freedom?");
		}
	}
}

int main(int argc, char *argv[]) {
	options_parse(argc, argv);
	
	struct Pipe *rpmpipe = packages_openPipe();
	if(rpmpipe == NULL) {
		fprintf(stderr, "vrms-rpm: failed to open pipe to rpm\n");
		exit(EXIT_FAILURE);
	}
	
	if(licences_read() < 0) {
		fprintf(stderr, "vrms-rpm: failed to read list of good licences\n");
		exit(EXIT_FAILURE);
	}
	
	if(packages_read(rpmpipe) < 0) {
		fprintf(stderr, "vrms-rpm: failed to read from rpm-pipe\n");
		exit(EXIT_FAILURE);
	}
	
	packages_list();
	easteregg();
	
	packages_free();
	licences_free();
	return 0;
}
