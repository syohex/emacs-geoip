/*
  Copyright (C) 2016 by Syohei YOSHIDA

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <emacs-module.h>

#include <GeoIP.h>
#include <GeoIPCity.h>

int plugin_is_GPL_compatible;

static char*
retrieve_string(emacs_env *env, emacs_value str, ptrdiff_t *size)
{
	*size = 0;

	env->copy_string_contents(env, str, NULL, size);
	char *p = malloc(*size);
	if (p == NULL) {
		*size = 0;
		return NULL;
	}
	env->copy_string_contents(env, str, p, size);

	return p;
}

static void
el_geoip_free(void *data)
{
	GeoIP_delete((GeoIP*)data);
}

static emacs_value
Fgeoip_init(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	emacs_value db = args[0];
	ptrdiff_t size;
	char *db_str = retrieve_string(env, db, &size);
	if (db_str == NULL)
		return env->intern(env, "nil");

	GeoIP *gi = GeoIP_open(db_str, GEOIP_INDEX_CACHE);
	free(db_str);

	if (gi == NULL) {
		return env->intern(env, "nil");
	}

	return env->make_user_ptr(env, el_geoip_free, gi);
}

static void
el_geoip_record_free(void *data)
{
	GeoIPRecord_delete((GeoIPRecord*)data);
}

static emacs_value
Fgeoip_record_by_name(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIP *gi = env->get_user_ptr(env, args[0]);
	ptrdiff_t size;
	char *host = retrieve_string(env, args[1], &size);
	if (host == NULL)
		return env->intern(env, "nil");

	GeoIPRecord *gir = GeoIP_record_by_name(gi, host);
	free(host);

	if (gir == NULL) {
		return env->intern(env, "nil");
	}

	return env->make_user_ptr(env, el_geoip_record_free, gir);
}

static emacs_value
Fgeoip_record_by_addr(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIP *gi = env->get_user_ptr(env, args[0]);
	ptrdiff_t size;
	char *addr = retrieve_string(env, args[1], &size);
	if (addr == NULL)
		return env->intern(env, "nil");

	GeoIPRecord *gir = GeoIP_record_by_addr(gi, addr);
	free(addr);

	if (gir == NULL) {
		return env->intern(env, "nil");
	}

	return env->make_user_ptr(env, el_geoip_record_free, gir);
}

static emacs_value
Fgeoip_country_code(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	if (gir->country_code == NULL)
		return env->make_string(env, "N/A", 3);

	return env->make_string(env, gir->country_code, strlen(gir->country_code));
}

static emacs_value
Fgeoip_region(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	if (gir->region == NULL)
		return env->make_string(env, "N/A", 3);

	return env->make_string(env, gir->region, strlen(gir->region));
}

static emacs_value
Fgeoip_region_name(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);

	const char *region_name = GeoIP_region_name_by_code(gir->country_code, gir->region);
	if (region_name == NULL)
		return env->make_string(env, "N/A", 3);

	return env->make_string(env, region_name, strlen(region_name));
}

static emacs_value
Fgeoip_city(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	if (gir->city == NULL)
		return env->make_string(env, "N/A", 3);

	return env->make_string(env, gir->city, strlen(gir->city));
}

static emacs_value
Fgeoip_postal_code(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	if (gir->postal_code == NULL)
		return env->make_string(env, "N/A", 3);

	return env->make_string(env, gir->postal_code, strlen(gir->postal_code));
}
static emacs_value
Fgeoip_latitude(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	return env->make_float(env, gir->latitude);
}

static emacs_value
Fgeoip_longitude(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	return env->make_float(env, gir->longitude);
}

static emacs_value
Fgeoip_metro_code(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	return env->make_integer(env, gir->metro_code);
}

static emacs_value
Fgeoip_area_code(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	return env->make_integer(env, gir->area_code);
}

static emacs_value
Fgeoip_time_zone(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	GeoIPRecord *gir = env->get_user_ptr(env, args[0]);
	const char *tz = GeoIP_time_zone_by_country_and_region(gir->country_code,
							       gir->region);
	if (tz == NULL)
		return env->make_string(env, "N/A", 3);

	return env->make_string(env, tz, strlen(tz));
}

static void
bind_function(emacs_env *env, const char *name, emacs_value Sfun)
{
	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qsym = env->intern(env, name);
	emacs_value args[] = { Qsym, Sfun };

	env->funcall(env, Qfset, 2, args);
}

static void
provide(emacs_env *env, const char *feature)
{
	emacs_value Qfeat = env->intern(env, feature);
	emacs_value Qprovide = env->intern (env, "provide");
	emacs_value args[] = { Qfeat };

	env->funcall(env, Qprovide, 1, args);
}

int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env *env = ert->get_environment(ert);

#define DEFUN(lsym, csym, amin, amax, doc, data) \
	bind_function (env, lsym, env->make_function(env, amin, amax, csym, doc, data))

	DEFUN("geoip-core-init", Fgeoip_init, 1, 1, NULL, NULL);
	DEFUN("geoip-core-record-by-name", Fgeoip_record_by_name, 2, 2, NULL, NULL);
	DEFUN("geoip-core-record-by-addr", Fgeoip_record_by_addr, 2, 2, NULL, NULL);

	DEFUN("geoip-core-country-code", Fgeoip_country_code, 1, 1, NULL, NULL);
	DEFUN("geoip-core-region", Fgeoip_region, 1, 1, NULL, NULL);
	DEFUN("geoip-core-region-name", Fgeoip_region_name, 1, 1, NULL, NULL);
	DEFUN("geoip-core-city", Fgeoip_city, 1, 1, NULL, NULL);
	DEFUN("geoip-core-postal-code", Fgeoip_postal_code, 1, 1, NULL, NULL);
	DEFUN("geoip-core-latitude", Fgeoip_latitude, 1, 1, NULL, NULL);
	DEFUN("geoip-core-longitude", Fgeoip_longitude, 1, 1, NULL, NULL);
	DEFUN("geoip-core-metro-code", Fgeoip_metro_code, 1, 1, NULL, NULL);
	DEFUN("geoip-core-area-code", Fgeoip_area_code, 1, 1, NULL, NULL);
	DEFUN("geoip-core-time-zone", Fgeoip_time_zone, 1, 1, NULL, NULL);

#undef DEFUN

	provide(env, "geoip-core");
	return 0;
}

/*
  Local Variables:
  c-basic-offset: 8
  indent-tabs-mode: t
  End:
*/
