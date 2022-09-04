/******************************************************************************
    Copyright (C) 2022 by Jacob Thornton

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <stdio.h>

#include <obs-module.h>
#include <obs-avc.h>
#include <obs.h>
#include <util/platform.h>
#include <util/dstr.h>
#include <inttypes.h>

#ifndef _STDINT_H_INCLUDED
#define _STDINT_H_INCLUDED
#endif


/* ------------------------------------------------------------------------- */

struct stream_output {
    obs_output_t *output;
    volatile bool active;
    volatile bool stopping;
};

static const char *stream_output_getname(void *unused)
{
    UNUSED_PARAMETER(unused);
    return obs_module_text("StreamOutput");
}

static void stream_output_stop(void *data, uint64_t ts);

static void stream_output_destroy(void *data)
{
    struct stream_output *stream = data;
    bfree(stream);
}

static void *stream_output_create(obs_data_t *settings, obs_output_t *output)
{
    struct stream_output *stream = bzalloc(sizeof(struct stream_output));
    stream->output = output;

    UNUSED_PARAMETER(settings);
    return stream;
}

static bool stream_output_start(void *data)
{
    struct stream_output *stream = data;
	obs_data_t *settings;
	const char *path;

	if (!obs_output_can_begin_data_capture(stream->output, 0))
		return false;
    
    stream->stopping = false;
    stream->active = true;
	// os_atomic_set_bool(&stream->stopping, false);
    // os_atomic_set_bool(&stream->active, true);
    UNUSED_PARAMETER(settings);
    return true;
}

static void stream_output_stop(void *data, uint64_t ts)
{
    struct stream_output *stream = data;
    stream->stopping = true;
    // os_atomic_set_bool(&stream->stopping, true);
}

static void stream_output_raw_video(void *data, struct video_data* frame)
{

}

/* ------------------------------------------------------------------------- */

struct obs_output_info stream_output_info = {
    .id                   = "kekw",
    .flags                = OBS_OUTPUT_VIDEO,
    .get_name             = stream_output_getname,
    .create               = stream_output_create,
    .destroy              = stream_output_destroy,
    .start                = stream_output_start,
    .stop                 = stream_output_stop,
    .raw_video            = stream_output_raw_video,
};