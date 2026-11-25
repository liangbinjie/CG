#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "../object/object.h"

static char *copy_string(const char *text) {
    size_t length = strlen(text) + 1;
    char *copy = malloc(length);
    if (copy) memcpy(copy, text, length);
    return copy;
}

static int parse_cut_plane(const char *line, CutPlane **planes, int *count) {
    CutPlane plane;
    int used = 0;
    if (sscanf(line, " %d %lf %lf %lf %lf %n", &plane.id, &plane.a,
               &plane.b, &plane.c, &plane.d, &used) != 5 || plane.id < 0)
        return -1;

    const char *end = line + used;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0' && *end != '#') return -1;

    double length = hypot(hypot(plane.a, plane.b), plane.c);
    if (!isfinite(length) || length == 0.0 || !isfinite(plane.d))
        return -1;
    plane.a /= length;
    plane.b /= length;
    plane.c /= length;
    plane.d /= length;
    if (!isfinite(plane.d)) return -1;

    for (int i = 0; i < *count; i++)
        if ((*planes)[i].id == plane.id) return -1;

    CutPlane *resized = realloc(*planes, (size_t)(*count + 1) * sizeof(**planes));
    if (!resized) return -1;
    *planes = resized;
    (*planes)[(*count)++] = plane;
    return 0;
}

static int resolve_cut_planes(char *line, const CutPlane *available,
                              int available_count, CutPlane **resolved,
                              int *resolved_count) {
    char *suffix = strstr(line, " cut_planes=");
    *resolved = NULL;
    *resolved_count = 0;
    if (!suffix) return 0;

    int count = -1, used = 0;
    if (sscanf(suffix, " cut_planes=%d%n", &count, &used) != 1 ||
        count < 0 || count > available_count) {
        fprintf(stderr, "ERROR: Invalid cut plane count in object (count=%d, available=%d).\n",
                count, available_count);
        return -1;
    }

    CutPlane *planes = count ? malloc((size_t)count * sizeof(*planes)) : NULL;
    if (count && !planes) return -1;
    const char *cursor = suffix + used;
    for (int i = 0; i < count; i++) {
        while (isspace((unsigned char)*cursor)) cursor++;
        char *end;
        long id = strtol(cursor, &end, 10);
        if (end == cursor || id < 0 || id > 2147483647L) {
            free(planes);
            return -1;
        }
        cursor = end;

        int found = 0;
        for (int j = 0; j < available_count; j++) {
            if (available[j].id == id) {
                planes[i] = available[j];
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "ERROR: Unknown cut plane ID %ld (available: %d).\n",
                    id, available_count);
            free(planes);
            return -1;
        }
    }

    while (isspace((unsigned char)*cursor)) cursor++;
    if (*cursor != '\0' && *cursor != '#') {
        free(planes);
        return -1;
    }
    *suffix = '\0';
    *resolved = planes;
    *resolved_count = count;
    return 0;
}

static void release_pending_objects(char **lines, int count) {
    for (int i = 0; i < count; i++) free(lines[i]);
    free(lines);
}

static void release_config_data(Config *config, CutPlane *planes,
                                char **object_lines, int object_count) {
    free(config->lights);
    config->lights = NULL;
    config->light_count = 0;
    free(planes);
    free_objects(config->objects);
    config->objects = NULL;
    release_pending_objects(object_lines, object_count);
}

int load_config(Config *config) {
    if (!config) return 1;
    config->objects = NULL;
    config->lights = NULL;
    config->light_count = 0;

    double background_alpha = 1.0;
    CutPlane *cut_planes = NULL;
    int cut_plane_count = 0;
    char **object_lines = NULL;
    int object_count = 0;
    FILE *file = fopen(CONFIG_FILE, "r");
    char line[4096];
    if (!file) return 1;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "object=", 7) == 0) {
            char *copy = copy_string(line + 7);
            if (!copy) goto fail;
            char **resized = realloc(object_lines,
                                     (size_t)(object_count + 1) * sizeof(*object_lines));
            if (!resized) {
                free(copy);
                goto fail;
            }
            object_lines = resized;
            object_lines[object_count++] = copy;
        } else if (strncmp(line, "cut_plane=", 10) == 0) {
            if (parse_cut_plane(line + 10, &cut_planes, &cut_plane_count) != 0) {
                fprintf(stderr, "ERROR: Invalid cut plane:\n%s\n", line);
                goto fail;
            }
        } else if (strncmp(line, "light=", 6) == 0) {
            Light light;
            if (sscanf(line + 6, " %lf %lf %lf %lf %lf %lf %lf",
                       &light.position.x, &light.position.y, &light.position.z,
                       &light.light_intensity, &light.c1, &light.c2,
                       &light.c3) != 7) {
                fprintf(stderr, "ERROR: Invalid light:\n%s\n", line);
                goto fail;
            }
            Light *resized = realloc(config->lights,
                (size_t)(config->light_count + 1) * sizeof(*config->lights));
            if (!resized) goto fail;
            config->lights = resized;
            config->lights[config->light_count++] = light;
        } else {
            sscanf(line, "width=%d", &config->width);
            sscanf(line, "height=%d", &config->height);
            sscanf(line, "window_xmin=%d", &config->window.xmin);
            sscanf(line, "window_ymin=%d", &config->window.ymin);
            sscanf(line, "window_xmax=%d", &config->window.xmax);
            sscanf(line, "window_ymax=%d", &config->window.ymax);
            sscanf(line, "output_name=%99s", config->output_name);
            sscanf(line, "eye_x=%lf", &config->eye.x);
            sscanf(line, "eye_y=%lf", &config->eye.y);
            sscanf(line, "eye_z=%lf", &config->eye.z);
            sscanf(line, "background_color_a=%lf", &background_alpha);
            sscanf(line, "background_color_r=%Lf", &config->background_color.r);
            sscanf(line, "background_color_g=%Lf", &config->background_color.g);
            sscanf(line, "background_color_b=%Lf", &config->background_color.b);
            sscanf(line, "ambient_intensity=%lf", &config->ambient_intensity);
        }
    }
    if (ferror(file)) goto fail;
    fclose(file);
    file = NULL;

    if (background_alpha < 0.0 || background_alpha > 1.0 ||
        config->background_color.r < 0.0 || config->background_color.r > 1.0 ||
        config->background_color.g < 0.0 || config->background_color.g > 1.0 ||
        config->background_color.b < 0.0 || config->background_color.b > 1.0) {
        goto fail;
    }
    config->background_color.a = (uint8_t)(background_alpha * 255.0);

    for (int i = 0; i < object_count; i++) {
        CutPlane *resolved = NULL;
        int resolved_count = 0;
        if (resolve_cut_planes(object_lines[i], cut_planes, cut_plane_count,
                               &resolved, &resolved_count) != 0) {
            fprintf(stderr, "ERROR: Invalid cut plane reference in object:\n%s\n",
                    object_lines[i]);
            free(resolved);
            goto fail;
        }
        if (parse_object(object_lines[i], &config->objects) != 0) {
            fprintf(stderr, "ERROR: Failed to parse object:\n%s\n",
                    object_lines[i]);
            free(resolved);
            goto fail;
        }
        config->objects->cut_planes = resolved;
        config->objects->cut_plane_count = resolved_count;
    }

    free(cut_planes);
    release_pending_objects(object_lines, object_count);
    return 0;

fail:
    if (file) fclose(file);
    release_config_data(config, cut_planes, object_lines, object_count);
    return 1;
}
