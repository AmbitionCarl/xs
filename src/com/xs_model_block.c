/*
 * This file is part of the xs Library.
 *
 * Copyright (C) 2011 by Guoliang Xue <xueguoliang@gmail.com>
 *
 * The xs Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The xs Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the xs Library. If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef __cplusplus
extern "C"{
#endif
#include "xs.h"

int xs_model_send_block(xs_model_t* model, int fd, int timeout)
{
    char* buf;
    int len;
    buf = xs_model_to_buf(model, &len); 

    int slen = htonl(len);
    int ret = xs_sock_send_block(fd, &slen, 4, timeout);
    if(ret == 4)
    {
        ret = xs_sock_send_block(fd, buf, len, timeout);
        if(ret == len) return 0;
    }
    return -1;
}

int xs_model_recv_block(xs_model_t** model, int fd, int timeout)
{
    int len;
    int ret = xs_sock_recv_block(fd, &len, 4, timeout);
    if(ret != 4)
        return -1;

    len = ntohl(len);
    char* buf = xs_malloc(len);
    ret = xs_sock_recv_block(fd, buf, len, timeout);
    if(ret != len)
    {
        xs_free(buf); return -1;
    }

    *model = xs_model_from_buf(buf);
    xs_free(buf);
    return 0;
}

int xs_model_recv_arg(int fd, int argc, ...)
{
    xs_model_t* model = NULL;
    va_list ap;
    int i = xs_model_recv_block(&model, fd, 60000);
    if(i < 0)
        return i;

    if(argc != model->argc)
    {
        xs_logd("argc error");
        xs_model_delete(model);
        return -1;
    }
    va_start(ap, argc);

    for(i=0; i<argc; ++i)
    {
        *va_arg(ap, char**) = xs_strdup(model->argv[i]);
    }

    xs_model_delete(model);
    va_end(ap);
    return 0;
}

int xs_model_send_arg(int fd, int argc, ...)
{
    va_list ap;
    va_start(ap, argc);

    xs_model_t* model = xs_model_create(argc);
    int i;
    for(i=0; i<argc; ++i)
    {
        model->argv[i] = xs_strdup(va_arg(ap, char*));
    }
    va_end(ap);

    i = xs_model_send_block(model, fd, 60000);
    xs_model_delete(model);
    return i;
}

#ifdef __cplusplus
}
#endif

