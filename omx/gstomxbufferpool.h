/*
 * Copyright 2014 Advanced Micro Devices, Inc.
 *   Author: Christian König <christian.koenig@amd.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef __GST_OMX_BUFFER_POOL_H__
#define __GST_OMX_BUFFER_POOL_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/gstvideometa.h>
#include <gst/video/gstvideopool.h>

#include "gstomx.h"

G_BEGIN_DECLS

#define GST_TYPE_OMX_BUFFER_POOL \
  (gst_omx_buffer_pool_get_type())
#define GST_OMX_BUFFER_POOL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OMX_BUFFER_POOL,GstOMXBufferPool))
#define GST_IS_OMX_BUFFER_POOL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OMX_BUFFER_POOL))

typedef struct _GstOMXBufferPool GstOMXBufferPool;
typedef struct _GstOMXBufferPoolClass GstOMXBufferPoolClass;

struct _GstOMXBufferPool
{
  GstVideoBufferPool parent;

  GstElement *element;

  GstCaps *caps;
  gboolean add_videometa;
  gboolean need_copy;
  GstVideoInfo video_info;

  /* Owned by element, element has to stop this pool before
   * it destroys component or port */
  GstOMXComponent *component;
  GstOMXPort *port;

  /* For handling OpenMAX allocated memory */
  GstAllocator *allocator;

  /* Set from outside this pool */
  /* TRUE if we're currently allocating all our buffers */
  gboolean allocating;
  /* TRUE if the pool is not used anymore */
  gboolean deactivated;

  /* For populating the pool from another one */
  GstBufferPool *other_pool;
  GPtrArray *buffers;

  /* Used during acquire for output ports to
   * specify which buffer has to be retrieved
   * and during alloc, which buffer has to be
   * wrapped
   */
  gint current_buffer_index;
};

struct _GstOMXBufferPoolClass
{
  GstVideoBufferPoolClass parent_class;
};

GType gst_omx_buffer_pool_get_type (void);

GstBufferPool *gst_omx_buffer_pool_new (GstElement * element, GstOMXComponent * component, GstOMXPort * port);

G_END_DECLS

#endif /* __GST_OMX_BUFFER_POOL_H__ */
